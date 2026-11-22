---
title: "Stops receiving RF after a while"
---

# Stops receiving RF after a while

[Source wiki page](https://aprsisce.wikidot.com/forum/t-5388296/stops-receiving-rf-after-a-while) · [Archived HTML](../archive/pages/forum_t-5388296_stops-receiving-rf-after-a-while--3a93fd0ab4d2.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Stops receiving RF after a while

Started by: [![Michael KE4DHK](../archive/assets/avatar--821230a2b6c3.php)](https://www.wikidot.com/user:info/michael-ke4dhk)[Michael KE4DHK](https://www.wikidot.com/user:info/michael-ke4dhk)  
Date: 02 Mar 2018 03:17  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-5388296.xml)

Summary:

APRSIS32 seems to stop receiving RF after a while

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

<span id="fpc-3726143"></span>

<span id="post-3726143"></span>

Fold

<span id="post-title-3726143"></span>

Stops receiving RF after a while

[![Michael KE4DHK](../archive/assets/avatar--821230a2b6c3.php)](https://www.wikidot.com/user:info/michael-ke4dhk)[Michael KE4DHK](https://www.wikidot.com/user:info/michael-ke4dhk) 02 Mar 2018 03:17

<span id="post-content-3726143"></span>

Hi. I'm hoping someone can suggest what might be wrong. I'm sure it is probably something obvious.

I have APRSIS32 running, which I am trying to operate from my QTH as a fill-in digi and iGate. I initially had it setup and running through a Mobilinkd TNC. It worked fine, but it seemed like after a period of time (maybe couple hours) I would come back and the scroller was not showing anything received through RF. More recently, I got a PK-232 and set it up to run the home station. At first, it appeared to be working like a charm (and perhaps better than the Mobilinkd with my home radio). But same issue…after an hour or few I look and I'm not showing RF received stations in the scroller.

Any ideas on what the problem might be? (The station id is KE4DHK-10)

Thanks,  
Michael  
KE4DHK

**Reply**
Options

<span id="post-options-3726143"></span>

Unfold
Stops receiving RF after a while by [![Michael KE4DHK](../archive/assets/avatar--821230a2b6c3.php)](https://www.wikidot.com/user:info/michael-ke4dhk)[Michael KE4DHK](https://www.wikidot.com/user:info/michael-ke4dhk), 02 Mar 2018 03:17

<span id="fpc-3739819"></span>

<span id="post-3739819"></span>

Fold

<span id="post-title-3739819"></span>

Re: Stops receiving RF after a while

[![Mark VA2MM](../archive/assets/avatar--c41407aa1fc2.php)](https://www.wikidot.com/user:info/mark-va2mm)[Mark VA2MM](https://www.wikidot.com/user:info/mark-va2mm) 15 Mar 2018 22:16

<span id="post-content-3739819"></span>

Something similar was happening on mine. I discovered from the RF port log that my TNC was getting out of KISS mode. I had set a Quiet Time of 60 minutes on the port configuration (which I thought should force the TNC back into KISS) but setting the Quiet Time to zero actually solved the problem for me. I think maybe I should be using Simply KISS instead. Anyway try setting the Quiet Time to zero on your TNC port to see if that helps.

Mark VA2MM

**Reply**
Options

<span id="post-options-3739819"></span>

Unfold
Re: Stops receiving RF after a while by [![Mark VA2MM](../archive/assets/avatar--c41407aa1fc2.php)](https://www.wikidot.com/user:info/mark-va2mm)[Mark VA2MM](https://www.wikidot.com/user:info/mark-va2mm), 15 Mar 2018 22:16

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-5388296/stops-receiving-rf-after-a-while#post-
