---
title: "Screen Menu"
---

# Screen Menu

The Screen menu provides access to the more-frequently changed screen and display features of APRSISCE/32. See also [Configure](menu-configure.md) / Screen for additional features.

**Dead Reckoning**  
Toggles the display of the extrapolated position of a moving station based on their last known speed and heading. Up to three extrapolated positions may be displayed based on the selections in the Screen / Labels / Speed menu. The extrapolated station symbol is connected to the last received position by a line that changes from green to red depending on the time since the last beacon was received. Normal stations will turn fully red and cease being displayed 5 minutes after their last position update. NWS stations received with a speed and heading will continue to dead reckon until their expiration time is reached.

**Tracks**  
Toggles the display of historically captured track lines for stations that have moved. Actual track transmission points will be highlighted for the currently Centered station.

**Direction Finding**  
Enables the display of direction finding circles

**Follow**

* **Find**  
  Allows typing a station name into the box and moving the map to their location. The Drop down menu shows all the received stations, if you select the drop down and start typing a station  
  name the drop down, will move through the list narrowing down the selection. Selecting find will move to the station on the map. Multitrack will open a new window centred on the station.  
  If the station has not been received the option to wait for a transmission from the station will appear.

* **Centre**  
  This option will centre the map on the last selected station. Or your own station if no station has been previously selected.

* **ME**  
  This option will move the map to centre your own station on screen

* **Locked**  
  Will lock the screen to follow the currently selected station

**Labels**

* **Altitude**  
  Option to display the altitude of a station if the station broadcasts it. the display unit can be configured under [Configure Metric](menu-configure-metric.md)

* **Ambiguity**  
  Shows a circle around stations with position ambiguity, the circle shows the area that the station should be within. Ambiguity can be set [Configure Beacon Menu](menu-configure-beacon.md)

* **Callsign**  
  Select the display of callsigns for stations

* **Nicknames**  
  Nicknames for stations that provide an alternate label (possibly blank) as well as an optional symbol and comment override.

* **NWS Calls**

* **Speed**  
  Selects to display the speed of a station. Beaconed displays the speed actually beaconed by the station, if the station is moving but not beaconing a speed calculate will attempt to display the speed

* **Weather**

**PHG Range Circles**  
Enables the display of full (fixed station) or half (mobile station) scale range circles as well as setting the desired opacity of said circles. APRSISCE/32 displays a cardoid shape to indicate any directionality specified by the PHG values. PHG circles are colored green (full) or red (half).

**Preferred**  
Allows a the current view to be Saved or the previously saved view to be restored. The preferred settings saves the map center and zoom level. Direct access to the saved perferred view is also available by double-clicking the [Scale](pane-scale.md) number.

**Tile Set**  
Provides direct access to select what map will be used as a background. See also [Configure](menu-configure.md) / Map / Tile Sets for more information.
