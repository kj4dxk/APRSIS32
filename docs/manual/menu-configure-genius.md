---
title: "Configure Genius Dialog"
---

# Configure Genius Dialog

APRSISCE/32 extends the [Smartbeaconing](http://www.hamhud.net/hh2/smartbeacon.html) concept to a whole new level made possible by running the client on platforms more powerful than a typical PIC.

You should not need to make any changes to the GeniusBeaconing™ settings until you have run APRSISCE/32 for a while. If you don't have a GPS, then you definitely do not need to change anything here except maybe the Max Time which will be the only thing governing your beacon interval.

A good description of the GeniusBeaconing can be found in the description of the [red-dot](doc-red-dot.md) behavior.

**Min Time** - Under no circumstances will APRSISCE/32 automatically beacon in less elapsed time than the Min Time seconds.

**Max Time** - If nothing else triggers a beacon, APRSISCE/32 will transmit one every Max Time minutes. If there is no GPS input, or if the GPS is disabled, then beacons (if enabled) will be transmitted at this rate.

**Time Only** - If this is checked, then only the time parameters are used and the remainder of the settings on this dialog are ignored.

**Start/Stop** - If checked, APRSISCE/32 will transmit a position beacon whenever you speed drops to zero or leaves zero, governed by the Min Time of course. If you frequently drive in start/stop traffic, you may want to uncheck this to avoid frequent (Min Time) beaconing.

**Heading Change** - If your heading changes by more than this many degrees since the last beacon, a new beacon will be triggered. If you have problems with frequent beaconing due to a drifting GPS, simply set this value to 180. The Forecast Error will still provide very accurate, predictable beacons. (This parameter may be removed in some future version because Forecast Error works so well).

**Forecast Error** - This is the "Genius" of GeniusBeaconing(tm). If an outside observer were to calculate your position based on the speed, heading, and elapsed time since your last beacon and your actual current position is more than "Forecast Error" from that position, a beacon will be transmitted. The magnitude and direction of the error between a forecast from your last beacon and your actual current position is shown by the position of the [red-dot](doc-red-dot.md) relative to the range circle. When the [red-dot](doc-red-dot.md) touches the circle, the Forecast Error has been exceeded and a beacon will be triggered (if allowed by Min Time).

**Max Distance** - If you have "made good" more than this distance since your last beacon, a new beacon will be triggered. Note that this is expressed in 1/10 mile increments, so remember to multiply by 10.

**Accept / Cancel** - Obvious operation here (I hope).
