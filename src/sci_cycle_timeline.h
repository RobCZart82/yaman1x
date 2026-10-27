#pragma once
#include "cycle_dispatcher.h"
#include "h83002_sci_mmio.h"
#include <span>

namespace yamanix {
// Project-specific owner: RX input order, then SCI0, then SCI1 at ties.
// This is an explicit software convention, not measured hardware behavior.
class SciCycleTimeline {
public:
    struct RxEvent { uint64_t cycle; unsigned channel; bool level; };
    // Adapter/controller and immutable event storage must outlive this owner.
    SciCycleTimeline(H83002SciMmio& sci, std::span<const RxEvent> input,
                     uint64_t initial_cycle=0)
        : sci_(sci), input_(input), dispatcher_(initial_cycle) {
        uint64_t previous=initial_cycle;
        for(const auto& e:input_) {
            if(e.channel>=2 || e.cycle<previous)
                throw std::invalid_argument("Unsorted, stale or invalid SCI input");
            previous=e.cycle;
        }
        if(const auto event=deadline(); event && *event<initial_cycle)
            throw std::invalid_argument("Stale initial SCI deadline");
    }
    uint64_t cycle() const { return dispatcher_.cycle(); }
    bool failed() const { return dispatcher_.failed(); }
    std::size_t consumed_input() const { return index_; }
    // Sink: tx(channel,cycle,level), clock(channel,cycle,level),
    // bool dma_request(vector,cycle). No callback reentry or peripheral mutation.
    template<class Sink>
    CycleDispatcher::Result advance_to(uint64_t horizon, std::size_t budget, Sink& sink) {
        Owner<Sink> owner{*this,sink};
        return dispatcher_.advance_to(horizon,budget,owner);
    }
private:
    std::optional<uint64_t> deadline() const {
        const auto edge=sci_.next_event(); // SCI uses zero for no clock event.
        std::optional<uint64_t> next;
        if(edge) next=edge;
        if(index_<input_.size() && (!next || input_[index_].cycle<=*next))
            next=input_[index_].cycle;
        return next;
    }
    template<class Sink> struct TimedSink {
        Sink& sink; uint64_t cycle;
        void tx(unsigned channel,bool level){sink.tx(channel,cycle,level);}
        void clock(unsigned channel,bool level){sink.clock(channel,cycle,level);}
        bool dma_request(unsigned vector){return sink.dma_request(vector,cycle);}
    };
    template<class Sink> struct Owner {
        SciCycleTimeline& timeline; Sink& sink;
        std::optional<uint64_t> next_event() const {return timeline.deadline();}
        void service(uint64_t cycle) {
            auto& t=timeline;
            if(t.index_<t.input_.size() && t.input_[t.index_].cycle==cycle) {
                const auto e=t.input_[t.index_];
                t.sci_.write_rx(e.channel,e.level,cycle);
                ++t.index_;
                return;
            }
            TimedSink<Sink> bound{sink,cycle};
            for(unsigned channel=0;channel<2;++channel) {
                if(t.sci_.next_event(channel)==cycle) {
                    t.sci_.service_one(channel,cycle,bound);
                    return;
                }
            }
            throw std::logic_error("SCI timeline event disappeared");
        }
    };
    H83002SciMmio& sci_;
    std::span<const RxEvent> input_;
    std::size_t index_=0;
    CycleDispatcher dispatcher_;
};
} // namespace yamanix
