---
title: "futuresym"
---

# futuresym

[Source wiki page](https://aprsisce.wikidot.com/futuresym) · [Archived HTML](../archive/pages/futuresym--441fce023254.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Symbol and tocall file specification discussion** |

<table class="aprs-figure-table" style="width:100%;margin-right:auto;margin-left:auto;border:1px solid #AAA;">
<tr>
<td style="padding: 10px; text-align: left; background-color: #DDDDEE; border: 1px solid silver; width: 100%;">
<div style="text-align: center;">
<p><strong>Existing APRS Symbols Revision H 28/09/2013</strong><br/>
<span style="color: blue">This page is intended for software/firmware authors only. For discussion on format and distribution of Symbol and tocall files</span></p>
</div>
<div class="image-container aligncenter" style="text-align: center"><img alt="sym3.png" class="image" src="../archive/assets/local--files_futuresym_sym3--5c34ce751955.png"/></div>
<div style="text-align: center;">
<p>Note that the table characters shown in yellow were the original overlay characters, but since 2007, any character may have an overlay.</p>
</div>
<div class="image-container aligncenter" style="text-align: center"><img alt="sym1.png" class="image" src="../archive/assets/local--files_futuresym_sym1--6265d69e0f88.png"/></div>
<div style="text-align: center;">
<p>Primary Symbols</p>
</div>
<div class="image-container aligncenter" style="text-align: center"><img alt="sym2.png" class="image" src="../archive/assets/local--files_futuresym_sym2--98f8de2f77c9.png"/></div>
<div style="text-align: center;">
<p>Secondary Symbols</p>
</div>
<br/>
Here is my idea of how to present the new symbols One row per 92 symbols (though broken up into 4 vertical blocks by Punctuation, Numerals, Upper and Lower case.
<div class="image-container aligncenter" style="text-align: center"><img alt="newsym.png" class="image" src="../archive/assets/local--files_futuresym_newsym--527456e53ea0.png"/></div>
<p>It shows the original Primary and alternate and new (if any) plus overlays. I call the new alternate the New "Base" which is what simple systems will show if they dont show special overlay symbols</p>
<p>Bob</p>
<p>I intend to edit this page as discussion continues, and hopefully end up with a file format specification page.<br/>
Of course when we are done the text should of course go on the APRS Specification pages, it's only here because it's easy for me to edit it.<br/>
Steve G6UIM<br/></p>
<div style="text-align: center;">
<p><strong>Future Symbol File Specification and tocall file specification Proposal</strong></p>
</div>
<p>MACHINE READABLE CONFIGURATION FILE</p>
<p>(JSON, XML, CSV, or YAML file) that could be loaded into different client applications, without any need for the client software author to manually track magic human-readable .html / .txt files in various URLs. That sort of work is a completely unnecessarily duplicated effort for us all.</p>
<p>Such a configuration file defining symbols could possibly even be</p>
<p>UPDATED AUTOMATICALLY</p>
<p>to different client applications around the world, by those clients downloading it from somewhere, without any need for the application author to actually build and redistribute a new version of the client.</p>
<p>Something like this happens already: People can actually download Steven's icon set files and install them in UI-View and a number of other apps (that's what I put in aprs.fi actually). It's the closest thing to a standard we have now! Too bad it only includes graphics, and not the text definitions.</p>
<p>I currently manually maintain the Ham::APRS::DeviceID module, which contains a machine-readable definition of APRS device identifiers (tocalls, mic-e identifiers). Take a look at <a href="http://cpansearch.perl.org/src/HESSU/Ham-APRS-DeviceID-1.06/DeviceID.pm">http://cpansearch.perl.org/src/HESSU/Ham-APRS-DeviceID-1.06/DeviceID.pm</a> and scroll back a bit to see the @dstcall_regexps hash definition. I have to manually track changes in <a href="http://www.aprs.org/aprs11/tocalls.txt">http://www.aprs.org/aprs11/tocalls.txt</a> and update them in my code. If tocalls.txt was published in a machine-readable config file format, that could simply be read in and used by all the client apps, without the need for everyone to manually track quiet changes in tocalls.txt.</p>
<p>Maybe I should extract that stuff from DeviceID's perl code and publish it in YAML format instead. That'd make it more usable for APRSIS32 and other apps in other languages. Lynn, would you be interested in using that for device identification?</p>
<p>- Hessu</p>
<p>One of the things that needs some thought is file security. if files are downloadable a user needs to know that the file is official otherwise edited version might spring up and it all get very messy. Client software could warn of non official files, but still allow the use of them. Perhaps an MD5 checksum?</p>
<p>We also need to discuss image format, we don't need to be creating images in different formats and sizes. And of course attempt to future think. Also to make any images and file formats easy to convert to older clients that cannot be upgraded.</p>
<p><strong>Lynn KJ4ERJ and Bob WB4APR Discussion to start things off</strong><br/>
This display format falls apart as soon as we define symbols for additional overlays on each primary symbol. In that case, (which is very sparse), Id suggest maybe a 92 deep table that is 36 icons wide.<br/>
For now, every line has the minimum two symbols. But many already ahve several defined overlay distinctions.</p>
<p>See: <a href="http://www.example.com">http://aprs.org/symbols/symbols-new.txt</a></p>
<p>Actually, I was more thinking 92 tables, one per overlayable symbols, arranged 12 columns 3 rows for 36 symbols each.</p>
<p>Visually, a 92x36 matrix will be nearly impossible to line anything up.<br/>
Individual tables allows only those symbols with overlays to be defined as needed. and 3x12. Or maybe 3x13 where the first is 0-9 with 3 unused and the other two are A-M and O-P for easier visual alignment.</p>
<p>This is also makes each matrix small enough to be easily editable.</p>
<p>Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32</p>
<p>But a single line per basic symbol is much more representative of the structure. It would be only 92 deep and only a little wider than double the width of the present table (36 vs 32).</p>
<p>Actually, Id break it up into at least three vertical sections for punctuation/numerals, uppercase, and lower case to make it printable on standard sized paper.</p>
<p>Bob</p>
<p>92 x 36 = 1932 x 756 pixels at the current 21x21 (I think) size for each. And that's actually too low a resolution for current platforms.</p>
<p>I'm looking at blowing that up 2x or more likely 4x and cleaning up the artifacts as I finish development on the Android port. There are screens out there in excess of 400dpi. The current symbol icons could look a lot better. But if I do that with 92x36 I'll end up with a single image at 7728 x 3024 pixels. Not very manageable.</p>
<p>And I'm still concerned about being able to eye-ball a point in the center of a 92 x 36 grid if you're starting from the look and not the name.</p>
<p>Lynn (D) - KJ4ERJ - Author of APRSISCE for Windows Mobile and Win32</p>
<p>1) Far easier than trying to find it in 92 separate tables spread over multiple pages all with three rows of colors</p>
<p>2) The 92 y 36 grid is vrey sparse, and very easy to see isolated icons in the center. Besides if it looks like some kind of vehicle, go down the list to CARS and go right until you find it.</p>
<p>The Name would be the left most column, then ALL 36 unique symbols of that base would appear to the right.</p>
<p>Bob, WB4APR</p>
<p>No, the 92 depth should be broken up into the existing three or four vertical splits,</p>
<ul>
<li>one for the 16 Punctuation table,</li>
<li>one for the next 16 numerals and punctuation,</li>
<li>one for the Uppercase, and</li>
<li>one for the Lower case.</li>
</ul>
<p>These tables will be clean breaks, easy to view and easy to find entries.</p>
<p>None are more than 32 lines tall and only 36 sparse icons per line.</p>
<p>Bob, WB4APR</p>
</td>
</tr>
</table>

|  |
| --- |
|  |
