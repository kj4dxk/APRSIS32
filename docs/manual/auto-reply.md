---
title: "Auto-Reply"
---

# Auto-Reply

From the release notes:

Configure / Messages / RF Eavesdrop supports eavesdropping on just RF-received (not -IS) messages. This lets you display only "locally" heard messages rather than All. Clear Eavesdrops clears these as well.

Messages / Auto-Reply has Message (what to send) and "Inactive For" and "No More Than" sub-menus. "InActive For" is a delay since the last interaction with APRSISCE/32 before a configured Auto-Reply will be sent. "No More Than" sets the minimum interval between Auto-Replies to a single station.

And expanding on them…

The definition of "activity" is any mouse clicks, menu selections, or messaging activity in the client. If you have an "Inactive For" then any configured Auto-Reply Message won't be sent to anyone until after that time has elapsed. Eventually I hope to go back and notify every one that send you messages after the last activity when the "Inactive For" times out, but I haven't figured out where to do that yet. If you set "Inactive For" to Forever, you can see how that would effectively disable the Auto-Reply while still allowing you to have a preferred text selected (rather than None).

Once a station has received an Auto-Reply message, that station won't receive another one until at least "No More Than" has elapsed. However, if you change the Auto-Reply message, all remembered station times are cleared to allow them to receive the new "away" status on their next message (assuming the "Inactive For" has elapsed since you changed the Message.

Clear as Mud? I thought it might be. Play with it and let me know if it's working for you. Oh, and the Auto-Reply messages will show up in your Chat window(s) as well so you will hopefully be able to tell when they fired.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

PS. The possible values for "Inactive For" are Immediate, 1, 5, 10, 30 minutes or Forever (effectively disabled)

The possible values for "No More than" are 1, 10, 30, 60 minutes, 1 day, 1 year (doubt you'll get there) or Forever (effectively one shot per message text change).

The minimum of 1 in "No More Than" works around the 30 second APRS-IS dupe detector, but still might flood the RF channel if someone sends you a multi-line message. Each line is a separate APRS message and will receive an Auto-Answer via RF. Not a pretty picture, but that's what Kenwood does when you enable Auto-Answer as far as I can tell, with no delays either! One reply sent for each message received.
