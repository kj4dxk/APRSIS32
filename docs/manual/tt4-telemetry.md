---
title: "TT4 Telemetry"
---

# TT4 Telemetry

[Source wiki page](https://aprsisce.wikidot.com/tt4-telemetry) · [Archived HTML](../archive/pages/tt4-telemetry--a6c0249a6c1d.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **TT4 Telemetry** |

|  |
| --- |
| Using findu.com's message sending facility at <http://www.findu.com/cgi-bin/entermsg.cgi>  I sent the following APRS messages from/to the unit's callsign (KJ4ERJ-7 in my case):  PARM.BattVin,Temp,JP5,JP4,JP3,PC7,PC6,PC5,PC4,PC3,P2,P1,P0  UNIT.Volts,degF,Volts,Volts,Volts,on,on,on,on,on,off,off,off  EQNS.0,.109,0,0,3.528,-459.4,0,.0196,0,0,.0196,0,0,.0196,0  BITS.11111000,KJ4ERJ-7 TinyTrak4  PARM sets the names of the various parameters  UNIT sets the units of the various parameters  EQNS sets the quadratic coefficients for the UNIT conversion  BITS sets the UNIT state of the bit and names the project  You may need different coefficients depending on if you're using the higher resolution settings. Mine are on low resolution and seem to track some semblance of reality (the TT4 is on a West-facing window sill in Palm Bay, Florida).  You can read more about setting up telemetry at  <http://www.aprs.net/vm/DOS/TELEMTRY.HTM>  as well as in Chapter 13 of the APRS Spec from  <ftp://ftp.tapr.org/aprssig/aprsspec/spec/aprs101/APRS101.pdf> |

|  |
| --- |
|  |
