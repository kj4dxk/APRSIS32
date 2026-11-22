---
title: "D700 UIFLOOD Digipeating"
---

# D700 UIFLOOD Digipeating

During the testing procedures, the unit under test was programmed to respond to an alias of HOPn-N as per the [Golden Packet Event](http://www.aprs.org/at-golden-packet.html) setup page. The [metasyntactic variables](http://en.wikipedia.org/wiki/Metasyntactic_variable) foo, bar, baz, and qux are used as placeholders where any path element could be used. DIGI1, DIGI2, etc. are used to denote the name of the digipeater that previously handled the currently active path element.

| Incoming path | UIFLOOD ID | UIFLOOD NOID | UIFLOOD FIRST |
| --- | --- | --- | --- |
| <no path> | No Action | No Action | No Action |
| LWD1-1 | KJ4ERJ-11\*,LWD1\* | LWD1-1 | KJ4ERJ-11\*,LWD1\* |
| LWD1-1,LWD2-1 | KJ4ERJ-11\*,LWD1\*,LWD2-1 | LWD1-1,LWD2-1 | KJ4ERJ-11\*,LWD1\*,LWD2-1 |
| LWD1-1,LWD2-2 | KJ4ERJ-11\*,LWD1\*,LWD2-2 | LWD1-1,LWD2-2 | KJ4ERJ-11\*,LWD1\*,LWD2-2 |

|  |  |  |  |
| --- | --- | --- | --- |
| FOOBAR\*,LWD1\*,LWD2-1 | KJ4ERJ-11\*,LWD2\* | FOOBAR\*,LWD1\*,LWD2-1 |  |
| FOOBAR\*,LWD1\*,LWD2-2 | KJ4ERJ-11\*,LWD2-1 | FOOBAR\*,LWD1\*,LWD2-2 |  |
| FOOBAR\*,LWD1\*,2UTHAR\*,LWD2-1 | KJ4ERJ-11\*,LWD2\* | FOOBAR\*,LWD1\*,2UTHAR\*,LWD2-1 | FOOBAR\*,LWD1\*,2UTHAR\*,LWD2\* |
| MPD1-1,LWD2-1 | No Action | No Action | No Action |
| FOOBAR\*,MPD1\*,LWD2-1 | KJ4ERJ-11\*,LWD2\* | FOOBAR\*,MPD1\*,LWD2-1 | KJ4ERJ-11\*,LWD2\* |
| FOOBAR\*,MPD1\*,LWD2-2 | KJ4ERJ-11\*,LWD2\* | FOOBAR\*,MPD1\*,LWD2-2 | KJ4ERJ-11\*,LWD2-1 |
| FOOBAR\*,MPD1\*,2UTHAR\*,LWD2-1 | KJ4ERJ-11\*,LWD2\* | FOOBAR\*,MPD1\*,2UTHAR\*,LWD2-1 | FOOBAR\*,MPD1\*,2UTHAR\*,LWD2\* |

|  |  |  |  |
| --- | --- | --- | --- |
| LWD7-7,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,LWD7-6,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,2UTHAR\*,LWD7-5,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,LWD7\*,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,2UTHAR\*,LWD7\*,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,LWD7\*,FOOBAR-2\*,LWD7-6 | cell-content | cell-content | cell-content |
| FOOBAR\*,2UTHAR\*,LWD7\*,FOOBAR-2\*,2UTHAR-2\*,LWD7-5 | cell-content | cell-content | cell-content |
| FOOBAR\*,2UTHAR\*,LWD7\*,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,2UTHAR\*,LWD7\*,LWD7-7 | cell-content | cell-content | cell-content |
| FOOBAR\*,2UTHAR\*,LWD7\*,LWD7-7 | cell-content | cell-content | cell-content |

Back to [UIFLOOD Hardware Testing](uiflood-hardware-testing.md)
