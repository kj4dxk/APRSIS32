---
title: "APRSIS32 Received Status Messages"
---

# APRSIS32 Received Status Messages

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» APRSIS32 Received Status Messages

Started by: [![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000)  
Date: 07 Mar 2016 18:15  
Number of posts: 5  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

Stopping received random status messages.

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

<span id="fpc-2468985"></span>

<span id="post-2468985"></span>

Fold

<span id="post-title-2468985"></span>

APRSIS32 Received Status Messages

[![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000) 07 Mar 2016 18:15

<span id="post-content-2468985"></span>

Hello

I've been using APRSIS32 for a while now without issue, but recently there are a number of stations sending random status reports which I won't answer and I don't want to see. I've tried to budcall the calls but without effect. Does anyone have an answer to this issue. Nothing I've unticked with respect to messaging etc. has made any difference.

**Reply**
Options

<span id="post-options-2468985"></span>

Unfold
APRSIS32 Received Status Messages by [![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000), 07 Mar 2016 18:15

<span id="fpc-2469066"></span>

<span id="post-2469066"></span>

Fold

<span id="post-title-2469066"></span>

Re: APRSIS32 Received Status Messages

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 07 Mar 2016 21:37

<span id="post-content-2469066"></span>

I'll need specifics on what the messages are. I suspect they're Mic-E notifications that you can individually disable via Configure / Mic-E Notifications.

**Reply**
Options

<span id="post-options-2469066"></span>

Unfold
Re: APRSIS32 Received Status Messages by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 07 Mar 2016 21:37

<span id="fpc-2469077"></span>

<span id="post-2469077"></span>

Fold

<span id="post-title-2469077"></span>

Re: APRSIS32 Received Status Messages

[![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000) 07 Mar 2016 22:08

<span id="post-content-2469077"></span>

Hi

Thanks for replying. There is no message as such, just these wretched notifications i.e. 21:47:08> Mic-E:2E0AVQ Transmitted Status(Special) that keep appearing… and there seems to be more and more of them from different stations that keeping popping up in the message box at the top right hand corner of the APRSIS32 window.

**Reply**
Options

<span id="post-options-2469077"></span>

Unfold
Re: APRSIS32 Received Status Messages by [![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000), 07 Mar 2016 22:08

<span id="fpc-2469082"></span>

<span id="post-2469082"></span>

Fold

<span id="post-title-2469082"></span>

Re: APRSIS32 Received Status Messages

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 07 Mar 2016 22:16

<span id="post-content-2469082"></span>

Yep, that's a Mic-E notification. See <http://aprs.org/aprs12/EmergencyCode.txt> for a description of the default actions taken when a station uses the various Mic-E statuses.

It seems a bunch of people consider themselves to be "special", so I recommend un-checking Configure / Mic-E Notifications / Special. If you do that, most of your spurious notifications will probably stop.

**Reply**
Options

<span id="post-options-2469082"></span>

Unfold
Re: APRSIS32 Received Status Messages by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 07 Mar 2016 22:16

<span id="fpc-2469081"></span>

<span id="post-2469081"></span>

Fold

<span id="post-title-2469081"></span>

Re: APRSIS32 Received Status Messages

[![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000) 07 Mar 2016 22:14

<span id="post-content-2469081"></span>

Ah ha, after reading your reply, I see where I can surpress these notifications and that I have just done, so hopefully they won't appear any more!! I'll shout if they do. Thank you for your help.

regards

Richard

**Reply**
Options

<span id="post-options-2469081"></span>

Unfold
Re: APRSIS32 Received Status Messages by [![Varaderoxlv1000](../archive/assets/avatar--e296ced05816.php)](https://www.wikidot.com/user:info/varaderoxlv1000)[Varaderoxlv1000](https://www.wikidot.com/user:info/varaderoxlv1000), 07 Mar 2016 22:14

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-1637993/aprsis32-received-status-messages#post-
