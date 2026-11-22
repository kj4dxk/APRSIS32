---
title: "Linux"
---

# Linux

For connecting a Serial TNC to APRSISCE/32 running in Wine or Crossover, you must add an entry to the Windows "Registry" (system.reg) for the following, in addition to the usual setting up of symlinks to the physical ports.

[HARDWARE\\DEVICEMAP\\SERIALCOMM] 1131331688  
"COM1"="COM1"  
"COM2"="COM2"
