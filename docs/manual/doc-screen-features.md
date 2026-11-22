---
title: "Transmit Pressure"
---

# Transmit Pressure

Oh, one other thing. APRSISCE puts a line of text below the "APRS OK" line that shows the pressure to transmit an updated position. Can you tell me what your's is saying? It will be something like:

Dist nn% - Traveled Percentage of Max Distance  
Fore nn% - Forecasted Percentage of Max Distance  
Proj nn% - Percentage of Forecast Error  
Head nn% - Percentage of Heading Change  
Time hh:mm:ss - Count down from Max Time  
STOP - Stopped having been moving  
START - Started having been stopped  
FORCE - Clicked the Transmit button  
LostFix - GPS Lost a Fix  
GotFix - GPS Acquired a Fix

It DOES say "APRS OK", right? If not, then you're having trouble getting connected to an APRS-IS server.

There are also three colored bars below that text that indicate (top to bottom):  
**Transmit Pressure** increasing left to right (Transmits when it gets to the right, see also last line)  
**APRS-IS Quiet Time** - Increasing left to right, reconnects if it ever gets to the right  
**Transmit Throttle** decreasing right to left (Counts down Min Time after beacon)

These bars aren't very wide, but the normal behavior is that the top one will crawl to the right and a beacon will be sent when it hits the right edge. At that time, the bottom bar will jump immediately to the right and start shrinking to the left as the Min Time elapses.

The center bar will grow from the left until a new packet is received from APRS-IS. If it ever gets to the right edge (set by Quiet Time which should be 60 seconds), the APRS-IS connection is dropped and a new connection is established. APRS-IS servers send out a heartbeat every 20-30 seconds if no packet has been sent, so this bar should just hover in the left half of the display, growing longer and then disappearing only to start over. If your filter has lots of stations coming in, you might never actually see this bar (although you station list will be scrolling fast!).
