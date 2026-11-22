---
title: "HF iGATE"
---

# HF iGATE

[Source wiki page](https://aprsisce.wikidot.com/forum/t-691285/hf-igate) · [Archived HTML](../archive/pages/forum_t-691285_hf-igate--afd295725435.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» HF iGATE

Started by: [![Selwyn Staples](../archive/assets/avatar--5c63551ccc2a.php)](https://www.wikidot.com/user:info/selwyn-staples)[Selwyn Staples](https://www.wikidot.com/user:info/selwyn-staples)  
Date: 04 Sep 2013 09:43  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-691285.xml)

Summary:

Map overflowing with VHF traffic

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

<span id="fpc-1845455"></span>

<span id="post-1845455"></span>

Fold

<span id="post-title-1845455"></span>

HF iGATE

[![Selwyn Staples](../archive/assets/avatar--5c63551ccc2a.php)](https://www.wikidot.com/user:info/selwyn-staples)[Selwyn Staples](https://www.wikidot.com/user:info/selwyn-staples) 04 Sep 2013 09:43

<span id="post-content-1845455"></span>

Greetings,

I am using APRSIS32 as a HF iGate on 10.147MHz  
Settings are as follows:  
Config - port - Localhost [AGW] - RF to IS  
Config - scroller - RF only  
View- chooser - RF Direct  
The scroller only shows incoming HF traffic, however the map is cluttered with VHF traffic.  
How do I eliminate the VHF symbols???

Regards,

Sel [zs6ss]

**Reply**
Options

<span id="post-options-1845455"></span>

Unfold
HF iGATE by [![Selwyn Staples](../archive/assets/avatar--5c63551ccc2a.php)](https://www.wikidot.com/user:info/selwyn-staples)[Selwyn Staples](https://www.wikidot.com/user:info/selwyn-staples), 04 Sep 2013 09:43

<span id="fpc-1851421"></span>

<span id="post-1851421"></span>

Fold

<span id="post-title-1851421"></span>

Re: HF iGATE

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 13 Sep 2013 18:48

<span id="post-content-1851421"></span>

The APRS-IS system really doesn't know if a packet arrived via HF or VHF. I suspect the reason you're getting VHF stations is that you have not changed the default Configure / General / Range of 50 miles to be something smaller, like 1 or 0. APRSISCE/32 automatically provides a filter for the APRS-IS and the Range is included in that filter resulting in "close" stations being received with no user effort (unlike some other clients that require entry of a filter to receive anything).

Also make sure that you don't have anything in Configure / General / Add Filter because that will also cause APRS-IS traffic to be received.

Another way is to check View / None followed by View / RF / All. This will cause the map to only show stations that you received on your own RF port and will suppress everything received from the APRS-IS.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1851421"></span>

Unfold
Re: HF iGATE by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 13 Sep 2013 18:48

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-691285/hf-igate#post-
