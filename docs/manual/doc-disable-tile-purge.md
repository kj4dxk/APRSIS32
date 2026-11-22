---
title: "Disable OSMTiles Purging"
---

# Disable OSMTiles Purging

OK, about 5 minutes after the yellow circle has expanded and disappeared, double-clicking an empty spot on the map will tell you the current state of your cache. For example, my Windows Mobile has 8,386 tiles over 38.77MB while my desktop has 55,349 tiles over 323.41MB (yes, I zoom all over the world from my desktop). So, how do I make sure they don't get purged?

Simply uncheck Configure / Maps / Purger Enabled!

Disabling Internet access by unchecking Enables / Internet Access also disables map fetching and purging, but they're also individually controllable.

Also, you can prefetch on one machine, lift a copy of the OSMTiles directory tree (or whatever you called it) and drop it onto another machine wherever their configuration is looking for the tiles. You can even pre-fetch on your desktop and drop it all onto your mobile phone if you have a way to preserve the directory/file hierarchy!
