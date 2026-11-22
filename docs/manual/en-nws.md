---
title: "EN_NWS"
---

# EN_NWS

[Source wiki page](https://aprsisce.wikidot.com/en-nws) · [Archived HTML](../archive/pages/en-nws--2f8c21b2c969.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **National Weather Service** |

<table class="aprs-figure-table" style="width:100%;margin-right:auto;margin-left:auto;border:1px solid #AAA;">
<tr>
<td style="padding: 10px; text-align: left; background-color: #DDDDEE; border: 1px solid silver; width: 100%;"><strong>National Weather Service Setup for APRSIS32</strong><br/>
The US National Weather Service (NWS) puts out information that can be displayed on your APRSIS32 display. This information includes Severe weather warnings. First thing you need to know, is which NWS office you want to display information from.<br/>
<a href="http://en.wikipedia.org/wiki/List_of_National_Weather_Service_Weather_Forecast_Offices" target="_blank">Forecast_Offices</a><br/>
Is one resource website for finding the office abbreviation. The other thing that you will need to do is Download the NWS shape files from <a href="http://www.nws.noaa.gov/geodata/" target="_blank">US NWS</a> or for Australian offices <a href="http://wxsvr.aprs.net.au/shapefiles-new.html" target="_blank">Here</a> There are a lot of them, and it depends on what you want to see for which ones you download. <strong>(Do Not use files dated in the future, but do check regularly for updates)</strong>
<p><strong>Configure-&gt;NWS Shapefiles</strong><br/>
Once you have the files downloaded, you have to point the program to them.</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="nwsshapes.png" class="image" src="../archive/assets/local--files_en-nws_nwsshapes--0d89d9785ba6.png"/></div>
<p>Add each individual file using the add file menu, and enable them by making sure each file has a tick next to it and enabled is also ticked. Set your display settings.</p>
<p><strong>Configure-&gt;NWS-&gt;</strong><br/>
<strong>New Office-&gt;</strong> Enter NWS Office to Monitor</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="newoffice.jpg" class="image" src="../archive/assets/local--files_en-nws_newoffice--416e2d866ba5.jpg"/></div>
<p>In this box, you enter the 3 letter abbreviation for the office you want to receive alerts from <a href="http://en.wikipedia.org/wiki/List_of_National_Weather_Service_Weather_Forecast_Offices" target="_blank">Forecast_Offices</a><br/>
If you enter “*” into this box, you will get alerts from ALL NWS offices. This is a lot of information, especially during storm seasons. Make sure that your PC and internet connection will support this much information.</p>
<p><strong>Messages-&gt;</strong> This is a check box for receiving messages containing NWS information. If you want to see what creates the shapefiles then check this, if you can’t read programming language, then you might want to leave this unchecked.<br/>
<strong>Message NOT all-&gt;</strong> This is for messages that are not directed to ALL stations<br/>
<strong>Messages Notify-&gt;</strong> Do you want to be notified of these messages<br/>
<strong>Notify Line Type-&gt;</strong> What line types do you want to see on your screen.</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="notifylinetype.jpg" class="image" src="../archive/assets/local--files_en-nws_notifylinetype--371afefb086a.jpg"/></div>
<p>Default is all checked.<br/>
<strong>Notify Products-&gt;</strong> This determines which weather Icons you want to be notified of</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="notifyproduct.jpg" class="image" src="../archive/assets/local--files_en-nws_notifyproduct--e6789c9b29b0.jpg"/></div>
<p>Default is all enabled.<br/>
<strong>Notify Multitrack-&gt;</strong> Under what circumstances do you want a multitrack window to pop up</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="notifymultitrack.jpg" class="image" src="../archive/assets/local--files_en-nws_notifymultitrack--16fa6315f19d.jpg"/></div>
<p>Multitracks are memory intensive, be sure that you want to see what you check. Strongly suggested that the windows close when the warning they display expires.<br/>
<strong>Show Offices-&gt;</strong> This will highlight the office coverage area on your main display. If you have more than one office selected, it will overlap.</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="showoffices.jpg" class="image" src="../archive/assets/local--files_en-nws_showoffices--4efb90be5654.jpg"/></div>
<p><strong>NWS Entry Servers-&gt;</strong> what servers do you want to receive data from</p>
<div class="image-container aligncenter" style="text-align: center"><img alt="nwsentryservers.jpg" class="image" src="../archive/assets/local--files_en-nws_nwsentryservers--a00f97e64ab0.jpg"/></div>
<p>This will auto check servers. Notice the Australian server. If you don’t care about the weather, make sure you uncheck it.<br/>
Under that will be a list of configured NWS offices, the checked ones are the ones you will display information for. Again, make sure that your PC and Internet connection can support the data that you’re requesting. If the program locks up shortly after configuring NWS offices, you should think about reducing the number of offices you display.<br/>
<strong>Example NWS alerts</strong></p>
<div class="image-container aligncenter" style="text-align: center"><img alt="image001.jpg" class="image" src="../archive/assets/local--files_en-nws_image001--ab83d24849be.jpg"/></div>
<div class="image-container aligncenter" style="text-align: center"><img alt="image002.jpg" class="image" src="../archive/assets/local--files_en-nws_image002--8a735298fe85.jpg"/></div>
<div class="image-container aligncenter" style="text-align: center"><img alt="image003.jpg" class="image" src="../archive/assets/local--files_en-nws_image003--b4cdc2752daa.jpg"/></div>
<div class="image-container aligncenter" style="text-align: center"><img alt="image004.jpg" class="image" src="../archive/assets/local--files_en-nws_image004--98486d147680.jpg"/></div>
<p>Good luck,<br/>
Adam KC2ANT First version 11/6/12</p>
</td>
</tr>
</table>

|  |
| --- |
|  |
