---
title: "Setup TNC to transmit with squelch open???"
---

# Setup TNC to transmit with squelch open???

[Source wiki page](https://aprsisce.wikidot.com/forum/t-619199/setup-tnc-to-transmit-with-squelch-open) · [Archived HTML](../archive/pages/forum_t-619199_setup-tnc-to-transmit-with-squelch-open--d958412da667.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Setup TNC to transmit with squelch open???

Started by: [![AF8F](../archive/assets/avatar--1c21c5fa5c9b.php)](https://www.wikidot.com/user:info/af8f)[AF8F](https://www.wikidot.com/user:info/af8f)  
Date: 26 Jan 2013 20:12  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-619199.xml)

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

<span id="fpc-1695822"></span>

<span id="post-1695822"></span>

Fold

<span id="post-title-1695822"></span>

Setup TNC to transmit with squelch open???

[![AF8F](../archive/assets/avatar--1c21c5fa5c9b.php)](https://www.wikidot.com/user:info/af8f)[AF8F](https://www.wikidot.com/user:info/af8f) 26 Jan 2013 20:12

<span id="post-content-1695822"></span>

I have a Timewave PK-96 TNC. APRSIS32 will only transmit when the squelch is not open. Does anyone know which TNC command will enable transmit with an open squelch? Transmit should only be restricted when a data carrier is detected.

Thanks for your help.

**Reply**
Options

<span id="post-options-1695822"></span>

Unfold
Setup TNC to transmit with squelch open??? by [![AF8F](../archive/assets/avatar--1c21c5fa5c9b.php)](https://www.wikidot.com/user:info/af8f)[AF8F](https://www.wikidot.com/user:info/af8f), 26 Jan 2013 20:12

<span id="fpc-1703038"></span>

<span id="post-1703038"></span>

Fold

<span id="post-title-1703038"></span>

Re: Setup TNC to transmit with squelch open???

[![AF8F](../archive/assets/avatar--1c21c5fa5c9b.php)](https://www.wikidot.com/user:info/af8f)[AF8F](https://www.wikidot.com/user:info/af8f) 04 Feb 2013 22:40

<span id="post-content-1703038"></span>

Found it:

DCdconn ON|OFF Default: OFF

OFF RS-232 cable Pin 8 is permanently set high (default).  
ON RS-232 cable Pin 8 follows the state of the CON (or DCD) LED.

DCDCONN defines how the DCD (Data Carrier Detect) signal affects pin 8 in the RS-232 interface to  
your computer or terminal. Some programs such as PBBS software require that DCDCONN be ON.  
DCDCONN also works in the RAWHDLC and KISS modes. In RAWHDLC and KISS, no packet connections  
are known to the PK-96. With DCDCONN ON, the state of the radio DCD is sent to the RS-232  
DCD pin (pin-8). This may be necessary to some Host applications that need to know when the radio  
channel is busy.

**Reply**
Options

<span id="post-options-1703038"></span>

Unfold
Re: Setup TNC to transmit with squelch open??? by [![AF8F](../archive/assets/avatar--1c21c5fa5c9b.php)](https://www.wikidot.com/user:info/af8f)[AF8F](https://www.wikidot.com/user:info/af8f), 04 Feb 2013 22:40

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-619199/setup-tnc-to-transmit-with-squelch-open#post-
