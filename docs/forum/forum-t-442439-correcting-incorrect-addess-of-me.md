---
title: "Correcting incorrect addess of \"ME\""
---

# Correcting incorrect addess of "ME"

[Source wiki page](https://aprsisce.wikidot.com/forum/t-442439/correcting-incorrect-addess-of-me) · [Archived HTML](../archive/pages/forum_t-442439_correcting-incorrect-addess-of-me--55f2880facf7.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Correcting incorrect addess of "ME"

Started by: [![gfmucci](../archive/assets/avatar--71706d42835c.php)](https://www.wikidot.com/user:info/gfmucci)[gfmucci](https://www.wikidot.com/user:info/gfmucci)  
Date: 12 Feb 2012 03:52  
Number of posts: 4  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-442439.xml)

Summary:

The wrong address is shown for me

Unfold All
Fold All
<span id="thread-toggle-options"></span>
More Options

<span id="thread-options-2"></span>

Edit Title & Description
Stickness
Lock Thread
Move Thread

<span id="thread-action-area"></span><span id="thread-container"></span>

<span id="thread-container-posts"></span>

<span id="fpc-1372133"></span>

<span id="post-1372133"></span>

Fold

<span id="post-title-1372133"></span>

Correcting incorrect addess of "ME"

[![gfmucci](../archive/assets/avatar--71706d42835c.php)](https://www.wikidot.com/user:info/gfmucci)[gfmucci](https://www.wikidot.com/user:info/gfmucci) 12 Feb 2012 03:52

<span id="post-content-1372133"></span>

How do I correct the incorrect ME address shown on the map. I properly and precisely located my ME location.

I reviewed these instructions:

When clicking on a station and then lookup address APRSIS attempts to find the street address of the stations current location via Geonames

GeoNames is integrating geographical data such as names of places in various languages, elevation, population and others from various sources. All lat/long coordinates are in WGS84 (World Geodetic System 1984). **Users may manually edit, correct and add new names using a user friendly wiki interface.**

But that last sentence is unclear to me. Where/what is "a user friendly wiki interface?" That needs to be better explained.

Last edited on 12 Feb 2012 03:57
by [gfmucci](https://www.wikidot.com/user:info/gfmucci)
Show more

**Reply**
Options

<span id="post-options-1372133"></span>

Unfold
Correcting incorrect addess of "ME" by [![gfmucci](../archive/assets/avatar--71706d42835c.php)](https://www.wikidot.com/user:info/gfmucci)[gfmucci](https://www.wikidot.com/user:info/gfmucci), 12 Feb 2012 03:52

<span id="fpc-1374569"></span>

<span id="post-1374569"></span>

Fold

<span id="post-title-1374569"></span>

Re: Correcting incorrect addess of "ME"

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 15 Feb 2012 14:48

<span id="post-content-1374569"></span>

APRSISCE/32's Lookup Address menu option uses data from GeoNames.org which provided the quote above contained in the Wiki. If you visit the GeoNames User Manual at <http://www.geonames.org/manual.html> you'll find information on editing their data.

Addresses are not internal to APRSISCE/32, but come from GeoNames.org, specifically the extendedFindNearby API for those interested in the internal details.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

PS. I hope to one day support address entry and resolution to a Lat/Lon for centering as well, but don't hold your breath waiting for it!

**Reply**
Options

<span id="post-options-1374569"></span>

Unfold
Re: Correcting incorrect addess of "ME" by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 15 Feb 2012 14:48

<span id="fpc-1374549"></span>

<span id="post-1374549"></span>

Fold

<span id="post-title-1374549"></span>

Re: Correcting incorrect addess of "ME"

[![gfmucci](../archive/assets/avatar--71706d42835c.php)](https://www.wikidot.com/user:info/gfmucci)[gfmucci](https://www.wikidot.com/user:info/gfmucci) 15 Feb 2012 14:20

<span id="post-content-1374549"></span>

I think I fouond the solution to the incorrect addess for "Me". I had "Ambiguity" checked under Screen:Labels:Ambiguity. Uncheck "Ambiguity" and the correct address for "Me" will be displayed when you check left click "Me":Address. Ambiguity is checked for those who don't want their exact whereabouts known.

**Reply**
Options

<span id="post-options-1374549"></span>

Unfold
Re: Correcting incorrect addess of "ME" by [![gfmucci](../archive/assets/avatar--71706d42835c.php)](https://www.wikidot.com/user:info/gfmucci)[gfmucci](https://www.wikidot.com/user:info/gfmucci), 15 Feb 2012 14:20

<span id="fpc-1374564"></span>

<span id="post-1374564"></span>

Fold

<span id="post-title-1374564"></span>

Re: Correcting incorrect addess of "ME"

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 15 Feb 2012 14:43

<span id="post-content-1374564"></span>

Sorry, but Screen / Labels / Ambiguity isn't what fixed your address. That option simply controls whether or not APRSISCE/32 will display purple ambiguity circles around stations that are transmitting ambiguous coordinates. APRSISCE/32 will transmit ambiguity (but not use it for displaying ME) via the Configure / Beacon / Precision. This same setting supports transmitting enhanced precision via the !DAO! construct (<http://www.aprs.org/datum.txt>).

As soon as I can research it, I'll respond to your original question.

Lynn (D) - KJ4ERJ - Author of APRSISSCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1374564"></span>

Unfold
Re: Correcting incorrect addess of "ME" by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 15 Feb 2012 14:43

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-442439/correcting-incorrect-addess-of-me#post-
