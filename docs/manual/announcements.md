---
title: "Announcements"
---

# Announcements

This messaging option is designed to easily allow use of the ANSRVR server. The idea being that groups are created for sending announcements to others in the group.

**Active Groups**  
This option requests a message from ANSRVR listing the Active Groups and the number of members of each group.

**Describe Groups**  
This option requests a message from ANSRVR listing the Active Groups, number of members, the description of the group, (if it has one) and the owner of the group.

**My Groups**  
This option requests a message from ANSRVR listing the Groups you are currently a member of, includes number of members of each group.

**Join Group**  
This option allows you to join a group or create a group if it does not exist. Selecting this option will popup a config box

* **Keep Alive**  
  This tick box enables the keep alive function, a request will be sent every few hours as selected. Recommended to leave this at the default 8 Hours.  
  ANSRVR will drop you from a group if no keep alive or message sent within 12 Hours.

* **Quietly**  
  This tick box allows you to keep your group connection alive, without sending your CQ Text to every member of the group. If unselected your CQ Text will be sent to every member of the group.

* **use CQSRVR**  
  Select this tick box to use CQSRVR instead of ANSRVR. The only difference between the two is that CQSRVR does not allow quiet keep alives.

* **Only if present**  
  Currently not in use

* **IS Only**  
  Only send messages and keep alives via the IS stream if selected, otherwise send via IS and RF.

* **CQ Text**  
  The text you wish to send to the group and use as a keep alive.

* **Accept**  
  Update settings and CQ Text quietly. Not sent to every member.

* **Send**  
  Update settings and CQ Text, send the CQ Text to every member of the group. Can be used in conjunction with CQ Text to send short messages to the group.

* **Delete**  
  Remove yourself from the group. **Note** If you are the owner of a group and wish to remove the group, make sure you are the only member of the group.  
  and you will need to send U group via a normal message to ANSRVR.

Long messages can be sent via ANSRVR but these need to be done via the normal messaging system.

* **Group membership list**  
  Clicking on the list will bring up the same box as the join group box, you can however temporarily suspend your activity within a group

To send a message to an announcement group, you can either change the  
Announcement's CQ Text and poke Send, or send "CQ <Group> Announcement  
Text" to ANSRVR via a Send Message / Chat.

ANSRVR supports the following APRS message-based commands:

? - Show all groups (# in parens is number of members)  
? group - Show member count in group  
L - List groups you are a member of (with member count including self)  
J group - Join group quietly  
CQ group text - Join group and send text to all current members, also used to send a message to a group you already belong to.  
U group - Unjoin group

ANSRVR is like CQSRVR except for the following:

Currently no minimum time between submissions to the group. This is in keeping with the "Announcement" intended use.  
Ability to monitor a group without sending periodic messages to everyone.  
Directly supported in APRSISCE/32! (although CQSRVR is just as supported)
