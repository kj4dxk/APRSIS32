---
title: "TNC-SCS-DSP"
---

# TNC-SCS-DSP

While you're collecting commands for TNCs, here's what I use with the SCS Tracker/DSP TNC to get into KISS mods on start-up and out of it at shutdown:

<OpenCmd>^027~!!1</OpenCmd>  
<OpenCmd>^064^075!!0</OpenCmd>  
<CloseCmd>^192^255^192~!!1</CloseCmd>  
<CloseCmd>^027~!!1</CloseCmd>

Julian, G4ILO
