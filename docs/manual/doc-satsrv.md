---
title: "SATSRV"
---

# SATSRV

There's a new ISS object flying in APRS-IS, but this object is different than others that have flown. It is flying with a [Multiline description](http://www.aprs-is.net/WX/MultilineProtocol.aspx) of the approximate reception footprint. It moves every two minutes, so if you're using an APRS client that displays MultiLine information (APRSISCE/32 and xastir at a minimum), you'll be able to see the footprint coming and know to anticipate a pass (or realize that you just missed one).

The ISS object has a comment that says **Msg4Pass**. That means to send an APRS message to "**ISS**" (without the quotes) from any message-capable APRS platform. This can be APRS radios (if you're in range of a [message-gating IGate](doc-aprs-messaging-explained.md)), any of the PC-based APRS messaging clients (like APRSIS32, UI-View, or xastir), or any of the new mobile APRS-IS clients - provided that they support message sending, acking, and receiving. You can always tell what the server is doing by checking [ISS messages at aprs.fi](http://aprs.fi/?c=message&limit=25&call=ISS). Change the trailing callsign to the satellite that you are querying. You can even see acks and retries by switching to the [raw view](http://aprs.fi/?c=raw&limit=25&view=normal&call=ISS).

When **ISS** (not the real space station, but the APRS satellite server) receives your query (via the APRS-IS), it will calculate the current or next pass of the ISS for an observer at your station's last beaconed position (it will say "Please beacon Position" if it doesn't know where you are). The response depends on when the pass begins.

| Pass Time | Response Format | Sample Response |
| --- | --- | --- |
| More than an Hour away | AOS **time** (**dd hhmmz**) **dir^mx** | AOS 1h13m (1936z) N^76 |
| Less than an hour away | AOS **time** **dir** **dir^mx** **dir** +**duration** | AOS 49m57s W NW^76 E +11m |
| Ascending in a pass | **dir^el** **dir^mx** **dir** LOS **remaining** | W^15 N^76 E LOS 7m48s |
| Descending in a pass | **dir^el** **dir** LOS **remaining** | NNE^75 E LOS 5m11s |
| No passes for your QTH | No AOS Within 2 Days | No AOS Within 2 Days |

**Where:**  

|  |  |
| --- | --- |
| AOS | Acquisition of Signal |
| LOS | Loss of Signal |
| **time** | time in hours/minute or minutes/seconds until AOS |
| **dd hhmmz** | zulu time of AOS (dd is suppressed if today) |
| **dir** | compass point (8 points pre-pass, 16 points within pass) |
| **^mx** | maximum elevation expected |
| **^el** | current elevation during a pass (always first) |
| **duration** | expected duration of the pass (in minutes) |
| **remaining** | time remaining (minutes/seconds) of the pass |

**But Wait! There's MORE!** If the text you send to ISS is a known station identifier (say **KJ4ERJ-12**), the returned pass information will not be for your location, but for that station's last known location. To be clear, "@ call-SSID" is appended to the end of the response so you know which observer it is for. Of course, if the satellite server doesn't know where your specified station is located, you will receive "Please Beacon Position" as a response or a forecast for your own QTH (because it may be just "anything" in the query for yourself).

**And that's not all!** This doesn't only work for the ISS, but for any of the other satellites found in the TLE set from [Celestrak Amateur TLE](http://www.celestrak.com/NORAD/elements/amateur.txt). You can send an APRS message to "**AO51**" to find out when that satellite will pass within range. If you have any trouble figuring out which satellite name a message can be sent to, send an APRS message to KJ4ERJ-12 (or an e-mail to here) and I'll see if I can map the TLE entry to the responder station ID. And of course, you can also send any other known station ID to any of the satellites to get the current or next pass information for that station's location.

Oh, and if the satellite name is too long for APRS, send to "**SATnnnnn**" where **nnnnn** is the satellite number from the TLE (first number on line 1 before the U). (Thanks KO6TH!)

Give the service a try, and let me know how it works out. There's nothing to remember except the satellite name (dashes not required) and the fact that you send a message to the satellite. Nothing like ANSRVR or CQSRVR or SATSRV, just send the message to the satellite by name and it will return the pass information.

An Example **ISS** pass is as follows (each of the bold lines was received in response to a single APRS message to ISS):

| Response | Interpretation |
| --- | --- |
| **AOS 1h13m (1936z) N^76** | The pass is over an hour away, **1h13m** is the time until the pass starts, **(1936z)** the actual start time of the pass, **N^76** is the direction of the maximum Elevation and the maximum elevation, relative to your most recently transmitted position. |
| **AOS 49m57s W NW^75 E +11m** | The pass begins within an hour, **49m57s** is the time until the pass starts, **W** the direction of AOS, **NW^75** the direction and maximum elevation. **E** the direction of LOS and **+11m** the duration of the pass |
| **W^15 N^76 E LOS 7m48s** | The satellite is ascending, **W^15** current direction and elevation, **N^76** maximum Elevation and direction, **E** the direction of LOS, **7m48s** the time until LOS |
| **NNE^75 E LOS 5m11s** | The satellite is descending, **NNE^75** the current direction and elevation, **E** direction of LOS, **5m11s** time until LOS |
| **ENE^4 E LOS 55s** | Pass nearly over |
