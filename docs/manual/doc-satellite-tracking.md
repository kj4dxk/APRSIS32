---
title: "Satellite Tracking"
---

# Satellite Tracking

More soon but press CTRL O

You will then be given the option to view all satellites or just the ISS. In either case, once satellite tracking is activated, a "Satellite Details" window will be available from the Left-Click popup menu. From there you can select what satellite objects are actually generated and under what conditions. The checkbox in Satellite Details means:

Checked - Always generate satellite objects  
Unchecked - Never generate satellite objects  
Tri-State (faded check) - Generate killed satellite objects 10 minutes before AOS and after LOS, generate Visible (V overlay) satellite objects while in view.

TLEs for the satellites come from [Celestrak Amateur TLE](http://www.celestrak.com/NORAD/elements/amateur.txt) and are filtered to the currently active satellites listed below. To completely turn off satellite tracking, simply close the Satellite Details window and answer the confirmation prompt. If the client is closed with satellite tracking active, it will automatically re-activate on restart with the most recent checkbox settings intact.

The common reasons for errors in tracking satellites are the following:

Very old TLE data, especially with the ISS please use less than week old data, TLE and tracking in general is an estimate and the newer the data the better the estimate. (Not so much an issue with APRSISCE/32 until we support user-supplied TLEs). Note that not all TLE sources are created equal. See below for a specific example.

Also make sure the time on your PC is accurate, windows has a time sync function thats good enough for satellite tracking. But I recommend [Meinberg](http://www.meinberg.de/english/sw/index.htm) for accurate timing and automatic correction.

And the time zone setting is also important. Don't say you're in the Eastern Time Zone and set the time for UTC or vice versa. Your time zone is used to offset the displayed time to UTC for satellite orbit calculations.

Also of course make sure you have not got your location wrong! This is not the center of the screen, but the location of "ME"!

Here's the list of currently known active satellites:

/\* From Steve via [AMSAT Status](http://www.amsat.org/amsat-new/satellites/status.php) \*/

|  |  |
| --- | --- |
| Name | Cat No. |
| SRM | 99999 |
| ARRISSat-1 | 37772 |
| FASTRAC 2 | 37380 |
| FASTRAC 1 | 37227 |
| OREOS | 37224 |
| HO-68 | 36122 |
| ITUpSAT1 | 35935 |
| BEESAT | 35933 |
| SwissCube | 35932 |
| SO-67 | 35870 |
| KKS-1 | 33499 |
| STARS | 33498 |
| PRISM | 33493 |
| Rs-30 | 32953 |
| CO-66 | 32791 |
| DO-64 | 32789 |
| Compass-1 | 32787 |
| Co-65 | 32785 |
| GENESAT-1 | 29655 |
| CO-58 | 28895 |
| VO-52 | 28650 |
| AO-51 | 28375 |
| RS-22 | 27939 |
| CO-57 | 27848 |
| CO-55 | 27844 |
| SAUDISAT 1C | 27607 |
| NO-44 | 26931 |
| SAUDISAT 1B | 26549 |
| SAUDISAT 1A | 26545 |
| ARISS | 25544 |
| SO-33 | 25509 |
| GO-32 | 25397 |
| FO-29 | 24278 |
| RS-15 | 23439 |
| IO-26 | 22826 |
| AO-27 | 22825 |
| AO-16 | 20439 |
| UO-11 | 14781 |
| AO-7 | 7530 |

links for elements  
[All Amateur Satellites](http://www.celestrak.com/NORAD/elements/amateur.txt)

On 10/14/2011 I researched a report that APRSISCE/32 was 2+ minutes different in an AOS forecast than Ham Radio Delux. After tracking down the sources of their respective TLEs, I found that APRSISCE/32's source of [Celestrak Amateur TLE](http://www.celestrak.com/NORAD/elements/amateur.txt) had the following from day 286 of year 11.

ISS (ZARYA)  
1 25544U 98067A 11286.81211653 .00009560 00000-0 11861-3 0 2381  
2 25544 51.6369 283.5817 0016212 325.3429 93.8255 15.60650478739436

HRD was using TLEs from [AMSAT NASAbare TLE](http://www.amsat.org/amsat/ftp/keps/current/nasabare.txt) which included the following from day 285 of year 11.

ISS  
1 25544U 98067A 11285.91900602 .00007956 00000-0 10000-3 0 02347  
2 25544 051.6409 288.0749 0016408 320.9217 117.1587 15.60589610739299

A source TLE difference of Just under 9/10 of a day resulted in a 2 minute difference in the predictions. Yes, TLE freshness DOES make a difference. (It didn't help that the ISS had just undergone two orbital boosts in the past week, but still…)
