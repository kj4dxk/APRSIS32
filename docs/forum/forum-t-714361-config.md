---
title: "Config"
---

# Config

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Config

Started by: [![Selwyn Staples](../archive/assets/avatar--16af28ef69b3.php)](https://www.wikidot.com/user:info/selwyn-staples)[Selwyn Staples](https://www.wikidot.com/user:info/selwyn-staples)  
Date: 25 Oct 2013 08:34  
Number of posts: 11  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

Config setup

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

<span id="fpc-1877817"></span>

<span id="post-1877817"></span>

Fold

<span id="post-title-1877817"></span>

Config

[![Selwyn Staples](../archive/assets/avatar--16af28ef69b3.php)](https://www.wikidot.com/user:info/selwyn-staples)[Selwyn Staples](https://www.wikidot.com/user:info/selwyn-staples) 25 Oct 2013 08:34

<span id="post-content-1877817"></span>

Please assist with the following:

1] How do I change 'ME' to my own call sign?  
2] What are 'TACTICAL Assignments', and how can they be stopped?  
3] Can 'Send Message' number of retries be limited, to say 3??

73  
de Sel [zs6ss]

**Reply**
Options

<span id="post-options-1877817"></span>

Unfold
Config by [![Selwyn Staples](../archive/assets/avatar--16af28ef69b3.php)](https://www.wikidot.com/user:info/selwyn-staples)[Selwyn Staples](https://www.wikidot.com/user:info/selwyn-staples), 25 Oct 2013 08:34

<span id="fpc-1877873"></span>

<span id="post-1877873"></span>

Fold

<span id="post-title-1877873"></span>

Re: Config

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 25 Oct 2013 10:38

<span id="post-content-1877873"></span>

1) Click on ME and select "Configure Nickname". Set the Label to whatever you'd like displayed and make sure Enabled is checked. The label is allowed to be your station's callsign-SSID, but for backward compatibility with the original documentation, ME will force itself as the label unless you enable your own specifically configured nickname. (Note: Just changing the label without enabling the nickname will have no effect)

2) See <http://www.xastir.org/wiki/Search_and_Rescue,_%28SAR%29#Tactical_Callsigns> The SARTrack APRS client has recently implemented this specification, but instead of making it an on-demand transmission as it is in xastir, it is done repeatedly on a timer (see <http://aprs.fi/?c=message&call=TACTICAL&limit=50>). The lastest development version of APRSISCE/32 (see [Related manual page](../manual/doc-development-version.md)) provides a "Never" option to ignore all future TACTICAL assignments from a given source. It seems that many of the SARTrack users are unaware of the reach of their TACTICAL assigments based on the non-uniqueness and redundancy that many of them are configuring.

3) Not at this time. The internal message retries are fixed as described at [Related manual page](../manual/message-retries.md) including the second retry set. Five retries is better that some other APRS clients that remember non-acked messages persistently and continue to retry them on every restart for months (or years). If there is a particular message you want to terminate before receiving an ack, do Messages / Pending Messages, select the message from the menu and hit Cancel.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

PS. Responses sometimes come faster via the APRSISCE Yahoo group (<http://groups.yahoo.com/neo/groups/APRSISCE/info>) as there are multiple people there to answer questions.

**Reply**
Options

<span id="post-options-1877873"></span>

Unfold
Re: Config by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 25 Oct 2013 10:38

<span id="fpc-2241977"></span>

<span id="post-2241977"></span>

Fold

<span id="post-title-2241977"></span>

Re: Config Ripping my hair out

[![Dobroplayer](../archive/assets/avatar--bbbb465c2baf.php)](https://www.wikidot.com/user:info/dobroplayer)[Dobroplayer](https://www.wikidot.com/user:info/dobroplayer) 01 Mar 2015 04:55

<span id="post-content-2241977"></span>

Hello, I have spent the last two days googling and watching youtube videos on this program for ARPS. I have the software downloaded installed and it receives just fine. I have a brand new Kantronics KPC-3+ and it works just fine with any/all other com software except ARPSISCE. I have a valid passcode and have verified that it actually is valid. It would really have been nice if that were explained in one of these videos or documentation. In the config screen the default passcode is -1. So I have tried every possible way of entering my passcode…. ie… -1XXXXX or XXXXX-1 or XXXXX. I am guessing this is why transmit is disabled? What the heck am I doing wrong? The videos make it sound like falling off a rock easy….. maybe for someone else. I have even switch com ports changed to a USB/serial connection…. again all my programs will cause the radio to transmit except this one…. I am at wits end and out of ideas at this point. Thanks for any help to come!

**Reply**
Options

<span id="post-options-2241977"></span>

Unfold
Re: Config Ripping my hair out by [![Dobroplayer](../archive/assets/avatar--bbbb465c2baf.php)](https://www.wikidot.com/user:info/dobroplayer)[Dobroplayer](https://www.wikidot.com/user:info/dobroplayer), 01 Mar 2015 04:55

<span id="fpc-2471542"></span>

<span id="post-2471542"></span>

Fold

<span id="post-title-2471542"></span>

Re: Config

[![n4gmu](../archive/assets/avatar--e47e0c06303c.php)](https://www.wikidot.com/user:info/n4gmu)[n4gmu](https://www.wikidot.com/user:info/n4gmu) 12 Mar 2016 14:19

<span id="post-content-2471542"></span>

Gave up

Last edited on 15 Mar 2016 17:36
by [n4gmu](https://www.wikidot.com/user:info/n4gmu)
Show more

**Reply**
Options

<span id="post-options-2471542"></span>

Unfold
Re: Config by [![n4gmu](../archive/assets/avatar--e47e0c06303c.php)](https://www.wikidot.com/user:info/n4gmu)[n4gmu](https://www.wikidot.com/user:info/n4gmu), 12 Mar 2016 14:19

<span id="fpc-2519180"></span>

<span id="post-2519180"></span>

Fold

<span id="post-title-2519180"></span>

Re: Config Symbol

[![KF5LBT](../archive/assets/avatar--0c753ef02d6e.php)](https://www.wikidot.com/user:info/kf5lbt)[KF5LBT](https://www.wikidot.com/user:info/kf5lbt) 04 Jun 2016 12:11

<span id="post-content-2519180"></span>

Igate symbol selection:  
I can't seem to figure out how to change my symbol to be a black diamond with a letter overlay. Can only find black diamond with numbers. Would like to make my symbol a black diamond with the letter R to represent a receive only igate.

Last edited on 04 Jun 2016 12:14
by [KF5LBT](https://www.wikidot.com/user:info/kf5lbt)
Show more

**Reply**
Options

<span id="post-options-2519180"></span>

Unfold
Re: Config Symbol by [![KF5LBT](../archive/assets/avatar--0c753ef02d6e.php)](https://www.wikidot.com/user:info/kf5lbt)[KF5LBT](https://www.wikidot.com/user:info/kf5lbt), 04 Jun 2016 12:11

<span id="fpc-2519202"></span>

<span id="post-2519202"></span>

Fold

<span id="post-title-2519202"></span>

Re: Config Symbol

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 04 Jun 2016 13:23

<span id="post-content-2519202"></span>

Configure / General and click on the symbol (but you probably already know that bit).

Select the Overlay table radio button.

Select Symbol 38 (&) and Overlay 82 (R). It should then show RO IGate.

I think saw what your issue was. The up/down buttons on the overlay character seem to only support 0-9. There are characters in there that are not valid overlays, so it doesn't do them, but you can directly enter the ASCII value of the overlay you want.  
Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-2519202"></span>

Unfold
Re: Config Symbol by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 04 Jun 2016 13:23

<span id="fpc-2519218"></span>

<span id="post-2519218"></span>

Fold

<span id="post-title-2519218"></span>

Re: Config Symbol

[![KF5LBT](../archive/assets/avatar--0c753ef02d6e.php)](https://www.wikidot.com/user:info/kf5lbt)[KF5LBT](https://www.wikidot.com/user:info/kf5lbt) 04 Jun 2016 13:46

<span id="post-content-2519218"></span>

Thanks Lynn!!!

**Reply**
Options

<span id="post-options-2519218"></span>

Unfold
Re: Config Symbol by [![KF5LBT](../archive/assets/avatar--0c753ef02d6e.php)](https://www.wikidot.com/user:info/kf5lbt)[KF5LBT](https://www.wikidot.com/user:info/kf5lbt), 04 Jun 2016 13:46

<span id="fpc-2531954"></span>

<span id="post-2531954"></span>

Fold

<span id="post-title-2531954"></span>

Re: Config

[![JoAnn-AB8YZ](../archive/assets/avatar--0c2923c96389.php)](https://www.wikidot.com/user:info/joann-ab8yz)[JoAnn-AB8YZ](https://www.wikidot.com/user:info/joann-ab8yz) 03 Jul 2016 23:57

<span id="post-content-2531954"></span>

Where in the Documentation can I find where it explains the colors used in window that shows the call signs.

**Reply**
Options

<span id="post-options-2531954"></span>

Unfold
Re: Config by [![JoAnn-AB8YZ](../archive/assets/avatar--0c2923c96389.php)](https://www.wikidot.com/user:info/joann-ab8yz)[JoAnn-AB8YZ](https://www.wikidot.com/user:info/joann-ab8yz), 03 Jul 2016 23:57

<span id="fpc-2531971"></span>

<span id="post-2531971"></span>

Fold

<span id="post-title-2531971"></span>

Re: Config

[![JoAnn-AB8YZ](../archive/assets/avatar--0c2923c96389.php)](https://www.wikidot.com/user:info/joann-ab8yz)[JoAnn-AB8YZ](https://www.wikidot.com/user:info/joann-ab8yz) 04 Jul 2016 01:49

<span id="post-content-2531971"></span>

Can you make the Call Sign window longer. It only shows the last 6 stations.

**Reply**
Options

<span id="post-options-2531971"></span>

Unfold
Re: Config by [![JoAnn-AB8YZ](../archive/assets/avatar--0c2923c96389.php)](https://www.wikidot.com/user:info/joann-ab8yz)[JoAnn-AB8YZ](https://www.wikidot.com/user:info/joann-ab8yz), 04 Jul 2016 01:49

<span id="fpc-2532813"></span>

<span id="post-2532813"></span>

Fold

<span id="post-title-2532813"></span>

Re: Config

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 05 Jul 2016 12:19

<span id="post-content-2532813"></span>

Play with the options in Configure / Screen / Orientation or if you have that on automatic, play with the window shape (tall vs wide).

**Reply**
Options

<span id="post-options-2532813"></span>

Unfold
Re: Config by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 05 Jul 2016 12:19

<span id="fpc-2768321"></span>

<span id="post-2768321"></span>

Fold

<span id="post-title-2768321"></span>

Re: Config - xml file language

[![gmenga](../archive/assets/avatar--43d63e214117.php)](https://www.wikidot.com/user:info/gmenga)[gmenga](https://www.wikidot.com/user:info/gmenga) 15 Mar 2017 23:37

<span id="post-content-2768321"></span>

I'm starting to play around APRSIS32 that I want to use during my next passage in sailing boat from the Mediterranean to the Caribbeans with one of the SCS Modems for RPR. The configuration of the program end up with a xml configuration file. Reading some RPR manuals I also saw some scripts to add to that xml configuration file, in particular to operate with the different SCS modems: PTCII, tracker or P4Dragon, but also to interpose between the APRSIS32 and the modem the AGWPE program. I'm a programmer myself, and I would like to understand, then my question is: where come from the language used in those scripts?  
In other words: how can I interpret those instructions?  
Giuseppe  
IZ1GCL

**Reply**
Options

<span id="post-options-2768321"></span>

Unfold
Re: Config - xml file language by [![gmenga](../archive/assets/avatar--43d63e214117.php)](https://www.wikidot.com/user:info/gmenga)[gmenga](https://www.wikidot.com/user:info/gmenga), 15 Mar 2017 23:37

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-714361/config#post-
