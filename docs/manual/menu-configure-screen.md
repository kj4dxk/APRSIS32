---
title: "Configure Screen"
---

# Configure Screen

Configuring options for the screen display. (partial work)

**Label**  
Covers the option of what labels are displayed and how, Callsigns and other info

* **Allow Overlap**  
  This option decides if you would like labels to overwrite other labels or would like them to be spaced away from the station or object if they will overwrite another stations information.

* **CallSign**  
  **Not Me** Selecting turns of the me Label for the station  
  **Not Mine** Turns of the callsign label for every SSID of your callsign

* **Footprint**  
  Allows the setting off the minimum and maximum altitudes for triggering [Balloon Range circles](doc-alt-range.md) to disable the circles set the minimum altitude to a high level

* **GeoCache**  
  Select the various labels to be displayed if you have uploaded a Geocache gpx file via the view menu (not complete)

* **Max Visible**  
  The maximum number of station/object labels visible on screen. This option has a performance hit on screen display updates.

**Altitude**  
Selecting this option displays the altitude of a station if the station or object is broadcasting one. (the unit used is configurable under the configure/ metric menu option)

**Battery**  
Displays the battery level of a station that has a battery option. Shown as a bar far right on the screen  
**Circle**  
Enables or disables the range circle, used in conjunction with the Red Dot in GeniusBeaconing™ info here [configure-genius](menu-configure-genius.md)

**CrossHairs**

* **Always** Displays a crosshair on the screen allowing more accurate selection of a location for placing objects or setting your location if you have no GPS attached.
* **Never** No Crosshairs displayed.
* **Timed** Displays a crosshair on the screen for a limited time allowing more accurate selection of a location for placing objects or setting your location if you have no GPS attached.

**Date/Time**  
Changes what information is displayed in the bottom right corner.

* **None** No information
* **Local** Display Local Time/Date
* **GMT/UTC** Display Date/Time in GMT/UTC
* **GPS Only** will display the Time/Date as received from the GPS loss of GPS will result in no time being displayed.
* **Performance** Display software performance figures
* **Date Only** only display the date and no time.
* **Default** resets to displaying Time and Date, alternating.
* **Time Only** only display the date, no time

**GridSquare**  
Selecting this option enables the display of your location as a gridsquare at the bottom left of the screen

**Lat/Lon**  
Selecting this option enables the display of your location as a Latitude and Longitude at the bottom left of the screen

**Orientation**  
Chooses a screen layout based on the width of the map area. Wide puts more information along the top while Narrow puts information down the side.

* **Wide** Puts most information along the top of the window providing a wider map area, but the scroller is short.
* **Narrow** Stretches the scroller down the left edge of the screen causing a narrower map, but more packets visible.
* **Automatic** Chooses an orientation based on the height to width ratio of the screen - Nice for AT&T Tilts

**RedDot**  
Enables or disable the display of the Red Dot more information here [red-dot](doc-red-dot.md)

**Satellites (GPS)**  
Selecting this option if you have a GPS device attached shows data about the GPS Satellites in view. The positions of the satellites will show on the main map as if you were to look up facing north. These continue to be relative to your location, even if you move the map elsewhere.  
2 bars will show across the bottom of the screen, the top bar show the satellites in use for tracking. The bottom bar will show the satellites that should be in view and that are being searched for.  
The colours vary from red to green depending on signal quality, red being not received through to green a good signal. (with a GPS attached the Operating mode always shows in the right hand bottom corner of the screen

**Speed font Size**  
This option allows for changing the font size of your speed at top left on the screen, the best option for fixed stations is 0, which will turn the speed display off. This applies to stations with a GPS attached

**Tracks**  
The Tracks cascade allows setting of line style (color, width, and length) preferences for both Followed and Other (non-followed) stations. For complete details of track customization options, please read [Track Colors](track-colors.md).
