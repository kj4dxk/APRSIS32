---
title: "TNC-KPC-3+"
---

# TNC-KPC-3+

[Source wiki page](https://aprsisce.wikidot.com/tnc-kpc-3) · [Archived HTML](../archive/pages/tnc-kpc-3--de35330391b1.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **TNC-KPC-3+** |

|  |
| --- |
| Note: The latest version(s) of the KPC-3+ firmware have been demonstrated to have a bug that causes them to delay delivery of packets out the serial port. This problem gets worse over the time that the TNC is in KISS mode. Power cycling or dropping the KPC-3+ out of KISS mode has been shown to eliminate the delay for a while. The problem occurs faster in areas with heavy APRS activity. This problem has only been observed in the 3+, not the non-plus 3. See also: <http://blog.aprs.fi/2011/03/kantronics-kpc3-considered-harmful.html>  By far, the easiest way to use the KPC-3+ is to keep your KPC\* locked into KISS mode, you can simply use the following pair of No-Operation commands. This IS the default set of <Open/CloseCmd>s for a "Simply(KISS)" port type, but it requires you to have your KPC-3+ always in KISS mode from power-up reset.  <OpenCmd>~!!0</OpenCmd>  <CloseCmd>~!!0</CloseCmd>  If you goof and configure a KISS (not Simply()) port to talk to your KISS-mode KPC-3+, the default <CloseCmd>s will knock it out of KISS mode until the next power cycle.  Note that all of the following examples require a KISS-type port to be configured in APRSISCE/32. After configuring the port, you need to close the client and edit the XML file to replace the default <Open/CloseCmd>s for the port with one set of the following. You might also want to power-cycle the KPC-3+ while the client is closed to make sure it is in a default power-up state.  Here's the Open/Close Cmd I (Eric VA2EC) use with my KPC3+  When I run APRSISCE/32, it goes in KISS mode, and when I close the software the KPC3 goes back to "cmd" mode (configured to send beacons each 1/2 hour by other configuration commands issued to the TNC manually)  For participation in a shared APRS RF network:  <OpenCmd>^M~</OpenCmd>  <OpenCmd>^M~</OpenCmd>  <OpenCmd>XFLOW ON!!0</OpenCmd>  <OpenCmd>FULLDUP OFF!!0</OpenCmd>  <OpenCmd>INT KISS!!0</OpenCmd>  <OpenCmd>RESET!!0</OpenCmd>  <CloseCmd>^192^255^192~!!0</CloseCmd>  <CloseCmd>^C^C^C~!!0</CloseCmd>  For Satellite Work: (Thanks to Kevin W7B0Z)  <OpenCmd>^M~</OpenCmd>  <OpenCmd>^M~</OpenCmd>  <OpenCmd>XFLOW OFF!!0</OpenCmd>  <OpenCmd>FULLDUP ON!!0</OpenCmd>  <OpenCmd>INT KISS!!0</OpenCmd>  <OpenCmd>RESET!!0</OpenCmd>  <CloseCmd>^192^255^192~!!0</CloseCmd>  <CloseCmd>^C^C^C~!!0</CloseCmd>  Also, if you KPC won't transmit, double check your CD (Carrier Detect) setting. If it is INTERNAL, the TNC won't transmit if it hears any audio and many radios with "data" ports emit non-squelched audio. In that mode, and for generally better digital operations, the CD setting should be SOFTWARE to only hold of transmitting while packet data is actually being decoded. (Thanks to Chris G4HYG) |

|  |
| --- |
|  |
