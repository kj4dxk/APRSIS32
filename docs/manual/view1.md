---
title: "View1"
---

# View1

The View Menu allows various filters on what stations will actually be displayed on the map. View settings are not retained across restarts but will revert back to ALL to avoid wondering why you don't see any stations when you return to APRSISCE/32 after having been away for a few days.

**ALL**  
Retains any individually selected options below to allow quick restoration to a filtered view, but displays all stations.

**NONE**  
Unchecks all selections on the view menu. Centered stations and ME will always be displayed.

**Ambiguous**  
Displays all stations that are transmitting beacons including position ambiguity. Non-filled circles are drawn around such stations to indicate that they may more may not be precisely at their displayed position.

**Buddies**  
Displays all stations that APRSISCE/32 considered to be a "Buddy". This includes any station matching any b/xxx or f/xxx filter (including wildcards) and also any station currently centered in any APRSISCE/32 MultiTrack window.

**Duplicates**  
Displays all stations for which suspected duplicate position packets have been received. Duplicates can occur when a portion of the APRS Infrastructure delays packet delivery for more than the default 30 second duplicate detection filter of the APRS-IS transport system. Duplicate packets can make a mobile station appear to ping-pong between new and old positions. APRSISCE/32 highlights such points in purple and does not include them in the track lines.

**Invalids**  
???

**Message-able**  
Displays all stations whose transmissions, either by APRS packet type or application identifier, indicate that they support APRS messaging. If you have ever sent messages to or received messages from a station, it will also be included in the display. These stations will offer "Message" on their popup menu to provide a direct way to open a new [chat](chat.md) with that station.

**Nicknamed**  
Displays all stations for which you have defined a Nickname. See also [Screen](menu-screen.md) / Labels / Nicknames

**NWS Objects**  
Displays all NWS weather objects. These objects are either directly received via APRS or are internally generated from NWS alerts in conjunction with locally defined [shapefiles](doc-shapefiles.md).

**Objects**  
Displays all objects received from APRS. Objects are typically points of interest like repeaters, hospitals, and other non-APRS stations.

**SkyWarn**  
!SN!

**Telemetry**  
Displays stations for which Telemetry has been received.

**Tracks**  
Displays stations that have moved and therefore have accumulated tracks to display.

**EchoLinks**

**GeoCaches**

**Platforms**  
Allows selection of specific APRS platforms to display. Note that counts of stations determined to be of a specific platform are displayed on this menu. Some of the platforms provide additional breakdowns, but selecting any one of the detailed categories sometimes selects the entire category. More work needs to be done here (2011/04/20) to make this behave more consistently.

**RF**

* **All**
* **Direct**
* **Local**
* **Third Party**

**Symbols**  
Allows selection of specific symbols to display. The currently centered station's symbol is sorted to the top of the list to allow easy selection of other stations like the centered station. Note that counts of stations using a symbol are displayed on this menu.

**Transport**

* **RF Only**
* **RF+IS**
* **IS Only**

APRSISCE/32 attempts to determine the transports available to a given station based on the contents of received packets. This information is used to optimize some communications with stations on various platforms.

also Digipeater, IGate (these last will display stations that have been observed acting in the specified capacity regardless of what symbol they may be transmitting).
