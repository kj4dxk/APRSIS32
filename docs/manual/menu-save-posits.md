---
title: "Save Posits"
---

# Save Posits

[Source wiki page](https://aprsisce.wikidot.com/menu:save-posits) · [Archived HTML](../archive/pages/menu_save-posits--b70bd4134de1.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

From the 2011/11/17 15:07 Release Notes:

Finally added Configure / Save Posits / Filter… and Reload Paths. The  
former shows how many stations will be saved with the current filter (0  
if you have an invalid filter or no filter) and allows entry of an  
APRS-IS-style filter. The latter does a double-pass over the saved  
posits during reload in an attempt to re-establish the paths between  
them. A double-pass is necessary as a path is not recorded unless the  
station is already known, so the first loading pass generates an  
incomplete path picture. If Reload Paths is not checked, then NO paths  
will be reloaded (speeding the reload somewhat), but allowing new paths  
to be learned quickly as the digi/igate stations will (hopefully) have  
been reloaded (if you restart soon enough that your saved stations don't  
age out 30 seconds after loading).

From the 2011/11/15 16:02 Release Notes:

For the adventurous among you, there's a new XML element called  
<SavedPositFilter> that you'll find at the bottom of the file. Set it  
to an APRS-IS-style filter (b/\* for all) and it will save and reload the  
matching most-recent posit packets when the client is closed and  
restarted. I do NOT recommend this on a full-feed instance unless you  
really clamp down on the filtering! (Read: VERY slow to restart!)  
Also, an amazing number of posits contain non-printable characters. It  
turns out that the XML format doesn't support saving characters with  
ASCII values less than 32, so some stations may not be restored. Also,  
the saved posits are just that, posit packets. There's no recalling of  
the RF/IS status of said station, but they all appear to have been heard  
from APRS-IS. That said, the AGE of the posit is recorded and restored  
and used for expiring old stations, so if your client is down for a long  
time, you can expect to see the loaded stations for up to 30 seconds  
before they disappear due to age.
