---
title: "APRS Messaging EXPLAINED!"
---

# APRS Messaging EXPLAINED!

## APRS Messaging Explained

APRS messaging is a complicated beast that needs a whole bunch of components to be in place and working in order for it to succeed. I'll build up to the full story starting with RF to RF simplex (but your stations have to be in range of each other) and end with RF to IS to RF.

As you might know, APRS uses packet radio's UI packet which is connectionless, non-sequenced, non-guaranteed delivery, and non-acknowledged. Basically, in APRS, a station throws a packet onto the airwaves and has no clue where it went if anywhere. Good for general beaconing, but not so good for messaging. APRS also uses a path similar to packet in that it is in the AX.25 header, but it can contain aliases which to which multiple digipeaters will respond to repeat a packet over larger areas (like WIDE1-1,WIDE2-1, and so forth).

Ok, so an APRS message is actually a standard APRS UI packet, but the destination callsign of the message isn't in the AX.25 header, but is within the data payload, delimited by colons (:KJ4ERJ-15: for instance). APRS messages can be sent either asking for an acknowledgment or not. If an ack is desired, the sender will put a {xxxxx at the end of the message text. The ack sent back by the recipient is just another APRS message directed to the originator with the text of ackxxxxx (and of course NOT including an { ack request!).

So, in an RF simplex situation, the originating APRS station simply sends out an APRS message (typically formatted by an APRS client and sent out through some sort of TNC), the receiver has to hear the packet, decode it successfully in some sort of TNC (AX.25 packets have a checksum), optionally format an ack message, send it back onto the RF where the original sender has to hear it, decode it, and match it up with the original message. Typically APRS client software (even if that software is embedded in a transceiver like a D7, D700, D710 or VX-84) will implement some number of retransmissions until the ack is heard. If a reply is desired, simply reverse the roles of the stations and repeat. And that's the simple case!

Enter the APRS-IS, the Internet side of APRS. There is no "central" APRS server as some people believe. The APRS-IS is simply a series of connections through which APRS packets flow in realtime. Sites like aprs.fi and findu.com are not really part of the APRS-IS, but monitor the packets going through the APRS-IS and store them into a database. The APRS-IS itself stores nothing. It's just a transport.

Note: In order to do direct messaging via the APRS-IS, a verified connection is required. This means you have to have a passcode that matches your callsign. -1 as a passcode will not work for messaging as the APRS-IS will not accept any of your packets.

So, sending an APRS message from one -IS connected station to another -IS connected station is just like the RF simplex situation above, but without the need for a TNC and without the possibility of needing to retry in the event of checksums. The -IS only delivers good packets. However, the recipient of a message must be on-line and connected for a successful message interchange. There is no store and forward APRS message delivery (yet).

What about RF to -IS? What's an IGate? How does it figure into messaging? Well, an IGate is simply an APRS RF station that listens for packets on the radio, decodes them, validates their checksums and injects them into the APRS-IS for transport. Once injected, only one copy of any given packet is forwarded to all currently connected -IS nodes (well, there's some filtering in there, but messages go everywhere for the most part). That's the receiving part of an IGate. All IGates at least do that much.

IGates can also be configured to gate selected APRS packets from the -IS back out to RF. Typically, this is just APRS message traffic, but other APRS messages can also be gated from the -IS to RF. But wait, you say, the -IS is much faster than the 1200 baud (or 300 on HF) APRS RF channel. You're right, so not all messages get gated. IGate software watches the stations that have been heard on the RF channel and records a list of "recently" (typically 30 minutes) heard "local" stations (limited by used hops). Only messages heard on the -IS targeting one of these recent local stations will be gated to RF.

So, for an -IS station to send a message to an RF station, the APRS client software creates a message for another station and injects it into the APRS-IS. This message goes everywhere and all connected IGates check to see if it trying to go to a local station that it has recently heard. Every IGate that has recently heard the station locally will put the message onto RF. Well, not every IGate, unfortunately, but only those configured to gate APRS messages from the Internet to RF. Once the message is on RF, the intended recipient must heard the packet, decode it, validate the checksum, and issue an ack if requested by the originator. This piece is identical to the RF simplex case above, but this time an IGate (any receiving IGate, not necessarily the one that gated the outbound message) needs to hear, decode, validate, and inject the ack back into the APRS-IS so that the originating client gets the ack and doesn't retransmit.

Ok, so for -IS to RF, we needed at least one bi-directional IGate that had heard the destination station locally recently. What about RF to -IS?

Simply pull the right parts from the above discussions and you'll have it. The RF station formats the message and transmits it. An IGate hears it, decodes it, verifies the checksum and injects it into APRS-IS. Once there, if there is a client connected to the APRS-IS with the destination callsign, it will hear the message and format an ack if requested by the originator. That ack goes back into the -IS as just another message to be gated. Some IGate must have heard the originating station recently locally and will transmit that message back onto the RF where it must be heard, decoded, and verified. If the ack doesn't make it back out (for instance, because it's only in range of a receive-only IGate), the RF originator will continue retrying the message until the retires are exhausted. The APRS-IS receiving station will receive multiple copies of this message, issuing an ack for each one that never makes it back out onto RF.

So, what about RF to IS to RF (which is what you'll need for someone in Florida to APRS message someone in Virginia unless you're doing it simplex over HF. Here's the sequence:

1) Originating station formats an APRS message for the intended recipient  
2) IGate hears the message, decodes it, verifies checksum and injects into APRS-IS  
3) The whole world hears the message including a few IGates in Virginia  
4) Hopefully one of those IGates has recently heard the recipient local enough AND that IGate is configured to gate messages from -IS to RF  
5) The IGate transmits the message onto RF  
6) Hopefully the recipient hears the message, decodes it, verifies the checksum, and displays it to the operator.  
7) Because most of the time acks are requested, the receiver formats an ack message and sends it over RF.  
8) Repeat step 2 in Virginia  
9) Repeat step 3 moving to Florida  
10) Repeat step 4 in Florida (remember, an ack is just another APRS message)  
11) Repeat step 5  
12) Repeat step 6 but don't display to the operator, just mark the message received and quit retransmitting.

So you see, there's a WHOLE LOT of stuff that has to a) exist (IGates) b) be configured (gate messages IS to RF), c) work (decoding in remote stations) to even get ONE message through RF-IS-RF. Now, do that round trip for EVERY SINGLE message in a QSO, and you can see the miracle that APRS messaging QSOs represent!

Lynn (D) - KJ4ERJ - Palm Bay, FL IGate Operator - APRS-IS messaging as KJ4ERJ-12
