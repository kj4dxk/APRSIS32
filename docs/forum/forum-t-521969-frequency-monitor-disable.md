---
title: "Frequency Monitor Disable?"
---

# Frequency Monitor Disable?

[Source wiki page](https://aprsisce.wikidot.com/forum/t-521969/frequency-monitor-disable) · [Archived HTML](../archive/pages/forum_t-521969_frequency-monitor-disable--31412c73353a.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Frequency Monitor Disable?

Started by: [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu)  
Date: 13 Jul 2012 13:10  
Number of posts: 5  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-521969.xml)

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

<span id="fpc-1507073"></span>

<span id="post-1507073"></span>

Fold

<span id="post-title-1507073"></span>

Frequency Monitor Disable?

[![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu) 13 Jul 2012 13:10

<span id="post-content-1507073"></span>

Hello,

Just started using the latest 5/25 release. I've turned on Frequency Monitor under the Enables menu, but I can't seem to turn it off again. Clicking the menu option again just brings up another frequency monitor window.

I was able to turn it off by editing the aprsis32.xml file directly.

N4TQU

**Reply**
Options

<span id="post-options-1507073"></span>

Unfold
Frequency Monitor Disable? by [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu), 13 Jul 2012 13:10

<span id="fpc-1507084"></span>

<span id="post-1507084"></span>

Fold

<span id="post-title-1507084"></span>

Re: Frequency Monitor Disable?

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 13 Jul 2012 13:23

<span id="post-content-1507084"></span>

With a Frequency Monitor visible, try the unchecking the Enables / Frequency Monitor.

Or easier yet, just right click on the Frequency Monitor window and uncheck Enabled in that popup menu.

APRSISCE/32's action when selecting the original Enable is to show the monitor if it is not already visible. If it IS visible when you try to uncheck the Enables / Frequency Monitor, it will actually disable it.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1507084"></span>

Unfold
Re: Frequency Monitor Disable? by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 13 Jul 2012 13:23

<span id="fpc-1507099"></span>

<span id="post-1507099"></span>

Fold

<span id="post-title-1507099"></span>

Re: Frequency Monitor Disable?

[![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu) 13 Jul 2012 13:38

<span id="post-content-1507099"></span>

Hello Lynn,

Thanks for the quick response. Yes that does work just like you said. As a long time Windows user this does not feel like expected behavior.

Another thing I've noticed is that when this monitor is enabled it pulls application focus to APRSIS when a new frequency is detected. Is this expected?

N4TQU

**Reply**
Options

<span id="post-options-1507099"></span>

Unfold
Re: Frequency Monitor Disable? by [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu), 13 Jul 2012 13:38

<span id="fpc-1507113"></span>

<span id="post-1507113"></span>

Fold

<span id="post-title-1507113"></span>

Re: Frequency Monitor Disable?

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 13 Jul 2012 13:54

<span id="post-content-1507113"></span>

I play fast and loose with some menu options. Sometimes selecting a checked option will simply uncheck it. Sometimes it will bring up whatever was enabled and expect you to uncheck it again if that was your intent (using the menu option as a temporary "show me"), and sometimes unchecking an enabled option will come up and ask if you want to simply disable or completely remove the option.

No, that is not the expected behavior. Mine doesn't do that, but then again, mine is constantly updating and never disappears on the time.

Does it only grab focus when a new frequency is detected and the window isn't already visible, or does it even do it with an existing window that updates with a new frequency?

What version of Windows are you running, it may make a difference as I'm on Win 7 Pro 64 bit.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1507113"></span>

Unfold
Re: Frequency Monitor Disable? by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 13 Jul 2012 13:54

<span id="fpc-1507122"></span>

<span id="post-1507122"></span>

Fold

<span id="post-title-1507122"></span>

Re: Frequency Monitor Disable?

[![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu) 13 Jul 2012 14:03

<span id="post-content-1507122"></span>

I've only seen it grab focus when the window is not visible. Running Windows 7 Home Premium 64bit on this particular machine.

I like the functionality of the frequency monitoring feature, but seeing it as a small overlay on the map would be more useful to me. Just my 2 cents.

N4TQU

**Reply**
Options

<span id="post-options-1507122"></span>

Unfold
Re: Frequency Monitor Disable? by [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu), 13 Jul 2012 14:03

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-521969/frequency-monitor-disable#post-
