---
title: "iGate Setup Help"
---

# iGate Setup Help

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» iGate Setup Help

Started by: [![W8TDS](../archive/assets/avatar--5e0e5326f050.php)](https://www.wikidot.com/user:info/w8tds)[W8TDS](https://www.wikidot.com/user:info/w8tds)  
Date: 07 Sep 2012 13:45  
Number of posts: 2  
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

<span id="fpc-1558672"></span>

<span id="post-1558672"></span>

Fold

<span id="post-title-1558672"></span>

iGate Setup Help

[![W8TDS](../archive/assets/avatar--5e0e5326f050.php)](https://www.wikidot.com/user:info/w8tds)[W8TDS](https://www.wikidot.com/user:info/w8tds) 07 Sep 2012 13:45

<span id="post-content-1558672"></span>

I'm new to this software but what I'm trying to do is to get a RF -> Internet only iGate up and running to fill a void in my area (i'm not yet comfortable running Internet -> RF). I have a Kenwood TM-281 connected to an eight foot 2M antenna and with it's audio out going to my computer running APRSIS32. I have gotten my password. Now I just am not sure what steps to take in the software to start using this as a receive only iGate (call sign W8TDS-5). Any help would be greatly appreciated!! 73s!

W8TDS  
-Trevor

**Reply**
Options

<span id="post-options-1558672"></span>

Unfold
iGate Setup Help by [![W8TDS](../archive/assets/avatar--5e0e5326f050.php)](https://www.wikidot.com/user:info/w8tds)[W8TDS](https://www.wikidot.com/user:info/w8tds), 07 Sep 2012 13:45

<span id="fpc-1582769"></span>

<span id="post-1582769"></span>

Fold

<span id="post-title-1582769"></span>

Re: iGate Setup Help

[![Jay911](../archive/assets/avatar--f020fe839c1a.php)](https://www.wikidot.com/user:info/jay911)[Jay911](https://www.wikidot.com/user:info/jay911) 07 Oct 2012 16:00

<span id="post-content-1582769"></span>

I'm doing something similar, except my input radio is an old Uniden scanner I wasn't using. There's a piece of software out there by WB5IPM called a Sound Card TNC Utility. Hopefully the link will paste into here, but if it doesn't, just google that callsign and those words.

The software is pretty easy to configure. Make sure your Windows sound settings for recording are configured to use the audio jack you plugged your radio in to, put the software somewhere on your PC, and run it. It always asks if you want to "activate" or "deactivate" APRS. (This is one of the only things I don't like - this means you can't really effectively run this software unattended. I was away from my PC for a few weeks last month, and a Windows update caused it to reboot - all the software came back up, but because the program wanted to know if I wanted to activate it, I never got data from RF until I came home and clicked the button.)

Once you start the software, you should see an oscilloscope-y waveform on the bottom half of the window, and as APRS packets are received by your radio (and passed through the audio cable to the PC), they should show up on the top half of the screen in raw format.

Almost done - in APRSIS32, create a new port (Configure>Ports>New Port). Type is "Simply(KISS)" and name is whatever you want (I called mine "RX Scanner"). Once you hit OK on that dialog box, you will get a pop-up asking what port type you want. Click TCP/IP. You'll get two more pop-ups. First, fill out the one marked TCP Configuration. The IP address is 127.0.0.1, and the port is 4000.

After hitting OK on the TCP Configuration screen, you'll have a small pop-up marked "yourportname(Simply(KISS))" left. Uncheck "IS to RF" and "Xmit Enable", and you should be ready to hit OK. (For the meanings behind other check boxes on that screen, check the documentation here.) Lastly, you may have to go to the "Enables" menu, then "Ports", then select whatever you called your new port, so that a checkmark appears beside it. If it's running, you'll see "yourportname OK" alternating with "APRS-IS OK" in the upper left corner of the APRSIS32 screen, above the scroller.

Hope this helps. If you eventually move to a bidirectional gate, you're going to need more than just a sound input to run on your radio, of course.

—j.  
Jason Low, VE6SRT

**Reply**
Options

<span id="post-options-1582769"></span>

Unfold
Re: iGate Setup Help by [![Jay911](../archive/assets/avatar--f020fe839c1a.php)](https://www.wikidot.com/user:info/jay911)[Jay911](https://www.wikidot.com/user:info/jay911), 07 Oct 2012 16:00

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-561212/igate-setup-help#post-
