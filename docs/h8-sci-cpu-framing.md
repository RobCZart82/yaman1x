# CPU SCI framing-error acknowledgement and recovery

Integration candidate after merged #70; no production engine or ROM changes.
The valid duplex host is extracted unchanged into a shared fixture, with optional
stop-level/start-time inputs. Existing valid-frame tests retain their defaults.
A low stop bit produces FER and ERI vector52/56 rather than RXI53/57. A synthetic
CPU ISR reads SSR, stores it in RAM, clears receive-error bits with the read latch
and executes RTE. Tests require FER alone among receive flags, correct ERI,
cleared error status, no eligible request, restored stack and completed TX.

All four TX/RX channel pairings and all 256 data bytes give 1024 cases. Every
split0..900 across each pairing gives 3604 trace/state comparison histories.
A further 512 cases (two receive channels, all good data bytes) restore RX idle
high after the bad frame, then inject a valid frame. The CPU's replacement RXI
handler stores that byte at RAM0x401; expected exception sequence is ERI then
RXI, and status/stack must be restored. The synthetic ISR replacement happens
between runs before the new external frame; no production self-modification or
firmware recovery claim is made. Initial recovery fixture omitted the idle-high
return, correctly failed all512 cases, and was fixed at the pin stimulus.

Limits: fixed asynchronous BRR=0, single framing error followed by one valid
frame. Overrun, parity, repeated break, simultaneous pending errors, in-flight
baud changes and physical AN1x tie ordering remain unverified. Firmware boot,
MIDI and audio acceptance remain open. Parent merge and final-head CI/review
are required; no deliberate multi-milestone hold.
