---
title: "EN_ContextHelp"
---

# EN_ContextHelp

[Source wiki page](https://aprsisce.wikidot.com/en-contexthelp) · [Archived HTML](../archive/pages/en-contexthelp--7d6a13974073.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

[Transmit](#Transmit)  
[Enables](#Enables)  
[Enables\_APRS-IS](#Enables_APRS-IS)  
[Enables\_GPX](#Enables_GPX)  
[Enables\_Beaconing](#Enables_Beaconing)  
[Enables\_CSV](#Enables_CSV)  
[Enables\_FreqMon](#Enables_FreqMon)  
[Enables\_GPS](#Enables_GPS)  
[Enables\_Internet](#Enables_Internet)  
[Enables\_OSM](#Enables_OSM)  
[Enables\_Sound](#Enables_Sound)  
[Enables\_Telem](#Enables_Telem)  
[Enables\_Ports](#Enables_Ports)  
[Enables\_Logging](#Enables_Logging)  
[Enables\_ViewLog](#Enables_ViewLog)  
[Messages](#Messages)  
[Messages\_RMsg](#Messages_RMsg)  
[Messages\_SMsg](#Messages_SMsg)  
[Messages\_SEmail](#Messages_SEmail)  
[Messages\_Pending](#Messages_Pending)  
[Messages\_Announce](#Messages_Announce)  
[Messages\_AutoR](#Messages_AutoR)  
[Messages\_Bulletins](#Messages_Bulletins)  
[Messages\_Chats](#Messages_Chats)  
[Screen](#Screen)  
[Screen\_DeadReck](#Screen_DeadReck)  
[Screen\_FilterCirc](#Screen_FilterCirc)  
[Screen\_Tracks](#Screen_Tracks)  
[Screen\_AutoZ](#Screen_AutoZ)  
[Screen\_DF](#Screen_DF)  
[Screen\_Follow](#Screen_Follow)  
[Screen\_Labels](#Screen_Labels)  
[Screen\_Paths](#Screen_Paths)  
[Screen\_PHG](#Screen_PHG)  
[Screen\_Preferred](#Screen_Preferred)  
[Screen\_TileSet](#Screen_TileSet)  
[View](#View)  
[View\_All](#View_All)  
[View None](#View_None)  
[View Chooser](#View_Chooser)  
[View Alt](#View_Alt)  
[View Ambiguity](#View_Amb)  
[View](#View_Bud)  
[View Duplicates](#View_Dup)  
[View FreqSpec](#View_FreqSpec)  
[View Freq W/issues](#View_FreqIss)  
[View Invalids](#View_invalid)  
[View Message-able](#View_Message-able)  
[View Nicknamed](#View_NickNamed)  
[View NWS Objects](#View_NWSObj)  
[View Objects](#View_Objs)  
[View Paths](#View_Paths)  
[View Telemetry](#View_Telem)  
[View Tracks](#View_Tracks)  
[View Mic-E](#View_Mic-E)  
[View Platforms](#View_Platforms)  
[View RF](#View_RF)  
[View Shrieks](#View_Shrieks)  
[View Symbols](#View_Symbols)  
[View Transport](#View_Transport)  
[View Echolinks](#View_Echolinks)  
[View GeoCaches](#View_GeoCaches)  
[Configure](#Configure)  
[Configure General](#Configure_General)  
[Configure Genius](#Configure_Genius)  
[Configure Status](#Configure_Status)  
[Configure Aliases](#Configure_Aliases)  
[Configure AltNet](#Configure_AltNet)  
[Configure Beacon](#Configure_Beacon)  
[Configure Companions](#Configure_Companions)  
[Configure DX](#Configure_DX)  
[Configure Map](#Configure_Map)  
[Configure Messages](#Configure_Messages)  
[Configure Metric](#Configure_Metric)  
[Configure Mic-E Notification](#Configure_MiceNot)  
[Configure NWS](#Configure_NWS)  
[Configure NWS Shapes](#Configure_NWSShapes)  
[Configure Objects](#Configure_Objects)  
[Configure Overlays](#Configure_Overlays)  
[Configure Ports](#Configure_Ports)  
[Configure Save Posits](#Configure_SavePosit)  
[Configure Screen](#Configure_Screen)  
[Configure Scroller](#Configure_Scroller)  
[Configure Status Report](#Configure_StatusRep)  
[Clear](#Clear)  
[Clear Eavesdrops](#Clear_Eavesdrops)  
[Clear CQ/All](#Clear_CQ)  
[Clear Group Messages](#Clear_Group)  
[Clear My Messages](#Clear_My)  
[Clear NWS](#Clear_NWS)  
[Clear Paths](#Clear_Paths)  
[Clear Stations](#Clear_Stations)  
[Clear Tracks](#Clear_Tracks)  
[Clear Accumulated](#Clear_Accumulated)  
[Clear Configured](#Clear_Configured)  
[About](#About)  
[Exit](#Exit)

<span id="Transmit"></span>
  
**Transmit**  
Force a beacon to be Transmitted, Also used to set location with no GPS attached

<span id="Enables"></span>
  
**Enables**  
The Enables menu allows you to Enable (and Disable) various settings within APRSISCE/32. Currently the TraceLogs are also accessed via this menu.

<span id="Enables_APRS-IS"></span>
  
**Enables APRS-IS Enabled**  
This option enables a live connection to the configured APRS-IS server. Access to the server also requires that Internet Access also be enabled.

<span id="Enables_GPX"></span>
  
**Enables AutoSave GPX**  
f checked, a .GPX file is automatically saved containing your recorded track if your station moves. The file will be named <callsign-SSID>-YYYYMMDD-HHMM.gpx and stored in the same directory as the XML configuration file. The "Save Track" on a station popup menu will record the same format file for any station.

<span id="Enables_Beaconing"></span>
  
**Enable Beaconing Enabled**  
If checked, allows APRSISCE/32 to generate position updates (beacons) for transmission via APRS. Individual settings on the APRS-IS and RF Ports control where such beacon packets are actually transmitted.

<span id="Enables_CSV"></span>
  
**Enables CSV Enabled**  
f checked, causes APRSISCE/32 to write a YYYYMMDD-HHMM.csv (Comma-Separated Values) file containing each unique location where a beacon was triggered. This value is written whether or not an actual beacon was actually transmitted.

<span id="Enables_FreqMon"></span>
  
**Enables Frequency Monitor**  
which creates a new window for easier spotting of stations transmitting frequency information, especially for mobile operators. The size of the frequency is governed by Configure / Screen / Speed Font Size.

<span id="Enables_GPS"></span>
  
**Enables GPS Enabled**  
Enables or disables the GPS. If this option is disabled, then a GPS has not yet been configured via Configure / Ports / NMEA.

<span id="Enables_Internet"></span>
  
**Enables Internet Access**  
If this option is not checked, all Internet access by APRSISCE/32 is suspended. This will disable APRS-IS, Map Tile fetching, and Version update checking.

<span id="Enables_OSM"></span>
  
**Enables OSM Fetch Enabled**  
Enables the fetching of new map tiles if a required one is not available in the local cache. Disabling Internet Access also disables this function. The tile purger is also disabled if this function is not checked. (See also Configure / Map / Purger)

<span id="Enables_Sound"></span>
  
**Enables Sound Enabled**  
Enables or disables sounds generated by APRSISCE/32.

<span id="Enables_Telem"></span>
  
**Enables Telemetry Enabled**  
If checked, allows APRSISCE/32 to generate telemetry packets for transmission via APRS. Individual settings on the APRS-IS and RF Ports control where such telemetry packets are actually transmitted. Even if telemetry is disabled, it will be processed locally to display if requested.

<span id="Enables_Ports"></span>
  
**Enables Ports**  
Enables the data ports, KISS and IP ports, APRS-IS and NEMA ports are enabled elsewhere .  
**All**  
Enables all ports listed in the menu.  
**Log-All**  
Enable logging of all the listed ports  
**Receive-Only**  
Disables transmission on the listed ports

<span id="Enables_Logging"></span>
  
**Enables Logging**  
Enable debug logging of various data

<span id="Enables_ViewLog"></span>
  
**Enables View Logs**  
View the various debug logs if they are enabled

<span id="Messages"></span>
  
**Messages**  
The Messages menu provides access to the APRS message-based features of APRSISCE/32. If you're not familiar with APRS messaging,

<span id="Messages_RMsg"></span>
  
**Read Messages**  
Brings up the highest priority unread message for a preview or directly into an existing chat window.

<span id="Messages_SMsg"></span>
  
**Send Messages**  
Allows entry or selection of a station for which a new Chat window will be opened.

<span id="Messages_SEmail"></span>
  
**Send Email**  
Allows entry or selection of a destination e-mail address to which a short (?? character) message may be sent. See also Email

<span id="Messages_Pending"></span>
  
**Messages Pending Messages**  
If enabled, will initiate a popup menu of sent messages for which an ack has not been received. This will color the Message yellow or orange. See also Message Retries.

<span id="Messages_Announce"></span>
  
**Messages Announcements**  
This messaging option is designed to easily allow use of the ANSRVR server. The idea being that groups are created for sending announcements to others in the group.  
**Active Groups**  
This option requests a message from ANSRVR listing the Active Groups and the number of members of each group.  
**Describe Groups**  
This option requests a message from ANSRVR listing the Active Groups, number of members, the description of the group, (if it has one) and the owner of the group.  
**My Groups**  
This option requests a message from ANSRVR listing the Groups you are currently a member of, includes number of members of each group.  
**Join Group**  
This option allows you to join a group or create a group if it does not exist. Selecting this option will popup a config box  
**Keep Alive**  
This tick box enables the keep alive function, a request will be sent every few hours as selected. Recommended to leave this at the default 8 Hours. ANSRVR will drop you from a group if no keep alive or message sent within 12 Hours.  
**Quietly**  
This tick box allows you to keep your group connection alive, without sending your CQ Text to every member of the group. If unselected your CQ Text will be sent to every member of the group.  
**use CQSRVR**  
Select this tick box to use CQSRVR instead of ANSRVR. The only difference between the two is that CQSRVR does not allow quiet keep alives.  
**IS Only**  
Only send messages and keep alives via the IS stream if selected, otherwise send via IS and RF.  
**CQ Text**  
The text you wish to send to the group and use as a keep alive.  
**Accept**  
Update settings and CQ Text quietly. Not sent to every member.  
**Send**  
Update settings and CQ Text, send the CQ Text to every member of the group. Can be used in conjunction with CQ Text to send short messages to the group.  
**Delete**  
Remove yourself from the group. Note If you are the owner of a group and wish to remove the group, make sure you are the only member of the group. and you will need to send U group via a normal message to ANSRVR.  
Long messages can be sent via ANSRVR but these need to be done via the normal messaging system.  
**Group membership list**  
Clicking on the list will bring up the same box as the join group box, you can however temporarily suspend your activity within a group

<span id="Messages_AutoR"></span>
  
**Messages Auto Reply**  
Messages / Auto-Reply has Message (what to send) and "Inactive For" and "No More Than" sub-menus. "InActive For" is a delay since the last interaction with APRSISCE/32 before a configured Auto-Reply will be sent. "No More Than" sets the minimum interval between Auto-Replies to a single station.

<span id="Messages_Bulletins"></span>
  
**Messages Bulletins**  
Creation and viewing of Bulletins  
**View**  
This option opens a new window to view the currently received Bulletins. Bulletins will age and disappear from this window over a period of 48 Hours, if they have not been retransmitted. The ageing can be changed, currently only be editing the XML file  
**Notify**  
Selecting this option will cause a pop notification balloon to appear when a new bulletin is received.  
**Create**  
Selecting this will create a popup window to allow creation of a bulletin.  
The length of a bulletin line is limited to ? Characters, multi line bulletins can be created by assigning a line number to each comment. This is done with the unlabeled drop down menu, next to the enabled tickbox.  
Interval selects how often the bulletin is sent out, 0 disables Via-IS/RF tickboxes select to send via IS RF or both  
Clicking send message from the message menu brings up a box to enable entering of the callsign of the station you wish to message. You then proceed to the message window. (clicking a station and selecting send message bypasses the callsign selection box)

<span id="Messages_Chats"></span>
  
**Messages Chats**  
The message window allows entering of up to 300 characters, which are split automatically into 64 character APRS messages. There several options on this window.  
**Clear**  
Clears the message window  
**Copy**  
Copies the text in the window to the buffer for pasting into another application  
**Multitrack**  
Opens a new tracking window centered on the station being messaged  
**Ack**  
unselect this if you do not wish to have an acknowledgement of your message. (useful for satellite work)  
**Best**  
This option changes it's title. and can be slightly confusing at first  
**Best**  
use RF or IS as the program determines the best option.  
**RF Only**  
Unticked this will send the message via IS only, Ticked the message will go RF only (useful for Satellite work)

<span id="Screen"></span>
  
**Screen**  
The Screen menu provides access to the more-frequently changed screen and display features of APRSISCE/32. See also Configure / Screen for additional features.

<span id="Screen_DeadReck"></span>
  
**Screen Dead Reckoning**  
Toggles the display of the extrapolated position of a moving station based on their last known speed and heading. Up to three extrapolated positions may be displayed based on the selections in the Screen / Labels / Speed menu. The extrapolated station symbol is connected to the last received position by a line that changes from green to red depending on the time since the last beacon was received. Normal stations will turn fully red and cease being displayed 5 minutes after their last position update. NWS stations received with a speed and heading will continue to dead reckon until their expiration time is reached.

<span id="Screen_FilterCirc"></span>
  
**Screen Filter Circle**

<span id="Screen_Tracks"></span>
  
**Screen Tracks**  
Toggles the display of historically captured track lines for stations that have moved. Actual track transmission points will be highlighted for the currently Centered station.

<span id="Screen_AutoZ"></span>
  
**Screen AutoZoom View**

<span id="Screen_DF"></span>
  
**Screen Direction Finding**  
Enables the display of direction finding circles

<span id="Screen_Follow"></span>
  
**Screen Follow**  
Allows following a moving Station  
**Find**  
Allows typing a station name into the box and moving the map to their location. The Drop down menu shows all the received stations, if you select the drop down and start typing a station name the drop down, will move through the list narrowing down the selection. Selecting find will move to the station on the map. Multitrack will open a new window centred on the station. If the station has not been received the option to wait for a transmission from the station will appear.  
**Centre**  
This option will centre the map on the last selected station. Or your own station if no station has been previously selected.  
**ME**  
This option will move the map to centre your own station on screen  
**Locked**  
Will lock the screen to follow the currently selected station

<span id="Screen_Labels"></span>
  
**Screen Labels**  
Selects labels to show on screen  
**Altitude**  
Option to display the altitude of a station if the station broadcasts it. the display unit can be configured under Configure Metric  
**Ambiguity**  
Shows a circle around stations with position ambiguity, the circle shows the area that the station should be within. Ambiguity can be set Configure Beacon Menu  
**Callsign**  
Select the display of callsigns for stations  
**Nicknames**  
Nicknames for stations that provide an alternate label (possibly blank) as well as an optional symbol and comment override.  
**NWS Calls**  
**Speed**  
Selects to display the speed of a station. Beaconed displays the speed actually beaconed by the station, if the station is moving but not beaconing a speed calculate will attempt to display the speed  
**Weather**

<span id="Screen_Paths"></span>
  
**Screen Paths**

<span id="Screen_PHG"></span>
  
**Screen PHG Range Circles**  
Enables the display of full (fixed station) or half (mobile station) scale range circles as well as setting the desired opacity of said circles. APRSISCE/32 displays a cardoid shape to indicate any directionality specified by the PHG values. PHG circles are colored green (full) or red (half).

<span id="Screen_Preferred"></span>
  
**Screen Preferred**  
Allows a the current view to be Saved or the previously saved view to be restored. The preferred settings saves the map center and zoom level. Direct access to the saved perferred view is also available by double-clicking the Scale number.

<span id="Screen_TileSet"></span>
  
**Screen Tile Set**  
Provides direct access to select what map will be used as a background. See also Configure / Map / Tile Sets for more information.

<span id="View"></span>
  
**View**  
The View Menu allows various filters on what stations will actually be displayed on the map. View settings are not retained across restarts but will revert back to ALL to avoid wondering why you don't see any stations when you return to APRSISCE/32 after having been away for a few days.

<span id="View_All"></span>
  
**View ALL**  
Retains any individually selected options below to allow quick restoration to a filtered view, but displays all stations.

<span id="View_None"></span>
  
**View NONE**  
Unchecks all selections on the view menu. Centered stations and ME will always be displayed.

<span id="View_Chooser"></span>
  
**View Chooser**

<span id="View_Alt"></span>
  
**View Altitudes**

<span id="View_Amb"></span>
  
**View Ambiguous**  
Displays all stations that are transmitting beacons including position ambiguity. Non-filled circles are drawn around such stations to indicate that they may more may not be precisely at their displayed position.

<span id="View_Bud"></span>
  
**View Buddies**  
Displays all stations that APRSISCE/32 considered to be a "Buddy". This includes any station matching any b/xxx or f/xxx filter (including wildcards) and also any station currently centered in any APRSISCE/32 MultiTrack window.

<span id="View_Dup"></span>
  
**View Duplicates**  
Displays all stations for which suspected duplicate position packets have been received. Duplicates can occur when a portion of the APRS Infrastructure delays packet delivery for more than the default 30 second duplicate detection filter of the APRS-IS transport system. Duplicate packets can make a mobile station appear to ping-pong between new and old positions. APRSISCE/32 highlights such points in purple and does not include them in the track lines.

<span id="View_FreqSpec"></span>
  
**View FreqSpec**

<span id="View_FreqIss"></span>
  
**View Freq w/Issues**

<span id="View_invalid"></span>
  
**View Invalids**

<span id="View_Message-able"></span>
  
**View Message-able**  
Displays all stations whose transmissions, either by APRS packet type or application identifier, indicate that they support APRS messaging. If you have ever sent messages to or received messages from a station, it will also be included in the display. These stations will offer "Message" on their popup menu to provide a direct way to open a new chat with that station.

<span id="View_NickNamed"></span>
  
**View Nicknamed**  
Displays all stations for which you have defined a Nickname. See also Screen / Labels / Nicknames

<span id="View_NWSObj"></span>
  
**View NWS Objects**  
Displays all NWS weather objects. These objects are either directly received via APRS or are internally generated from NWS alerts in conjunction with locally defined shapefiles.

<span id="View_Objs"></span>
  
**View Objects**  
Displays all objects received from APRS. Objects are typically points of interest like repeaters, hospitals, and other non-APRS stations.

<span id="View_Paths"></span>
  
**View Paths**  
<span id="View_Telem"></span>
  
**View Telemetry**  
Displays stations for which Telemetry has been received.

<span id="View_Tracks"></span>
  
**View Tracks**  
Displays stations that have moved and therefore have accumulated tracks to display.

<span id="View_Mic-E"></span>
  
**View Mic-E**  
<span id="View_Platforms"></span>
  
**View Platforms**  
Allows selection of specific APRS platforms to display. Note that counts of stations determined to be of a specific platform are displayed on this menu. Some of the platforms provide additional breakdowns, but selecting any one of the detailed categories sometimes selects the entire category. More work needs to be done here (2011/04/20) to make this behave more consistently.

<span id="View_RF"></span>
  
**View RF**

All  
Direct  
Local  
Third Party

<span id="View_Shrieks"></span>
  
**View Shrieks**  
APRSISCE/32 parses any !<nonwhitespace>! from comments and groups (up to 32 of) them into a View / Shrieks submenu. This allows event coordinators to pick a suitable !EVENT! for stations to put in their beacon comment to provide easy on-screen filtering of only the stations participating in the event. The Spotter Network requires their users to put !SN! in their comment, so you can expect to see !SN! in the View / Shrieks menu if any such stations are in your range.

!Shriek!s are also parsed from nickname comments EVEN if the nickname or comment is not enabled! This allows you to nickname your favorite stations with a !WATCHME! comment and then View / Shrieks / !WATCHME! to see ONLY those stations! You could even nickname stations as !VIEW1!, !VIEW2!, !VIEW3!, and so forth to build your own local MultiTrack views of specific stations. A single station can even have multiple !VIEWn! tags in the comment to put them on multiple views.

If nicknames are enabled, and a station has a nickname, and that nickname overrides the comment, then ONLY the comment's Shrieks are used. The comment received from the station will be ignored completely for Shriek purposes. After these conditions are no longer met, a new packet from the station must be received to get a proper !Shriek! for the station.

<span id="View_Symbols"></span>
  
**View Symbols**  
Allows selection of specific symbols to display. The currently centered station's symbol is sorted to the top of the list to allow easy selection of other stations like the centered station. Note that counts of stations using a symbol are displayed on this menu.

<span id="View_Transport"></span>
  
**View Transport**

RF Only  
RF+IS  
IS Only

APRSISCE/32 attempts to determine the transports available to a given station based on the contents of received packets. This information is used to optimize some communications with stations on various platforms.

also Digipeater, IGate (these last will display stations that have been observed acting in the specified capacity regardless of what symbol they may be transmitting).

<span id="View_Echolinks"></span>
  
**View Echolinks**

<span id="View_GeoCaches"></span>
  
**View Geocaches**

<span id="Configure"></span>
  
**Configure**  
The Configure menu provides access to all of the not-so-frequently changed and/or initial setup options of APRSISCE/32.

<span id="Configure_General"></span>
  
**Configure General**  
The General configuration dialog is the first dialog that is presented on a clean installation of APRSISCE/32 and collects the basic configuration information necessary to operate an APRS-IS client.

<span id="Configure_Genius"></span>
  
**Configure GeniusBeaconing™**

APRSISCE/32 extends the Smartbeaconing concept to a whole new level made possible by running the client on platforms more powerful than a typical PIC.

You should not need to make any changes to the GeniusBeaconing™ settings until you have run APRSISCE/32 for a while. If you don't have a GPS, then you definitely do not need to change anything here except maybe the Max Time which will be the only thing governing your beacon interval.

A good description of the GeniusBeaconing can be found in the description of the red-dot behavior.

Min Time - Under no circumstances will APRSISCE/32 automatically beacon in less elapsed time than the Min Time seconds.

Max Time - If nothing else triggers a beacon, APRSISCE/32 will transmit one every Max Time minutes. If there is no GPS input, or if the GPS is disabled, then beacons (if enabled) will be transmitted at this rate.

Time Only - If this is checked, then only the time parameters are used and the remainder of the settings on this dialog are ignored.

Start/Stop - If checked, APRSISCE/32 will transmit a position beacon whenever you speed drops to zero or leaves zero, governed by the Min Time of course. If you frequently drive in start/stop traffic, you may want to uncheck this to avoid frequent (Min Time) beaconing.

Heading Change - If your heading changes by more than this many degrees since the last beacon, a new beacon will be triggered. If you have problems with frequent beaconing due to a drifting GPS, simply set this value to 180. The Forecast Error will still provide very accurate, predictable beacons. (This parameter may be removed in some future version because Forecast Error works so well).

Forecast Error - This is the "Genius" of GeniusBeaconing(tm). If an outside observer were to calculate your position based on the speed, heading, and elapsed time since your last beacon and your actual current position is more than "Forecast Error" from that position, a beacon will be transmitted. The magnitude and direction of the error between a forecast from your last beacon and your actual current position is shown by the position of the red-dot relative to the range circle. When the red-dot touches the circle, the Forecast Error has been exceeded and a beacon will be triggered (if allowed by Min Time).

Max Distance - If you have "made good" more than this distance since your last beacon, a new beacon will be triggered. Note that this is expressed in 1/10 mile increments, so remember to multiply by 10.

Accept / Cancel - Obvious operation here (I hope).

<span id="Configure_Status"></span>
  
**Configure Status**  
The Status Configuration dialog allows setting both the Beacon Comment (redundant with Configure General and the Configure Status Report).  
**Beacon Comment**  
Comment - This is the same comment string configurable in Configure General and also selectable via Configure Beacon.

Interval - The default value of 0 causes the comment to be included with every position beacon (if enabled in Configure Beacon. APRSISCE/32 also supports only including the comment on position beacons at a configurable interval. A position beacon won't be triggered just to carry the comment, but the comment will be sent at most once in each configured interval. Note that some APRS radios think the station has significantly changed if the comment is or is not present, so you might be asked why your station keeps triggering the display on those radios as if the station was newly received.

Status Report - The status report is a completely different specification in APRS than the beacon comment. See chapter 16 in aprs101.pdf for details.

Enabled - If checked, status report packets will be periodically generated by APRSISCE/32.

GridSquare - If checked, the current GridSquare will be included in the Status Report. Use this with caution if you are also transmitting position beacons as the position resolution between the GridSquare and Lat/Lon coordinates is significantly different, possibly resulting in a station that ping-pongs between two locations depending on which packet was heard most recently.

Timestamp - If checked, the current timestamp (ddhhmm zulu, unfortunately not hhmmss) will be included in the Status Report. This can be handy in demonstrating packet delays in the APRS RF network possibly in excess of the 30 second APRS-IS duplicate filter causing two instances of a single Status Report to appear on APRS-IS. Without a timestamp, it's hard to prove that the packet was duplicated.

Status - This is the actual text included in the Status Report. Also selectable via Configure Status Report.

Interval - Status Report packets will be generated every Interval minutes.

PHG - Inserts a Power/Height/Gain string into the Beacon Comment. See Configure General.

Accept / Cancel - IOMCO (Intuitively Obvious to the Most Casual Observer)

<span id="Configure_Aliases"></span>
  
**Configure Aliases**

Aliases are used within paths of APRS so that users don't need to know the specific digipeaters in order to use the installed RF network. The most popular alias is WIDE in the typical incarnations of WIDE1-1 and WIDE2-1 or WIDE2-2. Older (pre-WIDEn-N) aliases are RELAY and WIDE (yep, same alias as we're still using). There are also local state or province aliases referred to as SSn-N, but there is no central clearing house for the values of SS. Finally, there are other aliases that allow a packet originator to request specific behaviors from infrastructure components that may process a packet. Some examples of these are GATE, NOGATE, and RFONLY.

APRSISCE/32 needs to know what aliases it might encounter in order to properly count used hops when making decisions on "local" stations for message gating decisions from APRS-IS to RF. To that end, APRSISCE/32 allows the configuration of aliases which will not be counted when encountered within the used portion of a packet's path. This list of known aliases comes pre-populated with well-known aliases which are visible in the Known cascading menu. Selecting a specific alias will toggle it between active (not counted) and inactive (counted).

Note: Until I determine the performance impact of this, you MUST enable at least one of the Alias() Trace Logs before any of the following will work. Enabling Alias(New) is recommended because it doesn't actually increase any logging, but enables the processing necessary for the following functionality. Also, this will probably only be available in Development mode.

During packet processing, APRSISCE/32 attempts to identify possible new aliases conforming to the SSn-N format (a string of alpha characters followed by a single digit either used (\*) or followed by a dash and another single digit). These "learned" aliases are visible in the Possible cascading menu. Selecting a possible alias will move it to the known list after a confirmation.

Finally, there are some digipeaters that seem to not be marking fully-decremented n-N aliases as used. This results in a path containing just SSn which implies a -0. APRSISCE/32 detects these possible SSn-0 aliases when they are the first unused path component and makes them visible in the Unused-0 cascading menu along with the preceding digipeater and platform of that station. This is done primarily for diagnosing the local network configuration and behavior.

To back up this alias processing, APRSISCE/32 supports the following Trace Logs.

Alias(-0) - Shows information on "Unused-0" alias detection and the associated packet. The first appearance of a given alias and digipeater is always logged. Subsequent references are only logged when the trace is enabled.

Alias(First-0) - Shows information on packets with no used components where the first path component appears to be an alias without an -SSID, implying -0 which may mean that an SSn-N is missing the -N.

Alias(Dbl-0) - Similar to Alias(First-0), this shows packets whose first unused component appears to be an -SSID-less alias and comes after a path alias that is marked used. This happens with things like WIDE2\*,VA2 and may indicate an SSn-N that is missing the -N.

Alias(New) - Shows all newly discovered "Possible" aliases and the first referencing packet.

Alias(Numeric) - When enabled, shows all n-N path components and associated packets. These are most often an incorrectly entered path with extra commas (WIDE,1-1,WIDE,2-1) or missing aliases (WIDE1-1,2-1).

Alias(Used) - When enabled, shows all packets referencing any Possible alias, regardless of New status.

<span id="Configure_AltNet"></span>
  
**Configure AltNet**  
ALTNETS are uses of the AX-25 tocall to distinguish specialized traffic  
that may be flowing on the APRS-IS, but that are not intended to be  
part of normal APRS distribution to all normal APRS software operating  
in normal (default) modes. Proper APRS software that honors this  
design are supposed to IGNORE all ALTNETS unless the particular  
operator has selected an ALTNET to monitor for.

An example is when testing; an author may want to transmit objects  
all over his map for on-air testing, but does not want these to clutter  
everyone's maps or databases. He could use the ALTNET of "TEST" and  
client APRS software that respects the ALTNET concept should ignore  
these packets.

An ALTNET is defined to be ANY AX.25 TOCALL that is NOT one of the  
normal APRS TOCALL's. The normal TOCALL's that APRS is supposed to  
process are: ALL, BEACON, CQ, QST, GPSxxx and of course APxxxx.

<span id="Configure_Beacon"></span>
  
**Configure Beacon**  
The Configure / Beacon menu provides direct access to the various allowed components and contents of a position beacon.

After Transmit - In a semi-mobile (not fixed) GPS-less installation, it may be desirable to not transmit a position beacon on initial startup until the last known position can be confirmed and possibly corrected. If this option is checked, APRSISCE/32 will not transmit a position beacon until the Transmit menu option has been manually activated. Given that the Transmit option allows the user to move "ME" to the center of the screen, this allows the actual current position to be verified and/or changed before any position beacons are sent.

Timestamp - If checked, a timestamp is included in each position packet. The default timestamp is ddhhmm zulu unless HHMMSS is also checked.

HHMMSS - Modifies the timestamp to use the hhmmss format. Including this timestamp on RF beacons can allow easy detection and identification of delayed duplicate packets being injected into APRS-IS beyond the default 30 second duplicate detection interval. When delayed packets are introduced, a mobile station can appear to ping-pong along a path as obsolete positions arrive after newer positions.

Altitude - If checked, the current altitude is included in the position packet. Note that altitude is only supported with an active GPS and then only if the GPS is providing an altitude in the fix. Fixed (non-GPS) stations cannot specify an altitude for transmission.

Compressed - If checked, position packets are transmitted in a compressed format per chapter 9 of aprs101.pdf. This Compressed format provides more resolution than the humanly readable format. This compression is NOT Mic-E which is a format parsed and displayed by APRSISCE/32, but not supported for transmitting positions.

CourseSpeed - If checked and a GPS is providing speed and heading data, a CSE/SPD (Chapter 7 of aprs101.pdf) is included in the position packet. If compressed is checked, then either Altitude or Course/Speed can be selected, but not both as the APRS protocol doesn't support it.

Comment - The Comment cascading menu provides direct access to a set of "remembered" comment texts. You can also specify a New comment text (optionally remembered) or None to eliminate the comment from being transmitted with the position beacons.

Symbol - The Symbol cascading menu provides direct access to a set of "remembered" symbols. You can also select a New symbol using the same dialog as used for Configure General.

Why - If checked, causes APRSISCE/32 to include the Transmit Pressure that triggered the position beacon to be included in the position packet. This can be quite handy when determining the cause of too-frequent beaconing or to see which trigger is actually causing various beacons along a route. Because this text is included in the station's comment, some APRS radios will consider each beacon to have significantly changed causing a new display on each reception.

Precision - The default precision for a non-compressed position beacon is 1/100 minute of latitude and logitude. APRSISCE/32 supports increased precision via the !DAO! extension Datum as well as ambiguous position as described in Chapter 6 (page 24) of aprs101.pdf). Both 1 digit (1/1000 minute - humanly readable) and 2 digit (91/10000 minute - don't ask) additional precision is supported as well as all levels of ambiguity. Compressed packets are, by definition, accurate to within 1 foot.

Path… - Specifies the path to use when transmitting position beacons. (see New Paradigm or just set it to WIDE1-1,WIDE2-1 as a "safe" value) This is likely to be relocated in some future APRSISCE/32 revision to allow more discrete setting of different paths on different ports.

<span id="Configure_Companions"></span>
  
**Configure Companions**

<span id="Configure_DX"></span>
  
**Configure DX**  
DX in APRSISCE/32 is defined as receiving a position packet with no hops used from a station located further away than some minimum distance. The current furthest DX may be included in the periodic Status Report packet and also returned in response to a ?DX directed query. Additionally, newly set DX stations beyond a configurable distance may be immediately reported via a DX cluster formatted packet and may also cause an immediate Status Report to be issued.

The following menu options govern the DX behaviors of APRSISCE/32.

Min Dist specifies the minimum distance required for a packet to be considered "DX". The parameter is specified in miles or kilometers depending on the Configure / Metric / Distance setting.

Min Trigger specifies the minimum distance that may trigger an immediate transmission of either a DX cluster or Status Report packet provided that no such packet has been triggered within the past Min Interval minutes, or the new DX is further than the previous DX and from a different station.

Min Interval specifies the minimum time in minutes that must elapse before an additional DX cluster packet can be triggered for the current DX station even if said station is getting further away.

Window specifies the time in minutes before the furthest DX station and distance is forgotten opening the way for another station to become DX at a closer distance. Expiration of a DX station at the end of the Window time does not trigger any transmissions, but the next Status Report will report a different DX status.

Max Ever shows and optionally clears the furthest DX station ever recorded by APRSISCE/32.

Ignore provides visibility to an optional list of stations that will not be considered for DX, regardless of distance. Stations are added to this list via their station popup menu provided that they have been recently received directly over an RF port. Individual or all exception stations can be cleared by selecting from this menu.

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

<span id="Configure_Map"></span>
  
**Configure Map**

<span id="Configure_Messages"></span>
  
**Configure Messages**  
This menu drop down is used to configure what messages are received.

My Messages  
Selecting this option will enable reception and display of any message addressed to your callsign regardless of the SSID the message is sent to.

RF Eavesdrop  
This option enables the display of any message received over RF regardless of who the message is addressed to

Eavesdrop  
Enables the reception and display of any message that the software sees, depends very much on your APRS server filter settings. NOT recommended if you are receiving a full feed!

Hide Queries  
This option hides any queries someone makes to your station such as ?ARPSH etc. Otherwise queries will show in the message box.

Notify Queries  
When a query arrives at your station a popup balloon will appear to inform you of the query, otherwise the query will just show under messages. In any case, the query will be automatically answered.

Notify New  
When a new message arrives a popup balloon will appear to inform you, handy if you have the software minimized, but somewhat bothersome when Windows queues notifications on an unattended machine.

Multitrack New Items  
When an object arrives as an item in a message, each object will show in a new multitrack window, if not selected the objects will only  
show on the main map.

Lookup (WHOIS)  
This option allow selection of the amount of data received when a WHOIS search is done, if checked WHOIS will hopefully respond with the address related to the callsign looked up.

<span id="Configure_Metric"></span>
  
**Configure Metric**  
This configure option selects which measurements you would like to display as a metric rather then imperial value. Defaults are imperial.  
Altitude  
Selects between displaying altitude in feet (unchecked) or metres (checked)  
Distance  
Selects between displaying distance in miles (unchecked) or kilometres (checked)  
Pressure  
Selects between displaying atmospheric pressure in inches (unchecked) or milliBars (checked)  
Rainfall  
Selects between displaying inches in feet (unchecked) or millimetres (checked)  
Temperature  
Selects between displaying temperature in Fahrenheit (unchecked) or Celsius (checked)  
Wind (Knots)  
Selects between displaying wind speed in mph (unchecked) or knots (checked). (not strictly a metric option)

<span id="Configure_MiceNot"></span>
  
**Configure Mic-E Notifications**

<span id="Configure_NWS"></span>
  
**Configure NWS**

<span id="Configure_NWSShapes"></span>
  
**Configure NWS Shapes**  
Ok, so now the cat is out of the proverbial bag, so what can I actually do with ShapeFiles and what should I see?

First, you need to follow the instructions at WA8LMF's excellent page at WA8LMF Shapefiles. He has links to the pages with the .ZIP files for the US NWS ShapeFiles, but you may also want the Australian ones from Australian Shapefiles Download the .ZIP fies and unzip them all into a single new directory where ever you want to put it. Note: it is \*MUCH\* easier if you just dump them all into a \*single\* directory! (Links to the currently valid NWS shape files are also maintained at <http://www.aprs-is.net/wx>)

Once that is done, you need to tell APRSISCE/32 which file(s) you want it to use. Be careful with the US NWS ones as they've got pending revisions on two of them that aren't actually valid yet. Pick the ones whose date has past, not the ones whose date is in the future.

Use Configure / Shapefiles / Add File and pick each of the .SHP files that came out of the .ZIPs. The corresponding, same-named .SHX and .DBF files needs to be in the same directory for this all to work. As each file is added, the Open File dialog re-appears to add additional file(s) as needed. When all desired files have been added, hit Cancel.

Once you have the shapefiles enabled, you'll probably want to enable receiving alerts from your local NWS office (or CWA - County Warning Area). You can get your county's code at NWS County Codes just pick your state from the drop-down and scan for your county. Or you can center the map at the point of interest and hit Configure / NWS Shapes / ShowMe. APRSISCE/32 will pop up a MultiTrack window for each zone that the point is within. Some of these zones will start with a 3 character code that is the NWS office (CWA) responsible for that area. Once you have your CWA, use Configure / NWS / New Office… to enable it in APRSISCE/32. You can add as many offices as you need to cover your area of interest.

After that, you wait….and wait…and wait…for bad weather to happen. Or just Configure / NWS / New Office… and add \* to get weather for the whole world (ok, at least as much of the world as is pushing weather alerts into APRS-IS). Within 15-30 minutes you should have stuff popping up in Australia. They've consistently had alerts running for me.

If you want to see useless "callsigns", check Screen / Labels / NWS Calls. These labels are a concatenation of the 3 character Seq and the 6 character county/zone of any given alert. Quite useless if you ask me, which is why they're suppressed by default.

Ok, with that all done, you should be able to pan and zoom around and see just how closely the shapefile boundaries match the underlying OSM map tiles. Freaky if you ask me!

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

PS. WE7U is now feeding ALL NWS alerts into FireNet servers for display via APRS clients. To access this stream, you need to reconfigure your APRS-IS feed to come from firenet.us instead of an APRS-IS server. All APRS-IS data is available through FireNet and MORE!

Configure / Ports / APRS-IS  
Click Device  
Change the DNS or IP to "firenet.us" (without the quotes)  
Keep your port the same (14580, most likely)  
Ok and Accept and APRSISCE/32 will switch to the new server.

If you don't like all the water stations that you'll start getting, add "-s/w" to your Configure / General / Add Filter string (again, without the quotes).

Note: if you already have a -s/ element in your filter, just add the w to the first part.

<span id="Configure_Objects"></span>
  
**Configure Objects**  
Create  
Create a new object at the centre of the screen. more information here createobject

New Weather  
Prompts for a file name and location and otherwise works like a new object. If the weather file cannot be accessed during a poll, the object automatically disables itself with a message in the Trace Log which is also brought to the front. File must be in WxNow.txt-format data file input.

Show  
Show all of your configured objects on screen. (They are not broadcast)

Max Group objects  
The maximum number of objects in a group that will be sent, in response to a direct query from a station. Groups with more objects will respond with an error message.

QRU

Enabled  
Enable the QRU server function of the software. Details on QRU Here qru

Interval  
The interval for normal transmission of objects. Best set to 0 (Disable) if you have a large number of objects.

Max Objects  
The maximum number of objects to send in response to a QRU query.

Range  
The maximum range between an object on the server and a querying station. The server will ignore queries if it has no object within range of the station.

Retry Messages  
Enables the retry system system for objects sent in item as a message format

QRU Setup  
Detail on QRU setup is available here qru

Radio Tuning Format (QSY)  
Some APRS-aware radios, notably the Kenwood D710 and D72 (?) and the Yaesu FTM-350, are capable of tuning the voice "side" of the radio to a frequency received in an APRS packet from another station. In order for this to work, the frequency, tone, and offset must be formatted in a very specific fashion. The following describes the format that is known to be the most compatible with the various QSY-capable radios.

(Special thanks to Mike, AA9VI, for this information)

a brief description of how to sort the correct format to enable auto tune of objects, for permanent objects and QRU ones, thanks to Mike

442.725MHz T114 +500 comment

or

147.345MHz T107 +060 comment

works universally on older and newer Kenwood and Yaesu mobile and portable radios radios for the position comment. Note that the MHz is CaSe SeNsItIvE (the z is the only lower case)!

The corresponding digipeater object syntax would be (in KPC format):  
;442.725+ \*111111z4158.60N/08739.29Wr442.725MHz T114 +500 comment  
;147.345+R\*111111z4208.15N/08748.38Wr147.345MHz T107 +060 comment

The comment for a repeater frequency is a 3.3 decimal frequency followed by a 3 digit tone (or CTCSS or DCS) with no decimal and the the offset in 10KHz units i.e. 145.650MHz T077 -060

where the R in the 2m listing serves to make the repeater object unique… not everyone can list 147.345+ since it is a common repeater pair. Some can list 147.345+R, 147.345+MI, etc. The total object name is 9 characters, so you have 2 or 3 characters after the frequency. Note that the object name must also be space-filled to 9 characters. Many people think the + or - in a frequency object name sets the offset direction, but that is only a visual clue for a human operator. It's the +060 or -060 or whatever that sets the direction AND offset.

The other digi format mentioned on WB4APR's site:

;FFF.FFxyz\*111111zDDMM.hhN/DDDMM.hhWrTnnn RXXm NETxxxxxx MTGxxxxx

will work with newer firmware versions. So, safe to use the one I suggested above to make it work for nearly everyone regardless of whether they updated their firmware or not.

You may be tempted to list your repeater offset as decimal MHz (+6.0M), but that's not the standard. Offsets are specified in 10KHz increments so a 600KHz offset would be +060 or -060. For 440, you might use +500 to get a 5MHz offset.

Bob WB4APR's spec for sending frequency information is here

<span id="Configure_Overlays"></span>
  
**Configure Overlays**  
his option allows you to load a GPX file. Simply, click on Add GPX file, find the file of interest it will load.

GPX files can be a bit varied. This can cause interesting results since not every feature of the GPX may be supported. Try it out anyway, you will probably like it!

The scope here is not about GPX, but if you want to learn more about it, check out: GPX: the GPS Exchange Format.

There are many tools that can create a GPX file. It may be a GPS application that can read the tracks, routes and waypoints inside a GPS. Since this is the exchange format, it is almost always a supported input/output. Another tool could be a web application.

No matter how the GPX is created, it can be a very useful tool. There are many waypoint files of potentially interesting items. They could be waterfalls, covered bridges, WX transmitters or maybe even the Bat Cave. The file might contain tracks of places others have been. Content describing a track or route might be the directions to get from here to there. Or, maybe it defines a route/track of an event (e.g., bicycle race to raise awareness/money for Multiple Sclerosis).

Monitoring resources in the course of an event often utilizes APRS. And having the course, waypoints, etc, defined is quite useful.

Mapping software comes in many varieties and at varying cost. If your route stays on a drivable surface found on Google Maps, then there is a great online tool. That tool is GMAPtoGPX. The best thing to do is just to try it.

Please visit the GMAPtoGPX information page to get running with this tool.

Follow the Instructions. Do the first step! This is what makes the tool operational. Steps 2-5 are included below with a little more information added.

Once the first step is done, the next step is to acquire "Driving Directions" from Google Maps. Choose a starting point and a finish point. The "directions" Google returns may not be what you need or want. Hover over the route and you will see "Drag to change route". Modify the route as desired.

Note: there are only so many points along a route that can be added (~50?). If you have a route that needs more them 50 points to complete, break the route into sections.

Once you are satisfied with your route, click on the bookmark for GMAPtoGPX. This will open a window with text inside it. The data inside is basically what is needed for a GPS to follow. Read again, the third bullet under Notes on the GMAPtoGPX information page.

For cases other than stuffing the data back into a GPS, it is usually useful to click on "Full" just above the text. This will expand the data to add many more points.

The next step is to create a GPX file. A quick select all data (left-click in the data field and press CTRL-A). Copy the data (CTRL-C) and paste the data into NotePad (CTRL-V). Save the file with the file extension GPX (e.g, Here2There.gpx).

I like the fifth step on the information page. Try it and see what happens! So, load the file into APRSISCE/32. This is accomplished by:  
Toolbar-> Configure -> Overlays -> Add GPX file…

Not only can you see the route, it can be "driven". This may cause APRSISCE/32 to fetch tiles that are local. Repeat at the required zoom levels. Always remember the tile usage requirements!

In addition, new waypoints can be made into a transmittable object. Scroll to bottom of this page for more details.

Option: Add Overlay…

This option allows you to load and enable/disable APRSdata/UI-View-style .POS overlay files.

Overlay files allow you to show information about objects on your map which are not transmitted to other stations, but only displayed locally. But if need be, new positions can be made into a transmittable object. Scroll to bottom of this page for more details.

Enabled files are remembered and re-loaded across client restarts as well. Comments on the position lines are used, but the status lines are not yet supported.

For more information and sample .POS files, read VE3KBR's excellent guide.

An example .POS file is (note: the first line is a description, not a position):

* MB7Ux Unattended 24/7 Digis - Created by M0CYP

MB7UA!5147.85N\00101.84En  
MB7UA>144.800 Point Clear, Essex  
MB7UB!5125.70N\00224.43Wn  
MB7UB>144.800 G4KVI  
MB7UC!5155.31N\00200.63Wn  
MB7UC>144.800 G4GVZ www.g0lgs.myby.co.uk/grg  
MB7UH!5154.53N\00053.93En  
MB7UH>144.800 G6NHU www.mb7uiv.co.uk

And another one:

* APRS (MIC-E) Over Voice Repeater

MB7IP!5246.08N\00209.56Wn  
MB7IP>144.800 < GB3IP 145.7625 CTCSS 103.5

History:

Overlays were first known to exist in APRSdata where it says:

"APRSdata uses all of the POS\\*.POS data files that you already have. To see these in any version of APRSdos, just hit the MAPS-OVERLAYS commands. These POS\\*.POS files are distributed with each of the major APRSdos Map distributions."

And here's a link to the original documentation with this priceless client-centric statement:

"Do not worry about exact lat/longs. Just use your eyeball on the most detailed APRS map that you have. The exact location will never matter, since anyone who is viewing the overlay data will be using the SAME map to VIEW it as you did to estimate the position in the first place. "

Plagiarizing the UI-View Manual (UI-VIEW32V203MANUAL.pdf):

The format of an overlay file is very simple. The first line is a description, subsequent lines are location lines or status text lines. Location lines consist of up to nine characters for a callsign/identifier, followed by a '!', followed by a position beacon. Status text lines consist of a callsign/identifier, a '>' , and some status text.

NOTE - UI-View32 overlay files are not fully compatible with APRS overlay files, because APRS doesn't support the status text lines. However, if you create overlay files for UI-View32 and include status text, anyone wanting to use them with APRS can easily remove the status text lines.

And from Chip, VA3KGB:

How to create your own Overlay File

Create the overlay file using an ASCII text editor such as "Notepad". Notepad is great for creating short overlay files but for large files such as creating overlay files for the IRLP Nodes, I have a spreadsheet that reads in the raw data , lets me check for errors in the data manually and then when I'm happy with the raw data, the spreadsheet then creates the files for WinAPRS, UI-View, and xastir for me.

Overlay files are one line per object except for UI-View and APRSISCE/32. With UI-View and APRSISCE/32, you can have a two lines to convey more information.

Included with the overlay files, I also add a text readme file listing the overlay files, explaining where the files need to be located, and any other information that the APRS operator should be aware of before using the file. X-astir users must turn OFF Object/Item transmit before using an overlay otherwise all the objects in the overly get transmitted out over the APRS network!

Format for a one line overlay file for WinAPRS, UI-View, APRSISCE/32

* 144 MHZ IRLP Nodes Callsign overlay for APRS
* irlp.net node data: 08/12/2008
* This file created using irlpnodelist-aprs.xls by CJ Chapman, VA3KGB

VE7RHS!4916.19N/12315.00WnNode 1000,145.270,T100.0  
VE7ISC!4913.64N/12358.45WnNode 1003,146.640(-600)  
VE7VIC!4825.80N/12335.99WnNode 1030,146.840(-600),T100.00  
VE7BYN!5049.52N/11859.48WnNode 1041,147.570  
VE7KJA!5035.03N/12705.61WnNode 1064,146.445

The Asterisk (\*) de3notes that this is a comment and is ignored by the APRS program. I usually add comment lines to identify what the overlay file is used for, the date the file was created, and the spreadsheet or source data that was used to generate the file.

Each line is a separate Object.

The format using one line as an example is:  
VE7RHS!4916.19N/12315.00WnNode 1000,145.270,T100.0

Callsign No APRS Message Lat Primary or Alt Icon Table Lon Icon comment  
VE7RHS ! 4916.19N / 12315.00W n Node 1000,145.270,T100.0

The callsign (or identifying label) should be 9 characters or less or it will get truncated.

The (!) tells the system not to send the object out as an APRS message. or is it saying primary or alternate table for the Icon

The Lat is in degrees and decimal minutes format (60°10.92' N 24°31.86' E) stripping out the degree and minute characters and is to 2 decimal places.

In between the Lat and Long is the symbol which identifies which symbol table to use for the icon to be displayed.

The Long is in degrees and decimal minutes format (60°10.92' N 24°31.86' E) stripping out the degree and minute characters and is to 2 decimal places.

The Icon is from APRS Icon tables and uses the alpha/numeric character in conjunction with the Primary/Alt table symbol to identify the graphical icon to display.

The comment field should be kept short, and in this example indicates the IRLP NODE ID, the Frequency and Tone required. Commas (,) are used to separate items within the comment. In the comment field using a spreadsheet to build the overlay files there will be comma with a space between them indicating that the source file had no information for that portion of the comment. If building the file by hand you can delete the space(s) and extra comma(s).

Format for a two line overlay file for UI-View and APRSISCE/32

* 144 MHZ IRLP Nodes Callsigns with Cities overlay for UI-View or APRSISCE/32
* irlp.net node data: 08/12/2008
* This file created using irlpnodelist-aprs.xls by CJ Chapman, VA3KGB

VE7RHS!4916.19N/12315.00WnNode 1000,145.270,T100.0  
VE7RHS> IRLP Node: 1000, Vancouver, BC, Canada  
VE7ISC!4913.64N/12358.45WnNode 1003,146.640(-600)  
VE7ISC> IRLP Node: 1003, Nanaimo, BC, Canada  
VE7VIC!4825.80N/12335.99WnNode 1030,146.840(-600),T100.00  
VE7VIC> IRLP Node: 1030, Victoria, BC, Canada

The format for the first line is as per the one line format above.  
The format for the second line as an example is:  
VE7RHS!4916.19N/12315.00WnNode 1000,145.270,T100.0  
VE7RHS> IRLP Node: 1000, Vancouver, BC, Canada

Callsign Comment-identifier Comment  
VE7RHS > IRLP Node: 1000, Vancouver, BC, Canada

Callsign will be exactly the same as the first line

Comment field again is short and in this example indicated that it is IRLP Node 1000 located in Vancouver BC Canada. (Commas (,) separate the items in the comment. In the comment field using a spreadsheet to build the overlay files there will be comma with a space between them indicating that the source file had no information for that portion of the comment. If building the file by hand you can delete the space(s) and extra comma(s).

Converting GPX Waypoint and POS data to Objects

In some cases, the new data is useful as an object. Once a waypoint or position is on the map, it is simple to convert to an object. Find the point on your display and left-click on it. In the dialog window, the last option is MAKE OBJECT. Select it and the Create Object dialog box opens which will have data from the GPX or POS already loaded for the select point. Perform any editing necessary and ACCEPT. Keep in mind that unique names make your object unique.

<span id="Configure_Ports"></span>
  
**Configure Ports**

In order to get APRSISCE/32 to talk to a GPS, a radio or indeed anything that provides or sends information, you need to configure a port. So how do you configure a port and what in the world does TCP/IP have to do with KISS and NMEA?

This nine minute video shows you the basics:

Here's a bit more detail if you're interested.

APRSISCE/32 supports two different ways of connecting to serial devices. The "normal" expected one would be via a "normal" Windows COM port with baud rates, parity, stop bits and such. However, I also support connected to a TCP/IP source of this same data. This allows the client to connect to remotely served TCP/IP serial ports which may or may not be virtualized by Windows. I also happen to have some TCP/IP servers that remotely serve out COM ports from one Windows machine to another. So…

When you click the KISS or NMEA buttons on the new configuration dialog, you will be asked what sort of connection it will be, TCP/IP, BlueTooth, or COMn Serial. Most of you will probably want COMn Serial for a normal RS-232 port. Clicking that will reveal a Port Configuration dialog with drop-downs of the known COM ports and support baud rates along with radio buttons for Parity and Data/Stop bits. (Ignore the Xmit checkbox). When you have things set up for your GPS or KISS TNC, click OK.

After configuring a COM port, the KISS or NMEA button will take you directly back to the Port Configuration dialog. If you clear the COM port and click OK, the next time you configure that port, the TCP/IP question will be asked.

The AGW interface only support TCP/IP connections to AGWPE. The TCP Configuration dialog prompts for an IP address or DNS name of the host where AGEPE is executing. Port 8000 is the default for AGW and should not be changed unless you know what you're doing.

After configuring the appropriate port(s), you can then enable whichever feature you want to use. If any communications errors are encountered, you may need to disable and re-enable the feature to get things moving again. I need to do more work on failure recovery.

If you are using a Bluetooth serial adapter for either KISS or NMEA GPS connections (I use a Delorme BlueLogger as my GPS), you can either go through the pairing process and assign a COM port outside of the APRSISCE/32 client or you can click Bluetooth back at the beginning. If you pair to an Outgoing COM port, follow the instructions above for configuring a COMn Serial port. If you opt for the direct Bluetooth, APRSISCE/32 will poll for available devices and present a drop-down of the device names. Select one of those for use and proceed into the main port configuration. Whenever a direct Bluetooth port is opened, the devices are again enumerated and a direct connection to the device is established. If a PIN is required, your Bluetooth stack will prompt for it.

Drop a note to the list if you are using any of these new features. I'm anxious to see how they work out and won't know if they're working or not if you don't tell me!

Raw capture of an e-mail describing Quiet Time from 2011/03/23:

The Quiet Time is the maximum length of time during which a packet should have been heard on that port. If no packet is heard within the specified time, the port is closed and re-opened using all of the configure <Close/OpenCmd>s. This is provided as a way to detect a possibly dropped network or bluetooth connection, or maybe even a TNC that has lost its mode and needs a restart to recover. Because there is no heartbeat/keepalive capability on a TNC connection, an RF Port Quiet Time must be set long enough to cover the maximum expected elapsed time between packet receptions. If set too short, the port will unnecessarily close and re-open itself periodically, possibly resulting in dropped packets across the restart. A Quiet Time of zero disables this function.

So, don't set this on a KISS-type port on a Kenwood radio. The default <Open/CloseCmd>s will leave the radio's TNC turned off until you manually turn it back on.

Don't set this on any port that doesn't have a steady stream of expected traffic (like an HF APRS radio port). It just doesn't work out very well.

If you have a steady APRS traffic hum on your radio port, set the Quiet Time to 2-3 TIMES the quiet time you'd expect to see on the channel. This gives APRSISCE/32 a chance to reset the channel if it goes quiet too long. If you have sounds enabled, you'll heard bings and bongs while the radio port transitions through the various states of a restart.

The Port configuration callsign box is disabled and will probably be removed in an upcoming release. It was a misguided original thought to have the ability to specify different callsign-SSIDs for different ports within a single APRSISCE/32 instance. I've since decided, after discussions on this list, that that approach would be overly complex when the same or better functionality can be achieved simply by running a second APRSIS32 instance.

Port Configuration  
CWOP  
APRS-IS Port  
NEMA Port  
KISS Port  
IS-Server Port

<span id="Configure_SavePosit"></span>
  
**Configure Save Posits**  
Finally added Configure / Save Posits / Filter… and Reload Paths. The  
former shows how many stations will be saved with the current filter (0  
if you have an invalid filter or no filter) and allows entry of an  
APRS-IS-style filter. The latter does a double-pass over the saved  
posits during reload in an attempt to re-establish the paths between  
them. A double-pass is necessary as a path is not recorded unless the  
station is already known, so the first loading pass generates an  
incomplete path picture. If Reload Paths is not checked, then NO paths  
will be reloaded (speeding the reload somewhat), but allowing new paths  
to be learned quickly as the digi/igate stations will (hopefully) have  
been reloaded (if you restart soon enough that your saved stations don't  
age out 30 seconds after loading).

<span id="Configure_Screen"></span>
  
**Configure Screen**

<span id="Configure_Scroller"></span>
  
**Configure Scroller**

The scroller is the list of packets that shows to the left of the map. Sometimes it's short (Wide Orientation) and sometimes it fills the left side (Narrow Orientation), but in either case, it displays packet information as it is received, governed by the options below.

Freeze On Click - If checked, left clicking on the scroller will freeze it, preventing any new packets from being added. Subsequently clicking on any station ID will present the station popup menu for that station as well as provide a preview of the packet at that entry across the bottom of the map. Clicking outside the scroller (or right clicking on the scroller) will un-freeze it and immediately add any new packets that were received while frozen. Right-clicking on a non-frozen scroller will bring up the station popup menu immediately and allow the scroller to continue moving in the background.

Show IGate/Digi - Adds the IGate (APRS-IS) or Digi (RF) that is determined to have last handled the packet. Requires configuration of <PathAlias> elements in the XML configuration to eliminate locally-defined aliases. Under Windows Mobile, the station ID is replaced by the IGate/Digi ID for space considerations.

The following settings govern which packets are added to the scrolling packet log.

Filter - Allows entry of an APRS-IS-style filter to control which packets are added to the scroller.

Hide NoParse - Hides packets that failed the parser (see also the NoParse Trace Log)

No Internals - Hides APRSISCE/32 Internally generated packets (see also the Packets(Internal) Trace Log)

Not Me - Hides packets from the configured callsign, even digipeats of your own packets (I think)

Not Mine - Hides packets from the same base callsign, regardless of -SSID.

RF Only - Shows ONLY packets received from RF Ports (handy for IGates, boring on an APRS-IS-only instance)

Show All - Forces ALL packets to be added to the scroller. Handy to set your preferences and toggle to ALL sometimes.

Note that none of these settings are retro-active. They are only checked when new packets arrive to determine if they will be added to the scroller. Once a packet is or is not added, it will be there or not until it scrolls off the top of the list.

<span id="Configure_StatusRep"></span>
  
**Status Report**

The Status Configuration dialog allows setting both the Beacon Comment (redundant with Configure General and the Configure Status Report).

Beacon Comment:

Comment - This is the same comment string configurable in Configure General and also selectable via Configure Beacon.

Interval - The default value of 0 causes the comment to be included with every position beacon (if enabled in Configure Beacon. APRSISCE/32 also supports only including the comment on position beacons at a configurable interval. A position beacon won't be triggered just to carry the comment, but the comment will be sent at most once in each configured interval. Note that some APRS radios think the station has significantly changed if the comment is or is not present, so you might be asked why your station keeps triggering the display on those radios as if the station was newly received.

Status Report - The status report is a completely different specification in APRS than the beacon comment. See chapter 16 in aprs101.pdf for details.

Enabled - If checked, status report packets will be periodically generated by APRSISCE/32.

GridSquare - If checked, the current GridSquare will be included in the Status Report. Use this with caution if you are also transmitting position beacons as the position resolution between the GridSquare and Lat/Lon coordinates is significantly different, possibly resulting in a station that ping-pongs between two locations depending on which packet was heard most recently.

Timestamp - If checked, the current timestamp (ddhhmm zulu, unfortunately not hhmmss) will be included in the Status Report. This can be handy in demonstrating packet delays in the APRS RF network possibly in excess of the 30 second APRS-IS duplicate filter causing two instances of a single Status Report to appear on APRS-IS. Without a timestamp, it's hard to prove that the packet was duplicated.

Status - This is the actual text included in the Status Report. Also selectable via Configure Status Report.

Interval - Status Report packets will be generated every Interval minutes.

PHG - Inserts a Power/Height/Gain string into the Beacon Comment. See Configure General.

Accept / Cancel - IOMCO (Intuitively Obvious to the Most Casual Observer)

<span id="Clear"></span>
  
**Clear**

<span id="Clear_Eavesdrops"></span>
  
**Clear Eavesdrops**  
If you have eavesdrop enabled in the messages configure menu. This option will remove all messages received that are NOT addressed to your callsign.

<span id="Clear_CQ"></span>
  
**Clear CQ/All Messages**  
Removes all messages received addressed to ALL, CQ and QST

<span id="Clear_Group"></span>
  
**Clear Group Messages**

<span id="Clear_My"></span>
  
**Clear My Messages**  
Removes all messages addressed to your callsign, including those addressed to different SSID's if My messages is configu

<span id="Clear_NWS"></span>
  
**Clear NWS Messages**  
If you have setup to use the National Weather Service, this option will clear all received messages.

<span id="Clear_Paths"></span>
  
**Clear Paths**

<span id="Clear_Stations"></span>
  
**Clear Stations**  
This option will clear the map of all displayed stations.

<span id="Clear_Tracks"></span>
  
**Clear Tracks**  
Clears the map display of all tracks

<span id="Clear_Accumulated"></span>
  
**Clear Accumulated**

Clear AltNets  
Clear the list of received Alternate Networks

Clear Bulletins  
Clear the list of received Bulletins

Reset MaxWidth Station  
Resets the width of the scrolling station list, to a minimum size, until a station with a longer name is received

Clear Telemetry  
Clears the list of received telemetry

<span id="Clear_Configured"></span>
  
**Clear Configured**

Clear Auto Replies  
Clears all of the configured Auto Reply responses. A new auto reply comment will need to be added afterwards. If auto replies are in use.

Beacon Comments  
Clears all of the configured beacon comments. The current Beacon comment will not be removed

Clear Status Reports  
Clears all of the configured status reports. A new Status report will need to be entered if in use.

Clear Symbols  
Clears the list of symbols except the symbol currently in use for the station.

<span id="About"></span>
  
**About**  
Displays the Version of the software that you are using, and copyright information.

The important part is that the about window also checks to make sure you have the latest version of the software. And gives the option to download a newer version if one is available

<span id="Exit"></span>
  
**Exit**  
If Exit Confirmations are enabled, a Restart option is also provided.
