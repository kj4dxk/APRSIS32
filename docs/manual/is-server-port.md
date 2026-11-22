---
title: "IS-Server Port"
---

# IS-Server Port

APRSIS32 has the ability to feed through the IS stream to other applications or APRS instances.  
To do this from the configure menu select ports and the New Port. Select IS-Server from the drop down menu and give the port a name  
In the following option box select TCP/IP enter either an IP address of 127.0.0.1 if only using on your PC or the IP address of your machine, if you wish to access from another machine.  
Enter a unique port number such as 8150.  
At this point things get a little confusing as the port configure box is the standard box, and the labels are slightly wrong.  
Make sure everything you want to pass through is ticked such as messages telemetry etc.  
RF-IS really means to pass data from IS-Server port to the main IS stream  
IS-RF means send data from the main IS stream through to the IS-Server port

Any filters you have set in your second instance will not change the filters in the main instance so you will need to make sure your main instance feeds the required data through.

In your second instance setup exactly as you would, as if you were connecting directly to the IS stream including your passcode, remember to give your instance a different SSID.  
For your IS port select the IP address and port of your main instance.
