---
title: "Beacon Remote Control and/or Timer"
---

# Beacon Remote Control and/or Timer

[Source wiki page](https://aprsisce.wikidot.com/forum/t-649282/beacon-remote-control-and-or-timer) · [Archived HTML](../archive/pages/forum_t-649282_beacon-remote-control-and-or-timer--dbb3dfd61874.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Beacon Remote Control and/or Timer

Started by: [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze)  
Date: 02 May 2013 15:38  
Number of posts: 3  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-649282.xml)

Summary:

Questions regarding remote control or preset timer on the beacon functionality.

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

<span id="fpc-1764890"></span>

<span id="post-1764890"></span>

Fold

<span id="post-title-1764890"></span>

Beacon Remote Control and/or Timer

[![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze) 02 May 2013 15:38

<span id="post-content-1764890"></span>

Greetings—

I see the ISS passes over North America this week have occurred largely when I'm at work. I'm curious whether there is a way to set up the APRSISCE/32 beacon to switch itself on at a particular time, then turn itself off. Or whether there is any sort of API that I might utilize remotely to turn it off/on. I do quite a bit of work in PHP, and often think it would be great to write a lightweight web front-end that would have a couple buttons (beacon on/off, etc.) that could be driven remotely (Kenwood RC-D710 + Windows XP).

Thanks & 73's  
KD0KZE

**Reply**
Options

<span id="post-options-1764890"></span>

Unfold
Beacon Remote Control and/or Timer by [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze), 02 May 2013 15:38

<span id="fpc-1765164"></span>

<span id="post-1765164"></span>

Fold

<span id="post-title-1765164"></span>

Re: Beacon Remote Control and/or Timer

[![G6UIM](../archive/assets/avatar--9d0ad7d7006f.php)](https://www.wikidot.com/user:info/g6uim)[G6UIM](https://www.wikidot.com/user:info/g6uim) 02 May 2013 21:59

<span id="post-content-1765164"></span>

Currently the only way to remote APRSIS32 is to remote control the PC. In Lynns plans are to include a LUA engine, so eventually you will be able to code modules to run with APRSIS32. How deep you can go is yet to be decided. And don't hold your breath for this to happen, it's likely a long way in the future.

I don't have issue with remote controlling the PC or even APRSIS32, but just setting up an unattended beacon I do. it's cluttering the short window that a lot of people are trying to use.  
Don't even get me started on people who try to use the mailbox, they are very lucky if they succeed and the messages are not read anyway, all that happens is an entire pass is wasted.

G6UIM steps of soapbox

**Reply**
Options

<span id="post-options-1765164"></span>

Unfold
Re: Beacon Remote Control and/or Timer by [![G6UIM](../archive/assets/avatar--9d0ad7d7006f.php)](https://www.wikidot.com/user:info/g6uim)[G6UIM](https://www.wikidot.com/user:info/g6uim), 02 May 2013 21:59

<span id="fpc-1776741"></span>

<span id="post-1776741"></span>

Fold

<span id="post-title-1776741"></span>

Re: Beacon Remote Control and/or Timer

[![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze) 16 May 2013 19:44

<span id="post-content-1776741"></span>

I agree on the unattended beacons as being problematic for several reasons.

For attended, but remote-control, I'm most familiar with Linux/Apache environment approaches. These may not be the ideal, but for example an SSL cert, .htaccess or other password-protected interface with a beacon on/off toggle button and simple status display. It might lean on PHP running an exec(), system() or related function — or an XML/REST call to run a lower-level executable. You'd turn on the beacon only for the duration of the pass itself, and just for 1 or 2 successful digipeats.

In my home networking environment, I run both Linux and Windows PC's, so I'd face the Linux box outward — as a firewall also — and conduct the RPC-type call behind the router on private IP space.

**Reply**
Options

<span id="post-options-1776741"></span>

Unfold
Re: Beacon Remote Control and/or Timer by [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze), 16 May 2013 19:44

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-649282/beacon-remote-control-and-or-timer#post-
