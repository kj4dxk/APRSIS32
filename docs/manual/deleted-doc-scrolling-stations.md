---
title: "Scrolling Stations"
---

# Scrolling Stations

The Scrolling Station window shows stations (or objects) from which packets were recently received. This window communicates many things about the station as follows.

Callsign-SSID - This one's pretty obvious, except it can be strange for object IDs.

\* - A \* after the callsign indicates that the packet was received from RF (AGW, KISS or TEXT ports)  
\* - A # after the callsign Received inside a third-party packet from RF  
\* - A @ indicates that you have digipeated the station  
\* - A ^ indicates that you have sent from IS to RF

n - If there's a number after the symbol, it counts how many times the packet was received (total of IS and RF digipeats).

Symbol - current station symbol including all spec-specified assumptions based on:  
1) Explicit statement in packet  
2) Weather data is a weather symbol  
3) GPSxxx specification in ToCall (Page 92/103 of aprs101.pdf)  
4) Assumption based on -SSID value (Page 93/103 of aprs101.pdf)  
5) APRSISCE/32 also uses ~ for Telemetry data if no other symbol was previously known

Colors: - This is where it gets interesting…

Black on White - No position known for the station

Red-Yellow-Green - Scaled color based on proximity (Red = close, Green = far)  
The scale of this conversion is the circle on the map. If the station is within the circle, the color will vary from Green (outer edge) to Red (dead center) based on how close the station is to the coordinates at the center of the screen.

Reversed Red-Yellow-Green - Scaled as above, but reverse video for callsigns that seem to be in your extra filter as a "buddy" (b/) or "friend" (f/). This may get some false positives until I add explicit Buddy and Friend filter configuration.

"Invalid" color Background - See details in the upcoming "Invalids" trace log  
Purple - Duplicate packet  
Pink - Too Quick packet (sub second)  
Red - Too Fast packet (>20,000mph or double rolling average)  
Aqua - "Restart" packet (long story, under development)  
Gray - Unknown Invalid (should never be seen)

Yellow background - Other APRSISCE/32 users have a yellow background unless something else overrides the color (like buddy).

I think that's all the colors that can show up there, but if you see something else, send a question in and ask!
