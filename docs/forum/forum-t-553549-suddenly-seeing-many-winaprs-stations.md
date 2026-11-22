---
title: "Suddenly seeing many WinAPRS stations"
---

# Suddenly seeing many WinAPRS stations

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Suddenly seeing many WinAPRS stations

Started by: [![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp)  
Date: 21 Aug 2012 04:10  
Number of posts: 4  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

Summary:

Suddenly seeing many WinAPRS stations in APRSIS

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

<span id="fpc-1543264"></span>

<span id="post-1543264"></span>

Fold

<span id="post-title-1543264"></span>

Suddenly seeing many WinAPRS stations

[![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp) 21 Aug 2012 04:10

<span id="post-content-1543264"></span>

I'm suddenly seeing a bunch of stations popping up on the left hand side call sign list. Hundreds are coming in on regular intervals at something like 10 or 30 minutes. Examples are XE2BNC-10, YN1BBS-10, KE6WX, WD9U-10, etc. They all have "WinAPRS. etc (L2K)" present on the status when you click on them. They also state Public Winlink Gateway.

I thought I had caused this by clicking on something somewhere, somewhen without realizing it. So I downloaded and setup APRIS32 in a new folder. And it's still happening.

**Reply**
Options

<span id="post-options-1543264"></span>

Unfold
Suddenly seeing many WinAPRS stations by [![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp), 21 Aug 2012 04:10

<span id="fpc-1543475"></span>

<span id="post-1543475"></span>

Fold

<span id="post-title-1543475"></span>

Re: Suddenly seeing many WinAPRS stations

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 21 Aug 2012 12:49

<span id="post-content-1543475"></span>

Those stations (\*-10 typically) aren't really APRS stations, but are objects injected by a single server called WINLINK. Depending on where WINLINK is thought to be, they may be picked up by your filter.

If you never want to see them, put -b/WINLINK or -u/APWL2K (both case sensitive) in your Configure / General / Add Filter box.

See also <http://aprs.fi/?c=raw&call=WINLINK&limit=50&view=normal>

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1543475"></span>

Unfold
Re: Suddenly seeing many WinAPRS stations by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 21 Aug 2012 12:49

<span id="fpc-1547666"></span>

<span id="post-1547666"></span>

Fold

<span id="post-title-1547666"></span>

Re: Suddenly seeing many WinAPRS stations

[![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp) 26 Aug 2012 00:02

<span id="post-content-1547666"></span>

Shortly after my posting here the stations are no longer being displayed. Not sure why the change but I'm happy. :)

**Reply**
Options

<span id="post-options-1547666"></span>

Unfold
Re: Suddenly seeing many WinAPRS stations by [![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp), 26 Aug 2012 00:02

<span id="fpc-1548358"></span>

<span id="post-1548358"></span>

Fold

<span id="post-title-1548358"></span>

Re: Suddenly seeing many WinAPRS stations

[![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp) 26 Aug 2012 23:52

<span id="post-content-1548358"></span>

And suddenly I'm seeing random stations appearing on APRSIS. Not just WinAPRS stations as before. I restarted APRSIS and I didn't see any for about a half hour and suddenly some are back. K6DTK-9, M5SMN-15, BADARC, SM2NLD-5, N1AGV-15, Hb9RF, UIDIGI, N8uKH-15, K7GL-9, IK3SVW-11. Dunno how the Swedish and Italian stations are getting in.

This sounds like a problem at the APRS tier then?

**Reply**
Options

<span id="post-options-1548358"></span>

Unfold
Re: Suddenly seeing many WinAPRS stations by [![Tony VE6MVP](../archive/assets/avatar--c46b6fe2f09f.php)](https://www.wikidot.com/user:info/tony-ve6mvp)[Tony VE6MVP](https://www.wikidot.com/user:info/tony-ve6mvp), 26 Aug 2012 23:52

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-553549/suddenly-seeing-many-winaprs-stations#post-
