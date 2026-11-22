---
title: "Custom Symbol Table For Duchifat-1 CubeSat"
---

# Custom Symbol Table For Duchifat-1 CubeSat

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / Feature Requests](forum-c-312348-feature-requests.md)
» Custom Symbol Table For Duchifat-1 CubeSat

Started by: [![KE5UUO](../archive/assets/avatar--e44710128f97.php)](https://www.wikidot.com/user:info/ke5uuo)[KE5UUO](https://www.wikidot.com/user:info/ke5uuo)  
Date: 01 Jun 2015 18:53  
Number of posts: 5  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

Duchifat-1 is using custom symbol tables for QSL

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

<span id="fpc-2308063"></span>

<span id="post-2308063"></span>

Fold

<span id="post-title-2308063"></span>

Custom Symbol Table For Duchifat-1 CubeSat

[![KE5UUO](../archive/assets/avatar--e44710128f97.php)](https://www.wikidot.com/user:info/ke5uuo)[KE5UUO](https://www.wikidot.com/user:info/ke5uuo) 01 Jun 2015 18:53

<span id="post-content-2308063"></span>

Lynn-

Instead of using call sign for someone's identity to verify QSL (that would just be too easy), they are assigning custom Symbol Tables and Symbol Codes when you register.

I can understand not being able to assign them in the GUI, but when I try to change Symbol Table from / to ! in the XML config (this is what I have been assigned), APRSIS32 changes it back to "/". Can you make a version of APRSIS32 that will accept other charters for Symbol Table except "/" and "\"

You can get more info from the website here:

> The Herzliya Science Center is inviting stations with the ability to  
> send compressed APRS packets to take part in testing the Duchifat-1  
> CubeSat, launched almost 1 year ago by high school students in  
> Israel. The satellite will collect packets worldwide and downlink  
> them from time to time over the Herzliya Science Center's ground  
> station.
>
> Participants must register in advance at  
> (can't include link due to low Karama) to have their  
> packets identified on the map.
>
> Received packets will be displayed on a map on the Science Center's  
> website, and QSL cards will be sent via the bureau. The Duchifat-1  
> sponsors say there is no way they can recognize packets from  
> unregistered stations "because the packet is limited to 14  
> characters at the satellite, so we assign two unique characters to  
> every registered station to enable us to identify them."
>
> The uplink is 435.220 MHz, the downlink is 145.980 MHz. The  
> satellite's beacon call sign is 4X4HSL.

**Reply**
Options

<span id="post-options-2308063"></span>

Unfold
Custom Symbol Table For Duchifat-1 CubeSat by [![KE5UUO](../archive/assets/avatar--e44710128f97.php)](https://www.wikidot.com/user:info/ke5uuo)[KE5UUO](https://www.wikidot.com/user:info/ke5uuo), 01 Jun 2015 18:53

<span id="fpc-2308136"></span>

<span id="post-2308136"></span>

Fold

<span id="post-title-2308136"></span>

Re: Custom Symbol Table For Duchifat-1 CubeSat

[![4z1ws](../archive/assets/avatar--57c3a9c00038.php)](https://www.wikidot.com/user:info/4z1ws)[4z1ws](https://www.wikidot.com/user:info/4z1ws) 01 Jun 2015 20:56

<span id="post-content-2308136"></span>

Hi,  
My name is Shamai, I am one of the teachers at the Herzliya Science Center that built Duchifat1, the high school students Cubesat.  
The Symbol Code and Symbol Table ID are two unique characters that one gets when registering at our web site, so that the ground station software can later identify the packets when downloading them from the satellite and attach to each position the original call sign and display it on a world map. I guess that we can change these characters to match APRSISCE/32 exactly.  
However, this satellite requires 14 characters long COMPRESSED APRS packets that begin with a '!'.  
I succeeded to generate with APRSISCE/32 (running with AGWPE sound card TNC/modem) a packet like this: =\>6CQW;{&t sT  
So since the satellite software unfortunately requires that the first character will be a '!', if we could replace the first character from '=' to '!', there is a good chance that APRSISCE/32 can be used to generate position packets for Duchifat1.  
Is there a way to do it?…  
Thanks, 73  
shamai, 4z1ws

**Reply**
Options

<span id="post-options-2308136"></span>

Unfold
Re: Custom Symbol Table For Duchifat-1 CubeSat by [![4z1ws](../archive/assets/avatar--57c3a9c00038.php)](https://www.wikidot.com/user:info/4z1ws)[4z1ws](https://www.wikidot.com/user:info/4z1ws), 01 Jun 2015 20:56

<span id="fpc-2308274"></span>

<span id="post-2308274"></span>

Fold

<span id="post-title-2308274"></span>

Re: Custom Symbol Table For Duchifat-1 CubeSat

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 02 Jun 2015 01:33

<span id="post-content-2308274"></span>

The first character in an APRS packet, compressed or not, is the datatype.

means that there is no timestamp, but the client supports messaging as APRSISCE does

! means that there is no timestamp, but the client does NOT support messaging which is invalid for APRSISCE

It's a shame that a satellite was launched with firmware that is not fully spec compliant. = and ! are the same packet contents. @ and / add a timestamp in front of the posit.

A compressed posit has nothing to do with the datatype, but the first character of the posit (after = or ! or after the timestamp in an @ or / packet). If it is non-numeric, then the posit is compressed and that character is the table or overlay character. To get a numeric overlay, use a-j (lower case). The only valid overlay characters are 0-9, a-j, or A-Z.

So, unless I modify APRSISCE/32 to mis-inform other clients of it's messaging capability, changing the = to a ! isn't going to happen. Sorry about your overly restrictive satellite processing.

**Reply**
Options

<span id="post-options-2308274"></span>

Unfold
Re: Custom Symbol Table For Duchifat-1 CubeSat by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 02 Jun 2015 01:33

<span id="fpc-2308275"></span>

<span id="post-2308275"></span>

Fold

<span id="post-title-2308275"></span>

Re: Custom Symbol Table For Duchifat-1 CubeSat

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 02 Jun 2015 01:34

<span id="post-content-2308275"></span>

The missing character is an equal sign which means that there is no timestamp, but the client supports messaging. Don't ask me why the Wiki did the strange formatting.

**Reply**
Options

<span id="post-options-2308275"></span>

Unfold
Re: Custom Symbol Table For Duchifat-1 CubeSat by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 02 Jun 2015 01:34

<span id="fpc-2308272"></span>

<span id="post-2308272"></span>

Fold

<span id="post-title-2308272"></span>

Re: Custom Symbol Table For Duchifat-1 CubeSat

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 02 Jun 2015 01:27

<span id="post-content-2308272"></span>

The symbol table is / for primary, \ for alternate and 0-9, a-j, or A-Z for an overlay on the Alternate table symbol. ! is not a valid overlay character and I have no idea why they are assigning invalid overlays.

See also aprs101.pdf page 91.

**Reply**
Options

<span id="post-options-2308272"></span>

Unfold
Re: Custom Symbol Table For Duchifat-1 CubeSat by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 02 Jun 2015 01:27

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-1223762/custom-symbol-table-for-duchifat-1-cubesat#post-
