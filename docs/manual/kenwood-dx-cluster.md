---
title: "Kenwood Dx Cluster"
---

# Kenwood Dx Cluster

The Kenwood APRS line of radios have the ability to decode standard DX Cluster reports which are sent via packet radio. DX Clusters are normally used to share HF band openings through the local community. HF enthusiasts can set up their Kenwood VHF radio, and tune into the local DX Cluster. When stations that are spotted by other users are entered into the local DX Cluster software, DX Spot announcements are sent out from the DX Cluster using Spot announcements using a very specific format. The Kenwood radios upon hearing those packets will beep and display the DX announcement on screen. Up to the last 10 DX Spots are saved in volatile memory, and can be scrolled through to review. With the appropriate cabling, a DX Spot can be sent selected, and tuning commands issued to an attached HF radio, switching bands and frequencies to match the selected DX Spot.

URCALL>RESORC:DX de YOURCALL.>11111111112222222222**\*33333333334444444444**\*55555  
URCALL>RESORC:DX de YOURCALL.>FIELD-ONE.ITEMNAME**\*FIELD-TWOFIELD-3333**\*FORTH  
URCALL>RESORC:DX de VE6SLP-10>144390.0 PIGEON 203 dg frm VE6SLP-10 74km

Leveraging this built in "database" that can be found in these radios, one can send information out to suitably equipped users for display and subsequent retrieval.

There is no correlation between the purpose of a true DX Cluster and the implementation and use of these packet types in APRS. By squeezing non-DX Cluster information into packets that "look like a DX Spot" to a Kenwood, we can eek more utility out of the radios that the designers never thought about.

<table class="wiki-content-table aprs-figure-table">
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_Kenwood_DX_Alert--56cb75f0f70a.jpg"><img alt="Kenwood%20DX%20Alert.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_Kenwood_DX_Alert.jpg_medium--44edba391284.jpg"/></a></td>
<td>The Kenwood line of radios will pop up a screen that alerts the operator to an incoming DX Spot packet. (I missed catching the DX Spot Alerts on the HTs in this image)</td>
</tr>
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_Kenwood_DX_List--4140311eeb37.jpg"><img alt="Kenwood%20DX%20List.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_Kenwood_DX_List.jpg_medium--03c617186ad7.jpg"/></a></td>
<td>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D700_DX_List1--eb9b2a961ef7.png"><img alt="D700%20DX%20List1.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D700_DX_List1.png_small--630fb4826d50.jpg"/></a></div>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D710_DX_List--33d49ffb8a5f.png"><img alt="D710%20DX%20List.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D710_DX_List.png_small--3c3cadc67b35.jpg"/></a></div>
<p>The D700 and D710 radios will show you a list of the packets saved in memory, which you can then choose to investigate further. In the HTs, you can see the first page, and scroll through the packets that way. There is no preview list.</p>
</td>
</tr>
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_Kenwood_DX_Detail--23b8bf148d94.jpg"><img alt="Kenwood%20DX%20Detail.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_Kenwood_DX_Detail.jpg_medium--c296651a1808.jpg"/></a></td>
<td>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D700_DX_Detail1--2dfa3d6f90ab.png"><img alt="D700%20DX%20Detail1.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D700_DX_Detail1.png_small--1fca7b3fefb4.jpg"/></a></div>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D710_DX_Detail--e75135ce665f.png"><img alt="D710%20DX%20Detail.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D710_DX_Detail.png_small--bc2559963ace.jpg"/></a></div>
<p>Full decode of the DX Spot packet is shown on a single screen in the mobile radios. In the HTs, some of the DX Spot information can be viewed over two pages</p>
</td>
</tr>
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_D7_DX1--ab897b7c7dfc.jpg"><img alt="D7%20DX1.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D7_DX1.jpg_small--7e6e7a636b52.jpg"/></a></td>
<td>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D7_DX_List--b35b0562b404.png"><img alt="D7%20DX%20List.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D7_DX_List.png_thumbnail--0f8bb675b85d.jpg"/></a></div>
<p>The first page on the D7 shows the spotted callsign, source, and time.</p>
</td>
</tr>
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_D7_DX2--98b48fa49718.jpg"><img alt="D7%20DX2.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D7_DX2.jpg_small--8126c9ca1357.jpg"/></a></td>
<td>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D7_DX_Detail--0688100ecbbc.png"><img alt="D7%20DX%20Detail.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D7_DX_Detail.png_thumbnail--3e3f4c9140f2.jpg"/></a></div>
<p>The second page on the D7 shows the spotted callsign and comment text</p>
</td>
</tr>
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_D72_DX1--756d5ac7aa72.jpg"><img alt="D72%20DX1.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D72_DX1.jpg_small--0cbbb6f96b98.jpg"/></a></td>
<td>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D72_DX_List--1c2f12f72993.png"><img alt="D72%20DX%20List.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D72_DX_List.png_thumbnail--be40fe96836f.jpg"/></a></div>
<p>The first page on the D72 shows the spotted callsign, source, and time.</p>
</td>
</tr>
<tr>
<td><a href="../archive/assets/local--files_kenwood-dx-cluster_D72_DX2--39007cb62ce5.jpg"><img alt="D72%20DX2.jpg" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D72_DX2.jpg_small--b6f11e77a4f0.jpg"/></a></td>
<td>
<div class="image-container aligncenter" style="text-align: center"><a href="../archive/assets/local--files_kenwood-dx-cluster_D72_DX_Detail--25ea8e0df207.png"><img alt="D72%20DX%20Detail.png" class="image" src="../archive/assets/local--resized-images_kenwood-dx-cluster_D72_DX_Detail.png_thumbnail--c4490916c5ff.jpg"/></a></div>
<p>The second page on the D72 shows the spotted callsign and comment text</p>
</td>
</tr>
</table>
