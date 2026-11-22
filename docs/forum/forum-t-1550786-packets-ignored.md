---
title: "Packets Ignored"
---

# Packets Ignored

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Packets Ignored

Started by: [![Brian - KG7AFQ](../archive/assets/avatar--4f0775757154.php)](https://www.wikidot.com/user:info/brian-kg7afq)[Brian - KG7AFQ](https://www.wikidot.com/user:info/brian-kg7afq)  
Date: 12 Jan 2016 23:49  
Number of posts: 1  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

APRSIS32 does not see some packets I am sending to it

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

<span id="fpc-2438534"></span>

<span id="post-2438534"></span>

Fold

<span id="post-title-2438534"></span>

Packets Ignored

[![Brian - KG7AFQ](../archive/assets/avatar--4f0775757154.php)](https://www.wikidot.com/user:info/brian-kg7afq)[Brian - KG7AFQ](https://www.wikidot.com/user:info/brian-kg7afq) 12 Jan 2016 23:49

<span id="post-content-2438534"></span>

I am developing an APRS IS Messaging server. When I test using APRSIS32 things go well, mostly.

I am using Windump to trace packets from my Win7 machine to/from an APRS Tier 2 server.

The first two packets below go well. APRSIS32 sends a message. It then receives and ACK. All is well.

The third packet below is the problem. It is sent by the server to the APRSIS32 client but the client seems to ignore it.  
I must be creating an ill formed packet at the server. Can anyone see the what may be wrong with the third packet here and why APRSIS32 would be ignoring it.?

Packet 1 - 10.0.222.99.51904 > 44.10.10.2.14580  
KG7AFQ-9>APWW10,WIDE1-1,WIDE2-1::PROVMAR..:r1,ly31,0224{EV}

Packet 2 - 44.10.10.2.14580 > 10.0.222.99.51904  
PROVMAR>APZ009,TCPIP\*,qAS,KG7AFQ::KG7AFQ-9.:ackEV}

Packet 3 - 44.10.10.2.14580 > 10.0.222.99.51904  
PROVMAR>APZ009,TCPIP\*,qAS,KG7AFQ::KG7AFQ-9:r1,ly31,0224,1452107238,1452107238,All.is.well{92135

**Reply**
Options

<span id="post-options-2438534"></span>

Unfold
Packets Ignored by [![Brian - KG7AFQ](../archive/assets/avatar--4f0775757154.php)](https://www.wikidot.com/user:info/brian-kg7afq)[Brian - KG7AFQ](https://www.wikidot.com/user:info/brian-kg7afq), 12 Jan 2016 23:49

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-1550786/packets-ignored#post-
