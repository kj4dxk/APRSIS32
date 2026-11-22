---
title: "tnc-d700"
---

# tnc-d700

From Arte Booten 2/12/2011 11:46am:

There are a few things you should do first. Using HyperTerminal (or a  
terminal program of your choice, connect the radio to your computer, turn on  
the radio, cycle the TNC until it's in PKT mode - even if its already there -  
and type DISPLAY ,then press ENTER. This will show you the status of all the  
parameters that have been set within the TNC.

Many packet clients, including some APRS clients, set some of these, and  
you may not be aware that they did! Pay particular attention to BEACON (set  
to B E 0 [that's a zero]), BTEXT (reset to blank with BT %) HID (causes QRM &  
should be off - HID %) and UNPROTO (set to blank U %).

Now, start configuring your new radio port with CONFIGURE > PORTS > NEW  
PORT. In the New RFPort window, leave KISS alone and name that port what you  
want. I use D700, but you might want to call it something else.

You're now asked if you want to configure it as a TCPIP port. Click No.  
This opens the Port Configuration window. You \*MUST\* set the Quiet Time to  
something other than zero, or the TNC will soon exit KISS mode. I use 300  
sec. (5 minutes) in a moderately busy RF APRS environment, but you might get  
by with a shorter interval in a busy network, or a long one in an RF desert.

RF baud is 1200 for normal APRS ops on 144.390 (in North America, or in  
most parts of the EU 144.800). If you want to port RF data onto the IS feed  
or the opposite, check those boxes. Check Enabled and Xmit Enable boxes.

Next, press the Genius button to set parameters for Smart Beaconing, so  
your position will be transmitted at the right interval if you turn or have  
moved more than a given distance. The Min Time and Max Time depend on how  
active your local RF APRS network is, and how fast you'll be moving. Don't  
set the Min Time too low (beaconing every minute in a busy area causes QRM,  
and is annoying!), but if you're on the highway, and use APRS ALERT (setting  
PL squelch to 100 to hear if other APRS'ers are in simplex range, so you can  
scare up a voice QSO) remember that, at 65 MPH, you'll be out of range in 5  
minutes. Similarly, when mobile, traveling in a straight line, a 5 minute  
Max Time would be appropriate, but not fixed stations, where an hour can go  
by between beacons.

Time Only and Start/Stop depend on whether you're fixed or mobile. A  
Fixed station doesn't start and stop in traffic - at least, I hope not - so  
Time Only would be appropriate. Mobils can check Start/Stop, and set their  
Smart Beaconing parameters of Heading Change (15 degrees ought to get rid of  
a mere lane change), and Max Distance of 5 miles gives a pretty good idea of  
where you are.

Now click Accept, taking you back to Port Configuration, and click on  
Device, where you'll set your Com Port settings. Select the COM port which  
your radio is connected to, the baud rate used between them (usually 9600),  
and the normal 8-N-1 parity settings. Click OK and Accept. And that's it.  
You may have to click ENABLES > RF PORTS > (Your Port) to get it going, but  
it ought to be "close enuf fer gu'mint work"!

One thing I've noticed is that the TNC seems to back out of KISS every  
so often so (reminds my of my YL), once in awhile, you can disable the port,  
turn the D700 off and back on, cycle the TNC through OFF, APRS (wait for it  
to initialize) and back to PKT (again, waiting for it to initialize), then  
re-enable the port.

+============================================================================+

>->-> When replying, remove "!rEm0vEn0sP@M!" from the reply address! <-<-<

+============================================================================+  
| Mr. Arthur "Arte" Booten Tampa, FL <n2zrc!rEm0vEn0sP@M!@arrl.net> |  
| Cruise Missile Coordinates: 2756.75 N / 08228.79 W [EL87sw] |  
| PGP Key Fingerprint: 86B2 D349 8B94 3534 4FDA B4CE C41A 4945 A16F 5A25 |  
+----------------+
