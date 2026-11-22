---
title: "Message Retries"
---

# Message Retries

So, how does APRSISCE/32 handle message retries? And how can I tell what's going on? I'm glad you asked!

The first indication that the client has outbound messages on the retry queue is that there's either a Yellow background on "No Msg" or an Orange background on Callsign-SSID in the message pane. Yellow means there's only outbound messages pending and Orange means there's both unread inbounds and outbound pending.

If you double-click on the Yellow pane, or bring up Messages / Pending Messages, one of two things will happen. If there's only one pending message, it will come up for viewing and disposition. If you have more than one, a popup menu will be presented with the pending callsigns. Selecting one of the callsigns from the popup will present the corresponding message.

A Pending Message dialog will show the recipient of the message, the body of the message, the retry status, and offer to Abort, Retry or Ignore. Ignore simply dismisses the dialog and things proceed as if you hadn't looked. Abort will delete the outbound message and kill all pending retries. Retry will cause the message to be immediately retried as if it was Retriggered (see below).

So, what's with all of the Retry, Final Retry, Second Retry, and FINAL retry?

APRSISCE/32 uses a decaying retry algorithm originally outlined by Bob Bruninga. As of this writing (12/10/2010), it does a total of 7 retries starting at 8 seconds and doubling through a total of 7 retries. This gives a total retry set of 8+16+8+32+64+96+128 seconds or just under 6 minutes total. That's the main Retry phase. After exhausting that sequence, the dialog will say "Final Retry (N) sent M minutes ago" where M increases with time.

The 8 after the 16 was added on 12/10/2010 after someone noticed that the first retry through a remote IGate was over a minute after the first transmission. This was due to 8+16=24 being just under the 30 second dupe detector and the next retry after 32 more seconds or 56 seconds after the first transmission. The additional 8 gives APRS-IS retries at 32, 32, 64, 96, and 128 seconds allowing about as quick of a retry as APRS-IS will allow.

And on 10/24/2011 (or so), I discovered that APRS-IS seems to reset the 30 second dupe filter on each duplicate reception totally 'nixing the previous paragraph's assumptions. So, APRSISCE/32 now suppresses all <30 second message retries to -IS so that the first retry >30 seconds (T+32 after +8+16+8) will actually pass through the dupe filter instead of being suppressed because of a dupe-filtered retry only 8 seconds prior. Before this correction, the first APRS-IS passed retry was actually 64 seconds (+8+16+8+32) seconds after the initial transmission. WAY longer than necessary or desired.

Now, here's the Retrigger feature, known as "Message-On-Heard" on page 10 (20 in Acrobat) in aprs101.pdf (the "bible" of APRS). If a beacon is heard from a station that has exhausted the Final Retry (or you hit the Retry button), a whole new set of retries is initiated. This is indicated by "Second Retry n of m in s seconds" being displayed in the dialog. When the full set of retriggered retries have been exhausted (another 7 transmissions over 6 minutes), the dialog will say "FINAL Retry (N) sent M minutes ago" and stay that way until a) the station sends a ?APRSM query or b) you manually re-retrigger (Retry) it or c) you Abort it.

Multiple messages for a single recipient are delivered in the order they were entered. This means that if a particular message to a specific recipient exhausts is retries, all subsequent pending messages will be blocked for that recipient. The Pending Messages list will show all messages for a given recipient, but only the oldest one is actually being transmitted and retried.

Now, which interfaces are messages sent over, you might ask? Here's the deal. The second (retriggered) set of retries are sent over all enabled ports. This is regardless of where or if the station was previously heard. Initial message transmissions and acks are only sent "smart". This means that if the station has not been heard, APRSISCE/32 doesn't know where it is and sends over all available ports. If the station is known and has not been heard on RF, the message is sent only via APRS-IS. If a station has been heard on RF, the message and/or ack is sent over RF and APRS-IS. Note that currently (10/24/2011) RF means ALL configured and enabled and message-enabled RF ports, regardless of which one the station was heard over.

One final caveat: If you originally specified that a message be sent RF Only, that will be honored throughout both retry sets.
