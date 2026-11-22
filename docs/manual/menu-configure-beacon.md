---
title: "Configure Beacon Menu"
---

# Configure Beacon Menu

The Configure / Beacon menu provides direct access to the various allowed components and contents of a position beacon.

**After Transmit** - In a semi-mobile (not fixed) GPS-less installation, it may be desirable to not transmit a position beacon on initial startup until the last known position can be confirmed and possibly corrected. If this option is checked, APRSISCE/32 will not transmit a position beacon until the Transmit menu option has been manually activated. Given that the Transmit option allows the user to move "ME" to the center of the screen, this allows the actual current position to be verified and/or changed before any position beacons are sent.

**Timestamp** - If checked, a timestamp is included in each position packet. The default timestamp is ddhhmm zulu unless HHMMSS is also checked.

**HHMMSS** - Modifies the timestamp to use the hhmmss format. Including this timestamp on RF beacons can allow easy detection and identification of delayed duplicate packets being injected into APRS-IS beyond the default 30 second duplicate detection interval. When delayed packets are introduced, a mobile station can appear to ping-pong along a path as obsolete positions arrive after newer positions.

**Altitude** - If checked, the current altitude is included in the position packet. Note that altitude is only supported with an active GPS and then only if the GPS is providing an altitude in the fix. Fixed (non-GPS) stations cannot specify an altitude for transmission.

**Compressed** - If checked, position packets are transmitted in a compressed format per chapter 9 of aprs101.pdf. This Compressed format provides more resolution than the humanly readable format. This compression is NOT Mic-E which is a format parsed and displayed by APRSISCE/32, but not supported for transmitting positions.

**CourseSpeed** - If checked and a GPS is providing speed and heading data, a CSE/SPD (Chapter 7 of aprs101.pdf) is included in the position packet. If compressed is checked, then either Altitude or Course/Speed can be selected, but not both as the APRS protocol doesn't support it.

**Comment** - The Comment cascading menu provides direct access to a set of "remembered" comment texts. You can also specify a New comment text (optionally remembered) or None to eliminate the comment from being transmitted with the position beacons.

**Symbol** - The Symbol cascading menu provides direct access to a set of "remembered" symbols. You can also select a New symbol using the same dialog as used for [Configure General](menu-configure-general.md).

**Why** - If checked, causes APRSISCE/32 to include the [Transmit Pressure](doc-screen-features.md) that triggered the position beacon to be included in the position packet. This can be quite handy when determining the cause of too-frequent beaconing or to see which trigger is actually causing various beacons along a route. Because this text is included in the station's comment, some APRS radios will consider each beacon to have significantly changed causing a new display on each reception.

**Precision** - The default precision for a non-compressed position beacon is 1/100 minute of latitude and logitude. APRSISCE/32 supports increased precision via the !DAO! extension [Datum](http://www.aprs.org/aprs12/datum.txt) as well as ambiguous position as described in Chapter 6 (page 24) of aprs101.pdf). Both 1 digit (1/1000 minute - humanly readable) and 2 digit (91/10000 minute - don't ask) additional precision is supported as well as all levels of ambiguity. Compressed packets are, by definition, accurate to within 1 foot.

**Path…** - Specifies the path to use when transmitting position beacons. (see [New Paradigm](http://www.aprs.org/fix14439.html) or just set it to WIDE1-1,WIDE2-1 as a "safe" mobile value or just WIDE2-1 for a fixed station) This is likely to be relocated in some future APRSISCE/32 revision to allow more discrete setting of different paths on different ports.
