---
title: "ISS + IGate & RF to Internet Settings"
---

# ISS + IGate & RF to Internet Settings

[Source wiki page](https://aprsisce.wikidot.com/forum/t-636884/iss-igate-rf-to-internet-settings) · [Archived HTML](../archive/pages/forum_t-636884_iss-igate-rf-to-internet-settings--06f67edef5a5.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» ISS + IGate & RF to Internet Settings

Started by: [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze)  
Date: 25 Mar 2013 16:53  
Number of posts: 5  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-636884.xml)

Summary:

Newbie question on settings.

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

<span id="fpc-1738100"></span>

<span id="post-1738100"></span>

Fold

<span id="post-title-1738100"></span>

ISS + IGate & RF to Internet Settings

[![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze) 25 Mar 2013 16:53

<span id="post-content-1738100"></span>

First of all — thanks for APRSICE/32, I've used while it working the ISS digipeater numerous times over the past few months.

My setup is a Kenwood TM-V71A + RC-D710, Windows XP, serial-to-USB converter cable. It took some fiddling to setup over the winter, but I've ironed out most/all the kinks that initially plagued me (mostly cabling, ports, etc.). My question has to do with how to best setup the software while working the ISS digipeater. Based on what I've seen on www.findu.com — I've noted that when I turn off the RF-Internet option on the PORTS->D710 screen, I'll see something like this reported:

KD0KZE>TUPX9R,RS0ISS\*,qAR,WA8LMF-SG:'yaIl -/]Greetings from Circle Pines MN=

However, when I leave that setting on, I typically see my successful call to the ISS digipeater as originating from a TCPIP location instead. When I turn off that setting, it reports (the more gratifying) RSOISS\* path. Am I basically digipeating my own traffic? Is there a way to not re-send your own RF traffic via TCP-IP (assuming this is the case), but still continue to report other stations you've heard? I'm happy to send my listening RX traffic the IGate, because it gives them credit.

Thanks & 73's  
KD0KZE  
Circle Pines, MN

**Reply**
Options

<span id="post-options-1738100"></span>

Unfold
ISS + IGate & RF to Internet Settings by [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze), 25 Mar 2013 16:53

<span id="fpc-1738106"></span>

<span id="post-1738106"></span>

Fold

<span id="post-title-1738106"></span>

Re: ISS + IGate & RF to Internet Settings

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 25 Mar 2013 17:08

<span id="post-content-1738106"></span>

Please read [http://aprsisce.wikidot.com/doc:satelliteops](../manual/doc-satelliteops.md) and I think you'll have your questions answered. The key pieces are:

1) It's a lot easier to run a separate ISS/Satellite instance.

2) Unchecking Configure / Ports / APRS-IS's Beacon checkbox will eliminate the directly injected TCPIP packet and allow only RF-to-IS gated packets to appear on the APRS-IS.

3) Definitely uncheck IS to RF in that same dialog to avoid gating other station's messages to the ISS.

4) Probably uncheck Messages there and your messages will only go out via RF, which is probably what you want.

Note that the TCPIP path you're seeing now is required by <http://www.aprs-is.net/Connecting.aspx> which states:

Packets originating from the client should only have TCPIP\* in the path, nothing more or less (AE5PL-TS>APRS,TCPIP\*:my packet).

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1738106"></span>

Unfold
Re: ISS + IGate & RF to Internet Settings by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 25 Mar 2013 17:08

<span id="fpc-1738221"></span>

<span id="post-1738221"></span>

Fold

<span id="post-title-1738221"></span>

Re: ISS + IGate & RF to Internet Settings

[![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze) 25 Mar 2013 19:58

<span id="post-content-1738221"></span>

Thanks for the quick reply, I'll begin working on the second instance. I do use packet terrestrially (144.390) as well, so I need re-tweak things when I go back and forth. It usually requires going into a few radio menus as well — I ought to make a quick cheat-sheet to keep handy.

-Paul / KD0KZE

**Reply**
Options

<span id="post-options-1738221"></span>

Unfold
Re: ISS + IGate & RF to Internet Settings by [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze), 25 Mar 2013 19:58

<span id="fpc-1764946"></span>

<span id="post-1764946"></span>

Fold

<span id="post-title-1764946"></span>

Re: ISS + IGate & RF to Internet Settings

[![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze) 02 May 2013 16:35

<span id="post-content-1764946"></span>

I'll follow-up to my own post here — I created a second instance as suggested, and this has indeed solved the issues I brought up a few weeks ago.

Thanks again,  
Paul / KD0KZE

**Reply**
Options

<span id="post-options-1764946"></span>

Unfold
Re: ISS + IGate & RF to Internet Settings by [![KD0KZE](../archive/assets/avatar--caa615378787.php)](https://www.wikidot.com/user:info/kd0kze)[KD0KZE](https://www.wikidot.com/user:info/kd0kze), 02 May 2013 16:35

<span id="fpc-1851439"></span>

<span id="post-1851439"></span>

Fold

<span id="post-title-1851439"></span>

Re: ISS + IGate & RF to Internet Settings

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 13 Sep 2013 19:09

<span id="post-content-1851439"></span>

That is most excellent to hear. Glad the instructions helped out.

If there's anything on the Wiki that you can help make better or more clear, please feel free to let us know. If there's information that you think should be on the Wiki that you couldn't find, please let us know that as well.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1851439"></span>

Unfold
Re: ISS + IGate & RF to Internet Settings by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 13 Sep 2013 19:09

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-636884/iss-igate-rf-to-internet-settings#post-
