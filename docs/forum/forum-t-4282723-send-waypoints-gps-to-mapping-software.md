---
title: "Send Waypoints & GPS to Mapping Software"
---

# Send Waypoints & GPS to Mapping Software

[Source wiki page](https://aprsisce.wikidot.com/forum/t-4282723/send-waypoints-gps-to-mapping-software) · [Archived HTML](../archive/pages/forum_t-4282723_send-waypoints-gps-to-mapping-software--51bac795a0a7.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / Feature Requests](forum-c-312348-feature-requests.md)
» Send Waypoints & GPS to Mapping Software

Started by: [![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick)  
Date: 15 Nov 2017 05:36  
Number of posts: 1  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-4282723.xml)

Summary:

A port or feature that sends APRS stations received in to mapping software as waypoints.

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

<span id="fpc-3632857"></span>

<span id="post-3632857"></span>

Fold

<span id="post-title-3632857"></span>

Send Waypoints & GPS to Mapping Software

[![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick) 15 Nov 2017 05:36

<span id="post-content-3632857"></span>

I have been trying to find a simpler way to plot a route on my mapping software (Delorme Street Atlas) to a station on APRS. As of yet, I haven't found anything much short of copying GPS coordinates and searching for them in the mapping software, and then plotting a route. Either that, or eye-balling the rough area.

But I have just learned that the Kenwood TH-D7 and other Kenwood radios have the ability to send waypoints (& and GPS information) to garmin devices. And those devices can then make routes to the received stations. I saw this in a video www(dot)youtube(dot)com/watch?v=yxK1O4duO4s, and also on this site www(dot)hulleng(dot)karoo(dot)net/g0vrm/content/aprs/garmin(dot)htm

It looks like the raw data heading to the Garmin looks like this.  
$GPWPL,5346.17,N,00030.15,W,M1BLR \*5C  
$GPWPL,5342.91,N,00027.05,W,N TOWE\*36  
$GPWPL,5344.03,N,00025.68,W,G0VRM\*6B

So this is what I want for Christmas! A way to take the stations that APRSIS/32 is receiving and forward them, along with my personal GPS position data, to my mapping software. This way I can plot routes to the waypoints.

73  
Tim D.  
KD8LKQ

**Reply**
Options

<span id="post-options-3632857"></span>

Unfold
Send Waypoints & GPS to Mapping Software by [![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick), 15 Nov 2017 05:36

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-4282723/send-waypoints-gps-to-mapping-software#post-
