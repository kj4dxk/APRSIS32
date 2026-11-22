---
title: "Automatic Filters"
---

# Automatic Filters

The client automatically does some filtering for you. It does:

m/dist - My Range filter based on the configured Range (converted from 1/10 mile to km) (Only if Range is non-zero)

If you track or center on someone (right click or popup menu/Center) in the main window, it adds a:

f/call/dist - Friend Range filter using the same range (changes to b/call if Range is zero)

This filter will be removed when you are no longer tracking a callsign.

If you drag the main window map around to establish a center other than yourself or a specific station, it adds a (if Range is non-zero):

r/lat/lon/dist - Range filter using the map center and the configured range

In the main window, if you check Screen / Filter Circle, an r/lat/lon/radius is added where lat/lon are the center of the circle and radius is the radius of the circle. This filter is only added if the circle radius is greater than the configured Range.

MultiTrack windows automatically add a Buddy (b/call) for the Track(station). If you check "Range" on that station's popup menu in the MultiTrack window, the buddy will change to a Friend (f/call/dist) filter using the configured Range. Due to APRS-IS server filter limits, only a limited number of MultiTrack windows can be Range tracked at any point in time.

If you have a remembered MultiTrack that is not known, a window will open that is "Await(CALL)". Because APRSISCE/32 doesn't know if CALL is a station or an object, it will add both a b/CALL and o/CALL\* to your filter string until the station is heard from. (Note: the o/ has a \* because it requires a wild card or exactly 9 character object names).

Due to an oversight in the t/n (NWS alert) filter that doesn't pick up alerts issued between 36 and 59 minutes into the hour (a lower-case sequence character), all enabled NWS Entry Servers are added with an e/ entry filter to ensure receipt of all NWS packets from that server. In addition, all enabled NWS Offices are added to a p/ prefix filter.

If you have an Alternate Network selected (Configure / AltNet), a u/AltNet filter is automatically added for the selected AltNet. When Configure / AltNet / None is selected, this automatic u/ filter is removed.

For any other filter, you can do your own. Here's a few of the Add Filter's I've got in a few instances of my APRSIS32 client:

r/27.99673/-80.659072/1 b/KJ4DXK\* b/KJ4ERJ\* b/KJ4OVQ\* u/APWW\*/APWM\* b/W4SGC\*

r/28.40/-81.50/2000 r/27.99673/-80.659072/1 -b/WINLINK -s/S/S -s//a/DW

b/KJ4OVQ\* b/KJ4ERJ\* b/KJ4DXK\* b/W4SGC\* b/WI8H\* -b/WINLINK -s/S/S -s//a/DW

and my APRSISCE phone has the following filter:

b/N4GVA\* b/KJ4ERJ\* b/KJ4DXK\* b/KJ4OVQ\* b/W4SGC\* s/@/@ u/APWW\*/APWM\* -b/WINLINK

Can you tell what callsigns I like to keep tabs on?

Oh, and obviously, if you add any additional filter, you only specify the actual filter components as shown above. You do NOT need to prefix it with filter or #filter or any other things that other APRS clients might have required.

For a complete list of APRS-IS-supported filters, please refer to [APRS-IS Filters](aprs-is-filters.md)
