// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Budget/deadline arithmetic adapted from pinned MAME h8.cpp execute_run /
// recompute_bcount. Scope: research/h8-execution-budget-provenance.json.
#pragma once
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace yamanix {
// CPU-owned arithmetic only: no instruction decoder, callbacks or scheduler.
// Order: begin_slice, recompute_deadline (before carry), apply_carry, owner
// full/partial execution plus deadline updates, finish_slice. Counter storage
// stays stable for H8hBusAccess; only its CPU owner may mutate it.
class H8ExecutionBudget {
public:
    H8ExecutionBudget()=default;
    H8ExecutionBudget(const H8ExecutionBudget&)=delete;
    H8ExecutionBudget& operator=(const H8ExecutionBudget&)=delete;
    void begin_slice(uint64_t start, int cycles) {
        if(phase_!=Phase::idle) throw std::logic_error("H8 slice already open");
        if(cycles<0) throw std::invalid_argument("Negative H8 slice budget");
        if(start>std::numeric_limits<uint64_t>::max()-uint64_t(cycles))
            throw std::overflow_error("H8 slice end overflow");
        if(started_ && start!=end_) throw std::invalid_argument("Noncontiguous H8 slice");
        end_=start+uint64_t(cycles); remaining_=cycles;
        boundary_=0; deadline_set_=false; phase_=Phase::before_carry; started_=true;
    }
    // Zero is upstream's no-event sentinel. End-equal deadlines are excluded
    // from this slice. Already-due deadlines are retained for owner catch-up.
    void recompute_deadline(uint64_t event) {
        require_open();
        if(!event || event>=end_) boundary_=0;
        else {
            const auto distance=end_-event;
            if(distance>uint64_t(std::numeric_limits<int>::max()))
                throw std::overflow_error("H8 event boundary exceeds counter range");
            boundary_=int(distance);
        }
        deadline_set_=true;
    }
    void apply_carry() {
        if(phase_!=Phase::before_carry || !deadline_set_ || remaining_<0)
            throw std::logic_error("H8 carry requires initial deadline update");
        remaining_-=debt_; // Both operands nonnegative int; no signed overflow.
        if(remaining_<0) {debt_=-remaining_;remaining_=0;}
        else debt_=0;
        phase_=Phase::executing;
    }
    void finish_slice() {
        require_executing();
        if(remaining_>0) throw std::logic_error("H8 slice still has execution budget");
        if(remaining_==std::numeric_limits<int>::min())
            throw std::overflow_error("H8 cycle debt cannot be represented");
        if(remaining_<0) {debt_=-remaining_;remaining_=0;}
        phase_=Phase::idle;
    }
    // Only the execution owner / checked bus helper may change this counter.
    int& counter() { return remaining_; }
    int remaining() const { return remaining_; }
    int debt() const { return debt_; }
    int boundary() const { return boundary_; }
    uint64_t slice_end() const { return end_; }
    bool event_due() const {require_executing();return boundary_ && remaining_<=boundary_;}
    uint64_t due_cycle() const {
        if(!event_due()) throw std::logic_error("No due H8 peripheral event");
        return end_-uint64_t(boundary_);
    }
    bool full_step_allowed() const {require_executing();return remaining_>boundary_;}
    bool can_execute() const {require_executing();return remaining_>0;}
private:
    enum class Phase {idle,before_carry,executing};
    void require_open() const {
        if(phase_==Phase::idle) throw std::logic_error("No open H8 slice");
    }
    void require_executing() const {
        if(phase_!=Phase::executing) throw std::logic_error("H8 carry not applied");
    }
    int remaining_=0, debt_=0, boundary_=0;
    uint64_t end_=0;
    bool started_=false, deadline_set_=false;
    Phase phase_=Phase::idle;
};
} // namespace yamanix
