---
title: "IS to RF not working"
---

# IS to RF not working

[Source wiki page](https://aprsisce.wikidot.com/forum/t-12464944/is-to-rf-not-working) · [Archived HTML](../archive/pages/forum_t-12464944_is-to-rf-not-working--f075ce4345ff.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» IS to RF not working

Started by: [![KD1ELK](../archive/assets/avatar--edac12f73601.php)](https://www.wikidot.com/user:info/kd1elk)[KD1ELK](https://www.wikidot.com/user:info/kd1elk)  
Date: 03 Sep 2019 15:58  
Number of posts: 4  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-12464944.xml)

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

<span id="fpc-4349454"></span>

<span id="post-4349454"></span>

Fold

<span id="post-title-4349454"></span>

IS to RF not working

[![KD1ELK](../archive/assets/avatar--edac12f73601.php)](https://www.wikidot.com/user:info/kd1elk)[KD1ELK](https://www.wikidot.com/user:info/kd1elk) 03 Sep 2019 15:58

<span id="post-content-4349454"></span>

I am using APRSIS32, FT2DR, an Arrow II Yagi 20 feet up, and TNC3 by Mobilinkd. Everything appears to be working except IS to RF, via the log it shows a ton of RF to IS being gated, and the radio has also transmitted when functioning as a digi repeater as well as my beacons so it isn't a hardware issue. Xmit is checked, Internet and aprs-is is enabled. RF to IS and IS to RF is also checked.

After a few days it has yet to RF a single item from the IS side. I would love to run a 2 way i gate as we have plenty of rx only in the area already.

I have also tested it myself with a HT on less than one watt of power, I get gated, but no acknowledgement and my own setup wont tx messages to me.

Any help would be appreciated!

**Reply**
Options

<span id="post-options-4349454"></span>

Unfold
IS to RF not working by [![KD1ELK](../archive/assets/avatar--edac12f73601.php)](https://www.wikidot.com/user:info/kd1elk)[KD1ELK](https://www.wikidot.com/user:info/kd1elk), 03 Sep 2019 15:58

<span id="fpc-4349464"></span>

<span id="post-4349464"></span>

Fold

<span id="post-title-4349464"></span>

Re: IS to RF not working

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 03 Sep 2019 16:09

<span id="post-content-4349464"></span>

Please double-check what options you have enabled on your APRS-IS and RF ports. I suspect you at least do not have messages enabled on the APRS-IS port because I didn't get any response to my status queries to your KD1ELK-10 station, which I assume is your intended IGate?

Have you confirmed that APRSISCE/32 can actually transmit on your RF? If it won't transmit, then the -IS to RF gating will obviously not work. You might want to check [http://aprsisce.wikidot.com/doc:transmitting](../manual/doc-transmitting.md)

Lynn (D) - KJ4ERJ

**Reply**
Options

<span id="post-options-4349464"></span>

Unfold
Re: IS to RF not working by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 03 Sep 2019 16:09

<span id="fpc-4349466"></span>

<span id="post-4349466"></span>

Fold

<span id="post-title-4349466"></span>

Re: IS to RF not working

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 03 Sep 2019 16:10

<span id="post-content-4349466"></span>

Also, questions like this are better asked on the APRSISCE Yahoo group. You'll likely get a lot more responses of things to check on there.

**Reply**
Options

<span id="post-options-4349466"></span>

Unfold
Re: IS to RF not working by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 03 Sep 2019 16:10

<span id="fpc-4349708"></span>

<span id="post-4349708"></span>

Fold

<span id="post-title-4349708"></span>

Re: IS to RF not working

[![KD1ELK](../archive/assets/avatar--edac12f73601.php)](https://www.wikidot.com/user:info/kd1elk)[KD1ELK](https://www.wikidot.com/user:info/kd1elk) 03 Sep 2019 21:05

<span id="post-content-4349708"></span>

Ok thanks for the tip on the Yahoo group. I will check the aprs-is messages check box but I was pretty careful in selecting everything I could. I can send messages and it triggers my radio TX, as does sending status beacons. I appreciate the help.

**Reply**
Options

<span id="post-options-4349708"></span>

Unfold
Re: IS to RF not working by [![KD1ELK](../archive/assets/avatar--edac12f73601.php)](https://www.wikidot.com/user:info/kd1elk)[KD1ELK](https://www.wikidot.com/user:info/kd1elk), 03 Sep 2019 21:05

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-12464944/is-to-rf-not-working#post-
