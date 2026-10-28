// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Execution ordering adapted from pinned MAME h8.cpp execute_run.
// See research/h8-execution-runner-provenance.json.
#pragma once
#include "h8_execution_budget.h"
#include <cstddef>

namespace yamanix {
// Owner: update(cycle) returns next absolute deadline (zero = none),
// has_partial() is a pure query, full(budget), partial(budget).
// full owns upstream PPC/debug-hook bookkeeping before dispatching a handler.
// Handlers own instruction state and checked cycle charges. No reentry.
class H8ExecutionRunner {
public:
    H8ExecutionRunner()=default;
    H8ExecutionRunner(const H8ExecutionRunner&)=delete;
    H8ExecutionRunner& operator=(const H8ExecutionRunner&)=delete;
    int& counter() { return budget_.counter(); }
    int debt() const { return budget_.debt(); }
    bool failed() const { return failed_; }

    // Callback limit is a fail-stop guard, NOT a resumable instruction budget.
    // On any execution failure discard runner AND owner (no rollback).
    template<class Owner>
    std::size_t run(uint64_t start, int cycles, std::size_t limit, Owner& owner) {
        if(busy_ || failed_) throw std::logic_error("Unavailable H8 execution runner");
        budget_.begin_slice(start,cycles); // Reject invalid caller input before callbacks.
        busy_=true;
        std::size_t calls=0;
        auto call=[&](auto action) {
            if(calls==limit) throw std::runtime_error("H8 execution callback limit");
            ++calls; action();
        };
        auto update=[&](uint64_t cycle) {
            call([&]{budget_.recompute_deadline(owner.update(cycle));});
        };
        auto catch_up=[&] {
            while(budget_.event_due()) update(budget_.due_cycle());
        };
        auto partial=[&] {
            if(budget_.can_execute() && owner.has_partial())
                call([&]{owner.partial(budget_);});
        };
        try {
            update(start);
            budget_.apply_carry();
            catch_up();
            partial();
            while(budget_.can_execute()) {
                while(budget_.full_step_allowed())
                    call([&]{owner.full(budget_);});
                if(budget_.can_execute()) catch_up();
                partial();
            }
            budget_.finish_slice();
            busy_=false;
            return calls;
        } catch(...) { failed_=true; busy_=false; throw; }
    }
private:
    H8ExecutionBudget budget_;
    bool busy_=false,failed_=false;
};
} // namespace yamanix
