---
title: "Map Prefetch"
---

# Map Prefetch

(Note: this page may be a bit redundant as it is composed of three different e-mail interactions. It'll get cleaned up, merged, and maybe split out eventually.)

What's with this new feature of "Prefetching" OSM Map Tiles? Why would I want to clutter up my hard drive with such stuff? Why not just let them download when they're needed?

Well, with the new KISS and AGW interfaces, APRSISCE/32 has moved beyond the assumption of Internet connectivity. In an RF-only APRS environment, it would still be nice to see the OSM map tiles; hence the prefetch option.

To use it, simply zoom in or out and pan around until your map view shows the area whose maps you want to prefetch. Then simply select Screen/Map/Prefetch. APRSISCE/32 will calculate how many tiles will be necessary to have the current zoom and two additional zoom levels at the current size of your window. When you accept this confirmation, ALL of those tiles are queued for fetching and you get to watch an expanding circle moving slower than you've probably seen it move to date. Don't be alarmed if the number of tiles queued doesn't match the original estimate. The second number takes into account tiles that were already resident in your tile cache.

After prefetching the tiles for a fairly large area, I zoom down a bit and continue prefetching more details and panning around fetching more and more details. It is, however, important to wait for each prefetch to complete before dragging the map around. APRSISCE/32 is smart enough to think that you've moved the map, so you must not need the stuff that was queued, so it will flush the queue and start over at the new location. Probably not what you want.

When you DO go mobile without an Internet connection, just toggle off Enables/OSM Fetch Enabled and APRSISCE/32 will not attempt to load any additional tiles. If you zoom in further than you did while prefetching, APRSISCE/32 will simplly stretch the closest available tile resulting in a grainier looking map than you would otherwise have, but at least you have a map!

Again, let me know if any of you are using this feature and how I can make it better (other than making the prefetch-queued tiles not flush when you pan around to queue more). BTW, double-clicking an empty space on the map will tell you how many tiles covering how much space you currently have on your machine as well as how long it has been since they've been referenced (since creation currently on Windows Mobile). The XML configuration file has a parameter called OSM.RetainDays, but you really don't want to mess with this. Instead, see Configure / Map / Tile Sets / Original (or your own custom tile set) and uncheck the Purge Enabled box. You can also change the purger days from this same configuration dialog.

Alternatively:

There's a menu option under Screen / Map / Prefetch that will handle the prefetching of the maps. Then you need to configure the map tile set to make sure the tile purger doesn't do away with them. Here's the procedure:

Only do the following when you have an Internet connection and time to wait for the prefetcher to pull in the map tiles! We're talking thousands of them!

1) Zoom and pan the map so that an area of interest is all that is visible. On Win32 you can even size the window to help focus on the interested area.

2) Select Screen / Map / Prefetch. The client will tell you how many tiles cover the area including several zoom levels closer. Don't panic, this will be a large number!

3) Once you click Yes, the client will queue all of the missing map tiles. This can take a while (even just to queue them) so be patient. Once all have been queued, the actual count will be displayed. This may be less than the original count because some of the tiles have probably already been cached.

4) The yellow circle will begin expanding as the tiles are fetched, but you don't need to wait for it. Double-clicking an empty spot on the map will provide you some visibility into the queue length and the states of the threads doing the retrieval.

5) You can now repeat steps 1 through 4 at various zoom levels to make sure you have all of the tiles you want. If you miss some, the client will simply stretch the next higher zoom level to cover the hole if/when you get into one.

You can still pre-fetch additional tiles whenever you have a connection. However, when you're going to be connectionless, I would recommend unchecking the Enables / OSM Fetch Enabled. This will prevent the client from even attempting a connection to the OSM server for any missing tiles in the event it needs one while you know you'll be out of coverage. It also turns off the tile purger, but only until you enable the fetcher again, so if you're really going Internet-less, you probably want to manually disable the purger by unchecking the Purge Enabled box in your current tile set. See Configure / Map / Tile Sets / Original (or your own custom tile set) and uncheck the Purge Enabled box. You can also change the purger days from this same configuration dialog.

from **N7FMH:**

Is there a reason to limit the prefetch depth? Why not allow the user the ability to download to any depth?

I limit depth because of the 4x factor that increasing zoom levels represents. When I say that I pre-fetch the next lower N zoom levels, it means the ENTIRE area expanded by 4x for each zoom level closer. Many times, this is not what the user really wants to see in detail. For instance, why pre-fetch lots of ocean tiles just because the lower left corner of the United States happens to be a peninsula?

I would entertain going deeper if the prefetch constrained itself to the same are that would be viewed by zooming in at the current screen size, but again, that's probably not what most users are looking for. Once you get in to a certain area, they really want the 4x expansion as it goes deeper.

My own manual method is to get a reasonable area at a reasonable zoom and trigger the prefetch. I then zoom in 2 or 3 times and pan around triggering more prefetches until I think I've got enough queued. Then I zoom in to a reasonable street level and pan around slowly while watching for the fuzziness to disappear. At my origination and destination points, I zoom in really deep to have the final detail and/or back off 1 or 2 levels and trigger a final few prefetchs.

The client does do some automatic prefetching of one zoom level further out and the 4 tiles one level closer (along with one additional tile at the current zoom level out each edge of the screen) each time a new tile is fetched from the server. This is in anticipation of traveling into those areas or doing a small zoom for less or more detail.

Note that the automatic prefetchs of map tiles due to panning and zooming are canceled if your attention has moved elsewhere (by more panning and zooming) before the tiles were fetched. Manual Prefetch requests are not canceled nor cancellable short of closing the client.

from **KC9GQR:**

When prefetching tiles, is there a way to change how many zoom levels is prefetched? I think its 5 but can you change it to like 15 in the xml file?

See my response to N7FMH to see why this is not necessarily a good idea. It has to do with what area coverage is desired for the increasing zoom levels, all of the area originally covered or just the center as if you had manually zoomed the whole way in at a constant screen center. The client currently does the former on the prefetch which results in a 4x tile count for each zoom closer. The latter does a constant tile count per level, but doesn't end up fetching detail for ALL of the area originally viewed.

See the table at <http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Zoom_levels> to get an idea of the total tile count for various zooms. For your suggestion of 15, if you did this while zoomed out to the planet view, you'd end up fetching (left column is zoom tile count, right column is cumulative total).

|  |  |  |
| --- | --- | --- |
| Zoom Level | Tile count | Cumulative Total |
| 1 | 1 |  |
| 2 | 4 | 5 |
| 3 | 16 | 21 |
| 4 | 64 | 85 |
| 5 | 256 | 341 |
| 6 | 1024 | 1365 |
| 7 | 4096 | 5461 |
| 8 | 16384 | 21845 |
| 9 | 65536 | 87381 |
| 10 | 262144 | 349525 |
| 11 | 1048576 | 1398101 |
| 12 | 4194304 | 5592405 |
| 13 | 16777216 | 22369621 |
| 14 | 67108864 | 89478485 |
| 15 | 268435456 | 357913941 |
| 16 | 1073741824 | 1431655765 |
| 17 | 4294967296 | 5726623061 |
| 18 | 17179869184 | 22906492245 |

Oh, and you'd have to multiply all of those numbers by the number of tiles it takes to fill the screen from which you did the Prefetch.
