---
title: "Autostart of APRS-IS (After reboot)"
---

# Autostart of APRS-IS (After reboot)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Autostart of APRS-IS (After reboot)

Started by: [![LA1BCA](../archive/assets/avatar--c6cbc25ddd02.php)](https://www.wikidot.com/user:info/la1bca)[LA1BCA](https://www.wikidot.com/user:info/la1bca)  
Date: 19 Sep 2017 15:35  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

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

<span id="fpc-3597429"></span>

<span id="post-3597429"></span>

Fold

<span id="post-title-3597429"></span>

Autostart of APRS-IS (After reboot)

[![LA1BCA](../archive/assets/avatar--c6cbc25ddd02.php)](https://www.wikidot.com/user:info/la1bca)[LA1BCA](https://www.wikidot.com/user:info/la1bca) 19 Sep 2017 15:35

<span id="post-content-3597429"></span>

Hi,

I have been surfing around here in the forum to find something about a problem I am not aware of what can be the reason.!  
What happen is - the APRS-IS software will not transmit when it starts after an 'exit' of the software or after a 'reboot' om my computer.!  
I have copied the shortcut to the software into the Windows Startup folder, so the software starts together with Windows startup.

(The APRS-IS starts in 'Suspended Mode' and the transmit need to be done 'manually' every time after starting up the software - to confirm the location.)

Are there any settings to fix this problem.??

This queston have already been up here in the forum - from KE5UUO,  
but no answers to that post.!

I would like to know if someone can give me a clue of sulution for this..!!

**Reply**
Options

<span id="post-options-3597429"></span>

Unfold
Autostart of APRS-IS (After reboot) by [![LA1BCA](../archive/assets/avatar--c6cbc25ddd02.php)](https://www.wikidot.com/user:info/la1bca)[LA1BCA](https://www.wikidot.com/user:info/la1bca), 19 Sep 2017 15:35

<span id="fpc-3597437"></span>

<span id="post-3597437"></span>

Fold

<span id="post-title-3597437"></span>

Re: Autostart of APRS-IS (After reboot)

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 19 Sep 2017 15:44

<span id="post-content-3597437"></span>

I suspect you have Configure / Beacon / After Transmit checked. This option is designed for users that have no GPS, but yet are mobile (for instance, on a trip checking into different hotels). If this is checked, APRSISCE/32 will not transmit the saved "stale" location until the operator has had a chance to re-center the map on the current location and then manually hit Transmit moving ME to the new screen center.

If you do not have Configure / Beacon / After Transmit checked, please repost and I'll noodle for another possible cause.

**Reply**
Options

<span id="post-options-3597437"></span>

Unfold
Re: Autostart of APRS-IS (After reboot) by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 19 Sep 2017 15:44

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-3662193/autostart-of-aprs-is-after-reboot#post-
