---
title: "IS to RF"
---

# IS to RF

[Source wiki page](https://aprsisce.wikidot.com/forum/t-871592/is-to-rf) · [Archived HTML](../archive/pages/forum_t-871592_is-to-rf--03c0e3c062d2.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» IS to RF

Started by: [![KK4LQX](../archive/assets/avatar--886263cc6d58.php)](https://www.wikidot.com/user:info/kk4lqx)[KK4LQX](https://www.wikidot.com/user:info/kk4lqx)  
Date: 08 Jun 2014 13:49  
Number of posts: 2  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: [New posts](https://aprsisce.wikidot.com/feed/forum/t-871592.xml)

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

<span id="fpc-2048050"></span>

<span id="post-2048050"></span>

Fold

<span id="post-title-2048050"></span>

IS to RF

[![KK4LQX](../archive/assets/avatar--886263cc6d58.php)](https://www.wikidot.com/user:info/kk4lqx)[KK4LQX](https://www.wikidot.com/user:info/kk4lqx) 08 Jun 2014 13:49

<span id="post-content-2048050"></span>

Hello Everyone,

I have been using APRSIS-32 for about a year now and have most things figured out. The one thing I can't find is how to have APRSIS-32 take objects that are on IS only and have them sent out over RF.

One specific example of this is my neighbor has a weather station that he has going out over IS only. I would like to take this data and packets and TX them over RF.

Any help with this would be greatly appreciated.

Thanks

**Reply**
Options

<span id="post-options-2048050"></span>

Unfold
IS to RF by [![KK4LQX](../archive/assets/avatar--886263cc6d58.php)](https://www.wikidot.com/user:info/kk4lqx)[KK4LQX](https://www.wikidot.com/user:info/kk4lqx), 08 Jun 2014 13:49

<span id="fpc-2380545"></span>

<span id="post-2380545"></span>

Fold

<span id="post-title-2380545"></span>

Re: IS to RF

[![Duarte Braga](../archive/assets/avatar--f8ade4aa8ee3.php)](https://www.wikidot.com/user:info/duarte-braga)[Duarte Braga](https://www.wikidot.com/user:info/duarte-braga) 23 Sep 2015 20:26

<span id="post-content-2380545"></span>

Hi.

You can do this very easily (it took me a while to find the info, as it is not very spread):  
1) Make sure you are running in development mode  
2) Make sure your APRS-IS port is configured to allow IS-RF  
3) Press CNTRL+G and insert a filter of the traffic you want to pass to RF (\*\*\*see notes below)  
4) Move to main Window and Press CTRL+I (then Enter) if nothing happens check the filter error log. If everything goes well, the igate log window will open and you are done.

Regarding the filters, go to page aprs-is-filters in this forum

**Reply**
Options

<span id="post-options-2380545"></span>

Unfold
Re: IS to RF by [![Duarte Braga](../archive/assets/avatar--f8ade4aa8ee3.php)](https://www.wikidot.com/user:info/duarte-braga)[Duarte Braga](https://www.wikidot.com/user:info/duarte-braga), 23 Sep 2015 20:26

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-871592/is-to-rf#post-
