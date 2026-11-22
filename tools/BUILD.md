# APRSIS32 2026.11.22.0813

This Windows revision is based on Lynn Deffenbaugh’s APRSIS32 2024.09.05.1714 source. The preserved executable and its source have their own [release](https://github.com/kj4dxk/APRSIS32/releases/tag/2024.09.05.1714).

## Map changes

The default OpenStreetMap profile stored HTTP. The preserved executable fetched tiles on the test PC by following the server’s redirect to HTTPS. The new build starts on HTTPS for the stock OpenStreetMap host and the archived `services.arcgisonline.com` profile when they use a standard port. It retains the protocol and port selected for other hosts. It writes the selected protocol to new MBTiles URL metadata, limits network redirects to HTTP and HTTPS, and uses Windows certificate validation through Schannel. OpenStreetMap displays attribution and skips prefetch.

New installations select APRSIS32 Tiles and include an HTTPS OpenStreetMap preset. Existing OpenStreetMap MBTiles caches with legacy HTTP metadata are upgraded in place.

The old OpenCycleMap hostname failed certificate validation during testing. A working provider URL and any provider-required credentials can be entered as a custom tile set.

## Build on Windows

Use an x86 MSVC environment with Windows SDK headers, libraries, and the resource compiler. Activate that environment before running the build script. This revision was built with MSVC 14.44 and Windows SDK 10.0.26100.

Stage the source tree and dependencies out of the repository, using the relative layout in `APRSIS32.vcxproj`:

- `code/aprs/APRSISCE/` contains this repository’s `source/APRSISCE/`.
- `code/aprs/` also contains `include/`, `source/parse.c` with its headers, `gpredict-1.3/`, and `shapelib-1.3.0b2/`.
- `code/` also contains `jpeg/source/`, `lpng/source/`, `lua/lua-5.1.5/src/`, `xp/source/`, `zl/source/`, and `curl/`.

Build the archived libcurl source as a static Win32 library with Schannel and the static C runtime. The NAS copy identifies itself as libcurl 7.71.0-DEV and enables Schannel with `-DCMAKE_USE_WINSSL=ON`. Then run:

```powershell
python tools/build-aprsis32.py --source C:/staging/code/aprs/APRSISCE --curl-library C:/staging/curl-build/lib/libcurl.lib --output C:/staging/APRSIS32-build
```

The script uses the original Visual Studio project’s file list and compiles the application resources. It produces a standalone Win32 `APRSIS32.exe`.

## Local verification

The rebuild of Lynn’s baseline reported Windows file version 2024.09.05.1714. The new executable reported 2026.11.22.0758. Fresh-cache tests fetched tiles from OpenStreetMap, the ArcGIS USA Topo preset, and a custom HTTP server with an explicit port. APRS-IS accepted the configured operator login. The installed local profile contains the operator’s settings and was kept outside Git.

The 2026.11.22.0813 executable migrated a copy of an existing MBTiles cache, preserved all 237 cached tile payloads byte for byte, and fetched three new tiles from OpenStreetMap. A clean install fetched from the default APRSIS32 Tiles server, then from public OpenStreetMap after selecting its preset.
