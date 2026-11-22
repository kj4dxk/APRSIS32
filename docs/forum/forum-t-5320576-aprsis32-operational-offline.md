---
title: "APRSIS32 Operational Offline?"
---

# APRSIS32 Operational Offline?

[Source wiki page](https://aprsisce.wikidot.com/forum/t-5320576/aprsis32-operational-offline) · [Archived HTML](../archive/pages/forum_t-5320576_aprsis32-operational-offline--3a554d58b573.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» APRSIS32 Operational Offline?

Started by: [![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta)  
Date: 21 Feb 2018 07:15  
Number of posts: 3  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-5320576.xml)

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

<span id="fpc-3717268"></span>

<span id="post-3717268"></span>

Fold

<span id="post-title-3717268"></span>

APRSIS32 Operational Offline?

[![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta) 21 Feb 2018 07:15

<span id="post-content-3717268"></span>

Greetings,

I am new to APRSIS32 and wish to be able to use it for offline operation during internet failures. So far I have been unable to get it to display my TNC received APRS data unless connected to the Internet. It seems to ignore my TNC data stream when offline even though it indicates it is connected OK to my bluetooth linked TNC. Any data in the small window is frozen. As soon as I go online the data stream starts scrolling through again and the stations are displayed on the map as intended.

I wonder if it is possible to use APRSIS32 when offline to display local APRS stations on the map? If yes I wonder what settings I need to change to get it to work?

Thanks in advance!

Tom

**Reply**
Options

<span id="post-options-3717268"></span>

Unfold
APRSIS32 Operational Offline? by [![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta), 21 Feb 2018 07:15

<span id="fpc-3717954"></span>

<span id="post-3717954"></span>

Fold

<span id="post-title-3717954"></span>

Re: APRSIS32 Operational Offline?

[![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta) 22 Feb 2018 00:18

<span id="post-content-3717954"></span>

Greetings,

Further to my last I wish to thank you for accepting my registration to this forum! I would also like those responsible for the development of APRSIS32 which looks like a fine program - no doubt a major effort involving lots of fine work!

Since discovering APRSIS32 yesterday I have found and reviewed parts of the well written pdf user manual. I gather from it that when an RF APRS packet is received and delivered from my BlueTooth TNC that the scroll list entry should be marked with an '\*'. It is also my understanding if the packets are received from the network they should alternately be marked with an '@'. In my case the only associated mark is the station type icon. I wonder if I need to change a setting to get the data source markers?

It would appear APRSIS32 is either not getting or is rejecting my BlueTooth interfaced TNC data. APRSIS32 flashes that it is connected OK and the bluetooth transceiver shows that it is in fact connected to my PC BT port. I have confirmed the data will flow nicely to my favorite ASCII terminal software and to my development system console thus I have confidence in the connection performance. I am wondering if there is a protocol issue. The data stream is the same ASCII APRS report that my smart TNC transmits onto the VHF network which is recognized OK by the aprs.fi web site. I am wondering if APRSIS32 needs a prefix character or something similar possibly needed to discriminate between position reports and other types of data such as messages?

Thanks in advance for any help!

Tom, VA7TA

**Reply**
Options

<span id="post-options-3717954"></span>

Unfold
Re: APRSIS32 Operational Offline? by [![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta), 22 Feb 2018 00:18

<span id="fpc-3718073"></span>

<span id="post-3718073"></span>

Fold

<span id="post-title-3718073"></span>

Re: APRSIS32 Operational Offline?

[![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta) 22 Feb 2018 02:35

<span id="post-content-3718073"></span>

Again further to my last I discovered I needed to use text mode for the port instead of KISS. Thus bluetooth seems to be working OK. Although there are still some close by stations which report OK to aprs.fi that don't appear in the scroll window - but it seems it is now functional.

Tom, VA7TA

**Reply**
Options

<span id="post-options-3718073"></span>

Unfold
Re: APRSIS32 Operational Offline? by [![va7ta](../archive/assets/avatar--8b2522d1cd28.php)](https://www.wikidot.com/user:info/va7ta)[va7ta](https://www.wikidot.com/user:info/va7ta), 22 Feb 2018 02:35

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-5320576/aprsis32-operational-offline#post-
