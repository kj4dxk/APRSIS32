---
title: "UIFLOOD Hardware Testing"
---

# UIFLOOD Hardware Testing

[Source wiki page](https://aprsisce.wikidot.com/uiflood-hardware-testing) · [Archived HTML](../archive/pages/uiflood-hardware-testing--efed9d74ec2e.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

The Kenwood D700 and it's brethern are described as the preferred hardware for use in the [Golden Packet Event](http://www.aprs.org/at-golden-packet.html), an event that sees up to 15 APRS digipeaters operating from mountain tops along the Appalachian Trail on the east coast of the USA. This event creates an RF only long haul linear digipeater chain, and using a clear VHF frequency attempts to pass an RF packet along the full 15 digipeater hop chain.

This type of long distance multi-hop RF type of operation is normally frowned upon on the regular APRS network due to the extreme loading that it puts on the APRS network. This special event operating on a clear VHF frequency with only very specific stations involved however makes an attempt to exploit the very operating circumstances that are detrimental to normal day-to-day APRS network operations.

In order to be able to request such a long path (AX.25 allows for a maximum of 7 hops), use of the UIFLOOD style digipeating path is required. The UIFLOOD concept requires that an alias with an SSID be used, where the SSID describes the number of digipeats desired. The standard usage of this routine is to set an outgoing path for your packets to ALIASn-N where ALIAS is the generic digipeater name, n indicates the initial number of hops requested, and N indicates the number of hops remaining. The [Golden Packet Event](http://www.aprs.org/at-golden-packet.html) uses the ALIAS HOP, and the maximum number of hops available, HOP7-7.

The UIFLOOD routine can either simply pass the packet along, decrementing the SSID in the process, or it can include its callsign in the used path, allowing one to see the last used digipeater as well as decrementing the SSID. Because this event is all about moving packets along the trail, the last used digipeater is an important piece of desirable information.

In order to insert the digipeater callsign into the packet, on the first digipeat request, the UIFLOOD routine must add it's callsign to the path, marking it as used up, and then decrement the current path element's SSID. On subsequent action, the previous digipeater's callsign must be removed and replaced with the current digipeater callsign, and the SSID decremented until an SSID of zero is reached, whereupon the path element is also marked as used.

A packet entering the digipeater chain with a path request of HOP7-7 would be handled by the digipeaters as follows:

HOP7-7  
DIGI1\*,HOP7-6  
DIGI2\*,HOP7-5  
DIGI3\*,HOP7-4  
DIGI4\*,HOP7-3  
DIGI5\*,HOP7-2  
DIGI6\*,HOP7-1  
DIGI7\*,HOP7\*

As can be observed, as each digipeater handles the packet, the associated callsign is inserted into the path, with an asterisk indicating that that path element has been "used up", and the next path element in the list should be considered as the current digipeat request. As there are 15 mountaintops in the Appalachian Trail [Golden Packet Event](http://www.aprs.org/at-golden-packet.html), more digipeat requests are needed, hence the use of the path HOP7-7,HOP7-7. As above, the digipeaters will act upon the path elements from left to right until all elements are used.

HOP7-7,HOP7-7  
DIGI1\*,HOP7-6,HOP7-7  
DIGI2\*,HOP7-5,HOP7-7  
DIGI3\*,HOP7-4,HOP7-7  
DIGI4\*,HOP7-3,HOP7-7  
DIGI5\*,HOP7-2,HOP7-7  
DIGI6\*,HOP7-1,HOP7-7  
DIGI7\*,HOP7\*,HOP7-7  
DIGI7\*,HOP7\*,DIGI8\*,HOP7-6  
DIGI7\*,HOP7\*,DIGI9\*,HOP7-5  
DIGI7\*,HOP7\*,DIGI10\*,HOP7-4  
DIGI7\*,HOP7\*,DIGI11\*,HOP7-3  
DIGI7\*,HOP7\*,DIGI12\*,HOP7-2  
DIGI7\*,HOP7\*,DIGI13\*,HOP7-1  
DIGI7\*,HOP7\*,DIGI14\*,HOP7\*

Proper implementation of the UIFLOOD routine is a requirement for the used digipeaters and decrementing SSIDs to be modified correctly. The UIFLOOD routine also keeps a CRC checksum of the source and payload portions of the packet in order to keep from having the same packet handled multiple times by the same digipeater.

Observation of packet logs from past [Golden Packet Event](http://www.aprs.org/at-golden-packet.html) activities indicates that there may be some anomalous activity taking place where packets are not being marked up properly, and digipeaters are possibly handling packets a second time.

To that end, some test packets have been generated, handled by the hardware under test with various UIFLOOD configurations enabled. These are the results:

[D700 UIFLOOD Digipeating](d700-uiflood-digipeating.md)
