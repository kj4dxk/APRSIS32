---
title: "Open/CloseCmds"
---

# Open/CloseCmds

Note: <Open/CloseCmd>s are in the configuration file. Please see [editing-xml-configuration](doc-editing-xml-configuration.md) for information on editing this file.

<Open/CloseCmd> Definition

The <OpenCmd> and <CloseCmd> values follow the syntax precedented by UI-View Here's the gist of it:

The general form of an <OpenCmd> or <CloseCmd> is:

> **Output!Response!Delay!Text**
>
> **Output** = Text to send to the TNC  
> **Response** = Expected response from TNC  
> **Delay** = Seconds to wait  
> **Text** = Descriptive text of command (for future use)

* **Output** : Specifies a string of characters to be sent to the device on the associated port.
  + Output allows/requires some character encoding via ^X or ^nnn to transmit control characters or decimal number (must be all three digits). For instance ^C is a control-C. ^255 transmits a byte whose value is 255. ^192^255^192 is one typical sequence to take some TNCs out of KISS mode.
  + By default, a <CR> (Carriage return, ^M or ^013) is appended to the end of the Output string. If you don't want this, put a ~ (tilde) at the end of Output to suppress the default <CR>.
  + To transmit a ^ (carat), you must put ^^ or ^094 (the ASCII value of a carat).
  + To transmit a ~ (tilde), you must put ^126 (the ASCII value of a tilde).
  + To transmit an ! (exclamation), you must put ^033 (the ASCII value of an exclamation).
  + To transmit a @ (at sign), you must put ^064 (the ASCII value of an at sign)

* **Response** : specifies a string that is returned by the device on the associated port, which APRSISCE/32 will watch for after the Output is sent. Any appearance of this string will allow APRSISCE/32 to continue on, regardless of the timeout. Reception of a specified Response or not doesn't change the delivery of the following commands. It may only allow those commands to go sooner rather than waiting for the full Delay.
  + If you don't specify a Response (no !), "cmd:" is expected.

* **Delay** : specified in seconds and governs either how long APRSISCE/32 will wait for the Response string, or simply an enforced delay in the command stream.
  + If you don't specify a Delay (only one !), 1 second is assumed.

* **Text** : Currently parsed but not used. Eventually it may be used to display missed responses, possibly only in the Port's Trace log as the commands are executed.

* @ is reserved for a future feature and therefore needs to be ^-coded.

Here are some sample commands:

<OpenCmd>~!!0</OpenCmd>

<!— This is a simple NOOP (No Operation) — >

<OpenCmd>~!!5</OpenCmd>

<!— This will just do a 5 second delay — >

<OpenCmd>KISS ON</OpenCmd>

<!— Transmit "KISS ON<cr>", Wait for "cmd:", for 1 second max — >

<OpenCmd>RESTART!!0</OpenCmd>

<!— Transmit "RESTART", Don't expect a response, and go on immediately (0 seconds) — >

<OpenCmd>TN 2.0!TN 2.0</OpenCmd>

<!— Transmit "TN 2.0", Wait for "TN 2.0", for 1 second max — >
