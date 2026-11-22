---
title: "Configure DX"
---

# Configure DX

DX in APRSISCE/32 is defined as receiving a position packet with no hops used from a station located further away than some minimum distance. The current furthest DX may be included in the periodic Status Report packet and also returned in response to a ?DX directed query. Additionally, newly set DX stations beyond a configurable distance may be immediately reported via a DX cluster formatted packet and may also cause an immediate Status Report to be issued.

The following menu options govern the DX behaviors of APRSISCE/32.

**Min Dist** specifies the minimum distance required for a packet to be considered "DX". The parameter is specified in miles or kilometers depending on the Configure / Metric / Distance setting.

**Min Trigger** specifies the minimum distance that may trigger an immediate transmission of either a DX cluster or Status Report packet provided that no such packet has been triggered within the past Min Interval minutes, or the new DX is further than the previous DX and from a different station.

**Min Interval** specifies the minimum time in minutes that must elapse before an additional DX cluster packet can be triggered for the current DX station even if said station is getting further away.

**Window** specifies the time in minutes before the furthest DX station and distance is forgotten opening the way for another station to become DX at a closer distance. Expiration of a DX station at the end of the Window time does not trigger any transmissions, but the next Status Report will report a different DX status.

**Max Ever** shows and optionally clears the furthest DX station ever recorded by APRSISCE/32.

**Ignore** provides visibility to an optional list of stations that will not be considered for DX, regardless of distance. Stations are added to this list via their station popup menu provided that they have been recently received directly over an RF port. Individual or all exception stations can be cleared by selecting from this menu.

To be considered for DX, a received packet must meet the following conditions:

\*Position-containing Packet\*  
\*Received via RF\*  
\*Not a Third-Party Packet\*  
\*No Hops Used\*  
\*Not an Object\*  
\*Not At lat/lon 0,0\*  
\*Not on the Ignore List\*  
\*Further than Min Dist\*

Once these conditions have been met, the packet is inserted into a distance-ordered list by time. Any older/closer packets are dropped from the list and the new distance is this distance. In the event of a tie on distance, the new station ID replaces the older one.

If a new furthest station is further than Min Trigger, then an immediate notification may occur in the following conditions:

\*New station further than original\*  
\*Longer than Min Interval since last trigger\*

In the event of a distance tie, or a station that is now further than before, a trigger will only happen if Min Interval minutes have elapsed since the last one. This serves as a safety to prevent a notification per packet for a steadily retreating station.

Notifications, when triggered, may be a DX cluster format packet via any RF port configured to transmit said packets or a Status Report if it is configured to include DX information. DX transitions suppressed by the Min Interval, Window expirations, or less than Min Trigger distance will be indicated in the next interval-based Status Report or may be issued in response to a directed ?DX query.

See the various [Kenwood DX Cluster](kenwood-dx-cluster.md) screen displays.
