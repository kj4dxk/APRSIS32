---
title: "Debug Logs"
---

# Debug Logs

[Source wiki page](https://aprsisce.wikidot.com/doc:debug-logs) · [Archived HTML](../archive/pages/doc_debug-logs--1c50161d38b8.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

|  |
| --- |
| **Debug Logs** |

|  |
| --- |
| The old Enables / Debug Logging option has been substantially enhanced and brought out for on-line visibility. Debug logs are now separated by category and will dynamically add themselves to cascading menus under Enables / Logging and Enables / View Logs.  * **Enables / Logging**    if checked, indicates that the program is buffering diagnostic data in that particular category. This data will also be going to the APRSISCE/32.LOG file while enabled.  * **Enables / View Logs**    brings up a specific category's window and implicitly enables it as well.  * **Enables / View Logs / All**    brings up and enables ALL currently known diagnostic categories. Categories are dynamic. If no data has ever been logged in a category, that category will not exist in the menus.   Memory considerations: When a category is enabled, it is buffering text data in memory for some period of time (to be configurable before release, currently 10 minutes) or maximum space usage (also to be configurable, but currently 512KB). However, the space constraint will be ignored if it would mean deleting data that is too new (…currently 5 minutes). So, this will consume more memory than you're used to, but overnight on my laptop with the full feed, it only grew to 40MB according to Task Manager.  HOWEVER, have a window open is a different story! The scrolling windows are actually multi-line edit controls (think NotePad) into which the buffered and new events are inserted. This means that they buffer ALL events from the time they were opened until they are closed (not minimized). In that same full-feed instance, overnight the client was occupying approximately 150MB of RAM. When I closed all of the windows (particularly Packets), the usage dropped immediately to more typical levels. So, open the window and minimize it if you want to capture a lot of history. But if you don't, go ahead and close the window knowing that when you re-open it, it will have at least the past 5 minutes of data, provided that you kept the category enabled.  If you try to scroll back an active window, you'll quickly get frustrated as new data arrives at the bottom and jumps the scrollbar back down. Simply select some portion of text and new additions are  suspended. They're still buffered and will be inserted when you finally click somewhere to remove the selection.  Oh, and the Copy menu option doesn't do anything yet. It will eventually copy the entire window contents to the clipboard. I'm also planning Save. You can already select a chunk of text, Right Click and choose Copy to get it into the clipboard if you want.  I'm still working on some additional features, including how to control which data goes to disk and which doesn't. Right now, I think all enabled data goes to the APRSISxx.LOG file.  Finally, you'll notice that some categories show up with data even when they were not enabled. The program considers some data to be especially pertinent, so it buffers it regardless of the Enable setting. This was true in the past for the APRSISxx.LOG file with the \* entries. |

|  |
| --- |
|  |
