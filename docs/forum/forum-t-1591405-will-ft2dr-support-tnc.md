---
title: "Will FT2DR support TNC?"
---

# Will FT2DR support TNC?

[Source wiki page](https://aprsisce.wikidot.com/forum/t-1591405/will-ft2dr-support-tnc) · [Archived HTML](../archive/pages/forum_t-1591405_will-ft2dr-support-tnc--9a4c211271c2.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Will FT2DR support TNC?

Started by: [![Jim McCusker](../archive/assets/avatar--5137d7b16c0c.php)](https://www.wikidot.com/user:info/jim-mccusker)[Jim McCusker](https://www.wikidot.com/user:info/jim-mccusker)  
Date: 06 Feb 2016 16:05  
Number of posts: 5  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-1591405.xml)

Summary:

Can FT2DR radio be setup to transmit/receive APRS packets using APRSIS32?

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

<span id="fpc-2452687"></span>

<span id="post-2452687"></span>

Fold

<span id="post-title-2452687"></span>

Will FT2DR support TNC?

[![Jim McCusker](../archive/assets/avatar--5137d7b16c0c.php)](https://www.wikidot.com/user:info/jim-mccusker)[Jim McCusker](https://www.wikidot.com/user:info/jim-mccusker) 06 Feb 2016 16:05

<span id="post-content-2452687"></span>

Hello,

I have an FT2DR radio which I've been able to successfully attached to my PC (COM3 port) to send NMEA GPS data. This works great but I'd really like the ability to use this radio as a TNC if that's possible. Does anyone know if there's a way to make this work? I've tried creating different ports but they all fail during communications to the radio.

Thanks,

Jim  
KC3FFX

**Reply**
Options

<span id="post-options-2452687"></span>

Unfold
Will FT2DR support TNC? by [![Jim McCusker](../archive/assets/avatar--5137d7b16c0c.php)](https://www.wikidot.com/user:info/jim-mccusker)[Jim McCusker](https://www.wikidot.com/user:info/jim-mccusker), 06 Feb 2016 16:05

<span id="fpc-2452703"></span>

<span id="post-2452703"></span>

Fold

<span id="post-title-2452703"></span>

Re: Will FT2DR support TNC?

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 06 Feb 2016 17:02

<span id="post-content-2452703"></span>

Yaesu doesn't provide access to the TNC in their APRS radios. The FTM-350 and FTM-400 provide a serial output stream of the APRS packets, but I don't think the APRS HTs provide that option.

**Reply**
Options

<span id="post-options-2452703"></span>

Unfold
Re: Will FT2DR support TNC? by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 06 Feb 2016 17:02

<span id="fpc-2452711"></span>

<span id="post-2452711"></span>

Fold

<span id="post-title-2452711"></span>

Re: Will FT2DR support TNC?

[![G6UIM](../archive/assets/avatar--282a08018b3a.php)](https://www.wikidot.com/user:info/g6uim)[G6UIM](https://www.wikidot.com/user:info/g6uim) 06 Feb 2016 17:45

<span id="post-content-2452711"></span>

I have an FT2DE, and you can output Waypoints, but as is usual with Yaesu you cannot input Waypoints. So it's RX only for APRS

**Reply**
Options

<span id="post-options-2452711"></span>

Unfold
Re: Will FT2DR support TNC? by [![G6UIM](../archive/assets/avatar--282a08018b3a.php)](https://www.wikidot.com/user:info/g6uim)[G6UIM](https://www.wikidot.com/user:info/g6uim), 06 Feb 2016 17:45

<span id="fpc-2452722"></span>

<span id="post-2452722"></span>

Fold

<span id="post-title-2452722"></span>

Re: Will FT2DR support TNC?

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 06 Feb 2016 18:36

<span id="post-content-2452722"></span>

And of course, APRSISCE/32 expects an APRS TNC, not GPS/NMEA waypoint input, so it's not even actually receive-only APRS because not all of the information makes it into the NMEA packets. IIRC, you get the location and station ID, but no information about symbol or comment or any other APRS packet beyond posits.

**Reply**
Options

<span id="post-options-2452722"></span>

Unfold
Re: Will FT2DR support TNC? by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 06 Feb 2016 18:36

<span id="fpc-2453005"></span>

<span id="post-2453005"></span>

Fold

<span id="post-title-2453005"></span>

Re: Will FT2DR support TNC?

[![Jim McCusker](../archive/assets/avatar--f7cd96484b96.php)](https://www.wikidot.com/user:info/jim-mccusker)[Jim McCusker](https://www.wikidot.com/user:info/jim-mccusker) 07 Feb 2016 05:19

<span id="post-content-2453005"></span>

That really stinks. You would think a relatively easy firmware change would enable this feature to output APRS through the data port.

Thanks for the confirmation. I can stop pulling my hair out now. :-)

—Jim

**Reply**
Options

<span id="post-options-2453005"></span>

Unfold
Re: Will FT2DR support TNC? by [![Jim McCusker](../archive/assets/avatar--f7cd96484b96.php)](https://www.wikidot.com/user:info/jim-mccusker)[Jim McCusker](https://www.wikidot.com/user:info/jim-mccusker), 07 Feb 2016 05:19

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-1591405/will-ft2dr-support-tnc#post-
