---
title: "APRSISCE not transmitting through TNC on a TH-D74."
---

# APRSISCE not transmitting through TNC on a TH-D74.

[Source wiki page](https://aprsisce.wikidot.com/forum/t-10224578/aprsisce-not-transmitting-through-tnc-on-a-th-d74) · [Archived HTML](../archive/pages/forum_t-10224578_aprsisce-not-transmitting-through-tnc-on-a-th-d74--4b71ca67015a.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» APRSISCE not transmitting through TNC on a TH-D74.

Started by: [![Michael Parkes](../archive/assets/avatar--68594d889fde.php)](https://www.wikidot.com/user:info/michael-parkes)[Michael Parkes](https://www.wikidot.com/user:info/michael-parkes)  
Date: 07 Feb 2019 19:03  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-10224578.xml)

Summary:

APRSISCE seems to be working, but isn't transmitting through BT TH-D74 Kiss TNC.

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

<span id="fpc-4145469"></span>

<span id="post-4145469"></span>

Fold

<span id="post-title-4145469"></span>

APRSISCE not transmitting through TNC on a TH-D74.

[![Michael Parkes](../archive/assets/avatar--68594d889fde.php)](https://www.wikidot.com/user:info/michael-parkes)[Michael Parkes](https://www.wikidot.com/user:info/michael-parkes) 07 Feb 2019 19:03

<span id="post-content-4145469"></span>

I am a totally new user here on APRSISCE and have it up and running for the first time. It seems to be working fine, I see local APRS stations on the map, but all of this appears to be coming in through the internet. I did go through the setup to connect a Kenwood TH-D74 over BT to aprsisce, and have that connecting fine and shows up in the program as an enabled port. I checked all the various settings in the program to make sure everything is enabled and that seems correct. So I am sort of stumped here. What is the purpose of having a TNC if the software is set up to run over the internet? Is the TNC just for cases where you don't have internet access? Can someone clarify that for me? Is there a way to force it to use the TNC and not the internet then?  
Thanks for any tips! :)  
Mike AB7RU  
Spokane, WA

Okay well I found the problem and it had nothing to do with the SW I didn't have the KISS mode set on the HT so now it is working. I don't have much of a signal here so not sure if my packets are being heard, is there a way to confirm that I am reaching a digipeater?

Last edited on 07 Feb 2019 19:22
by [Michael Parkes](https://www.wikidot.com/user:info/michael-parkes)
Show more

**Reply**
Options

<span id="post-options-4145469"></span>

Unfold
APRSISCE not transmitting through TNC on a TH-D74. by [![Michael Parkes](../archive/assets/avatar--68594d889fde.php)](https://www.wikidot.com/user:info/michael-parkes)[Michael Parkes](https://www.wikidot.com/user:info/michael-parkes), 07 Feb 2019 19:03

<span id="fpc-4145523"></span>

<span id="post-4145523"></span>

Fold

<span id="post-title-4145523"></span>

Re: APRSISCE not transmitting through TNC on a TH-D74.

[![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 07 Feb 2019 20:13

<span id="post-content-4145523"></span>

Glad you found the problem. If you have another radio, you can always listen on the frequency when you cause APRSIS32 to transmit and see the transmit light on the D74. Then listen for another packet immediately following your transmission. Short of having another station with solid reception of the digipeater, there's no way to actually KNOW that you've been heard and repeated.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

PS. You'll get faster answers if you post questions like this in the APRSISCE Yahoo group than as a WikiDot post.

**Reply**
Options

<span id="post-options-4145523"></span>

Unfold
Re: APRSISCE not transmitting through TNC on a TH-D74. by [![APRS IS CE](../archive/assets/avatar--613203fbbc57.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 07 Feb 2019 20:13

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-10224578/aprsisce-not-transmitting-through-tnc-on-a-th-d74#post-
