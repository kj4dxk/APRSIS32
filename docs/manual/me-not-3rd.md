---
title: "Me Not 3rd"
---

# Me Not 3rd

Me not 3rd is a rather cryptic term that is described as below:

To understand what this does, you have to understand how messages are gated from -IS to RF via "third party" packets. Basically, an Igate takes an entire APRS message packet from the -IS, puts it in the payload of a new packet with the header specifying the Igate itself. This is because the IGate owner (represented by the callsign) is responsible for all transmitted content. A 3rd party packet would look like:

> KJ4ERJ-1>APWW08,WIDE2-1:}KD4TVE-1>APU25N,TCPIP,KJ4ERJ-1\*::N3OS-5 :?APRST

This particular packet was a message from KD4TVE-1 to N3OS-5 who I apparently considered to be "recently" "local" at the time. (I haven't quite decided why KJ4ERJ-1 needs to appear twice, but see the footnote for the specification.)

Now, let's pretend that it was my -IS-only KJ4ERJ-12 station that actually wanted to do the ?APRST to N3OS-5. That would appear as follows in 3rd party format:

> KJ4ERJ-1>APWW08,WIDE2-1:}KJ4ERJ-12>APWW08,TCPIP,KJ4ERJ-1\*::N3OS-5 :?APRST

But the reason for the third party wrapper is to provide identification of the IGate owner. Well, my callsign is both the originator of the message and the owner of the IGate transmitting the message onto RF, so who's the third party? Me. So, if you check MeNot3rd, the IGate will detect that a gated message is coming from another of the IGate's -SSIDs and will transmit the following instead:

> KJ4ERJ-12>APWW08,TCPIP,KJ4ERJ-1\*,WIDE2-1::N3OS-5 :?APRST

This packet says that it comes from KJ4ERJ-12, was processed by KJ4ERJ-1 (hence the \*) and still has a remaining path on RF of WIDE2-1 (which is the outgoing path configured into the KJ4ERJ-1 IGate). This is a much shorter packet that contains the same information and identification as the original. Because the original TCPIP is still in the path, the packet should not gate back onto the -IS.<span id="footnoteref-1"></span>
1

Oh, and this only happens if the -IS-sourcing callsign is AX.25 compatible (-SSID between -0 (suppressed) and -15).

So, MeNot3rd is a way to allow an IGate operators to gate messages from their own -IS stations to RF in a shorter form while retaining all ownership and responsibility for said packet content.

Footnotes

<span id="footnote-1"></span>

1. From <http://www.aprs-is.net/IGateDetails.aspx>

IGates must use the 3rd-party format on RF of

> IGATECALL>APRS,GATEPATH}FROMCALL>TOCALL,TCPIP,IGATECALL\*:original packet data

where GATEPATH is the path that the gated packet is to follow on RF. This format will allow IGates to prevent gating the packet back to APRS-IS.

From the same page:

Gate all packets heard on RF to the Internet EXCEPT packets with TCPIP, TCPXX, NOGATE, or RFONLY in the header (last 2 are optional).

and

Except for within gated packets, TCPIP and TCPXX should not be used on RF.
