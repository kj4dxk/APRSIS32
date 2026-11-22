---
title: "Why won't APRSISCE/32 Transmit?"
---

# Why won't APRSISCE/32 Transmit?

[Source wiki page](https://aprsisce.wikidot.com/doc:transmitting) · [Archived HTML](../archive/pages/doc_transmitting--9cf5ebfffb36.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Why won't APRSISCE/32 Transmit?** |

|  |
| --- |
| With the basic assumption that your TNC/Radio configuration has everything wired and configured correctly to actually trigger PTT and key the transmitter (be careful of the squelched audio versus carrier detect setting (INTERNAL vs SOFTWARE on a KPC or LEVEL vs TONES on a TT4), the following settings should be checked in APRSISCE/32.  Is Enables / Ports / <YourPortName> checked?  A port that isn't enabled, won't transmit or receive.  Do you have Xmit Enable checked on Configure / Ports / <YourPortName>?  Unchecking this will disable all transmissions on that port. Transmitting the various other types of packets (like Messages) is also individually controlled here.  Do you have Beaconing enabled on Configure / Ports / <YourPortName>?  Unchecking this will not allow normal position beacons to be transmitted on that port.  Do you NOT have Enables / Ports / Receive-Only checked?  Checking this will disable transmissions on ALL ports. This is provided as an easy way for licensees that can only transmit from an attended station to quickly switch between Receive-Only and Transmitting.  Do you have Enables / Ports / All checked?  Unchecking this will disable ALL RF Ports instead of requiring each port to be individually disabled. The port itself must still be enabled, but All must also be enabled for ANY port to work. |

|  |
| --- |
|  |
