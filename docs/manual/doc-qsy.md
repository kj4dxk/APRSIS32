---
title: "Radio Tune Format"
---

# Radio Tune Format

**Radio Tuning Format (QSY)**  
Some APRS-aware radios, notably the Kenwood D710 and D72 (?) and the Yaesu FTM-350, are capable of tuning the voice "side" of the radio to a frequency received in an APRS packet from another station. In order for this to work, the frequency, tone, and offset must be formatted in a very specific fashion. The following describes the format that is known to be the most compatible with the various QSY-capable radios.

(Special thanks to Mike, AA9VI, for this information)

a brief description of how to sort the correct format to enable auto tune of objects, for permanent objects and QRU ones, thanks to Mike

442.725MHz T114 +500 comment

or

147.345MHz T107 +060 comment

works universally on older and newer Kenwood and Yaesu mobile and portable radios radios for the position comment. Note that the MHz is CaSe SeNsItIvE (the z is the only lower case)!

The corresponding digipeater object syntax would be (in KPC format):  
;442.725+ \*111111z4158.60N/08739.29Wr442.725MHz T114 +500 comment  
;147.345+R\*111111z4208.15N/08748.38Wr147.345MHz T107 +060 comment

The comment for a repeater frequency is a 3.3 decimal frequency followed by a 3 digit tone (or CTCSS or DCS) with no decimal and the the offset in 10KHz units i.e. 145.650MHz T077 -060

where the R in the 2m listing serves to make the repeater object unique… not everyone can list 147.345+ since it is a common repeater pair. Some can list 147.345+R, 147.345+MI, etc. The total object name is 9 characters, so you have 2 or 3 characters after the frequency. Note that the object name must also be space-filled to 9 characters. Many people think the + or - in a frequency object name sets the offset direction, but that is only a visual clue for a human operator. It's the +060 or -060 or whatever that sets the direction AND offset.

The other digi format mentioned on WB4APR's site:

;FFF.FFxyz\*111111zDDMM.hhN/DDDMM.hhWrTnnn RXXm NETxxxxxx MTGxxxxx

will work with newer firmware versions. So, safe to use the one I suggested above to make it work for nearly everyone regardless of whether they updated their firmware or not.

You may be tempted to list your repeater offset as decimal MHz (+6.0M), but that's not the standard. Offsets are specified in 10KHz increments so a 600KHz offset would be +060 or -060. For 440, you might use +500 to get a 5MHz offset.

Bob WB4APR's spec for sending frequency information is [here](http://www.aprs.org/info/freqspec.txt)
