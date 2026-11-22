---
title: "APRSIS32 and PK232MBX ????"
---

# APRSIS32 and PK232MBX ????

[Source wiki page](https://aprsisce.wikidot.com/forum/t-2276998/aprsis32-and-pk232mbx) · [Archived HTML](../archive/pages/forum_t-2276998_aprsis32-and-pk232mbx--c4312b866774.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» APRSIS32 and PK232MBX ????

Started by: [![AB4WS](../archive/assets/avatar--e7140d9f599e.php)](https://www.wikidot.com/user:info/ab4ws)[AB4WS](https://www.wikidot.com/user:info/ab4ws)  
Date: 12 May 2017 23:32  
Number of posts: 3  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-2276998.xml)

Summary:

Trying to connect APRSIS32 and AEA PK-232MBX and not having any success! Can it be done?

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

<span id="fpc-2819071"></span>

<span id="post-2819071"></span>

Fold

<span id="post-title-2819071"></span>

APRSIS32 and PK232MBX ????

[![AB4WS](../archive/assets/avatar--e7140d9f599e.php)](https://www.wikidot.com/user:info/ab4ws)[AB4WS](https://www.wikidot.com/user:info/ab4ws) 12 May 2017 23:32

<span id="post-content-2819071"></span>

I have a couple of the old AEA PK-232MBX TNC here that I am trying to put into use as an APRS iGate using the APRSIS32 software.

I have the APRSIS32 all set up and working fine, on the internet side, and I created the port and get a response that the TNC connection is good, however, no activity or signal comes from the radio and the radio never transmits, so there is no communication through the TNC to and from the radio.

The TNC Radio is good. I can hook up same Computer TNC and RADIO and use WINAPRS software and radio works fine however no igate connection. So I know the radio, TNC, and Computer are all communicating.

I believe the difference involves the WINAPRS software uses the AEA PK-232MBX commands, where the APRSIS32 wants the TNC to be in KISS mode. I have read the manual, and the command for putting the AEA PK-232MBX into KISS mode is KISS on or KISS $01. I use the PK Terminal software and access command mode, and other commands go in fine and return to the CMD: prompt, but when I use either of the KISS commands and hit return the screen advances to the next line and nothing ever happens. If I kill the process and exit the TNC has to be reset it no longer communicates with the PC. I have tried this with both of the AEA PK-232MBX that I have with same results They are sporting EPROM version 12/1/1993 $99

Any ideas, suggestions, recommendations, etc. would be greatly appreciated! I would really like to utilize these TNC's

Thanks,  
73 de AB4WS,  
Jack  
A

**Reply**
Options

<span id="post-options-2819071"></span>

Unfold
APRSIS32 and PK232MBX ???? by [![AB4WS](../archive/assets/avatar--e7140d9f599e.php)](https://www.wikidot.com/user:info/ab4ws)[AB4WS](https://www.wikidot.com/user:info/ab4ws), 12 May 2017 23:32

<span id="fpc-2819073"></span>

<span id="post-2819073"></span>

Fold

<span id="post-title-2819073"></span>

Re: APRSIS32 and PK232MBX ????

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 12 May 2017 23:39

<span id="post-content-2819073"></span>

To use the PK232 with APRSISCE/32, you'll need to get it into KISS mode and lock it there across power cycles. You can tell the TNC is in KISS mode if you monitor it with a terminal emulator when packets are received and decoded. The headers of those packets will be unreadable, but the APRS payloads will be understandable.

You may get more assistance if you post a question to the APRSISCE Yahoo group as there are probably other users there that have PK232 devices.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-2819073"></span>

Unfold
Re: APRSIS32 and PK232MBX ???? by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 12 May 2017 23:39

<span id="fpc-3094296"></span>

<span id="post-3094296"></span>

Fold

<span id="post-title-3094296"></span>

Re: APRSIS32 and PK232MBX ????

[![G0OFC](../archive/assets/avatar--a63b20acdafd.php)](https://www.wikidot.com/user:info/g0ofc)[G0OFC](https://www.wikidot.com/user:info/g0ofc) 05 Jul 2017 16:18

<span id="post-content-3094296"></span>

I'm using that same set up running an igate from work. I setup a port to talk to the PK232 via AGW Packet engine. all working within 10 minutes.  
I cant post a link because I'm a new user. but just google AGW packet engine. Dave

**Reply**
Options

<span id="post-options-3094296"></span>

Unfold
Re: APRSIS32 and PK232MBX ???? by [![G0OFC](../archive/assets/avatar--a63b20acdafd.php)](https://www.wikidot.com/user:info/g0ofc)[G0OFC](https://www.wikidot.com/user:info/g0ofc), 05 Jul 2017 16:18

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-2276998/aprsis32-and-pk232mbx#post-
