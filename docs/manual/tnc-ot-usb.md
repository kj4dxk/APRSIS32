---
title: "TNC-OT-USB"
---

# TNC-OT-USB

[Source wiki page](https://aprsisce.wikidot.com/tnc-ot-usb) · [Archived HTML](../archive/pages/tnc-ot-usb--a12c11e5ed9d.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **TNC-OT-USB** |

|  |
| --- |
| From Alan 1/3/2011 1:00am: Been playing with the new Argent Data OT-USB and APRSIS32.  It works great as a I-GATE, even as a mobile I-GATE/Tracker. The USB port KISS mode seamed to work best when APRSIS32 com setting was set at max 256000. It decoded a weak digi 60 miles away  I made an enter KISS, Exit KISS command with a few extra commands so feedback is in the Trace log. Monitor off speeds up USB KISS to IS.  <!RFPort[1]>  <RFPort Name="OT-USB">  <Protocol>KISS</Protocol>  <Device>COM4:256000,N,8,1</Device>  <RfBaud>1200</RfBaud>  <OpenCmd>^M!</OpenCmd>  <OpenCmd>MON OFF!</OpenCmd>  <OpenCmd>VER!1</OpenCmd>  <OpenCmd>USB ON!!0</OpenCmd>  <CloseCmd>^C^C^C~!!1</CloseCmd>  <CloseCmd>ver!!1</CloseCmd> |

|  |
| --- |
|  |
