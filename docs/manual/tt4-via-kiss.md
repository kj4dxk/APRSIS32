---
title: "TT4 via KISS"
---

# TT4 via KISS

[Source wiki page](https://aprsisce.wikidot.com/tt4-via-kiss) · [Archived HTML](../archive/pages/tt4-via-kiss--99f04f4c55a2.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **TT4 via KISS** |

|  |
| --- |
| Put the TT4 into KISS mode, note the baudrate that you have it set up for. In APRSISCE/32, click Configure / Port / Simply(KISS) / and then select COMn. Pick the COM port on the left and baud rate on the right. Click OK, restart the client (this will get fixed in a later version), and then select Enables / KISS Enabled.  Stations received over RF show up in the scrolling stations list with an \* beside them. Any number following the \* indicates that the same packet was received multiple times (typically from one or more digis)  All beacons and messages are sent over all active ports. Messages heard on the -IS whose recipient was "recently" heard "locally" on the RF are packaged into a 3rd party packet and transmitted.  You can filter the map to show only those stations received on RF either Local, Direct, or All via the View / RF menu.  Sending ?ARPSD or ?APRSL to your APRSIS32 callsign-SSID via APRS will send back a list of those stations heard Direct or Local respectively.  Note, if you set PKTOCOM, the TT4 will send all generated position updates directly to APRSISCE/32. I also plan to support the GKRELAY method of receiving NMEA GPS data over a configurable KISS sub-channel, but it is not yet done. (This may have been done I need to check G6UIM)  Gary, WG5L, would like to add: ATTENTION ALL FORMER TT3 OWNERS: YOU HAVE TO LEAVE THE NULL MODEM CABLE INLINE WITH THE TT4 WHEN HOOKING IT TO A COMPUTER FOR KISS OPERATIONS!  Another lesson from Gary is "When using AMODE KISS without a GPS…. DO NOT set BMODE to either KISS or TEXT when setting AMODE to KISS. Set it or keep it at GPS, otherwise one will get" a loopback of received packets back out over the RF with a new header added by the TT4. This is caused if you use "a F-F null cable that shorts pins 7 and 8 and so if one has BMODE set to either KISS or TEXT, it will repeat what it hears." |

|  |
| --- |
|  |
