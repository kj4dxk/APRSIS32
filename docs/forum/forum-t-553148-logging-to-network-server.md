---
title: "Logging to network server"
---

# Logging to network server

[Source wiki page](https://aprsisce.wikidot.com/forum/t-553148/logging-to-network-server) · [Archived HTML](../archive/pages/forum_t-553148_logging-to-network-server--bc4a35a8c069.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / Feature Requests](forum-c-312348-feature-requests.md)
» Logging to network server

Started by: [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu)  
Date: 19 Aug 2012 18:22  
Number of posts: 4  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-553148.xml)

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

<span id="fpc-1542001"></span>

<span id="post-1542001"></span>

Fold

<span id="post-title-1542001"></span>

Logging to network server

[![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu) 19 Aug 2012 18:22

<span id="post-content-1542001"></span>

I'm working on a solution to parse and store various bits of APRS traffic from the APRSIS32.log file. It would be nice to have a option to send logs to a network server directly from within the application. I'm currently using the Unix tail and netcat utilities that have been ported to Windows to accomplish this task.

Currently the flow looks like:  
APRSIS32 -> APRSIS32.log -> tail -> nc —-> remote server -> python script -> data store

What I'd like to see:  
APRSIS32 —-> remote server -> python script -> data store

**Reply**
Options

<span id="post-options-1542001"></span>

Unfold
Logging to network server by [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu), 19 Aug 2012 18:22

<span id="fpc-1543473"></span>

<span id="post-1543473"></span>

Fold

<span id="post-title-1543473"></span>

Re: Logging to network server

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 21 Aug 2012 12:44

<span id="post-content-1543473"></span>

Log file contents are meant for problem diagnosis and are subject to change without notice. If you're looking to take actions on APRS data I'd encourage you to use a direct TCP/IP connection to an APRS-IS server port and work from there. See <http://www.aprs-is.net/Connecting.aspx>

If you're looking to analyze RF-received data, you can simply use the same APRS-IS-connecting script to connect to the RF-enabled APRSISCE/32 instance on a Local-Server or IS-Server port to get a non-dupe-filtered feed of received packets.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1543473"></span>

Unfold
Re: Logging to network server by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 21 Aug 2012 12:44

<span id="fpc-1543479"></span>

<span id="post-1543479"></span>

Fold

<span id="post-title-1543479"></span>

Re: Logging to network server

[![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu) 21 Aug 2012 12:56

<span id="post-content-1543479"></span>

Thanks Lynn.

I am indeed looking to analyze RF-received data and reading a feed of received packets sounds like it will solve several headaches I'm having with the log file.

I've looked at the config menu documentation for setting up an IS-Server port, but I don't have that as a choice in the drop down menu list. I'm using the latest stable build, 2012/05/25. Do I need a different version that supports the IS-Server ports?

Thanks,  
Steve  
N4TQU

**Reply**
Options

<span id="post-options-1543479"></span>

Unfold
Re: Logging to network server by [![N4TQU](../archive/assets/avatar--a5ce01e5bc83.php)](https://www.wikidot.com/user:info/n4tqu)[N4TQU](https://www.wikidot.com/user:info/n4tqu), 21 Aug 2012 12:56

<span id="fpc-1543490"></span>

<span id="post-1543490"></span>

Fold

<span id="post-title-1543490"></span>

Re: Logging to network server

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 21 Aug 2012 13:08

<span id="post-content-1543490"></span>

I consider the iS-/Local-Server ports to be still in test, so they're only available in the Development version. See [http://aprsisce.wikidot.com/doc:development-version](../manual/doc-development-version.md)

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1543490"></span>

Unfold
Re: Logging to network server by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 21 Aug 2012 13:08

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-553148/logging-to-network-server#post-
