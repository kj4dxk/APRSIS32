---
title: "EN_CCW_Tracker"
---

# EN_CCW_Tracker

## Cross Country Wireless Digi Tracker

The Cross Country Wireless (CCW) Digi Tracker is currently only fully supported in the development version.

To use the Development version please follow the instructions [here](doc-development-version.md)

Note editing the XML file for both the development version and to set the required <Open/CloseCmd>s requires the software to not be running.

Once you have downloaded the development version and restarted. configure a new port  
Configure/Ports/New Port and in the type drop down menu select CONVerse port Type give it a name then select the COM port option. The parameters for the com port should be set to 4800 Baud, 8 Data bits, No Parity (or whatever you have the TNC configured for).

When you have created the port, close APRSISCE/32 and then edit the XML file to enter open/close commands for the port [info](doc-openclosecmd.md), enter the following open entries:

<OpenCmd>echo off|OK|3</OpenCmd>  
<OpenCmd>digi on|OK|3</OpenCmd>  
<OpenCmd>myalias wide1-1|OK|3</OpenCmd>  
<OpenCmd>beacon every 0|OK|3</OpenCmd>

If you want the TNC to go back to it's default configuration when disabling the port or closing APRSIS32, enter the following close entries:

<CloseCmd>echo on!OK!3</CloseCmd>  
<CloseCmd>digi on!OK!3</CloseCmd>  
<CloseCmd>restart!!0</CloseCmd>

Restart the software for these changes to take effect.

**Please make sure that APRSISCE/32 and the Tracker have different [SSIDs](doc-ssids.md)**
