#ifndef GOT_SYSHEADS
#define GOT_SYSHEADS

#pragma warning(disable : 4995)

#ifdef _DEBUG
#include <crtdbg.h>
#define HERE ,__FILE__,__LINE__
#define CALLER , char *file, int line
#define DEF_CALLER , char *file=__FILE__, int line=__LINE__
#define MY_CALLER , file, line
#else
#define HERE
#define CALLER
#define MY_CALLER
#ifdef UNDER_CE
#define _malloc_dbg(s,t,f,l) malloc(s)
#endif
#endif

#include <winsock2.h>
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#ifdef UNDER_CE
#ifndef NO_SHELL
#include <aygshell.h>
#endif
#ifndef NO_GPSAPI
#include <gpsapi.h>
#endif
#endif
#include <commctrl.h>

#include <math.h>	/* For sqrt() */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>

#pragma optimize("y", off)		//generate stack frame pointers for all functions - same as /Oy- in the project

#endif	/* GOT_SYSHEADS */
