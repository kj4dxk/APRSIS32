---
title: "TH-D7"
---

# TH-D7

[Source wiki page](https://aprsisce.wikidot.com/th-d7) · [Archived HTML](../archive/pages/th-d7--33d980d7c172.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **TH-D7** |

|  |
| --- |
| If you set up a KISS  port, set the right COM port and baud rate (whatever the D7's default  is), and hook up the cable it should work. The program currently  defaults the Open/CloseCmds for a D7 on a KISS port. Those of us  without D7's actually should manually edit the XML config to remove the  D7 stuff and change them to a no-op in most cases.  <OpenCmd>^M~</OpenCmd>  <OpenCmd>^M~</OpenCmd>  <OpenCmd>XFLOW OFF</OpenCmd>  <OpenCmd>FULLDUP OFF</OpenCmd>  <OpenCmd>KISS ON</OpenCmd>  <OpenCmd>RESTART!!0</OpenCmd>  <CloseCmd>^192^255^192~!!0</CloseCmd>  <CloseCmd>^C^C^C~!!0</CloseCmd>  <CloseCmd>TC 1!TS 1</CloseCmd>  <CloseCmd>TN 2,0!TN 2,0</CloseCmd>  <QuietTime>0</QuietTime> |

|  |
| --- |
|  |
