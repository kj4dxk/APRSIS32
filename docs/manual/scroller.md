---
title: "Scroller"
---

# Scroller

[Source wiki page](https://aprsisce.wikidot.com/scroller) · [Archived HTML](../archive/pages/scroller--65277e530c57.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Scroller** |

<table class="aprs-figure-table" style="width:100%;margin-right:auto;margin-left:auto;border:1px solid #AAA;">
<tr>
<td style="padding: 10px; text-align: left; background-color: #DDDDEE; border: 1px solid silver; width: 100%;">
<div class="image-container floatright"><a href="http://www.flickr.com/photos/62090831@N05/5747926675/"><img alt="flickr:5747926675" class="image" src="../archive/assets/5225_5747926675_b67a3e79ba--7f8989920e31.jpg"/></a></div>
<p>The scrolling packet window shows packets for stations (or objects) that were recently received. This window communicates many things about the packet as follows:</p>
<p><strong>Callsign-SSID</strong> - The assigned station callsign, or tactical station identifier<br/>
<strong><span style="white-space: pre-wrap;">*</span></strong> - An asterisk after the callsign indicates that the packet was received from RF (AGW, KISS or TEXT ports)<br/>
<strong><span style="white-space: pre-wrap;">#</span></strong> - A pound sign after the callsign indicates that the packet was received from RF inside a 3rd party packet<br/>
<strong>@</strong> - An at sign after the callsign indicates that the packet was digipeated by your station<br/>
<strong>n</strong> - If there's a number after the asterisk, it counts how many times the packet was received (total of IS and RF digipeats)<br/>
<strong>(n)</strong> - If there's a number in parenthesis in the callsign, it indicates that multiple copies of the packet were received via APRS-IS</p>
<p><strong>Symbol</strong> - current station symbol including all spec-specified assumptions based on:<br/>
1) Explicit statement in packet<br/>
2) Weather data is a weather symbol<br/>
3) GPSxxx specification in ToCall (Page 92/103 of aprs101.pdf)<br/>
4) Assumption based on -SSID value (Page 93/103 of aprs101.pdf)<br/>
5) APRSISCE/32 also uses ~ for Telemetry data if no other symbol was previously known</p>
<p>A box is drawn around the symbol if the packet was received direct (no paths used) via RF.</p>
<p>Clicking on an entry in the scrolling list will bring up that station's popup menu as well as present the packet for that entry across the bottom of the map.</p>
<p><strong>Note</strong> If the scrolling region gets a red border when you click it, it is frozen for review. Left click anywhere BUT on the scroller (or right click on the scroller) to unfreeze the list and allow new entries to be displayed.</p>
<p><strong>Note Also</strong>: The contents and behavior of the Packet scroller are controlled by <a href="menu-configure-scroller.md">Configure / Scroller Menu</a> options so YMMV.</p>
<hr/>
<h3 id="toc0"><span>Text Color</span></h3>
<p><strong>Black</strong> : No position known for the station<br/>
<span style="color: #ff0000"><strong>Red</strong></span>- <span style="color: #ffff00"><strong>Yellow</strong></span>- <span style="color: #008000"><strong>Green</strong></span> - Scaled color based on proximity (Red = close, Green = far)<br/>
The scale of this conversion is the circle on the map. If the station is within the circle, the color will vary from Green (outer edge) to Red (dead center) based on how close the station is to the coordinates at the center of the screen.<br/>
<strong>Reversed</strong> <span style="color: #ff0000"><strong>Red</strong></span>- <span style="color: #ffff00"><strong>Yellow</strong></span>- <span style="color: #008000"><strong>Green</strong></span> - Scaled as above, but reverse video for callsigns that seem to be in your extra filter as a "buddy" (b/) or "friend" (f/). This may get some false positives until I add explicit Buddy and Friend filter configuration.</p>
<h3 id="toc1"><span>Background Color</span></h3>
<p><strong>"Invalid"</strong> - See details in the upcoming "Invalids" trace log<br/>
<span style="color: #800080"><strong>Purple</strong></span> - Duplicate packet<br/>
<span style="color: #ffc0cb"><strong>Pink</strong></span> - Too Quick packet (sub second)<br/>
<span style="color: #ff0000"><strong>Red</strong></span> - Too Fast packet (&gt;20,000mph or double rolling average)<br/>
<span style="color: #00ffff"><strong>Aqua</strong></span> - "Restart" packet (long story, under development)<br/>
<span style="color: #808080"><strong>Gray</strong></span> - Unknown Invalid (should never be seen)<br/>
<span style="color: #ffff00"><strong>Yellow</strong></span> - Other APRSISCE/32 users have a yellow background unless something else overrides the color (like buddy).</p>
</td>
</tr>
</table>

|  |
| --- |
|  |
