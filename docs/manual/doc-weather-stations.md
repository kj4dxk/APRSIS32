---
title: "Weather Stations"
---

# Weather Stations

[Source wiki page](https://aprsisce.wikidot.com/doc:weather-stations) · [Archived HTML](../archive/pages/doc_weather-stations--a0cd1d1f904e.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Weather Stations** |

|  |
| --- |
| This space to be updated eventually!  New Configure / Objects / New Weather - Prompts for a file name and location and otherwise works like a new object. If the weather file cannot be accessed during a poll, the object automatically disables itself with a message in the Trace Log which is also brought to the front.  If your weather software creates and updates a file that looks like this [Wxnow.txt](https://cumuluswiki.org/a/Wxnow.txt), then APRSISCE/32 should be able to monitor it, format proper APRS packets, and transmit it over APRS-IS, RF, or both.  1) Pan and Zoom to where the weather station is (or just Screen / Follow / ME)  2) Configure / Objects / New Weather  3) Yes to create at screen center  4) Navigate to where the WxNow.txt file will be refreshed (must have one there)  5) Check Enabled, give it a station-SSID  6) If you want it to be a true station, check Station, otherwise it will be a weather object owned by your APRSISCE/32 callsign-SSID  7) Select a timestamp format, update interval, via IS, RF or both  8) If RF, set a path.  9) Accept and you're running!  Those of you using Davis Weatherlink V5.6 or above might like to try the following DLL for generating wxnow.txt files [WXnow addon](http://www.bpsmicro.com/Files/WXNOW_Generator.htm)  [CWOP](port-cwop.md)  [Tempest setup](weather-tempest.md) |

|  |
| --- |
|  |
