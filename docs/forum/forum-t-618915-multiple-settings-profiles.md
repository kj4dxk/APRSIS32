---
title: "Multiple Settings Profiles"
---

# Multiple Settings Profiles

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / Feature Requests](forum-c-312348-feature-requests.md)
» Multiple Settings Profiles

Started by: [![APRS-Jeff](../archive/assets/avatar--ec5b2bb153c2.php)](https://www.wikidot.com/user:info/aprs-jeff)[APRS-Jeff](https://www.wikidot.com/user:info/aprs-jeff)  
Date: 26 Jan 2013 04:55  
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

<span id="fpc-1695438"></span>

<span id="post-1695438"></span>

Fold

<span id="post-title-1695438"></span>

Multiple Settings Profiles

[![APRS-Jeff](../archive/assets/avatar--ec5b2bb153c2.php)](https://www.wikidot.com/user:info/aprs-jeff)[APRS-Jeff](https://www.wikidot.com/user:info/aprs-jeff) 26 Jan 2013 04:55

<span id="post-content-1695438"></span>

Lynn,

I was contemplating changing several of my settings the other day (for mobile ops from a sailboat), and I realized several things that I might manually want to change (SSID, map tile set, APRS icon, path, etc.) on a recurring basis. I then thought about the paradigm that the Kenwood Radios (D700, D710) utilize with their Programmable Memories (PMs)

My idea would be for APRSISCE/32 to employ the concept of "profiles". Using this concept, the operator could set, save, and recall groups of settings. These "profiles" could be named for easy recall, and be implemented by merely managing xml configuration file(s) with a little bit of program logic. In fact, I'm doing this now on my WinMo device with a scripting language. I call a script from a shortcut which renames/copies a certain copies of the setup xml files, and then calls the program. But, it would be more elegant if the program handled profile management internally.

Anyway, I thought I would pass this along as a suggestion if you deem it worthy for your next upgrade.

73,  
Jeff  
N5TEV

**Reply**
Options

<span id="post-options-1695438"></span>

Unfold
Multiple Settings Profiles by [![APRS-Jeff](../archive/assets/avatar--ec5b2bb153c2.php)](https://www.wikidot.com/user:info/aprs-jeff)[APRS-Jeff](https://www.wikidot.com/user:info/aprs-jeff), 26 Jan 2013 04:55

<span id="fpc-1851443"></span>

<span id="post-1851443"></span>

Fold

<span id="post-title-1851443"></span>

Re: Multiple Settings Profiles

[![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce) 13 Sep 2013 19:14

<span id="post-content-1851443"></span>

You can do this today by simply running APRSIS32 from a different directory. Since the XML configuration file is stored in the default directory when the .EXE is run, each directory ends up with it's own configuration. You can even copy the APRSIS32.XML file from one directory to a new one and then make changes from there.

I have multiple configurations available, some of which are concurrently active (with unique -SSIDs), and run them all from a single copy of APRSIS32.EXE with multiple short-cuts each specifying a different default directory for execution.

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

**Reply**
Options

<span id="post-options-1851443"></span>

Unfold
Re: Multiple Settings Profiles by [![APRS IS CE](../archive/assets/avatar--2329414266d6.php)](https://www.wikidot.com/user:info/aprs-is-ce)[APRS IS CE](https://www.wikidot.com/user:info/aprs-is-ce), 13 Sep 2013 19:14

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-618915/multiple-settings-profiles#post-
