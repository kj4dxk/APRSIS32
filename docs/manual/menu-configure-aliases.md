---
title: "Configure Aliases"
---

# Configure Aliases

Aliases are used within paths of APRS so that users don't need to know the specific digipeaters in order to use the installed RF network. The most popular alias is WIDE in the typical incarnations of WIDE1-1 and WIDE2-1 or WIDE2-2. Older (pre-WIDEn-N) aliases are RELAY and WIDE (yep, same alias as we're still using). There are also local state or province aliases referred to as SSn-N, but there is no central clearing house for the values of SS. Finally, there are other aliases that allow a packet originator to request specific behaviors from infrastructure components that may process a packet. Some examples of these are GATE, NOGATE, and RFONLY.

APRSISCE/32 needs to know what aliases it might encounter in order to properly count used hops when making decisions on "local" stations for message gating decisions from APRS-IS to RF. To that end, APRSISCE/32 allows the configuration of aliases which will not be counted when encountered within the used portion of a packet's path. This list of known aliases comes pre-populated with well-known aliases which are visible in the Known cascading menu. Selecting a specific alias will toggle it between active (not counted) and inactive (counted).

**Note:** Until I determine the performance impact of this, you MUST enable at least one of the Alias() Trace Logs before any of the following will work. Enabling Alias(New) is recommended because it doesn't actually increase any logging, but enables the processing necessary for the following functionality. Also, this will probably only be available in Development mode.

During packet processing, APRSISCE/32 attempts to identify possible new aliases conforming to the SSn-N format (a string of alpha characters followed by a single digit either used (\*) or followed by a dash and another single digit). These "learned" aliases are visible in the Possible cascading menu. Selecting a possible alias will move it to the known list after a confirmation.

Finally, there are some digipeaters that seem to not be marking fully-decremented n-N aliases as used. This results in a path containing just SSn which implies a -0. APRSISCE/32 detects these possible SSn-0 aliases when they are the first unused path component and makes them visible in the Unused-0 cascading menu along with the preceding digipeater and platform of that station. This is done primarily for diagnosing the local network configuration and behavior.

To back up this alias processing, APRSISCE/32 supports the following Trace Logs.

Alias(-0) - Shows information on "Unused-0" alias detection and the associated packet. The first appearance of a given alias and digipeater is always logged. Subsequent references are only logged when the trace is enabled.

Alias(First-0) - Shows information on packets with no used components where the first path component appears to be an alias without an -SSID, implying -0 which may mean that an SSn-N is missing the -N.

Alias(Dbl-0) - Similar to Alias(First-0), this shows packets whose first unused component appears to be an -SSID-less alias and comes after a path alias that is marked used. This happens with things like WIDE2\*,VA2 and may indicate an SSn-N that is missing the -N.

Alias(New) - Shows all newly discovered "Possible" aliases and the first referencing packet.

Alias(Numeric) - When enabled, shows all n-N path components and associated packets. These are most often an incorrectly entered path with extra commas (WIDE,1-1,WIDE,2-1) or missing aliases (WIDE1-1,2-1).

Alias(Used) - When enabled, shows all packets referencing any Possible alias, regardless of New status.
