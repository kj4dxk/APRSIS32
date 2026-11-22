---
title: "Freqspec"
---

# Freqspec

Hopefully this page will help document the current specifications pertaining to the **Frequency Object** and **Position Comment** specifications, and the various implementations in place.

| Implemented by | ;FFF.FFFxy/ FFF.FFxyz | Position Info | FFF.FFFMHz | FFF.FFFrx/ FFF.FF\_+ | Txxx/ Cxxx/ Dxxx/ 1750/ Exxm | Rxxm/ Rxxk/ Wxxm/ +060 | comments/ free text |
| --- | --- | --- | --- | --- | --- | --- | --- |
| FREQUENCY OBJECT | Required | standard | not allowed | not allowed | optional | Optional (only range defined) | optional |
| POSITION COMMENT | Callsign | standard | required | optional | optional (must have one of these to have next column) | optional | optional |
| Kenwood D710 Frequency Object | Required | standard | parsed | not supported | yes/ yes/ yes/ unknown/ unknown | yes/ yes/ unknown/ yes | optional |
| Kenwood D710 Position Comment | Callsign | standard | parsed | not supported | yes/ yes/ yes/ unknown/ unknown | yes/ yes/ unknown/ yes | optional |
| Kenwood D72 Frequency Object | Required | standard | parsed | not supported | yes/ yes/ yes/ unknown/ unknown | yes/ yes/ unknown/ yes | optional |
| Kenwood D72 Position Comment | Callsign | standard | parsed | not supported | yes/ yes/ yes/ unknown/ unknown | yes/ yes/ unknown/ yes | optional |
| Yaesu FT-350m Frequency Object | Not Supported | | | | | | |
| Yaesu FT-350m Position Comment | Callsign | standard | parsed | not supported | yes/ yes/ yes/ unknown/ unknown | yes/ yes/ unknown/ yes | optional |
