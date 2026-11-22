---
title: "APRS Queries"
---

# APRS Queries

[Source wiki page](https://aprsisce.wikidot.com/doc:queries) · [Archived HTML](../archive/pages/doc_queries--713a6bcb10ae.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **APRS Queries** |

|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Below is a list of standard and Non standard queries that the software will respond to   |  |  | | --- | --- | | ?APRSD | Respond with stations heard direct (no hops used) on RF recently\* | | ?APRSL | Respond stations heard locally\* on RF recently\* | | ?APRSH | Respond with station's packet counts heard via any path with the I+D+d+R type numbers. | | ?APRSH \* | Respond with stations heard on RF recently\* | | ?APRSM | Ask the station to requeue pending messages for the requester for transmission | | ?APRSO | Queue all normally beaconed objects (enabled w/interval) the station has for retransmission | | ?APRSP | Trigger a forced position beacon | | ?APRSS | Respond with the configured ports and status of the station | | ?APRST | Respond with the path over which the request was heard | | ?PING? | Is the same as ?APRST | | ?ABOUT | Station software and OS info | | ?APRSV | Same as about | | ?VER | Same as about | | ?DX | Respond with furthest heard RF station in the current clock hour | | ?IGATE | Information on igating status |  * Recently (time) and locally (hops) configurable in the future, default is 30 minutes and 2 used hops. These are the same parameters used to determine if messages will be gated for a station from -IS to RF.   ?APRSD  Directs:KD4APP-7 N4NE-2  ?APRSL  < Locals[1/3]:K4RGK-2(1) KD4APP-7 KD4UYP-10(1) KD4YDD-1(1)  < Locals[2/3]:KI4NBY-1(1) KX4ZN(1) N40KN-9(1) N4BFR-10(1) N4NE-2  < Locals[3/3]:W3CP-2(1) W4JDH-1(1) WA4EOC-2(1)  ?APRSH  VE6SRV I: 3  ?APRSH \*  < Heard[1/3]:K4RGK-2(1) KD4APP-7 KD4UYP-10(1) KD4YDD-1(1)  < Heard[2/3]:KI4NBY-1(1) KX4ZN(1) N40KN-9(1) N4BFR-10(1) N4NE-2  < Heard[3/3]:W3CP-2(1) W4JDH-1(1) WA4EOC-2(1)  ?APRSM  0 Messages Requeued  ?APRSO  0 Objects Transmitted  ?APRSP  Position packet sent by queried station  ?APRSS  < Ports[1/3] 08-15 14:04:51 APRS-IS OK 684 694 677 693 685 637 650+  < 681 /hr  < Ports[2/3] 08-15 14:04:51 PicoAPRS Disabled  < Ports[3/3] 08-20 11:30:04 TT4-NEW CloseCmd 52 68 69 84 82 67 52 56+  < /hr DX=110\*N4NE-2(45mi@196°)  ?APRST  < KD4APP-5>APWW11,TCPIP\*,qAC,T2DALLAS  ?PING  < Support[1/2]: ?APRSD ?APRSL ?APRSH ?APRSM ?APRSO ?APRSP ?APRSS  < Support[2/2]:?APRST/?PING? ?ABOUT/?APRSV/?VER ?CPU ?DX ?IGATE  ?ABOUT/?APRSV/?VER  < 2020/05/01 21:21: APRSIS32 Win v6.1 b7601 p2 0.0/0.0%  ?CPU  APRSIS32 Windows v6.1 b7601 p2 K:0.0/0.0% U:0.0/0.0%  ?DX  < DX:TT4-NEW[KISS] 111\*N4NE-2(45mi@196°) I+G+D+d+R: 7+11+0+15+4+  < 10+42+0+47+5 10+40+1+50+5 10+34+1+61+7 11+44+1+61+6 10+23+0+44+6+  < 10+20+0+31+6 10+18+0+34+6  I = Station-originated Packets received via APRS-IS  G = Station-gated Packets (callsign-SSID is after q-construct)  D = Station-digi'd Packets heard on -IS (1st copy digipeats)  d = Station-digi'd Packets heard on RF (local-only digipeats)  R = Station-originated Packets received via RF  ?IGATE  < IGate[1/2] MSG\_CNT=23 LOC\_CNT=14 DIR\_CNT=4 RF\_CNT=14  < Gate[2/2] TT4-NEW[KISS] Bi-Directional DX=111\*N4NE-2(45mi@196°) |

|  |
| --- |
|  |
