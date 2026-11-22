---
title: "Altitude Range Communication Circles"
---

# Altitude Range Communication Circles

When a Balloon launch is detected, well any altitude-transmitting station above your minimum altitude setting, a communication range (footprint) circle may be drawn using the following formula Range-in-miles = 1.23\*sqrt(Height-in-feet). This is a simplified (but good enough) formula that assumes the earth is a sphere (even though the earth is actually ellipsoid which you can read all about at [Line of sight propagation](http://en.wikipedia.org/wiki/Line-of-sight_propagation)).

As the altitude of the balloon increases the range circle will expand. Stations within the circle have a good chance of receiving APRS transmissions from the high altitude station.  
The range of the circle is also displayed as part of the label in the format altitude~range

The range of altitude that enables the range calculation and footprint circle display is specified under the configure / screen / labels / footprint menu in conjunction with Screen / Labels / Footprint which can toggle the circles on and off. If you have rotating colors enabled each range circle will be drawn in the same color as the owning station's track.

One nice effect of using footprint circles is to make it easier to see how high a balloon is from the size of a circle without having to zoom in to see the label. Eventually, the altitude label itself may include the calculated range, although probably not the labels of the track points.

**Any** station above the min altitude set for the range circles will show with a circle, the setting is not limited to balloons, but the function was added to allow communication range circles for Amateur Radio High Altitude Balloons (ARHAB) and/or high-flying, APRS-tracking airplanes.
