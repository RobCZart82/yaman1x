# CPU-configured SCI transmit and deadline refresh

Candidate after #68 absolute bus completion time. No original ROM input.
After each successful CPU bus operation, an optional pure host
next_bus_event() query republishes the next absolute deadline to the execution
budget and the current generated-handler boundary. This is necessary because a
CPU write can start an idle UART whose event was absent at slice entry.
Zero means no event; existing untimed hosts without the query are unchanged.
Exceptions stop the runner with committed bus effects retained. The query must
not re-enter, mutate peripherals or service events. It must report the earliest
pending event across the host's peripherals. A stuck past deadline remains
subject to the existing execution callback limit.

The synthetic transmit host uses bus_cycle to service due SCI events through
the completion timestamp before its memory callback, with channel0 before
channel1 for equal deadlines. CPU writes configure BRR=0 and SCR=TE, write TDR,
read SSR and clear TDRE. No preconfiguration shortcut is used. Expected output
is start, eight LSB-first bits, stop and a final idle callback, each 32 cycles
apart. TDRE/TEND must both be set at completion. All 256 bytes on each channel
and 1602 split positions compare wire timestamps/levels, bus traces, launch time
and CPU snapshots. The extra final idle callback was explicitly observed in the
existing UART model and included in the independent expected sequence.

The test program uses separate address and data registers; the initial fixture
incorrectly aliased r0l with its ER0 address and was corrected before validation.
No production CPU behavior was changed to accommodate the fixture.

Scope: synthetic split instruction/data host, one transmitted frame, fixed
asynchronous BRR=0 profile. No concurrent RX/TX or physical AN1x tie-order proof,
complete memory map, firmware boot, MIDI or audio acceptance. Generated method
bodies are unchanged. Parent merge and final-head CI/review remain required.
