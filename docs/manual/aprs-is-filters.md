---
title: "APRS-IS Filters"
---

# APRS-IS Filters

[Source wiki page](https://aprsisce.wikidot.com/aprs-is-filters) · [Archived HTML](../archive/pages/aprs-is-filters--4bc510474076.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |  |  |
| --- | --- | --- |
| **Parameter** | **Filter Type** | **Description** |
| a/latN/lonW/latS/lonE | Area | The area filter works the same as range filter but the filter is defined as a box of coordinates. The coordinates can also been seen as upper left coordinate and lower right. Lat/lon are decimal degrees. South and west are negative. Up to 9 area filters can be defined at the same time. |
| b/call1/call2… | Budlist | Pass all traffic from exact call: call1, call2, … (\* wild card allowed) |
| d/digi1/digi2… | Digipeater | The digipeater filter will pass all packets that have been digipeated by a particular station(s) (the station's call is in the path). This filter allows the \* wildcard. |
| e/call1/call1/… | Entry station | This filter passes all packets with the specified callsign-SSID(s) immediately following the q construct. This allows filtering based on receiving IGate, etc. Supports \* wildcard. |
| f/call/dist | Friend Range | This is the same as the range filter except that the center is defined as the last known position of call. Up to 9 friend filters can be defined at the same time. |
| g/call1/call1/… | Group Message | This filter passes all message packets with the specified callsign-SSID(s) as the addressee of the message. Supports \* wildcard. |
| m/dist | My Range | This is the same as the range filter except that the center is defined as the last known position of the logged in client. |
| o/obj1/obj2… | Object | Pass all objects with the exact name of obj1, obj2, … (\* wild card allowed)(spaces not allowed) (| => / and ~ => \*) |
| os/obj1/obj2… | Strict Object | Pass all objects with the exact name of obj1, obj2, … (\* wild card allowed) (| => / and ~ => \*) Objects are always 9 characters and Items are 3 to 9 characters. There can only be one os filter and that filter must be at the end of the line. |
| p/aa/bb/cc… | Prefix | Pass traffic with fromCall that start with aa or bb or cc… |
| q/con/ana | q Contruct | q = q Construct command  con = list of q Construct to pass (case sensitive)  ana = analysis based on q Construct.  I = Pass positions from IGATES identified by qAr or qAR. For example:  q/C Pass all traffic with qAC  q/rR Pass all traffic with qAr or qAR  q//I Pass all position packets from IGATES identified in other packets by qAr or qAR |
| r/lat/lon/dist | Range | Pass posits and objects within dist km from lat/lon. lat and lon are signed decimal degrees, i.e. negative for West/South and positive for East/North. Up to 9 range filters can be defined at the same time to allow better coverage. Messages addressed to stations within the range are also passed. |
| s/pri/alt/over | Symbol | pri = symbols in primary table (| => /)  alt = symbols in alternate table (| => /)  over = overlay character (case sensitive)  For example:  s/-> This will pass all House and Car symbols (primary table)  s//# This will pass all Digi with or without overlay  s//#/T This will pass all Digi with overlay of capital T |
| u/unproto1/unproto2/… | Unproto | This filter passes all packets with the specified destination callsign-SSID(s) (also known as the To call or unproto call). Supports \* wildcard. |
| t/poimqstunw  t/poimqstuw/call/km | Type | Pass all traffic based on packet type. One or more types can be defined at the same time, t/otq is a valid definition.  p = Position packets  o = Objects  i = Items  m = Message  q = Query  s = Status  t = Telemetry  u = User-defined  n = NWS Weather & Weather Objects  w = Weather Note: The weather type filter also passes positions packets for positionless weather packets.  The second format allows putting a radius limit around "call" (station callsign-SSID or object name) for the requested station types. |

<span id="toc0"></span>

## Sample filter entries

|  |  |
| --- | --- |
| r/27.99673/-80.659072/1 | Passes all packets within 1 km of Lynn's house |
| b/KJ4DXK\* | Passes all packets from any SSID belonging to KJ4DXK |
| u/APWW\*/APWM\* | Passes all packets sent to a TOCALL starting with APWW (APRSIS32) or APWM (APRSISCE) |
| -b/WINLINK | Blocks all packets from WINLINK |
| -s/S/S | Blocks all packets using a Space Shuttle (/S) or Satellite (\S) symbol |
| -s//a/DW | Blocks all packets using a red diamond (\a) overlaid with DW |
| s/@/@ | Passes all packets using the Hurricane prediction (/@) and Hurricane (\@) symbol |
| -b/METAR-2 | Blocks all packets originated from METAR-2 |
| -u/APFMET | Blocks all packets with a toCall of APFMET |
| -e/METAR-2 or -e/METAR\* | Blocks all packets entering the APRS-IS from METAR-2 or any METAR station |
| -o/MTR\* | Blocks all objects that start with MTR. Note that -b/MTR\* won't work because the water meters are objects owned by METAR2, not stations that start with MTR. |
| -s/w | Blocks all stations using Water Station (/w) symbol |
|  |  |
|  |  |
