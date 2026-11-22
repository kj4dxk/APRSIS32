---
title: "Windows CE vs Windows Mobile"
---

# Windows CE vs Windows Mobile

[Source wiki page](https://aprsisce.wikidot.com/cevswinmo) · [Archived HTML](../archive/pages/cevswinmo--25c208b3f4ef.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

One very frequently asked question is:

"Will APRSISCE run on xyz's GPS device using CE 5.0/6.0?"

Short answer: No.

Longer answer, there's no such thing as a "Standard" Windows CE. Every device manufacturer has to make a developer's kit available for their particular build of CE and every software program needs to be compiled individually for that specific platform.

Please read: <http://blogs.msdn.com/b/marcpe/archive/2005/04/22/410778.aspx>

The only CE platform I currently have a build for is the [WebDT 366](webdt-366.md) because that hardware manufacturer was willing and able to give me the SDK with which I could custom build an .EXE targeting their specific features. That version, for instance, doesn't fade the maps because they don't include the AlphaBlend API…

However, the [WebDT 366](webdt-366.md) does include a bluetooth stack with SPP support, so it's trivially easy to pair up a Bluetooth GPS and a Bluetooth to serial device to connect to your TNC. Preload maps via a WiFi feed and you're ready to go mobile.

All of that being said, if you can come up with the Software Developer's Kit (SDK) for one of these popular (and cheap) touch screen Windows CE GPS devices, I'd \*love\* to have it! Just get me the SDK and buy two of the units with one drop-shipped to my QTH. (Just kidding on that last part).

Interesting take on the Win32 API choice: <http://www.mobilephonedevelopment.com/archives/827>
