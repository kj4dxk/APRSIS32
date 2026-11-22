---
title: "Development Version"
---

# Development Version

[Source wiki page](https://aprsisce.wikidot.com/doc:development-version) · [Archived HTML](../archive/pages/doc_development-version--013d89ee608b.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Development Version** |

|  |
| --- |
| To those that were asking where the Development version is, you need to edit the <Update.Development> tag in the APRSISCE/32.XML file following the instructions at [Editing XML](doc-editing-xml-configuration.md) Once the element is set to 1, the normal version check (About box) and update mechanism automatically tracks the latest development build instead of the released build. Changing back to 0 will go back to the release build for the upgrade checker, but…  Be aware that some development versions make changes to the XML configuration file, so save a copy somewhere or set up the development execution from a different directory. APRSISCE/32 does make automatic version-dated copies of the About-upgraded .EXEs and .XML configuration files, but I have not yet documented how to use them (easy enough though, rename a matching pair to their original names and double-click).  I make no promise that any given Development release will function properly in all areas, although that is certainly my goal. As the product increases in features and function, it also increases in complexity and every increase in complexity introduces more and more places for errors in the code. My track record is pretty good (I think there's only been 3 or 4 times that the Dev release crashed in different installations), but please don't perform this change unless you're willing to put up with buggy software, even for a little while (wait, you're running Windows, you're used to putting up with buggy software, right?).  Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32 |

|  |
| --- |
|  |
