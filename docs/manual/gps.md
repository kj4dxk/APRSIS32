---
title: "GPS"
---

# GPS

One of the main pieces of an Automatic Packet Reporting System is the ability for the station to determine it's location automatically. This is the job of the Global Positioning System Receiver. The GPS uses signals from a constellation of GPS satellites which are constantly orbiting the globe. Using sophisticated timing routines, it is possible to determine a three dimensional position fix based on radio signals received by the GPSr.

The position fix calculated by the GPSr is passed on to the program as a latitude, longitude, altitude, along with some other information such as course and speed. All of this information is used by the program to determine where it is located, and what maps to display on screen. This information is also used by the Genius routine to determine when it is necessary to send out an updated location report to the APRS network.

There are a number of screen elements that contain information from the GPS unit.

**GPS Satellite Signal Level**  
Color code from Red meaning no signal through yellow to Green meaning good signal  
**Number of Satellites**  
The number of satellites that the receiver believes it should be able to see at the bottom bar, and the Number of satellites used in the solution on top of that (Note NEMA has a maximum of 12 the GPS may be using more).  
**GPS Fix Type**  
This display has 4 options **No Fix** Not enough satellites in view for a solution. **2D** Enough satellites for an aproximate location (No Altitude) **3D** Enough Satellites for an accurate location and altitude. **3D(D)** Differential calculation for more accurate location. Also the colour and length of Bar indicate the GPS accuracy of location (DOP)  
**Heading**  
The direction you are moving in as compass points  
**Speed**  
Your speed in the unit format selected  
**Time**  
Time in UTC  
**DOP Display**  
The V and H left at bottom. Vertical Dilution of Precision (VDOP) and Horizontal Dilution of Precision (HDOP). Small Red bars the GPS is not determining you location accurately, Long Green Bars GPS is Determining location much more accurately.
