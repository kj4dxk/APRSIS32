---
title: "Port Configuration"
---

# Port Configuration

In order to get APRSISCE/32 to talk to a GPS, a radio or indeed anything that provides or sends information, you need to configure a port. So how do you configure a port and what in the world does TCP/IP have to do with KISS and NMEA?

This nine minute video shows you the basics:

[Open embedded content](../reference/port-configuration-html-19da68415f92c283788eb1c78318eb48a078bc17-12339569201011270062.md)

Here's a bit more detail if you're interested.

APRSISCE/32 supports two different ways of connecting to serial devices. The "normal" expected one would be via a "normal" Windows COM port with baud rates, parity, stop bits and such. However, I also support connected to a TCP/IP source of this same data. This allows the client to connect to remotely served TCP/IP serial ports which may or may not be virtualized by Windows. I also happen to have some TCP/IP servers that remotely serve out COM ports from one Windows machine to another. So…

When you click the KISS or NMEA buttons on the new configuration dialog, you will be asked what sort of connection it will be, TCP/IP, BlueTooth, or COMn Serial. Most of you will probably want COMn Serial for a normal RS-232 port. Clicking that will reveal a Port Configuration dialog with drop-downs of the known COM ports and support baud rates along with radio buttons for Parity and Data/Stop bits. (Ignore the Xmit checkbox). When you have things set up for your GPS or KISS TNC, click OK.

After configuring a COM port, the KISS or NMEA button will take you directly back to the Port Configuration dialog. If you clear the COM port and click OK, the next time you configure that port, the TCP/IP question will be asked.

The AGW interface only support TCP/IP connections to AGWPE. The TCP Configuration dialog prompts for an IP address or DNS name of the host where AGWPE is executing. Port 8000 is the default for AGW and should not be changed unless you know what you're doing.

After configuring the appropriate port(s), you can then enable whichever feature you want to use. If any communications errors are encountered, you may need to disable and re-enable the feature to get things moving again. I need to do more work on failure recovery.

If you are using a Bluetooth serial adapter for either KISS or NMEA GPS connections (I use a Delorme BlueLogger as my GPS), you can either go through the pairing process and assign a COM port outside of the APRSISCE/32 client or you can click Bluetooth back at the beginning. If you pair to an Outgoing COM port, follow the instructions above for configuring a COMn Serial port. If you opt for the direct Bluetooth, APRSISCE/32 will poll for available devices and present a drop-down of the device names. Select one of those for use and proceed into the main port configuration. Whenever a direct Bluetooth port is opened, the devices are again enumerated and a direct connection to the device is established. If a PIN is required, your Bluetooth stack will prompt for it.

Drop a note to the list if you are using any of these new features. I'm anxious to see how they work out and won't know if they're working or not if you don't tell me!

Raw capture of an e-mail describing Quiet Time from 2011/03/23:

The Quiet Time is the maximum length of time during which a packet should have been heard on that port. If no packet is heard within the specified time, the port is closed and re-opened using all of the configure <Close/OpenCmd>s. This is provided as a way to detect a possibly dropped network or bluetooth connection, or maybe even a TNC that has lost its mode and needs a restart to recover. Because there is no heartbeat/keepalive capability on a TNC connection, an RF Port Quiet Time must be set long enough to cover the maximum expected elapsed time between packet receptions. If set too short, the port will unnecessarily close and re-open itself periodically, possibly resulting in dropped packets across the restart. A Quiet Time of zero disables this function.

So, don't set this on a KISS-type port on a Kenwood radio. The default <Open/CloseCmd>s will leave the radio's TNC turned off until you manually turn it back on.

Don't set this on any port that doesn't have a steady stream of expected traffic (like an HF APRS radio port). It just doesn't work out very well.

If you have a steady APRS traffic hum on your radio port, set the Quiet Time to 2-3 TIMES the quiet time you'd expect to see on the channel. This gives APRSISCE/32 a chance to reset the channel if it goes quiet too long. If you have sounds enabled, you'll heard bings and bongs while the radio port transitions through the various states of a restart.

The Port configuration callsign box is disabled and will probably be removed in an upcoming release. It was a misguided original thought to have the ability to specify different callsign-SSIDs for different ports within a single APRSISCE/32 instance. I've since decided, after discussions on this list, that that approach would be overly complex when the same or better functionality can be achieved simply by running a second APRSIS32 instance.

**Port Configuration**  
[CWOP](port-cwop.md)  
[APRS-IS Port](port-aprs-is.md)  
[NEMA Port](port-nema.md)  
[KISS Port](port-kiss.md)  
[IS-Server Port](is-server-port.md)
