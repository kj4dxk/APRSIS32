# APRSISCE/32 source snapshot

This directory preserves the newest source snapshot located in the maintainer’s archive.
The C and C++ files, Visual Studio projects, resources, and release notes are stored individually.
The standalone [APRSIS32.exe](APRSISCE/APRSIS32.exe) is the verified Windows build.

| Evidence | September 5, 2024 (EDT) |
| --- | --- |
| Main source file [APRSISCE.cpp](APRSISCE/APRSISCE.cpp), CVS revision 1.168 | 5:13:53 p.m. |
| Version header [Version.h](APRSISCE/Version.h), CVS revision 1.5 | 5:14:20 p.m. |
| Executable’s PE linker timestamp and embedded version 2024.09.05.1714 | 5:14:29 p.m. |

The executable’s SHA-256 is
`43ff6f5e10c64e287f70e37ebe7a10269ad138befdf83be4ffb0794d9c4f8f19`.
It matches the executable distributed through the former download ZIPs byte for byte.

The [Windows project](APRSISCE/APRSIS32/APRSIS32.vcxproj) uses the main source and headers in its parent directory. Its original relative paths also reference libraries and shared source outside the captured folder, including libcurl, Lua, Expat, zlib, Gpredict, and Shapelib. The legacy project has not been rebuilt from this snapshot.

## Preserved configurations and map caches

[Windows project files](APRSISCE/APRSIS32/) include Lynn’s APRSIS32 configuration and dated snapshots. The public copies retain program settings and omit credentials, email addresses, precise location fields, and stored station and message history. The malformed `bad.xml` could not be safely processed.

The [original tile cache](APRSISCE/APRSIS32/osmtiles/) and the three MBTiles containers are preserved byte-for-byte. Default map data is © [OpenStreetMap contributors](https://www.openstreetmap.org/copyright).
