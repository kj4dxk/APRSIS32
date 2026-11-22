---
title: "IS-Server Port / Discussion"
---

# IS-Server Port / Discussion

[Source wiki page](https://aprsisce.wikidot.com/forum/t-421543/is-server-port) · [Archived HTML](../archive/pages/forum_t-421543_is-server-port--a5e5871818ff.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [WIKI per page discussion / Per page discussions](forum-c-494256-per-page-discussions.md)
» IS-Server Port

Started by: Wikidot  
Date: 15 Dec 2011 05:35  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-421543.xml)

This is the discussion related to the wiki page [IS-Server Port](../manual/is-server-port.md).

Unfold All
Fold All
<span id="thread-toggle-options"></span>
More Options

<span id="thread-options-2"></span>

Lock Thread
Move Thread

<span id="thread-action-area"></span><span id="thread-container"></span>

<span id="thread-container-posts"></span>

<span id="fpc-1431623"></span>

<span id="post-1431623"></span>

Fold

<span id="post-title-1431623"></span>

Missing option

[![KD2AVG](../archive/assets/avatar--76acfaa5e14d.php)](https://www.wikidot.com/user:info/kd2avg)[KD2AVG](https://www.wikidot.com/user:info/kd2avg) 28 Apr 2012 19:15

<span id="post-content-1431623"></span>

Was this feature removed in Build 2012/04/06? I'm not seeing IS-Server listed in the available choices in new ports.

I'm hoping to use this feature for a server-client setup. I run my own server using VMWare ESXi 5.0 and want to run virtual machine as an APRS iGate. My main desktop computer I would then like to use as the APRIS32 client in order to connect to and pull/push information via the virtual machine.

Thanks for the help!

Tom  
KD2AVG

**Reply**
Options

<span id="post-options-1431623"></span>

Unfold
Missing option by [![KD2AVG](../archive/assets/avatar--76acfaa5e14d.php)](https://www.wikidot.com/user:info/kd2avg)[KD2AVG](https://www.wikidot.com/user:info/kd2avg), 28 Apr 2012 19:15

<span id="fpc-1431681"></span>

<span id="post-1431681"></span>

Fold

<span id="post-title-1431681"></span>

Re: Missing option

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 28 Apr 2012 20:57

<span id="post-content-1431681"></span>

This feature has not been removed, but in fact, extended. There is now a "Local-Server" in addition to the original "IS-server". The difference between them is that the IS-Server requires authentication and a filter before returning any data. The "Local-Server" type port will spew all packets received as soon as a connection is made. If no authentication is provided, incoming packets on a Local-Server port are ignored.

Both of these port types still require you to be running the Development version. You can check which version you're running via the About box.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1431681"></span>

Unfold
Re: Missing option by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 28 Apr 2012 20:57

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-421543/is-server-port#post-
