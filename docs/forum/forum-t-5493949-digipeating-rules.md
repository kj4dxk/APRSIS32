---
title: "Digipeating rules"
---

# Digipeating rules

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Digipeating rules

Started by: [![Mark VA2MM](../archive/assets/avatar--c41407aa1fc2.php)](https://www.wikidot.com/user:info/mark-va2mm)[Mark VA2MM](https://www.wikidot.com/user:info/mark-va2mm)  
Date: 15 Mar 2018 20:14  
Number of posts: 1  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

Fine-tuning of what stations to digipeat

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

<span id="fpc-3739744"></span>

<span id="post-3739744"></span>

Fold

<span id="post-title-3739744"></span>

Digipeating rules

[![Mark VA2MM](../archive/assets/avatar--c41407aa1fc2.php)](https://www.wikidot.com/user:info/mark-va2mm)[Mark VA2MM](https://www.wikidot.com/user:info/mark-va2mm) 15 Mar 2018 20:14

<span id="post-content-3739744"></span>

I have APRSIS32 set up as a fill-in <DigiXform>WIDE1-1=WIDE1\*</DigiXform> digipeater and it generally works well as intended. I live in a dense urban environment and the wide2 digis are located on mountains around the city, so my fill-in digi is very helpful for hand-held and low-power mobiles who cannot reliably reach the wide2 digis from my part of town.

I have noticed, however, that there are a few far-away home stations (high antenna, high-power) being heard and digipeated by my station because their path request includes Wide1-1, Wide2-X. These stations can easily reach the high Wide-2 digis directly, so going though me is unnecessarily cluttering the channel.

I tried setting a range filter in APRSIS to 10 miles (16 km) to exclude these far-away stations, but this does not seem to affect RF-to-RF digipeating. So, is there a way to edit the XML file to include range or callsign-specific rules about who is digipeated?

Thanks and 73,

Mark VA2MM

Last edited on 15 Mar 2018 20:17
by [Mark VA2MM](https://www.wikidot.com/user:info/mark-va2mm)
Show more

**Reply**
Options

<span id="post-options-3739744"></span>

Unfold
Digipeating rules by [![Mark VA2MM](../archive/assets/avatar--c41407aa1fc2.php)](https://www.wikidot.com/user:info/mark-va2mm)[Mark VA2MM](https://www.wikidot.com/user:info/mark-va2mm), 15 Mar 2018 20:14

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-5493949/digipeating-rules#post-
