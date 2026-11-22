---
title: "Receive Only Igates DO NOT Break Messaging!"
---

# Receive Only Igates DO NOT Break Messaging!

[Source wiki page](https://aprsisce.wikidot.com/doc:receive-only-igates-break-messaging) · [Archived HTML](../archive/pages/doc_receive-only-igates-break-messaging--4840f5a09bfa.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

TL;DR - Receive-Only IGates are not what breaks messaging. It is Transmit-Capable IGates connected to an APRS-IS Server that does not have an upstream full feed.

This is the raw text that I had posted in the APRS Foundation Discord:

So, how do receive-only IGates "break" APRS messaging? Well, the obvious one is that they break the expectation that messaging from the APRS-IS will work simply because a station's packets appear on the APRS-IS. But there's a more insidious bust as well. Brace yourselves, this journey may get a bit rocky along the way.

> First, the APRS-IS is not an intelligent routing network. Every IGate makes its own decision on what to transmit. It is independent of any other IGate's decision. The bust has to do with a receive-only IGate providing a packet into the APRS-IS quicker than a transmit-capable IGate that is connected to a filtered-feed APRS-IS server. And the issue stems from the APRS-IS servers each doing duplicate suppression of packets. I don't recall anyone ever putting together a diagram of this, but I'll try to draw you a word picture.

> Ok, before you can understand the issue, you need to understand how the APRS-IS does duplicate suppression and what it also does to support IGate connections. Duplicate suprression is easy: each individual APRS-IS server will summarily discard all duplicates of any packet received within 30 seconds (IIRC) of the original copy of the packet. Duplicate is defined as same source and destination callsign (before and after the greater than sign) and payload (after the colon). The path is not used by the duplicate suppression logic. This is for TNC2-formatted, humanly readable packets as in:
>
> > source>dest,path,path,q-construct,IGateCall:payload

> Notice that I said each individual APRS-IS server does this. This means that different copies of a single packet may be simultaneouosly traversing the APRS-IS and different clients connected to different servers may see a different copy than some other client sees from a different server. I won't go into how that happens because I think it will be demonstrated when I get to the full description.

> The APRS-IS servers support IGates by keeping track of the source callsign of every packet received on every IGate-supporting port connection (typically 14580). I qualify tht with "IGate-supporting" because the firenet -IS servers don't offer IGate-support even on prot 14580 (at least, that's what I've been told in the past). When an APRS-IS server receives a message packet, that packet is sent to any connection that has received a packet from the addressed callsign within the past 30 minutes. And the server will also forward the next position packet from the source of that addressed message to the same connection to which the message was sent.

> So, for example, an IGate running as IGATE-1 copies a packet from STATIN-1 and gates it to its APRS-IS server. The server duly notes the connection that copied STATIN-1. A station SOURCE-1 sends a message addressed to STATIN-1. This propates through the network and arrives at IGATE-1's APRS-IS server. It notes the address to STATIN-1 and forwards that message to the connection from IGATE-1. It is up to the software running as IGATE-1 to decide if that message will be transmitted on RF or not. The APRS-IS server actually forwards more traffic than IGates are supposed to transmit. Anyway, SOURCE-1 subsequently transmits a position report packet which also propagates through the network and when IGATE-1's server sees that posit, it notes that a message from SOURCE-1 was forwarded to IGATE-1's connection, so it forwards the position report as well. Again, it is up to IGATE-1's software to decide whether or not to transmit the postion report on the local RF.

> Now, with that (new?) knowledge of how things work (note that there wasn't any intelligent routing describe in the APRS-IS network above), here's how a receive-only IGate can "break" APRS messaging. It takes 2 IGates, and a few APRS-IS servers, one of wihch is a local server running on a filtered feed and this is the server that the transmit-capable IGate is connected to. Notice that it's the TRANSMIT-CAPABLE IGate that is NOT connected to a mainstream server that will be causing the issue, not actually the receive-only IGate by itself.

> We're going to put STATIN-1 in simplex range of 2 IGates - IGATE-1 which is transmit-capable and RGATE-2 which is receive-only. RGATE-1 is connected to port 14580 on a mainstream APRS-IS server (core or Tier2) but IGATE-1 has a local APRS-IS server which has an upstream filtered connection to a mainstream server on port 14580. To keep this a bit simpler, RGATE-2's APRS-IS server happens to be the SAME server that IGATE-1's local server is connected to. They can actually be any server in the APRS-IS network, but propagation and duplicate suppression is easier to envision if the are the a single server.

> One more pre-condition is that the receive-only IGate RGATE-2 is actually faster at delivering packets to the APRS-IS network than IGATE-1. And further, IGATE-1's local APRS-IS server is also a bit slower at delivering packets to its upstream server (the same server that RGATE-2 is directly delivering the packets to).

> So, what happens when STATIN-1 transmits any packet? Both IGATE-1 and RGATE-2 copy the packet, but RGATE-2 is faster and delivers the packet to the shared APRS-IS server which propagates it to the rest of the APRS-IS network. This packet happens to match the filter used by IGATE-1's connection to the server, so the packet is delivered to IGATE-1. Some (short) time later, IGATE-1 delivers its duplicate of the packet to the local server. The server notes it as a duplicate (remember, it got RGATE-2's copy already from the upstream server), so it doesn't foward the packet to the upstream server. Now, the upstream server knows that RGATE-2 has copied STATIN-1, and the local server knows that IGATE-1 copied STATIN-1 as well, but the filtered connection to the upstream server has never heard a STATIN-1 packet from the local server's connection.

> And what happens when SOURCE-1 sends a message to STATIN-1 from somewhere on the APRS-IS? That message will get to the shared APRS-IS server which will only send the message down to RGATE-2. IGATE-1's local APRS-IS server will not "see" the message because it has locally dupe-suppressed all local packets from STATIN-1 because they were quicker coming through from RGATE-2 and the upstream server. Boom, messaging to STATIN-1 is "broken" because of the receive-only IGate. With out that packet source of STATIN-1, dupe suppression would not happen and the upstream server would have seen STATIN-1 packets from the local APRS-IS server and would deliver the messages to IGATE-1.

But I contend that the issue is not just with the receive-only IGate, but also the fault of the transmit-capable IGate running on a local, filtered APRS-IS server. If this local server was running on a full feed, rather than filtered, upstream connection, then the message from SOURCE-1 to STATIN-1 would have been delivered to the local APRS-IS server which would then correctly deliver that message to IGATE-1's connection since STATIN-1 has been observed on that connection.

And I also found an email posted to the aprssig mailing list by Pete Loveall, AE5PL, the author of javAPRSsrvr from 7/15/2016 at 2:15pm (according to my e-mail client). The subject is "IGate Registering for ANSRVR (Messaging failures)".

> I will give you two specific examples that shows how a RO IGate can break messaging.

> #1 is Anthony's example. The RO IGate gates his HT's message packet (let's say it is a messaging capable HT) but a bidirectional packet does not because it never gets digipeated. The remote station sees his packet but the remote station's ack and possible responses are not seen since no bidirectional IGate considers his HT as local.

> #2 is a little more obscure but just as important since it is much harder to isolate. Bidirectional IGate (let's call it RW) is connected to a server (connection type is irrelevant for this connection). That server (let's call it RW's server since it is often times either integral or in the same location as the RW IGate) is connected to APRS-IS via a restricted feed ("filter" is an inaccurate word; the feed is "restricted" to packets for the connected station and any stations the connected station has recently gated to APRS-IS plus any packets defined in a filter if defined). Before you say it can't be or that is bad design, that is how most home installations of javAPRSSrvr are configured as they are on restricted bandwidth connections. There are others who also run in this type of configuration. Now, let's use the example in #1 but say the RW IGate does see that packet after one hop. But the RO IGate already got the packet through APRS-IS to RW's server so dupe elimination prevents RW's server from sending the RW-received packet upstream. Therefore, the upstream server to RW never sees the packet that RW gated to its server.

> Now you might claim close-coupling of the software in #2 could force RW's server to send the packet anyway regardless of dupes but that is a bit self-defeating for other reasons and does not resolve the issue when the IGate is completely separate software from the server.

> I am not assigning "fault" to anyone or anything. I am pointing out how RO IGates can and do interfere with APRS messaging. I am also pointing out how this can be very difficult to track down with the denial of such cases (and these are only 2 examples).

And here's Hessu Heikki Hannikainen, OH7LZB - the author of aprs.fi, article on the matter as well. Worded MUCH better than mine!

<https://blog.aprs.fi/2019/08/rx-only-igates-considered-beneficial-to.html>
