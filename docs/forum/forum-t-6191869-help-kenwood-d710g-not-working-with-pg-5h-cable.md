---
title: "Help Kenwood D710G not working with PG-5H cable"
---

# Help Kenwood D710G not working with PG-5H cable

[Forum](forum-start-1f3adaff829e.md)
» [APRSISCE/32 Discussion / General APRSIS](forum-c-312347-general-aprsis.md)
» Help Kenwood D710G not working with PG-5H cable

Started by: [![ Bruce roaf](../archive/assets/avatar--9e6e30748629.php)](https://www.wikidot.com/user:info/bruce-roaf) [Bruce roaf](https://www.wikidot.com/user:info/bruce-roaf)  
Date: 19 Jun 2018 14:37  
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

<span id="fpc-3841901"></span>

<span id="post-3841901"></span>

Fold

<span id="post-title-3841901"></span>

Help Kenwood D710G not working with PG-5H cable

[![ Bruce roaf](../archive/assets/avatar--9e6e30748629.php)](https://www.wikidot.com/user:info/bruce-roaf) [Bruce roaf](https://www.wikidot.com/user:info/bruce-roaf) 19 Jun 2018 14:37

<span id="post-content-3841901"></span>

Hi  
I have now spent hours on hours trying to get the GPS and APRS data from the control head of the Kenwood D710G to work. I am using the Kenwood PG-5H lead from the head directly into the serial port of my Panasonic Tough book. No matter what I do I cannot get the Program to receive any data through the lead. The lead works perfectly on the data outlets on the main body of the radio and I can read and write to the radio to and from the Panasonic laptop.  
Does anyone have any suggestions on what I may be doing wrong ? I thought that using the PG-5H lead directly into the laptop was going to be an easy Solution to getting all the APRS data into the software. I have a Raynet event in a few weeks and I would like to track all the other operators on my laptop using Kenwood D710G as well as showing my position on the map.

Any help gratefully received  
Bruce 2E0RJB

**Reply**
Options

<span id="post-options-3841901"></span>

Unfold
Help Kenwood D710G not working with PG-5H cable by [![ Bruce roaf](../archive/assets/avatar--9e6e30748629.php)](https://www.wikidot.com/user:info/bruce-roaf) [Bruce roaf](https://www.wikidot.com/user:info/bruce-roaf), 19 Jun 2018 14:37

<span id="fpc-4000832"></span>

<span id="post-4000832"></span>

Fold

<span id="post-title-4000832"></span>

Re: Help Kenwood D710G not working with PG-5H cable

[![VE5BNC](../archive/assets/avatar--7ae9ae550d9b.php)](https://www.wikidot.com/user:info/ve5bnc)[VE5BNC](https://www.wikidot.com/user:info/ve5bnc) 23 Sep 2018 14:44

<span id="post-content-4000832"></span>

Hi Bruce

You're decribing my exact configuration although I built my own cable. The configuration is documented on this site if you just search for D710. Keep in mind that I found that the development version after February 2017 isn't parsing the GPS pass through from the D710. The public stable release and all development releases including February 2017 work just fine.

You have to run the D710 in APRS mode and have the radio configured to output on the serial port on the head. I think this is menu 604.

Once you have the radio configured, connect your computer and use your favourite terminal program to monitor the output. You should see something like this:

$PKWDPOS,032136,A,5206,N,10637.002,W,0,0,221213,200.00\*2A  
$PKWDPOS,032138,A,5206,N,10637.002,W,0,0,221213,200.00\*24  
$PKWDPOS,032140,A,5206,N,10637.002,W,0,0,221213,200.00\*2B  
$PKWDPOS,032142,A,5206,N,10637.002,W,0,0,221213,200.00\*29  
$PKWDPOS,032143,A,5206,N,10637.002,W,0,0,221213,200.00\*28  
$PKWDPOS,032145,A,5206,N,10637.002,W,0,0,221213,200.00\*2E  
$PKWDPOS,032148,A,5206,N,10637.002,W,0,0,221213,200.00\*23  
VE5AA-11>APZBNC,WIDE2-2:!0000.00N/00000.00EO000/000/A=000000/V=10.2/Ti=-127/Te=-127 \*Stale\*  
$PKWDPOS,032150,A,5206,N,10637.002,W,0,0,221213,200.00\*2A

cmd:VE5AA-11>APZBNC,WIDE2-2:T#000,144,001,001,000,000,00000000  
VE5AA-11>APZBNC,WIDE2-2::VE5AA-11 :PARM.Battery,ITemp,Etemp $PKWDPOS,032152,A,5206,N,10637.002,W,0,0,221213,200.00\*28  
VE5AA-11>APZBNC,WIDE2-2::VE5AA-11 :UNIT.V,degC,degC  
VE5AA-11>APZBNC,WIDE2-2::VE5AA-11 :EQNS.0,0.0743,0,0,1,-128,0,1,-128,0,0,0,0,0,0  
VE5AA-11>APZBNC,WIDE2-2::VE5AA-11 :BITS.11111111  
$PKWDPOS,032154,A,5206,N,10637.002,W,0,0,221213,200.00\*2E  
$PKWDPOS,032155,A,5206,N,10637.002,W,0,0,221213,200.00\*2F

Once you've confirmed the radio is outputting the right data, you can configure APRSIS32. My configuration is slightly different than the documentation shows, my configuration doesn't configure the D710 at all because it wasn't valuable to me. The down side is that I lose the ability for the laptop to transmit via the D710. Here's my D710 port configuration from Aprsis32:

<!RFPort[1]>  
<RFPort Name="D710">  
<Protocol>KWD710(APRS)</Protocol>  
<Device>COM3:9600,N,8,1</Device>  
<RfBaud>1200</RfBaud>  
<OpenCmd>~!!0</OpenCmd>  
<CloseCmd>~!!0</CloseCmd>  
<QuietTime>0</QuietTime>  
<Enabled>1</Enabled>  
<XmitEnabled>0</XmitEnabled>  
<ProvidesNMEA>1</ProvidesNMEA>  
<RFtoISEnabled>1</RFtoISEnabled>  
<IStoRFEnabled>1</IStoRFEnabled>  
<MyCallNot3rd>0</MyCallNot3rd>  
<NoGateME>0</NoGateME>  
<BeaconingEnabled>0</BeaconingEnabled>  
<BeaconPath></BeaconPath>  
<BulletinObjectEnabled>0</BulletinObjectEnabled>  
<DXEnabled>0</DXEnabled>  
<DXPath>RFONLY</DXPath>  
<MessagesEnabled>0</MessagesEnabled>  
<MessagePath></MessagePath>  
<TelemetryEnabled>0</TelemetryEnabled>  
<TelemetryPath></TelemetryPath>  
<!DigiXform>  
</RFPort>  
<!RFPort[1]>

73, Bruce - VE5BNC

**Reply**
Options

<span id="post-options-4000832"></span>

Unfold
Re: Help Kenwood D710G not working with PG-5H cable by [![VE5BNC](../archive/assets/avatar--7ae9ae550d9b.php)](https://www.wikidot.com/user:info/ve5bnc)[VE5BNC](https://www.wikidot.com/user:info/ve5bnc), 23 Sep 2018 14:44

<span id="new-post-button"></span>
New Post

<span id="post-options-template"></span>

Permanent Link
Edit
Delete

<span id="post-options-permalink-template"></span>

/forum/t-6191869/help-kenwood-d710g-not-working-with-pg-5h-cable#post-
