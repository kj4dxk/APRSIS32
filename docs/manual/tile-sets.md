---
title: "Tile Sets"
---

# Tile Sets

[Source wiki page](https://aprsisce.wikidot.com/tile-sets) · [Archived HTML](../archive/pages/tile-sets--ce5ce3257b7d.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Tile Sets** |

<table class="aprs-figure-table" style="width:100%;margin-right:auto;margin-left:auto;border:1px solid #AAA;">
<tr>
<td style="padding: 10px; text-align: left; background-color: #DDDDEE; border: 1px solid silver; width: 100%;">
<p>APRSISCE/32 uses the <a href="http://www.openstreetmap.org">OpenStreetMap</a> tile set as a default background map. These map tiles are part of an Open Source mapping project which has the intention of creating a freely distributable non-encumbered map of the world for use in all kinds of projects, such as APRSISCE/32. Most of the map sources normally encountered have copyright or other use restrictions.</p>
<h2 id="toc0"><span>Map Tiles</span></h2>
<p>The OSM maps are delivered as 256 X 256 pixel tiles that APRSISCE/32 downloads from the server, and keeps in a cache on the local storage medium. These tiles are drawn on screen to produce the maps that are used to plot stations on. The tiles are purged from the cache after a preset time to keep the local storage from being filled. It is possible to change the purge time to have the tiles available for as long as desired, including disabling the purge time to allow the tiles to remain forever.</p>
<h2 id="toc1"><span>Default Server</span></h2>
<p>The default tile set "Original" is the Mapnik style used as the default at the <a href="http://www.openstreetmap.org">OpenStreetMap</a> website. There are a number of different tilesets available for use with APRSISCE/32. They all use the same map database, but use different rendering styles which may highlight different features, or just use different color sets that may be more desirable to the end user.</p>
<table class="wiki-content-table">
<tr>
<td><img alt="1715.png" class="image" src="../archive/assets/12_1130_1715--3d7ad99dcc9c.png"/></td>
<td><img alt="1130.jpg" class="image" src="../archive/assets/ArcGIS_rest_services_USA_Topo_Maps_MapServer_tile_12_1715_1130--9fbd051dd4f6.jpg"/></td>
</tr>
<tr>
<td>Original (Mapnik)</td>
<td>ArcGIS US Topo</td>
</tr>
<tr>
<td><img alt="1715.png" class="image" src="../archive/assets/cycle_12_1130_1715--4f6fbcc2c028.png"/></td>
<td><img alt="1715.png" class="image" src="http://otile3.mqcdn.com/tiles/1.0.0/osm/12/1130/1715.png"/></td>
</tr>
<tr>
<td>CycleMap</td>
<td>MapQuest</td>
</tr>
</table>
<p>Okay, there is one above that is not generated from OpenStreetMap data. That would be the ArcGIS tile. It does show that other tiles outside of the OSM series may be used. With a bit of exploration, others can be found.</p>
<h2 id="toc2"><span>Configure Tile Sets</span></h2>
<p>To configure APRSISCE/32 to use alternate tile sets, you will need to select Configure | Map | Tile Sets | New Tile Set</p>
<table class="wiki-content-table">
<tr>
<td><a href="https://www.flickr.com/photos/62090831@N05/5672527675/"><img alt="flickr:5672527675" class="image" src="../archive/assets/5264_5672527675_fd01cf2a02--cd932007121e.jpg"/></a></td>
</tr>
</table>
<p>A configuration dialog box will open up where you can enter the information require to set up your new tile set.</p>
<p><em>Note: Whatever Tile Set configuration you last Accepted becomes the default tile set for all new windows and across APRSISCE/32 restarts. The default tile set is indicated by the checkmark in the Configure | Map | Tile Sets menu (MapQuest Aerial in the screen-shot above).</em></p>
<table class="wiki-content-table">
<tr>
<td><a href="https://www.flickr.com/photos/62090831@N05/5672545907/"><img alt="flickr:5672545907" class="image" src="../archive/assets/5188_5672545907_13020bc076--5586674926ef.jpg"/></a></td>
</tr>
</table>
<p><em>Name</em> is the name you want to use for the tile set.<br/>
<em>Server</em> is the base URL for the server that will be providing the map tiles.<br/>
<em>80</em> is the port number used to access the tiles.<br/>
<em>URL</em> is the directory qualifier to point to the location of the tiles on the server<br/>
<em>status</em> check here if the server supports tile change status updates<br/>
<em>Purge Enabled</em> check here to have tiles purged from your file storage medium<br/>
<em>Retain</em> sets the number of days to keep tiles before being deleted<br/>
<em>Zoom Min Retain Max</em> configures the minimum and maximum zoom levels to be retained even if the global tile purger is configured to delete tiles.</p>
<h3 id="toc3"><span>Note: When you click Accept, APRSISCE/32 will ask you to locate a directory in which to store the tiles. This must be a new, empty directory for this tile set! If you put multiple tile sets into a single directory, you'll get a mish-mash of both tile sets!</span></h3>
<h3 id="toc4"><span>Available tile servers (only a subset - there are quite a few)</span></h3>
<table class="wiki-content-table">
<tr>
<th>Source</th>
<th>Name</th>
<th>Server</th>
<th>URL</th>
</tr>
<tr>
<td>Open Cycle Map</td>
<td>CycleMap</td>
<td>b.tile.opencyclemap.org</td>
<td><a href="http://b.tile.opencyclemap.org/cycle/13/2260/3431.png">/cycle/</a></td>
</tr>
<tr>
<td>TopOSM</td>
<td>MassTopo (Only MA)</td>
<td>tile2.toposm.com</td>
<td><a href="http://tile1.toposm.com/ma/final/14/4869/6040.png">/ma/final/</a></td>
</tr>
<tr>
<td>MapQuest</td>
<td>Street</td>
<td>otile3.mqcdn.com</td>
<td><a href="http://otile3.mqcdn.com/tiles/1.0.0/osm/13/2260/3431.png">/tiles/1.0.0/osm/</a></td>
</tr>
<tr>
<td>MapQuest</td>
<td>Aerial</td>
<td>otile1.mqcdn.com</td>
<td><a href="http://otile1.mqcdn.com/tiles/1.0.0/sat/13/2260/3431.png">/tiles/1.0.0/sat/</a></td>
</tr>
<tr>
<td>ArcGIS</td>
<td>USA Topo Maps</td>
<td>services.arcgisonline.com</td>
<td><a href="http://services.arcgisonline.com/ArcGIS/rest/services/USA_Topo_Maps/MapServer/tile/14/6013/4952.jpg">/ArcGIS/rest/services/USA_Topo_Maps/MapServer/tile/%z/%y/%x.jpg</a></td>
</tr>
<tr>
<td>ArcGIS</td>
<td>World Topo Maps</td>
<td>services.arcgisonline.com</td>
<td>/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/%z/%y/%x.jpg</td>
</tr>
<tr>
<td>ArcGIS</td>
<td>World Imagery</td>
<td>services.arcgisonline.com</td>
<td>/ArcGIS/rest/services/World_Imagery/MapServer/tile/%z/%y/%x</td>
</tr>
<tr>
<td>ArcGIS</td>
<td>World Street Map</td>
<td>services.arcgisonline.com</td>
<td>/ArcGIS/rest/services/World_Street_Map/MapServer/tile/%z/%y/%x.jpg</td>
</tr>
<tr>
<td>ArcGIS</td>
<td>National Geographic Map</td>
<td>services.arcgisonline.com</td>
<td>/ArcGIS/rest/services/NatGeo_World_Map/MapServer/tile/%z/%y/%x.png</td>
</tr>
<tr>
<td>ArcGIS</td>
<td>World Navigation Charts</td>
<td>services.arcgisonline.com</td>
<td>/ArcGIS/rest/services/Specialty/World_Navigation_Charts/MapServer/tile/%z/%y/%x.jpg</td>
</tr>
<tr>
<td>CalTopo</td>
<td>US Topo</td>
<td>s3-us-west-1.amazonaws.com</td>
<td><a href="http://s3-us-west-1.amazonaws.com/caltopo/topo/14/4952/6013.png">/caltopo/topo/%z/%x/%y.png?v=1</a></td>
</tr>
<tr>
<td>CalTopo</td>
<td>1900 US Topo</td>
<td>s3-us-west-1.amazonaws.com</td>
<td>/ctfun/1900/%z/%x/%y.png</td>
</tr>
<tr>
<td>CalTopo</td>
<td>1930 US Topo</td>
<td>s3-us-west-1.amazonaws.com</td>
<td>/ctfun/1930/%z/%x/%y.png</td>
</tr>
<tr>
<td>CalTopo</td>
<td>USFS</td>
<td>s3-us-west-1.amazonaws.com</td>
<td>/ctusfs/pbs/%z/%x/%y.png</td>
</tr>
<tr>
<td>CalTopo</td>
<td>Visitor</td>
<td>s3-us-west-1.amazonaws.com</td>
<td>/ctfun/visitor/</td>
</tr>
<tr>
<td>CalTopo</td>
<td>Canadian Matrix</td>
<td>s3-us-west-1.amazonaws.com</td>
<td>/nrcan/canmatrix/%z/%x/%y.png?v=1</td>
</tr>
</table>
<p><em>Be aware that some of the above servers only support limited geographic areas, and may not cover your area, or may only support limited zoom levels.</em></p>
<p>Above are some tile sets that have been predetermined. Rather than include every tile set that exist, some education is reasonable to expect from users that wish to use an alternate set. By closely examining the examples below, many more tile sets may be found and utilized.</p>
<p>Now, there are more tile servers available that can be used. Using the original tile paradigm, they just need to fit the pattern that is used by the tile fetching algorithm where the URL used to fetch the tiles is as such:</p>
<p>b.tile.opencyclemap.org/cycle/$z/$x/$y.png</p>
<p>Where b.tile.opencyclemap.org is the server, /cycle/ is the "URL", and the desired tile is designated by elements; z, x and y.</p>
<ul>
<li>zoom ($z)</li>
<li>horizontal ($x)</li>
<li>vertical ($y)</li>
</ul>
<p>Entering elements, z = 9, x = 141 and y = 214, will produce a Server/URL:<br/>
<a href="../archive/assets/cycle_9_141_214--a4e065abdb5b.png">http://b.tile.opencyclemap.org/cycle/9/141/214.png</a></p>
<p>The tile returned will be:</p>
<table class="wiki-content-table">
<tr>
<td><img alt="214.png" class="image" src="../archive/assets/cycle_9_141_214--a4e065abdb5b.png"/></td>
</tr>
</table>
<h3 id="toc5"><span>New Tile Paradigm</span></h3>
<p>The new tile paradigm has added more options. The original format still works. However, if the server has the requirement of re-ordered tile elements, z, y and x, or y, x and x, or an inverted y element is necessary, then more information needs to be added to the URL.</p>
<p>Specifically, the tile elements z, x and y need to be added with the correct formatting. The proper format is %z, %x and %y (or %!y for inverted y). <strong>Note: The server may need to be in a specific order.</strong> The tile extension for anything other than PNG will be needed as well. Current extension options are, JPG and PNG.</p>
<p>An example of a server that requires an inverted y is:<br/>
Server: earthncseamless.s3.amazonaws.com</p>
<p>The URL will demonstrate the use of an inverted-y element:<br/>
URL: /%z/%x/%!y.png</p>
<p>Inverted-Y = (2<sup>z</sup>-1)-y</p>
<p>If we want a tile from z=9, x=141 and y=214, then y must be inverted. Y becomes 297. The link to the tile becomes:<br/>
<a href="http://earthncseamless.s3.amazonaws.com/9/141/297.png">http://earthncseamless.s3.amazonaws.com/9/141/297.png</a></p>
The tile result is:<br/>
<table class="wiki-content-table">
<tr>
<td><img alt="297.png" class="image" src="http://earthncseamless.s3.amazonaws.com/9/141/297.png"/></td>
</tr>
</table>
<p>For more information on the earthnc.com server, please check:<br/>
<a href="http://earthnc.com/online-nautical-charts">http://earthnc.com/online-nautical-charts</a></p>
<p>The aeronautical and nautical tiles above were found by experimenting with URLs. During general surfing, new maps may be found. By examining the "behind the scenes" data, sometimes a useful link to a tile can be found. One such method for exploring behind the scenes data:<br/>
Firefox: Toolbar &gt; tools &gt; Page Info &gt; Media.</p>
<p>Suppose while examining some "behind the scenes" data, a link like this is found:<br/>
server.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/15/12104/9922.jpg</p>
<p>How would this be used? Some experimentation is in order. First, a simple copy and paste into a browser address bar will produce this:</p>
<table class="wiki-content-table">
<tr>
<td><img alt="9922.jpg" class="image" src="../archive/assets/ArcGIS_rest_services_World_Topo_Map_MapServer_tile_15_12104_9922--72d35563c22e.jpg"/></td>
</tr>
</table>
<p>Determining which element is z, x and y has to be done. Z is pretty easy. The X and Y are not always easy. Figuring this out tends to be bit trial and error. The simplest method is to navigate to the same location with APRSIS32 and check the current tile in the center by double clicking. By comparing the elements in the link above and the return in APRSIS32, it is simple to determine what each element is. In the example above, x and y are simply re-ordered.</p>
<p>The Server and URL still must be determined. The server starts at the beginning and continues up to the first stroke [/]. In this example:<br/>
Server: server.arcgisonline.com</p>
<p>The URL is the rest of the line, starting with the stroke. Each element will be replaced by its placeholder and in the same order as in the link. The result becomes:<br/>
URL: /ArcGIS/rest/services/World_Topo_Map/MapServer/tile/%z/%y/%x.jpg</p>
<p>And by the way, that is also an example of the X and Y in reversed positions.</p>
<p>Notice the .jpg at the end of the URL. The tiles from the ArcGIS server are not png as required in the past. This must be included at the end of the URL.</p>
<p>In the event two elements match but the third is "wrong". It is probably using the Invert-Y. As in the nautical tile example above, use the %!y and try it.</p>
<p>Looking around at the various servers may be enlightening. X and y may be depicted as i and j or maybe as "x=" and "y=". A little experimenting will often result in a new tile. At minimum, a bit of an education may be acquired.</p>
<p>One user has used 25-30 different sets, and the count continues, each with varying degrees of usefulness. Just how useful is a set of 1812 World Map Tiles to APRS anyway? Yet, they are available for an explorer. And, he just added the sets below. He enjoys surfing maps the world over.</p>
<p>For <strong>New Zealand</strong> users there is an option for you!</p>
<p>Recognize this location?</p>
<table class="wiki-content-table">
<tr>
<td><img alt="780.png" class="image" src="../archive/assets/11_2014_780--da31fef88663.png"/></td>
<td><img alt="3122.png" class="image" src="../archive/assets/13_8056_3122--6f952caf9a20.png"/></td>
</tr>
</table>
<p>The following links produce the tiles above:<br/>
<a href="../archive/assets/11_2014_780--da31fef88663.png">http://nz1.nztopomaps.com/11/2014/780.png</a><br/>
<a href="../archive/assets/13_8056_3122--6f952caf9a20.png">http://nz1.nztopomaps.com/13/8056/3122.png</a></p>
<p>These are the settings for the tile set:<br/>
Server: nz1.nztopomaps.com<br/>
URL: /%z-%x-%!y.png<br/>
Zoom Min 5, Zoom Max 15</p>
<p>From <strong>Germany, Switerland, Austria and Italian Alps Region?</strong><br/>
These are a bit strange but maybe usable. I think the world must be gone beyond the borders. :)</p>
<p>Server: s0.outdooractive.com<br/>
URL: /map/AlpsteinSummer/<br/>
Zoom Min 0, Zoom Max 17</p>
<p>And:<br/>
Server: s0.outdooractive.com<br/>
URL: /map/AlpsteinCHESummer/<br/>
Zoom Min 0, Zoom Max 17</p>
<h3 id="toc6"><span>GoogleMaps / YahooMaps</span></h3>
<p>While GoogleMaps, YahooMaps and a number of other online mapping services serve up map tiles in a manner similar to OpenStreetMap, they have restrictive licenses that do not allow unencumbered access to the map tiles, as well as offline tile caching and usage restrictions. Due to these limitations, APRSISCE/32 does not attempt to make use of the tiles from these sources.</p>
</td>
</tr>
</table>

|  |
| --- |
|  |
