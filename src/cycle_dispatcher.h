#pragma once
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace yamanix {
// Project-specific scheduling primitive; no CPU or hardware tie policy.
// Owner: optional<uint64_t> next_event(), void service(uint64_t cycle).
// A service call handles one owner-defined event at exactly that cycle.
class CycleDispatcher {
public:
    struct Result { uint64_t cycle; std::size_t events; bool complete; };
    explicit CycleDispatcher(uint64_t initial_cycle=0) : now_(initial_cycle) {}
    CycleDispatcher(const CycleDispatcher&)=delete;
    CycleDispatcher& operator=(const CycleDispatcher&)=delete;
    uint64_t cycle() const { return now_; }
    bool failed() const { return failed_; }

    // Inclusive horizon. Budget exhaustion leaves time at the last event;
    // callers may resume with the same horizon. Empty time needs no budget.
    // Owner callbacks may not re-enter this dispatcher. After a callback or
    // deadline failure, discard dispatcher AND owner: no rollback is possible.
    template<class Owner>
    Result advance_to(uint64_t horizon, std::size_t budget, Owner& owner) {
        if (busy_) throw std::logic_error("Reentrant cycle dispatch");
        if (failed_) throw std::logic_error("Failed cycle dispatcher");
        if (horizon<now_) throw std::invalid_argument("Cycle horizon moves backwards");
        busy_=true;
        std::size_t count=0;
        try {
            for (;;) {
                const std::optional<uint64_t> event=owner.next_event();
                if (event && *event<now_)
                    throw std::logic_error("Owner deadline moves backwards");
                if (!event || *event>horizon) {
                    now_=horizon; busy_=false;
                    return {now_,count,true};
                }
                if (count==budget) {
                    busy_=false;
                    return {now_,count,false};
                }
                now_=*event;
                owner.service(now_);
                ++count;
            }
        } catch (...) {
            failed_=true; busy_=false;
            throw;
        }
    }
private:
    uint64_t now_;
    bool busy_=false, failed_=false;
};
} // namespace yamanix
