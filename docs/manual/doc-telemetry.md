---
title: "Telemetry"
---

# Telemetry

First, you can view two of my station's telemetry at:

[APRS.FI KJ4ERJ-1](http://aprs.fi/telemetry/KJ4ERJ-1?range=day)  
[APRS.FI KJ4ERJ-12](http://aprs.fi/telemetry/KJ4ERJ-12?range=day)

KJ4ERJ-1 is APRSIS32 as my HF IGate talking to an NMEA Bluetooth GPS from my Dell laptop  
KJ4ERJ-12 is APRSISCE on my AT&T Tilt phone (at least, as of 2010/03/06)

As you can see, there are three telemetry values and three bits. They are:

Battery: Graphs the battery percentage. APRSISCE sends an update on each transition notification from Windows Mobile. APRSIS32 polls the battery periodically to generate telemetry (and graph the battery bar on the right edge of the screen). (Yes, the battery IS bad in KJ4ERJ-1, at least, Windows 7 says it is!)

Charging/AC: 2% is running on battery, 48% is running on A/C, 95% is running on A/C and charging. I chose those numbers because 0, 50, 100 was hard to see on the lines at aprs.fi.

GPS+Sat: Tracks the GPS Enable/Disable and satellite in use count. 2=Disabled, 50=Enabled, >50 = 50+SatellitesInUse\*5 max 95)

Bits show A/C connected, Charging or not, and GPS enabled.

So, how did aprs.fi know about these categories? I'm glad you asked! APRSISCE/32 send the following APRS messages addressed to itself when it thinks it needs to:

"PARM.Battery,Charging/AC,GPS+Sat,A4,A5,A/C,Charging,GPS,B4,B5,B6,B7,B8", /\* Channel names \*/  
"UNIT.Percent,Charge/On/Off,Sats/On/Off,N/A,N/A,On,Yes,On,N/A,N/A,N/A,N/A,N/A",/\* Units \*/  
"EQNS.0,1,0,0,1,0,0,1,0,0,1,0,0,1,0", /\* (a\*v^2 + b\*v + c) 5 times! \*/  
"BITS.11111111,Battery State Tracking" /\* Bit sense and project name \*/

APRSISCE for Windows Mobile adds Current and Phone Signal as 4 and 5:

"PARM.Battery,Charging/AC,GPS+Sat,Current,Phone Signal,A/C,Charging,GPS,B4,B5,B6,B7,B8", /\* Channel names \*/  
"UNIT.Percent,Charge/On/Off,Sats/On/Off,mA,Percent,On,Yes,On,N/A,N/A,N/A,N/A,N/A",/\* Units \*/  
"EQNS.0,1,0,0,1,0,0,1,0,0,1,-500,0,1,0", /\* (a\*v^2 + b\*v + c) 5 times! \*/  
"BITS.11111111,Battery State Tracking" }; /\* Bit sense and project name \*/

It does this once per telemetry format change and keeps track of what telemetry version it last transmitted in the XML file under <TelemetryDefined>. Telemetry is transmitted based on <Telemetry.Interval> as expressed in minutes. If your station doesn't have the proper labels on your telemetry page, or if you'd like telemetry to send more (or less) often, edit the config file and change these values. Note that APRSISCE is still going to send Telemetry on every battery state change (and cellular coverage change in WinMo), regardless of the value of the <Telemetry.Interval>. There is a <Telemetry.MinTime> parameter that restricts Telemetry transmissions to at most one per configured MinTime (default 15 seconds).

[TT4 Telemetry](tt4-telemetry.md)
