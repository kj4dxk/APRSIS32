---
title: "GeniusBeaconing™"
---

# GeniusBeaconing™

[Source wiki page](https://aprsisce.wikidot.com/en-geniusbeaconing) · [Archived HTML](../archive/pages/en-geniusbeaconing--4678b7e88ff8.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **GeniusBeaconing™ algorithm definition and use** |

|  |
| --- |
| GeniusBeaconing™'s Forecast Error works by calculating where an outside observer would think you are based on your last beaconed speed and heading. It takes that last information and extends it by the time it has been since your last beacon to arrive at an expected lat/lon. This is compared to your actual lat/lon (which, of course, only you know at this point) to derive an error vector (distance and direction). The red dot is plotted inside the circle in that direction and scaled such that the circle is the configured Genius Forecast Error distance. When the red dot hits the circle, the configured Forecast Error distance has been reached and a new beacon is sent (provided that at least the Min Time has been reached).  So, if you're driving along in a straight line at a steady speed, you won't see the red dot. (It's actually bouncing around a bit underneath your icon in the center of the circle).  If you slow down, that outside observer will still think you're driving fast and will have you further ahead of where you are and the red dot will start moving towards the top of the screen. If you then resume your original speed, the red dot will freeze at that distance ahead. If you go faster than your last beaconed speed the red dot will drift back to the center and will then start falling towards the bottom of the screen as your actual location gets ahead of where the elapsed-time forecast would place you.  Slight heading changes are even more fun. If you turn to the left (by less than the configured Heading Change or that will trigger a new beacon), the red dot will begin drifting to the right. Veer right and it will drift to the left. Resume straight line motion in the new heading and the dot will continue to drift as your actual location continues to get further away from the course indicated by the previously beaconed speed and heading.  **A requirement of the use of the algorithm is crediting Lynn KJ4ERJ and using the term GeniusBeaconing™**  Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32  PS. If you think that was complicated, try learning how a missile guidance system works: <http://tinyurl.com/Guidance-mp3> |

|  |
| --- |
|  |
