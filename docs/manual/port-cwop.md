---
title: "CWOP"
---

# CWOP

[Source wiki page](https://aprsisce.wikidot.com/port:cwop) · [Archived HTML](../archive/pages/port_cwop--2f09e0d41dba.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **CWOP** |

|  |
| --- |
| As of version 2010/04/28 you can now create a receive-only CWOP port by the following procedure: 1) Configure / Ports / New Port  2) Select a Type: of CWOP from the drop-down  3) Name the port appropriately (I used Weather)  4) Yes, you want a TCP/IP Port.  5) Set the IP or DNS to cwop.aprs.net  6) Set the port to 10152 for a full, world-wide feed  7) Check Enable / RFPorts / <Your Name> to start receiving data  Even though APRSISCE/32 sends the current filter to ports of this type, the CWOP servers don't offer a filtered port, so you're stuck with a full planetary feed even if you only want your neighborhood.  See also: [CWOP Servers](http://www.wxqa.com/servers2use.html)  If you want to quit getting this data, just uncheck Enables / RFPorts / <Your Name>.  Interesting tidbit - There's nothing actually different between a CWOP server and an APRS-IS server, just the network their tied into. So, if you want/need to connect a receive-only feed to an alternate APRS-IS server (like my non-filtered RF feed server), you can create a CWOP-type port and specify the appropriate TCP/IP "Device" information to get to the alternate APRS-IS-compatible server. THIS is why the CWOP-type ports still get the filter updates even though the CWOP-proper servers don't support a filtered port.  So, why is CWOP a separate network you ask? Read on for a bit of the history from someone that was there…  From Pete - author of javAPRSSrvr:  I was asked to weigh in on this regarding what those stations are and why they don't show up on APRS-IS. Considering it is because of my software they don't show up, I guess that is a reasonable qualification.  The CWOP (Citizens Weather Observation Program) is a volunteer weather reporting system implemented a little over 10 years ago using APRS-IS as its reporting network. This was done because it was born out of ham stations already reporting on APRS (and therefore APRS-IS) and promoted by Steve Dimse (findU owner/operator) and a few people in NOAA (the US national weather service).  About 3 years ago (I think), we moved the CWOP stations (CW or DW followed by 4 numbers) to their own network because of some poor programming techiques that caused massive spikes on the core servers and connected downstream servers. They have operated on their own network since then and have grown from about 3700 stations to over 5000 stations.  Now the beauty of this is that while their reports don't show up on APRS-IS (separate network), our weather reports show up on their servers. This is accomplished by a read-only server that takes feeds from APRS-IS and the CWOP servers. It is this feed that aprs.fi is using to populate its database. There is not bidirectional operation here; this is unidirectional to the read-only server and then database applications like findU and aprs.fi can pull their information from there or from APRS-IS (or both).  Hope this helps explain what you are seeing and why it won't affect your everyday operation. Basically, APRS-IS and CWOP go to a read-only server which can only be accessed by a database server or other server software that doesn't feed back into either network. Even if the CWOP traffic was fed back in, it would be blocked at the core servers as they do not use validated logins.  73,  Pete AE5PL |

|  |
| --- |
|  |
