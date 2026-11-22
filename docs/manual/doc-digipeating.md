---
title: "DigiPeating"
---

# DigiPeating

APRSISCE/32 currently supports a UITRACE style digipeating. That is to say that it supports the callsign insertion and SSID decrementing digipeat request on a specified alias.

A packet asking for a WIDE2-2 hop request will be digipeated where the resulting change in the hop request will take this form: MYCALL\*,WIDE2-1

APRSISCE/32 uses a rule based routine, where each specific hop request to be acted upon is explicitly defined, as well as the resultant output. This allows for a higher degree of control over the aliases acted upon, but also requires more work to be set up.

Digipeaters like the Kantronics line make it difficult to limit the number of hops supported, and it is easy to circumvent the pseudo-limits imposed by such means. In the APRSISCE/32 rule based digipeater, it is impossible to get the digipeater to act upon any hop request that is not explicitly defined.

To be able to configure APRSISCE/32 to support digipeating, you will need to shut down the program and edit the XML file.

Read about Editing the XML file [Editing XML](doc-editing-xml-configuration.md)

Note that <DigiXform> elements are currently only honored OUTSIDE of an <RFPort> and are considered to be global in their application. This means:

> <!- -RFPort[0]- ->  
> <RFPort Name="AGW">  
> <Protocol>AGW</Protocol>  
> <Device>@127.0.0.1:8000</Device>  
> <…..>  
> <!- -DigiXform- -><!— NOT this one! —>  
> </RFPort>  
> <!- -RFPort[0]- ->
>
> <!- -RFPort[1]- ->  
> <RFPort Name="KISS">  
> <…>  
> <!- -DigiXform- -><!- - NOT this one! —>  
> </RFPort>  
> <!- -RFPort[1]- ->
>
> <!- -DigiXform- -><!- - THIS One! - ->

Home fill-in digipeaters are set up so that they only act upon a single specific subset of the WIDEn-N requests, namely the WIDE1-1 request.

A WIDE1-1 digi would be configured as thus:

> <DigiXform>WIDE1-1=WIDE1\*</DigiXform>

A wide area digipeater that forms a part of the full infrastructure in the local APRS network, but limit the number of hops supported to 2 or less would need to be configured to support WIDE1-1 as above, but also WIDE2-2 and the second hop WIDE2-1.

A WIDE2-N digi would be configured as thus:

> <DigiXform>WIDE1-1=WIDE1\*</DigiXform>  
> <DigiXform>WIDE2-1=WIDE2\*</DigiXform>  
> <DigiXform>WIDE2-2=WIDE2-1</DigiXform>

It is very easily possible to bypass the psuedo hop limits that are implemented in the Kantronics style digipeaters by using a non-standard hop request. These non-standard requests are also sometimes accidentally entered by people that just don't understand the basic premise of digipeating and hop requests. Non-standard hop requests would look something like this: WIDE2-4

The configuration shown above would simply ignore these non-standard hop requests. The configuration below would act upon a non-standard hop request, but would decrement the SSID down to zero, effectively stopping any further digipeats.

A WIDE2-N that wanted to truncate paths would be configured as thus:

> <DigiXform>WIDE1-1=WIDE1\*</DigiXform>  
> <DigiXform>WIDE1-2=WIDE1\*</DigiXform>  
> <DigiXform>WIDE1-3=WIDE1\*</DigiXform>  
> <DigiXform>WIDE1-4=WIDE1\*</DigiXform>  
> <DigiXform>WIDE1-5=WIDE1\*</DigiXform>  
> <DigiXform>WIDE1-6=WIDE1\*</DigiXform>  
> <DigiXform>WIDE1-7=WIDE1\*</DigiXform>  
> <DigiXform>WIDE2-1=WIDE2\*</DigiXform>  
> <DigiXform>WIDE2-2=WIDE2-1</DigiXform>  
> <DigiXform>WIDE2-3=WIDE2\*</DigiXform>  
> <DigiXform>WIDE2-4=WIDE2\*</DigiXform>  
> <DigiXform>WIDE2-5=WIDE2\*</DigiXform>  
> <DigiXform>WIDE2-6=WIDE2\*</DigiXform>  
> <DigiXform>WIDE2-7=WIDE2\*</DigiXform>  
> <DigiXform>WIDE3-1=WIDE3\*</DigiXform>  
> <DigiXform>WIDE3-2=WIDE3\*</DigiXform>  
> <DigiXform>WIDE3-3=WIDE3\*</DigiXform>  
> <DigiXform>WIDE3-4=WIDE3\*</DigiXform>  
> <DigiXform>WIDE3-5=WIDE3\*</DigiXform>  
> <DigiXform>WIDE3-6=WIDE3\*</DigiXform>  
> <DigiXform>WIDE3-7=WIDE3\*</DigiXform>  
> <DigiXform>WIDE4-1=WIDE4\*</DigiXform>  
> <DigiXform>WIDE4-2=WIDE4\*</DigiXform>  
> <DigiXform>WIDE4-3=WIDE4\*</DigiXform>  
> <DigiXform>WIDE4-4=WIDE4\*</DigiXform>  
> <DigiXform>WIDE4-5=WIDE4\*</DigiXform>  
> <DigiXform>WIDE4-6=WIDE4\*</DigiXform>  
> <DigiXform>WIDE4-7=WIDE4\*</DigiXform>  
> <DigiXform>WIDE5-1=WIDE5\*</DigiXform>  
> <DigiXform>WIDE5-2=WIDE5\*</DigiXform>  
> <DigiXform>WIDE5-3=WIDE5\*</DigiXform>  
> <DigiXform>WIDE5-4=WIDE5\*</DigiXform>  
> <DigiXform>WIDE5-5=WIDE5\*</DigiXform>  
> <DigiXform>WIDE5-6=WIDE5\*</DigiXform>  
> <DigiXform>WIDE5-7=WIDE5\*</DigiXform>  
> <DigiXform>WIDE6-1=WIDE6\*</DigiXform>  
> <DigiXform>WIDE6-2=WIDE6\*</DigiXform>  
> <DigiXform>WIDE6-3=WIDE6\*</DigiXform>  
> <DigiXform>WIDE6-4=WIDE6\*</DigiXform>  
> <DigiXform>WIDE6-5=WIDE6\*</DigiXform>  
> <DigiXform>WIDE6-6=WIDE6\*</DigiXform>  
> <DigiXform>WIDE6-7=WIDE6\*</DigiXform>  
> <DigiXform>WIDE7-1=WIDE7\*</DigiXform>  
> <DigiXform>WIDE7-2=WIDE7\*</DigiXform>  
> <DigiXform>WIDE7-3=WIDE7\*</DigiXform>  
> <DigiXform>WIDE7-4=WIDE7\*</DigiXform>  
> <DigiXform>WIDE7-5=WIDE7\*</DigiXform>  
> <DigiXform>WIDE7-6=WIDE7\*</DigiXform>  
> <DigiXform>WIDE7-7=WIDE7\*</DigiXform>

To Regenerate a path (You really DON'T want to do this!):

> <DigiXform>WIDE1-1=WIDE1-1</DigiXform>  
> <DigiXform>WIDE2-1=WIDE2-2</DigiXform>  
> <DigiXform>WIDE2-2=WIDE2-2</DigiXform>

Or even (WORSE!):

> <DigiXform>WIDE1-1=WIDE7-7</DigiXform>

Longer Answer: I've got some interesting ideas on cross-band digipeating, but am currently working through the configuration of the beast so that mere mortals have a chance at making it work, while at the same time, it would be difficult to configure it badly which can have severe ramifications on our shared channels.

The work I'm doing now with renovating the AGW/KISS/TEXT interface implementation is being done with exactly that focus, cross-band digipeating and bi-directional IGating, but with varying paths per interface and individual enables of each feature.

Consider the following snippet of a configuration file I'm working on for VHF digipeating. And then figure out how to apply such a configuration approach to a cross-band, actually X-way cross, digipeater. Like a single APRSIS32 instance with KISS to 2m VHF, AGW to 30m AX.25, TEXT to 30m PSK-63 (via APRS Messenger), and some other interface to listen to the ISS and be a Sat Gate…

Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32

[UIFlood (sort of)](doc-uiflood.md)
