---
title: "Tempest Weather station setup"
---

# Tempest Weather station setup

[Source wiki page](https://aprsisce.wikidot.com/weather:tempest) · [Archived HTML](../archive/pages/weather_tempest--a2df0da978b0.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

I finally got around to setting up my [WeatherFlow Tempest](https://weatherflow.com/) station on the APRS-IS. Here's a brief walkthrough of what I did.

First of course, you need to have your station up and running and registered. This will get you your station ID ST-nnnnnnn which you will need later. My station can be viewed direct at [Tempest's web site](https://tempestwx.com/station/105677/grid). To find your station ID, start at your tempestwx.com display, hit Settings, Stations, YourStation, Manage Devices and you should see the ST-nnnnnnn ID.

I chose to use the [WeeWX](https://www.weewx.com/docs/latest/usersguide.htm) software to do the heavy lifting. After searching around a bit, it seemed like the least effort approach. I'm running WeeWX as a service on a headless Ubunutu server machine that I already had running following [these installation instructions](https://weewx.com/docs/latest/debian.htm) as Ubuntu is based on Debian.

But WeeWX doesn't support the Tempest out of the box, so you do the initial installation with a Simulator and then install the [WeatherFlow-UDP](https://github.com/captain-coredump/weatherflow-udp) plugin. I followed the instructions at [Tempest Integration with WeeWX was easy](https://www.reddit.com/r/myweatherstation/comments/pehrml/tempest_integration_with_weewx_was_easy/) particularly linuxweenie's stepwise response (the 4th one down).

After confirming that WeeWX was communicating properly and generating the local [web page](http://ldeffenb.dnsalias.net/weewx/) (web serving came for "free" because I already had apache running on this machine), I enabled the CWOP section. But since I wanted to post directly to the APRS-IS as a licensed amateur radio operator, I followed Dave Webb KB1PVH's example [CWOP section](https://groups.google.com/g/weewx-user/c/9iVEHv_OVbI/m/AP2KhU54DgAJ) but specified only noam.aprs2.net as my server.

> [[CWOP]]  
> # This section is for configuring posts to CWOP.
>
> # If you wish to do this, set the option 'enable' to true,  
> # and specify the station ID (e.g., CW1234).  
> enable = true  
> station = KJ4ERJ-WX  
> post\_interval = 600  
> log\_success = True  
> log\_failure = True  
> server\_list = noam.aprs2.net:14580  
> passcode = your\_passcode\_here

Finally, I was going to try to (quickly) get this weather data available to my APRSIS32 instance which would require a wxnow.txt file to come out of WeeWX. There is a [plug-in](https://github.com/matthewwall/weewx-cwxn) available to do that, but the installation instructions on that page didn't work for me. Then I found Matthew Wall's [Cumulus WXNow](https://bat-server.dyndns.org:3010/MIRROR/weewx/wiki/cwxn) installation description that worked flawlessly.

BUT! Right after the installation, the wxnow.txt file was updating with every wind update from the Tempest. And each of those updates did not include the rest of the data, only the wind. So, I found a reference [here](https://www.wxforum.net/index.php?topic=38817.0) that showed how to set the plugin to only react to the Archive events in WeeWX instead of the loop events. That ensured that all wxnow.txt updates had the complete information.

> [CumulusWXNow]  
> filename = /var/tmp/wxnow.txt  
> binding = archive

In the process of all of that research, I also found [this PR](https://github.com/matthewwall/weewx-cwxn/pull/1) (pull request) against the cwxn plugin that formats the humidity with the wrong number of digits. So I loaded the required file in my installation and manually fixed it according to the change show in the PR. You're on your own if you want to do this!

So, in the end, I have a wxnow.txt file updating on my Ubuntu machine, but I have run out of time (especially after documenting this) and do not yet have my APRSIS32 instance accessing this file to create a weather object. In actual fact, I'll probably end up writing wxnow.txt support into [APRSISMO/TestHost](https://tinyurl.com/Get-APRSISMO) since that's what currently runs my IGate.
