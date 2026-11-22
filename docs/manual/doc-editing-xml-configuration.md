---
title: "Editing XML Configuration"
---

# Editing XML Configuration

WARNING! There are parameters in the XML configuration file that can completely hose the proper operation of the APRSISCE/32 client. Please save a copy of your current configuration before changing ANYTHING!

WARNING! A busted XML file will cause APRSISCE/32 to prompt for a completely new configuration. Make sure you save a current copy.

1) Close the client. If you don't do this, any changes you make to the XML file will be lost when you DO close the client as it is completely rewritten by the client on close.

2) Locate and edit APRSISCE.xml (or APRSIS32.xml). On Windows Mobile, it will be in your My Documents folder and probably synced to you desktop. Make sure you wait for the sync to finish after closing the client. On Win32, it will be in the default directory where you run the client.

3) Edit this file with a plain text editor. Find your tag and make sure you edit ONLY the value between the <TagName> and </TagName> elements. Disrupting these matching pairs will result in a failure to load your XML file. When this happens, the client will prompt for a new configuration. You DID make a copy before editing it, right?

4) Save the file and wait for it to re-sync back over to your Windows Mobile device before restarting the client. Otherwise, the client will update the file and your changes will probably be overwritten by ActiveSync.

5) If the client prompts for a configuration like it did the first time you ran it, you hosed something. It's time to go back to your saved copy and start over. You DO have one, right?

Oh, and did I caution you to save a copy of the XML file before you edit it?
