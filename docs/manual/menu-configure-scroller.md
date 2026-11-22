---
title: "Configure Scroller"
---

# Configure Scroller

[Source wiki page](https://aprsisce.wikidot.com/menu:configure-scroller) · [Archived HTML](../archive/pages/menu_configure-scroller--d65cf2ea5fb8.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Configure Scroller** |

|  |
| --- |
| The [scroller](scroller.md) is the list of packets that shows to the left of the map. Sometimes it's short (Wide Orientation) and sometimes it fills the left side (Narrow Orientation), but in either case, it displays packet information as it is received, governed by the options below. **Freeze On Click** - If checked, left clicking on the scroller will freeze it, preventing any new packets from being added. Subsequently clicking on any station ID will present the station popup menu for that station as well as provide a preview of the packet at that entry across the bottom of the map. Clicking outside the scroller (or right clicking on the scroller) will un-freeze it and immediately add any new packets that were received while frozen. Right-clicking on a non-frozen scroller will bring up the station popup menu immediately and allow the scroller to continue moving in the background.  **Show IGate/Digi** - Adds the IGate (APRS-IS) or Digi (RF) that is determined to have last handled the packet. Requires configuration of <PathAlias> elements in the XML configuration to eliminate locally-defined aliases. Under Windows Mobile, the station ID is replaced by the IGate/Digi ID for space considerations.  The following settings govern which packets are added to the scrolling packet log.  **Filter** - Allows entry of an APRS-IS-style filter to control which packets are added to the scroller.  **Hide NoParse** - Hides packets that failed the parser (see also the NoParse Trace Log)  **No Internals** - Hides APRSISCE/32 Internally generated packets (see also the Packets(Internal) Trace Log)  **Not Me** - Hides packets from the configured callsign, even digipeats of your own packets (I think)  **Not Mine** - Hides packets from the same base callsign, regardless of -SSID.  **RF Only** - Shows ONLY packets received from RF Ports (handy for IGates, boring on an APRS-IS-only instance)  **Show All** - Forces ALL packets to be added to the scroller. Handy to set your preferences and toggle to ALL sometimes.  Note that none of these settings are retro-active. They are only checked when new packets arrive to determine if they will be added to the scroller. Once a packet is or is not added, it will be there or not until it scrolls off the top of the list. |

|  |
| --- |
|  |
