---
title: "Question on iGate setup"
---

# Question on iGate setup

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Question on iGate setup

Started by: [![KJ4RZN](../archive/assets/avatar--6e7e89450277.php)](https://www.wikidot.com/user:info/kj4rzn)[KJ4RZN](https://www.wikidot.com/user:info/kj4rzn)  
Date: 20 Dec 2011 18:37  
Number of posts: 4  
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

<span id="fpc-1332119"></span>

<span id="post-1332119"></span>

Fold

<span id="post-title-1332119"></span>

Question on iGate setup

[![KJ4RZN](../archive/assets/avatar--6e7e89450277.php)](https://www.wikidot.com/user:info/kj4rzn)[KJ4RZN](https://www.wikidot.com/user:info/kj4rzn) 20 Dec 2011 18:37

<span id="post-content-1332119"></span>

I am a new user and tried setting up the software today. My call sign is KJ4RZN and I set up the RF port as instructed in the wiki, etc. Thing is, I am not seeing my iGate on aprs.fi like I was before when I was using UI-View 32. Otherwise everything seems to be OK. Very nice. Any reason why I'm not seeing the iGate on aprs.fi?

Thanks!

**Reply**
Options

<span id="post-options-1332119"></span>

Unfold
Question on iGate setup by [![KJ4RZN](../archive/assets/avatar--6e7e89450277.php)](https://www.wikidot.com/user:info/kj4rzn)[KJ4RZN](https://www.wikidot.com/user:info/kj4rzn), 20 Dec 2011 18:37

<span id="fpc-1332124"></span>

<span id="post-1332124"></span>

Fold

<span id="post-title-1332124"></span>

Re: Question on iGate setup

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 20 Dec 2011 18:50

<span id="post-content-1332124"></span>

First what callsign-SSID are we discussing here? It always helps to know which instance you're asking about.

Are you sure you are receiving packets on your RF Port?

You can access the following Trace Logs via Enables / View Logs / <NameOfLog> and make sure they are enabled to see valuable debug information:

Can you make sure all of the following logs are enabled and do tests in  
both Packet and APRS modes and then e-mail the resulting logs to  
ten.lrra|JRE4JK#ten.lrra|JRE4JK.

<YourRFPortName> - Shows detailed information about traffic on your RF port

IGate - Shows IGate activities

IGate(NOT) - Show IGate denial activities

Transmit - Useful for debugging -IS to RF IGating

In addition, make sure the following menu option is checked:

Enables / Logging / File Enabled - This puts all other Trace Logs into APRSIS32\*.LOG

Also, the following Port options affect the IGate functions:

Configure / Ports / APRS-IS / RF to IS and IS to RF and Enabled and Xmit Enable

Configure / Ports / <YourPortName> / RF to IS and IS to RF and Enabled and Xmit Enable

Enables / APRS-IS Enabled (must be checked)

Enables / Internet Access (must be checked)

Enables / Ports / All (must be checked for any RF Port to work)

Enables / Ports / Receive Only (must NOT be checked for xmit-capable IGate)

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1332124"></span>

Unfold
Re: Question on iGate setup by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 20 Dec 2011 18:50

<span id="fpc-1332125"></span>

<span id="post-1332125"></span>

Fold

<span id="post-title-1332125"></span>

Re: Question on iGate setup

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 20 Dec 2011 18:52

<span id="post-content-1332125"></span>

Ok, so I missed the obvious first step.

Have you entered a valid APRS-IS password for your station's callsign?

Please see APRSISCE/32 documentation for details.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1332125"></span>

Unfold
Re: Question on iGate setup by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 20 Dec 2011 18:52

<span id="fpc-1593197"></span>

<span id="post-1593197"></span>

Fold

<span id="post-title-1593197"></span>

Re: Question on iGate setup

[![K4WHE](../archive/assets/avatar--2bfaf96028bc.php)](https://www.wikidot.com/user:info/k4whe)[K4WHE](https://www.wikidot.com/user:info/k4whe) 21 Oct 2012 01:19

<span id="post-content-1593197"></span>

Sorry to bring up an old thread.

I have been using my Yaesu FT-1900 as a receive only IGate for a few weeks now, but now I want to be able to transmit a few items, and also help with some digipeating. What do I need to do to my radio/computer to do that? I know the obvious make some sort of cable that connects to the RJ-11 port, but do I need something else in line or will the APRSIS software take care of that?

**Reply**
Options

<span id="post-options-1593197"></span>

Unfold
Re: Question on iGate setup by [![K4WHE](../archive/assets/avatar--2bfaf96028bc.php)](https://www.wikidot.com/user:info/k4whe)[K4WHE](https://www.wikidot.com/user:info/k4whe), 21 Oct 2012 01:19

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-422541/question-on-igate-setup#post-
