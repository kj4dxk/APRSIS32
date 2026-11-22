---
title: "Messages Menu"
---

# Messages Menu

The Messages menu provides access to the APRS message-based features of APRSISCE/32. If you're not familiar with APRS messaging, you may also want to read [aprs-messaging-explained](doc-aprs-messaging-explained.md).  
<span id="readmesage"></span>
  
**Read Message** - Brings up the highest priority unread message for a preview or directly into an existing chat window.  
<span id="sendmessage"></span>
  
**Send Message** - Allows entry or selection of a station for which a new [Chat](chat.md) window will be opened.  
<span id="sendemail"></span>
  
**Send EMail** Allows entry or selection of a destination e-mail address to which a short (?? character) message may be sent. See also [Email](http://www.aprs-is.net/Email.aspx)  
<span id="pendingmessages"></span>
  
**Pending Messages** - If enabled, will initiate a popup menu of sent messages for which an ack has not been received. This will color the [Message](panes-message.md) yellow or orange. See also [Message Retries](message-retries.md).  
<span id="anouncements"></span>
  
**Anouncements**  
This messaging option is designed to easily allow use of the ANSRVR server. The idea being that groups are created for sending announcements to others in the group.  
<span id="activegroups"></span>

* **Active Groups**  
  This option requests a message from ANSRVR listing the Active Groups and the number of members of each group.

<span id="describegroups"></span>

* **Describe Groups**  
  This option requests a message from ANSRVR listing the Active Groups, number of members, the description of the group, (if it has one) and the owner of the group.

<span id="mygroups"></span>

* **My Groups**  
  This option requests a message from ANSRVR listing the Groups you are currently a member of, includes number of members of each group.

<span id="joingroup"></span>

* **Join Group**  
  This option allows you to join a group or create a group if it does not exist. Selecting this option will popup a config box

<span id="keepactive"></span>

* **Keep Active**  
  This tick box enables the keep alive function, a request will be sent every few hours as selected. Recommended to leave this at the default 8 Hours.  
  ANSRVR will drop you from a group if no keep alive or message sent within 12 Hours.

<span id="quietly"></span>

* **Quietly**  
  This tick box allows you to keep your group connection alive, without sending your CQ Text to every member of the group. If unselected your CQ Text will be sent to every member of the group.

* **use CQSRVR**  
  Select this tick box to use CQSRVR instead of ANSRVR. The only difference between the two is that CQSRVR does not allow quiet keep alives.

* **Only if present**  
  Currently not in use

<span id="isonly"></span>

* **IS Only**  
  Only send messages and keep alives via the IS stream if selected, otherwise send via IS and RF.

<span id="cqrext"></span>

* **CQ Text**  
  The text you wish to send to the group and use as a keep alive.

<span id="accept"></span>

* **Accept**  
  Update settings and CQ Text quietly. Not sent to every member.

<span id="send"></span>

* **Send**  
  Update settings and CQ Text, send the CQ Text to every member of the group. Can be used in conjunction with CQ Text to send short messages to the group.

<span id="delete"></span>

* **Delete**  
  Remove yourself from the group. **Note** If you are the owner of a group and wish to remove the group, make sure you are the only member of the group.  
  and you will need to send U group via a normal message to ANSRVR.

Long messages can be sent via ANSRVR but these need to be done via the normal messaging system.  
<span id="groupmembership"></span>

* **Group membership list**  
  Clicking on the list will bring up the same box as the join group box, you can however temporarily suspend your activity within a group

To send a message to an announcement group, you can either change the  
Announcement's CQ Text and poke Send, or send "CQ <Group> Announcement  
Text" to ANSRVR via a Send Message / Chat.

ANSRVR supports the following APRS message-based commands:

? - Show all groups (# in parens is number of members)  
? group - Show member count in group  
D show groups with descriptions, also sending D <group> description text, will supply a description of the group to the server  
L - List groups you are a member of (with member count including self)  
J group - Join group quietly  
CQ group text - Join group and send text to all current members, also used to send a message to a group you already belong to.  
U group - Unjoin group

ANSRVR is like CQSRVR except for the following:

Currently no minimum time between submissions to the group. This is in keeping with the "Announcement" intended use.  
Ability to monitor a group without sending periodic messages to everyone.  
Directly supported in APRSISCE/32! (although CQSRVR is just as supported)  
<span id="autoreply"></span>

* **Auto-Reply**  
  Messages / Auto-Reply has Message (what to send) and "Inactive For" and "No More Than" sub-menus. "InActive For" is a delay since the last interaction with APRSISCE/32 before a configured Auto-Reply will be sent. "No More Than" sets the minimum interval between Auto-Replies to a single station. [More](auto-reply.md)

<span id="bulletins"></span>
  
**Bulletins**  
Creation and viewing of Bulletins  
<span id="view"></span>

* **View**  
  This option opens a new window to view the currently received Bulletins. Bulletins will age and disappear from this window over a period of 48 Hours, if they have not been retransmitted. The ageing can be changed, currently only be editing the XML file

<span id="notify"></span>

* **Notify**  
  Selecting this option will cause a pop notification balloon to appear when a new bulletin is received.

<span id="create"></span>

* **Create**  
  Selecting this will create a popup window to allow creation of a bulletin.  
  The length of a bulletin line is limited to ? Characters, multi line bulletins can be created by assigning a line number to each comment. This is done with the unlabeled drop down menu, next to the enabled tickbox.

Interval selects how often the bulletin is sent out, 0 disables Via-IS/RF tickboxes select to send via IS RF or both

Clicking send message from the message menu brings up a box to enable entering of the callsign of the station you wish to message. You then proceed to the message window. (clicking a station and selecting send message bypasses the callsign selection box)  
<span id="chats"></span>

* **Chats**  
  The message window allows entering of up to 300 characters, which are split automatically into 64 character APRS messages. There several options on this window.

* **Clear**  
  Clears the message window
* **Copy**  
  Copies the text in the window to the buffer for pasting into another application
* **Multitrack**  
  Opens a new tracking window centered on the station being messaged
* **Ack**  
  unselect this if you do not wish to have an acknowledgement of your message. (useful for satellite work)
* **Best**  
  This option changes it's title. and can be slightly confusing at first  
  \* **Best**  
  use RF or IS as the program determines the best option.  
  \* **RF Only**  
  Unticked this will send the message via IS only, Ticked the message will go RF only (useful for Satellite work)
