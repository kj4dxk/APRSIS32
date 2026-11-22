---
title: "DigiPeating / Discussion"
---

# DigiPeating / Discussion

[Forum](forum-start-1f3adaff829e.md)
» [WIKI per page discussion / Per page discussions](forum-c-494256-per-page-discussions.md)
» DigiPeating

Started by: Wikidot  
Date: 03 Mar 2013 22:25  
Number of posts: 1  
![rss icon](../archive/assets/common--theme_base_images_feed_feed-icon-14x14--ae830437c8b3.png)
RSS: New posts

This is the discussion related to the wiki page [DigiPeating](../manual/doc-digipeating.md).

Unfold All
Fold All
<span id="thread-toggle-options"></span>
More Options

<span id="thread-options-2"></span>

Lock Thread
Move Thread

<span id="thread-action-area"></span><span id="thread-container"></span>

<span id="thread-container-posts"></span>

<span id="fpc-5244303"></span>

<span id="post-5244303"></span>

Fold

<span id="post-title-5244303"></span>

Implementing Bob WB4APR's "Fixing LA" in APRSISCE/32 on Windows 10 Computer with TNC-2

[![Richard Driscoll](../archive/assets/avatar--5fee4c57951a.php)](https://www.wikidot.com/user:info/richard-driscoll)[Richard Driscoll](https://www.wikidot.com/user:info/richard-driscoll) 29 Mar 2022 03:58

<span id="post-content-5244303"></span>

Good Evening All,

David WB5HJV and I are trying to implement the recommendations contained in Bob's article from 2005 on the APRS website titled "Fixing LA". Unfortunately I am not familiar with the command structure in APRSISCE/32. David is the operator of the W9MAC-1 digitpeater/gateway in Houston and I am trying to assist him in this implementation.

This is what we have so far:

<!RFPort[0]>  
<RFPort Name="TNC2">  
<Protocol>Simply(KISS)</Protocol>  
<Device>COM5:9600,N,8,1</Device>  
<RfBaud>1200</RfBaud>  
<OpenCmd>~!!0</OpenCmd>  
<CloseCmd>~!!0</CloseCmd>  
<QuietTime>60</QuietTime>  
<Enabled>1</Enabled>  
<XmitEnabled>0</XmitEnabled>  
<ProvidesNMEA>0</ProvidesNMEA>  
<RFtoISEnabled>1</RFtoISEnabled>  
<IStoRFEnabled>0</IStoRFEnabled>  
<MyCallNot3rd>0</MyCallNot3rd>  
<NoGateME>1</NoGateME>  
<BeaconingEnabled>1</BeaconingEnabled>  
<BeaconPath></BeaconPath>  
<BulletinObjectEnabled>0</BulletinObjectEnabled>  
<DXEnabled>0</DXEnabled>  
<DXPath></DXPath>  
<MessagesEnabled>0</MessagesEnabled>  
<MessagePath></MessagePath>  
<TelemetryEnabled>0</TelemetryEnabled>  
<TelemetryPath></TelemetryPath>  
<!DigiXform>  
</RFPort>  
<!RFPort[0]>  
<DigiXform>WIDE2-1=W0MAC-1</DigiXform>  
<DigiXform>WIDE2-2=W0MAC-1</DigiXform>  
<DigiXform>WIDE3-1=W0MAC-1</DigiXform>  
<DigiXform>WIDE3-2=W0MAC-1</DigiXform>  
<DigiXform>WIDE3-3=W0MAC-1</DigiXform>  
<DigiXform>WIDE4-1=W0MAC-1</DigiXform>  
<DigiXform>WIDE4-2=W0MAC-1</DigiXform>  
<DigiXform>WIDE4-3=W0MAC-1</DigiXform>  
<DigiXform>WIDE4-4=W0MAC-1</DigiXform>  
<DigiXform>WIDE5-1=W0MAC-1</DigiXform>  
<DigiXform>WIDE5-2=W0MAC-1</DigiXform>  
<DigiXform>WIDE5-3=W0MAC-1</DigiXform>  
<DigiXform>WIDE5-4=W0MAC-1</DigiXform>  
<DigiXform>WIDE5-5=W0MAC-1</DigiXform>  
<DigiXform>WIDE6-1=W0MAC-1</DigiXform>  
<DigiXform>WIDE6-2=W0MAC-1</DigiXform>  
<DigiXform>WIDE6-3=W0MAC-1</DigiXform>  
<DigiXform>WIDE6-4=W0MAC-1</DigiXform>  
<DigiXform>WIDE6-5=W0MAC-1</DigiXform>  
<DigiXform>WIDE6-6=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-1=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-2=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-3=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-4=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-5=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-6=W0MAC-1</DigiXform>  
<DigiXform>WIDE7-7=W0MAC-1</DigiXform>

I would appreciate any useful comments or suggestion as to what I am missing.

Sincerely,

Richard  
KD5URB

**Reply**
Options

<span id="post-options-5244303"></span>

Unfold
Implementing Bob WB4APR's "Fixing LA" in APRSISCE/32 on Windows 10 Computer with TNC-2 by [![Richard Driscoll](../archive/assets/avatar--5fee4c57951a.php)](https://www.wikidot.com/user:info/richard-driscoll)[Richard Driscoll](https://www.wikidot.com/user:info/richard-driscoll), 29 Mar 2022 03:58

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-630615/digipeating#post-
