---
title: "Frequency Monitor"
---

# Frequency Monitor

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Frequency Monitor

Started by: [![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick)  
Date: 21 Jan 2017 05:52  
Number of posts: 3  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

How do I broadcast what frequencies I'm monitoring when I'm out and about?

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

<span id="fpc-2730866"></span>

<span id="post-2730866"></span>

Fold

<span id="post-title-2730866"></span>

Frequency Monitor

[![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick) 21 Jan 2017 05:52

<span id="post-content-2730866"></span>

I've seen a number of users broadcast what frequencies they are monitoring when they pop-up on the Frequency Monitor. How do I take advantage of this feature, and broadcast the frequencies I'm monitoring when I'm out and about?

I have done my due diligents in searching the wiki, but all I can figure is that it has something to do with creating objects.

Thanks!  
KD8LKQ - Tim D.

**Reply**
Options

<span id="post-options-2730866"></span>

Unfold
Frequency Monitor by [![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick), 21 Jan 2017 05:52

<span id="fpc-3621747"></span>

<span id="post-3621747"></span>

Fold

<span id="post-title-3621747"></span>

Re: Frequency Monitor

[![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick) 29 Oct 2017 00:49

<span id="post-content-3621747"></span>

Bump.

**Reply**
Options

<span id="post-options-3621747"></span>

Unfold
Re: Frequency Monitor by [![TchGimmick](../archive/assets/avatar--c0142ee4b3e8.php)](https://www.wikidot.com/user:info/tchgimmick)[TchGimmick](https://www.wikidot.com/user:info/tchgimmick), 29 Oct 2017 00:49

<span id="fpc-3621784"></span>

<span id="post-3621784"></span>

Fold

<span id="post-title-3621784"></span>

Re: Frequency Monitor

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 29 Oct 2017 02:47

<span id="post-content-3621784"></span>

No, it actually does't have anything to do with creating objects, but you can create objects that advertise repeater frequencies, tones, and offsets.

Kenwood and Yaesu APRS-capable radios have features that automatically embed the "other" band's frequency information into their APRS packets. If you want to do this, you'll have to modify your station's beacon text to include frequency information using the formats that you've apparently already found on the web.

In it's simplest form, simply put FFF.FFFMHz as the first characters of your beacon status text. It MUST be 3 digits decimal 3 digits. Zero fill and/or pad as necessary. It also MUST be MHz (not Mhz nor MHZ) in order for Yaesu radios to understand that it's a frequency specification.

Other optional parameters can follow with blank delimited 4 character sets of text providing tone, offset, and other things.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

PS. Frequency specification is all about the APRS protocol and doesn't really have anything to do with a particular APRS client.

**Reply**
Options

<span id="post-options-3621784"></span>

Unfold
Re: Frequency Monitor by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 29 Oct 2017 02:47

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-2067078/frequency-monitor#post-
