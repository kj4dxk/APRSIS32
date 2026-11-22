---
title: "ANSRVR"
---

# ANSRVR

ANSRVR is similar to CQSRVR but allows you to quietly remain joined to a group. ANSRVR is a great way to send announcements to people interested in the same thing

To send a message to an announcement group, you can either change the Announcement's CQ Text and poke Send, or send "CQ <Group> Announcement Text" to ANSRVR via a Send Message / Chat.

ANSRVR supports the following APRS message-based commands:

? - Show all groups (# in parens is number of members)  
? group - Show member count in group  
D show groups with descriptions, also sending D <group> description text, will supply a description of the group to the server  
L - List groups you are a member of (with member count including self)  
J group - Join group quietly  
K group1 group2 group3 … - Quietly Keepalive multiple groups (be careful of APRS message length limits)  
CQ group text - Join group and send text to all current members, if the group does not exist it will be created and you will be the owner  
U group - Unjoin group

if you are the owner of a group and there are no other members, remove yourself from the group from within APRSIS32 if you are using the client, but please remember to send a message to ANSRVR U group to remove the group from the server.

You can now safely check Keep Active AND Quietly to remain interested in an Announcement group. You will still receive a confirmation message for each refresh, but the group itself will remain quietly unaware of your lurking er monitoring.

Groups available at the time of writing (10/16/2011)  
APRSIS32(12) Notifications for APRSIS32 Own:KJ4ERJ-12  
APRSISCE(2) Notifications for APRSISCE Own:KJ4ERJ-12  
BALLOON(9) HA Balloon alerts Own:G6UIM  
CDGAPRS(3) Cumbria Dumfries & Galloway APRS Users Own:G4ILO  
CDGVHF(2) VHF propagation alerts for Cumbria, D & G. Own:G4ILO  
CQ(2) CQ Calling Channel Own:KJ4ERJ-12  
ECHOLINK(7) Echolink users. Send CQ, give local node  
FD(2) Field Day Contact Center Own:KJ4ERJ-AP  
ISS(6) ISS APRS Sched Own:G6UIM  
JOTA(1) Jamboree On The Air Own:KJ4ERJ-12  
MARC(2) This is s test of trying to learn Own:KA0AGC  
ODARC(2) Orange Amareur Radio Club Own:VK2ASY-2  
OSR(1) OSR Own:SQ9MLI-1  
ROCKETS(5) for rocket launches Own:G6UIM  
SARA(1) Sawnee Amateur Radio Association Own:N4AC-7  
SCOUTS(2) APRS for Scouting Own:KC7QJO  
UIMCAM(3) Webcam 91.84.215.75:81 Own:G6UIM  
WOTA(6) Wainwrights On The Air group for spots etc. Own:G4ILO

groups with no description  
Also:APRS(1) APRSIS23(1) BRARS(2) CCARC(1) CDGISS(4) DEERCAM(3) MOTORCYCLES(1) NWS(3) SKYWARN(4) WX(1)
