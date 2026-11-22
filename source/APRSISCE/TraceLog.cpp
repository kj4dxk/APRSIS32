
#define INITGUID

#include "sysheads.h"

#include "config.h"		/* For ActiveConfig */
#include "MsgBox.h"		/* For enables save prompt */
#include "resource.h"	/* GUI Resource Identifiers */
#include "tcputil.h"	/* Various helpers */

#include "TraceLog.h"

#ifdef UNDER_CE
#ifndef CE50
#ifndef NO_SHELL
#define USING_SHELL
#endif
#endif
#endif

static void SaveLogWindow(TRACE_LOG_S *Log);

static unsigned long LogCount;
static TRACE_LOG_S **Logs;
static HANDLE hmtxLogs = NULL;
static DWORD Owner, LastOwner, LockCount, LockMax, TotalLocks;
static HWND hwndLocks = NULL;
static DWORD FirstThread;
static const TCHAR g_szLogName[] = TEXT("TRACE_LOG");
static const TCHAR g_szLockName[] = TEXT("TRACE_LOCKS");

static char *SuppressThreads[] = { "Packets", "Buddies", "Filter", "FilterError", "MyCall", "QRU", "Transmit" };

extern BOOL FirstRun;	/* TRUE if this is first activation */
extern HINSTANCE g_hInstance;

char *GetTraceLogStats(BOOL Detailed)
{	size_t Available = LogCount*80+80;
	char *Output = (char*)malloc(Available);
	char *Next = Output;
	unsigned long StartCount = LogCount;	/* Freeze this */
	unsigned long TotalWindow=0, TotalBuffer=0;

	SortTraceLogs();
	StartCount = LogCount;	/* Reset after the sort */

	for (unsigned long l=0; l<StartCount; l++)
	{	TRACE_LOG_S *Log = Logs[l];
		unsigned long Window=0, Buffer=0;
		for (unsigned long t=0; t<Log->Count; t++)
		{	Buffer += sizeof(*Log->Lines[t])+Log->Lines[t]->Len;
		}
		if (Log->hwndEdit)
			Window = SendMessage(Log->hwndEdit,EM_GETLIMITTEXT,0,0)*sizeof(TCHAR);
		if (Detailed && (Buffer || Window))
		{
#ifdef DEBUG_LEAKS
			StringCbPrintfExA(Next, Available, &Next, &Available,
							STRSAFE_IGNORE_NULLS,
							"%s-%ld %lu+%lu=%lu\n",
							Log->Name, (long) Log->Which, Buffer, Window, Buffer+Window);
#else
			StringCbPrintfExA(Next, Available, &Next, &Available,
							STRSAFE_IGNORE_NULLS,
							"%s %lu+%lu=%lu\n",
							Log->Name, Buffer, Window, Buffer+Window);
#endif
		}
		TotalBuffer += Buffer;
		TotalWindow += Window;
	}
	StringCbPrintfExA(Next, Available, &Next, &Available,
						STRSAFE_IGNORE_NULLS,
						"%ld Logs %lu+%lu=%lu",
						LogCount, TotalBuffer, TotalWindow,
						TotalBuffer+TotalWindow);
	return Output;
}

char *GetTraceFile(HWND hwnd, char *Ext)
{static	char TrcFile[MAX_PATH+32];

#ifdef TRAKVIEW
	StringCbPrintfA(TrcFile,sizeof(TrcFile),"TRAKVIEW%s.log",Ext);
#else
#ifdef UNDER_CE
	TCHAR *Path = (TCHAR *)malloc(sizeof(*Path)*MAX_PATH);
	if (SHGetSpecialFolderPath(hwnd, Path, CSIDL_PERSONAL, TRUE))
	{	StringCbPrintfA(TrcFile, sizeof(TrcFile), "%S/APRSISCE%s.log", Path, Ext);
	} else
		StringCbPrintfA(TrcFile,sizeof(TrcFile),"APRSISCE%s.log",Ext);
	free(Path);
#else
	StringCbPrintfA(TrcFile,sizeof(TrcFile),"APRSIS32%s.log",Ext);
#endif
#endif
	return TrcFile;
}

#ifndef SUPPORT_TRACE_LOGS

void SetTraceThreadName(char *Name)
{
}

#else

static HANDLE hmtxThreads = NULL;
static unsigned long ThreadCount = 0;
static TRACE_THREAD_INFO_S **ThreadInfos = NULL;

BOOL GetThreadIDs(DWORD **IDs, unsigned long *IDCount)
{	if (!hmtxThreads) return FALSE;
	if (!ThreadCount) return FALSE;
	if (WaitForSingleObject(hmtxThreads, 1000) == WAIT_OBJECT_0)
	{	*IDCount = ThreadCount;
		*IDs = (DWORD*)malloc(sizeof(*IDs)*ThreadCount);
		for (unsigned long t=0; t<ThreadCount; t++)
		{	(*IDs)[t] = ThreadInfos[t]->ThreadID;
		}
		ReleaseMutex(hmtxThreads);
		return TRUE;
	} else return FALSE;
	return TRUE;
}

static TRACE_THREAD_INFO_S *GetTraceThreadInfo(void)
{	TRACE_THREAD_INFO_S *Info;
static	BOOL First = TRUE;
static DWORD tlsIndex = TLS_OUT_OF_INDEXES;

	if (First)
	{	tlsIndex = TlsAlloc();
		FirstThread = GetCurrentThreadId();
		First = FALSE;
	}
	if (tlsIndex == TLS_OUT_OF_INDEXES) return NULL;
	Info = (TRACE_THREAD_INFO_S *) TlsGetValue(tlsIndex);
	if (!Info)
	{	DWORD myThreadID = GetCurrentThreadId();
		if (!hmtxThreads)
		{	hmtxThreads = CreateMutex(NULL, FALSE, NULL);
		}
		if (WaitForSingleObject(hmtxThreads, 1000) == WAIT_OBJECT_0)
		{	unsigned long t;
			for (t=0; t<ThreadCount; t++)
			{	if (ThreadInfos[t]->ThreadID == myThreadID)
				{	Info = ThreadInfos[t];
					break;
				}
			}
			if (t >= ThreadCount)
			{	Info = (TRACE_THREAD_INFO_S *)calloc(1,sizeof(*Info));
				Info->ThreadID = myThreadID;
				StringCbPrintfA(Info->DefaultName, sizeof(Info->DefaultName),
								"Tid(%lX)", (long) Info->ThreadID);
				if (!TlsSetValue(tlsIndex, Info))
				{	free(Info);
					Info = NULL;
				} else
				{	unsigned long t = ThreadCount++;
					ThreadInfos = (TRACE_THREAD_INFO_S **)realloc(ThreadInfos,sizeof(*ThreadInfos)*ThreadCount);
					ThreadInfos[t] = Info;
				}
			}
			ReleaseMutex(hmtxThreads);
			if (Info) Info->ThreadName = Info->DefaultName;
		}
	}
	return Info;
}

char *GetThreadNameByID(DWORD tID)
{	char *Result = ((tID==GetCurrentThreadId())?"*Current*":((tID==FirstThread)?"*FIRST*":"*Unknown*"));

	if (hmtxThreads)
	{	if (WaitForSingleObject(hmtxThreads, 1000) == WAIT_OBJECT_0)
		{	for (unsigned long t=0; t<ThreadCount; t++)
			{	if (ThreadInfos[t]->ThreadID == tID)
				{	Result = ThreadInfos[t]->ThreadName;
					break;
				}
			}
			ReleaseMutex(hmtxThreads);
		} else Result = "*LockFail*";
	} else Result = "*NoThreads*";
	return Result;
}

static char *GetTraceThreadName(void)
{	TRACE_THREAD_INFO_S *Info = GetTraceThreadInfo();
	if (!Info || !Info->ThreadName) return NULL;
	return Info->ThreadName;
}

void SetTraceThreadName(char *Name)
{	TRACE_THREAD_INFO_S *Info = GetTraceThreadInfo();
	if (Info) Info->ThreadName = Name;
}

static char *GetDefaultTraceName(void)
{	TRACE_THREAD_INFO_S *Info = GetTraceThreadInfo();
	if (!Info || !Info->ThreadName) return "Activity";
	return Info->DefaultLogName;
}

void SetDefaultTraceName(char *Name)
{	TRACE_THREAD_INFO_S *Info = GetTraceThreadInfo();
	if (Info) Info->DefaultLogName = Name;
}

#ifdef USE_ALPHABLEND
void DrawAlphaBlend (HWND hWnd, HDC hdcwnd)
{
    HDC hdc;               // handle of the DC we will create  
    BLENDFUNCTION bf;      // structure for alpha blending 
    HBITMAP hbitmap;       // bitmap handle 
    BITMAPINFO bmi;        // bitmap header 
    VOID *pvBits;          // pointer to DIB section 
    ULONG   ulWindowWidth, ulWindowHeight;      // window width/height 
    ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height 
    RECT    rt;            // used for getting window dimensions 
    UINT32   x,y;          // stepping variables 
    UCHAR ubAlpha;         // used for doing transparent gradient 
    UCHAR ubRed;        
    UCHAR ubGreen;
    UCHAR ubBlue;
    float fAlphaFactor;    // used to do premultiply 
            
    // get window dimensions 
    GetClientRect(hWnd, &rt);
    
    // calculate window width/height 
    ulWindowWidth = rt.right - rt.left;  
    ulWindowHeight = rt.bottom - rt.top;  

    // make sure we have at least some window size 
    if ((!ulWindowWidth) || (!ulWindowHeight))
        return;

    // divide the window into 3 horizontal areas 
    ulWindowHeight = ulWindowHeight / 3;

    // create a DC for our bitmap -- the source DC for AlphaBlend  
    hdc = CreateCompatibleDC(hdcwnd);
    
    // zero the memory for the bitmap info 
    ZeroMemory(&bmi, sizeof(BITMAPINFO));

    // setup bitmap info  
    // set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth - (ulWindowWidth/5)*2;
    bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight - (ulWindowHeight/5)*2;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

    // create our DIB section and select the bitmap into the dc 
    hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
    SelectObject(hdc, hbitmap);

    // in top window area, constant alpha = 50%, but no source alpha 
    // the color format for each pixel is 0xaarrggbb  
    // set all pixels to blue and set source alpha to zero 
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
            ((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x000000ff; 

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 0x7f;  // half of 0xff = 50% transparency 
    bf.AlphaFormat = 0;             // ignore source alpha channel 

    if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5, 
                    ulBitmapWidth, ulBitmapHeight, 
                    hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
        return;                     // alpha blend failed 
    
    // in middle window area, constant alpha = 100% (disabled), source  
    // alpha is 0 in middle of bitmap and opaque in rest of bitmap  
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
            if ((x > (int)(ulBitmapWidth/5)) && (x < (ulBitmapWidth-ulBitmapWidth/5)) &&
                (y > (int)(ulBitmapHeight/5)) && (y < (ulBitmapHeight-ulBitmapHeight/5)))
                //in middle of bitmap: source alpha = 0 (transparent). 
                // This means multiply each color component by 0x00. 
                // Thus, after AlphaBlend, we have a, 0x00 * r,  
                // 0x00 * g,and 0x00 * b (which is 0x00000000) 
                // for now, set all pixels to red 
                ((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x00ff0000; 
            else
                // in the rest of bitmap, source alpha = 0xff (opaque)  
                // and set all pixels to blue  
                ((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0xff0000ff; 
    
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha  
    bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha) 
   
    if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5+ulWindowHeight, ulBitmapWidth, ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
        return;

    // bottom window area, use constant alpha = 75% and a changing 
    // source alpha. Create a gradient effect using source alpha, and  
    // then fade it even more with constant alpha 
    ubRed = 0x00;
    ubGreen = 0x00;
    ubBlue = 0xff;
    
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
        {
            // for a simple gradient, base the alpha value on the x  
            // value of the pixel  
            ubAlpha = (UCHAR)((float)x / (float)ulBitmapWidth * 255);
            //calculate the factor by which we multiply each component 
            fAlphaFactor = (float)ubAlpha / (float)0xff; 
            // multiply each pixel by fAlphaFactor, so each component  
            // is less than or equal to the alpha value. 
            ((UINT32 *)pvBits)[x + y * ulBitmapWidth] 
                = (ubAlpha << 24) |                       //0xaa000000 
                 ((UCHAR)(ubRed * fAlphaFactor) << 16) |  //0x00rr0000 
                 ((UCHAR)(ubGreen * fAlphaFactor) << 8) | //0x0000gg00 
                 ((UCHAR)(ubBlue   * fAlphaFactor));      //0x000000bb 
        }

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;   // use source alpha  
    bf.SourceConstantAlpha = 0xbf;   // use constant alpha, with  
                                     // 75% opaqueness 

    AlphaBlend(hdcwnd, ulWindowWidth/5, 
               ulWindowHeight/5+2*ulWindowHeight, ulBitmapWidth, 
               ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, 
               ulBitmapHeight, bf);

    // do cleanup 
    DeleteObject(hbitmap);
    DeleteDC(hdc);
    
}
#endif

LRESULT CALLBACK LockWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_CREATE:
	{	SetTimer(hwnd, 10, 100, NULL);
		return 0;
	}
	case WM_TIMER:
		if (wParam == 10)	/* Update timer */
		{	InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case WM_PAINT:
	{	PAINTSTRUCT ps;
// Start the paint operation
		if (BeginPaint(hwnd, &ps) == NULL)
		{	return 0;
		}

static TCHAR Buffer[1024];
		size_t Remaining = ARRAYSIZE(Buffer);
		TCHAR *Next = Buffer;
		unsigned long l;
		SYSTEMTIME stUTCTime;

		GetLocalTime(&stUTCTime);

		StringCbPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
						TEXT("%04ld-%02ld-%02ldT%02ld:%02ld:%02ld\n"),
						(long) stUTCTime.wYear, 
						(long) stUTCTime.wMonth, 
						(long) stUTCTime.wDay, 
						(long) stUTCTime.wHour, 
						(long) stUTCTime.wMinute, 
						(long) stUTCTime.wSecond);

		StringCbPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
						TEXT("%ld:  %ld(%ld)  %ld(%ld) (%ld)\n"), (long) LogCount, (long) Owner, (long) LastOwner, (long) LockCount, (long) LockMax, (long) TotalLocks);
		for (l=0; l<LogCount; l++)
			StringCbPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
							TEXT("%S:  %ld(%ld)  %ld(%ld) (%ld)\n"), Logs[l]->Name, (long) Logs[l]->Owner, (long) Logs[l]->LastOwner, (long) Logs[l]->LockCount, (long) Logs[l]->LockMax, (long) Logs[l]->TotalLocks);

		RECT rcWin;
		GetClientRect(hwnd, &rcWin);
		DrawText(ps.hdc, Buffer, -1, &rcWin, DT_LEFT | DT_TOP | DT_NOPREFIX);

#ifdef USE_ALPHABLEND
		DrawAlphaBlend(hwnd, ps.hdc);
#endif		
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		hwndLocks = NULL;
		return 0;
	case WM_DESTROY:
		/* Deinitialize stuff here */
		hwndLocks = NULL;	/* Redundancy */
		return 0;
	default:
		/* Handle registered window messages like Find/Replace */
		;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void CreateLockWindow(void)
{
#ifndef UNDER_CE
static	BOOL First = TRUE;
	if (First)
	{	WNDCLASSEX lockClass = {0};
		lockClass.cbSize = sizeof(lockClass);
		lockClass.style = CS_HREDRAW | CS_VREDRAW;
		lockClass.lpfnWndProc = LockWndProc;
		lockClass.hInstance = g_hInstance;
//		if (LoadIconMetric(g_hInstance, MAKEINTRESOURCE(IDI_MY_ICON), LIM_SMALL, &lockClass.hIcon) != S_OK)
//		if (CloseStationCount)
//			lockClass.hIcon = MakeSymbolIcon(hwndMain, CloseStations[0].symbol);
		if (!lockClass.hIcon)
			lockClass.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MY_ICON));
		if (!lockClass.hIcon)
			lockClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		lockClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		lockClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
		lockClass.lpszMenuName = MAKEINTRESOURCE(IDM_LOG_W32);
		lockClass.lpszClassName = g_szLockName;
		RegisterClassEx(&lockClass);
		First = FALSE;
	}
	if (!hwndLocks)
	{	static TCHAR Title[80];
		StringCbPrintf(Title, sizeof(Title), TEXT("TraceLocks - %S"), CALLSIGN);
		hwndLocks = CreateWindow(g_szLockName, Title,
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								NULL, NULL, g_hInstance, NULL);
	}
	ShowWindow(hwndLocks, SW_SHOW);
	SetForegroundWindow(hwndLocks);
	UpdateWindow(hwndLocks);
#endif
}

static void LockFailure(char *What, DWORD Owner, DWORD LockCount, size_t Line)
{	FILE *Log = fopen("LockFail.log","a+t");
	if (Log)
	{	SYSTEMTIME st;
		GetSystemTime(&st);
		fprintf(Log, "%4ld-%02ld-%02ld %02ld:%02ld:%02ld %s:Lock(%s) Failed at Line %ld, Owner %ld(%s), Locks %ld\n",
				(long) st.wYear, (long) st.wMonth, (long) st.wDay,
				(long) st.wHour, (long) st.wMinute, (long) st.wSecond,
				GetTraceThreadName(), What, (long) Line,
				(long) Owner, GetThreadNameByID(Owner), (long) LockCount);
		fclose(Log);
	}
#ifdef POPUP_LOCK_FAILURES
	TCHAR Buffer[80];
	StringCbPrintf(Buffer, sizeof(Buffer), TEXT("%S:Lock(%S) Failed at Line %ld, Owner %ld, Locks %ld"),
				GetTraceThreadName(), What, (long) Line, (long) Owner, (long) LockCount);
	MessageBox(NULL, Buffer, TEXT("LockFailure"), MB_OK);
#endif
}

void ShowTraceLog(char *Name, BOOL Enable)	/* NULL for all */
{	DWORD wStat;
	if (hmtxLogs)
	if ((wStat=WaitForSingleObject(hmtxLogs, 1000)) == WAIT_OBJECT_0)
	{	unsigned long l;
		Owner = GetCurrentThreadId(); if (++LockCount > LockMax) LockMax = LockCount; TotalLocks++;
		for (l=0; l<LogCount; l++)
		{	if (!Name || !strcmp(Name, Logs[l]->Name))
				DisplayTrace(Logs[l]->Name, SW_SHOW, Enable);
		}
		if (!--LockCount) { LastOwner = Owner; Owner = 0; }
		if (!ReleaseMutex(hmtxLogs))
		{
#ifdef PRINTF_LOCK_FAILURES
			printf("%lX:Unlock(%s) FAILED with %ld\n", GetCurrentThreadId(), "hmtxLogs", GetLastError());
			fflush(stdout);
#endif
		}
	}
	else
	{	LockFailure("hmtxLogs", Owner, LockCount, __LINE__);
#ifdef PRINTF_LOCK_FAILURES
		printf("%lX:Wait(hmtxLogs) Failed, Owner %lX %ld Locks\n",
			   (long) GetCurrentThreadId(), (long) Owner, (long) LockCount); fflush(stdout);
#endif
#ifdef POPUP_LOCK_FAILURES
		MessageBox(NULL, TEXT("WaitForSingleObject Failed"), TEXT("ShowTraceLog"), MB_OK);
#endif
	}
}

void UnShowTraceLog(char *Name)	/* NULL for all */
{	DWORD wStat;
	if (hmtxLogs)
	if ((wStat=WaitForSingleObject(hmtxLogs, 1000)) == WAIT_OBJECT_0)
	{	unsigned long l;
		Owner = GetCurrentThreadId(); if (++LockCount > LockMax) LockMax = LockCount; TotalLocks++;
		for (l=0; l<LogCount; l++)
		if (Logs[l]->Enabled)
		{	if (!Name || !strcmp(Name, Logs[l]->Name))
			{	TraceLogThread(Logs[l]->Name, TRUE, "Logging Disabled");
				Logs[l]->Enabled = FALSE;
			}
		}
		if (!--LockCount) { LastOwner = Owner; Owner = 0; }
		if (!ReleaseMutex(hmtxLogs))
		{
#ifdef PRINTF_LOCK_FAILURES
			printf("%lX:Unlock(%s) FAILED with %ld\n", GetCurrentThreadId(), "hmtxLogs", GetLastError());
			fflush(stdout);
#endif
		}
	}
	else
	{	LockFailure("hmtxLogs", Owner, LockCount, __LINE__);
#ifdef PRINTF_LOCK_FAILURES
		printf("%lX:Wait(hmtxLogs) Failed, Owner %lX %ld Locks\n",
			   (long) GetCurrentThreadId(), (long) Owner, (long) LockCount); fflush(stdout);
#endif
#ifdef POPUP_LOCK_FAILURES
		MessageBox(NULL, TEXT("WaitForSingleObject Failed"), TEXT("ShowTraceLog"), MB_OK);
#endif
	}
}
#ifdef UNUSED
void PopupTraceLogs(HWND hwnd)
{	DWORD wStat;
	if (hmtxLogs && LogCount)
	if ((wStat=WaitForSingleObject(hmtxLogs, 1000)) == WAIT_OBJECT_0)
	{	unsigned long l;
		HMENU hPopup = CreatePopupMenu();
		Owner = GetCurrentThreadId(); if (++LockCount > LockMax) LockMax = LockCount; TotalLocks++;
		for (l=0; l<LogCount; l++)
		{	TCHAR Name[80];
			StringCbPrintf(Name, sizeof(Name), TEXT("%S"), Logs[l]->Name);
			AppendMenu(hPopup, MF_STRING | ((Logs[l]->Enabled||Logs[l]->hwndLog)?MF_CHECKED:0), l+1, Name);
		}
		if (!--LockCount) { LastOwner = Owner; Owner = 0; }
		if (!ReleaseMutex(hmtxLogs))
		{
#ifdef PRINTF_LOCK_FAILURES
			printf("%lX:Unlock(%s) FAILED with %ld\n", GetCurrentThreadId(), "hmtxLogs", GetLastError());
			fflush(stdout);
#endif
		}

		{	RECT rc;
			POINT pt;
			GetWindowRect(hwnd,&rc);
			pt.x = (rc.left+rc.right)/2;
			pt.y = (rc.top+rc.bottom)/2;
			l = TrackPopupMenuEx(hPopup, TPM_CENTERALIGN | TPM_VCENTERALIGN | TPM_RETURNCMD, pt.x, pt.y, hwnd, NULL);
			if (l && l-1 < LogCount)	/* Got a selection */
			{	DisplayTrace(Logs[l-1]->Name, SW_SHOW);
			}
		}

		DestroyMenu(hPopup);
	}
else
{	LockFailure("hmtxLogs", Owner, LockCount, __LINE__);
#ifdef PRINTF_LOCK_FAILURES
	printf("%lX:Wait(hmtxLogs) Failed, Owner %lX %ld Locks\n",
		   (long) GetCurrentThreadId(), (long) Owner, (long) LockCount); fflush(stdout);
#endif
#ifdef POPUP_LOCK_FAILURES
	MessageBox(NULL, TEXT("WaitForSingleObject Failed"), TEXT("ShowTraceLog"), MB_OK);
#endif
}
}
#endif

BOOL LockTraceLog(TRACE_LOG_S *Log)
{	DWORD wStat;

#ifdef PRINTF_LOCK_FAILURES
	if (Log->Owner && Log->Owner != GetCurrentThreadId())
	{
		printf("%lX:DoubleLock(%s) Owner %lX\n", GetCurrentThreadId(), Log->Name, Log->Owner);
		fflush(stdout);
	}
#endif
	if ((wStat=WaitForSingleObject(Log->hmtx, 1000)) == WAIT_OBJECT_0)
	{	Log->Owner = GetCurrentThreadId(); Log->TotalLocks++;
		if (++Log->LockCount > Log->LockMax) Log->LockMax = Log->LockCount;
		return TRUE;
	} else
	{	LockFailure(Log->Name, Log->Owner, Log->LockCount, __LINE__);
#ifdef PRINTF_LOCK_FAILURES
		printf("%lX:Wait(%s) Failed, Owner %lX %ld Locks\n",
			   (long) GetCurrentThreadId(), Log->Name, (long) Log->Owner, (long) Log->LockCount); fflush(stdout);
#endif
#ifdef POPUP_LOCK_FAILURES
		TCHAR Buffer[80];
		StringCbPrintf(Buffer, sizeof(Buffer), TEXT("%lX:WaitforSingleObject(%S) Failed"),
						(long) GetCurrentThreadId(), Log->Name);
		MessageBox(NULL, Buffer, TEXT("LockTraceLog"), MB_OK);
#endif
		return FALSE;
	}
}

void UnlockTraceLog(TRACE_LOG_S *Log, long Line)
{
	if (!Log) return;
#ifdef PRINTF_LOCK_FAILURES
	if (!Log->Owner) { printf("%lX:Unlocking(%s) w/o Owner from %lX\n", GetCurrentThreadId(), Log->Name, Line); fflush(stdout); }
#endif
	if (!--Log->LockCount) { Log->LastOwner = Log->Owner; Log->Owner = 0; }
	if (!ReleaseMutex(Log->hmtx))
	{
#ifdef PRINTF_LOCK_FAILURES
		printf("%lX:Unlock(%s) FAILED with %ld from %ld\n", GetCurrentThreadId(), Log->Name, GetLastError(), Line);
		fflush(stdout);
#endif
	}
}

static int CompareLogNames(const void *One, const void *Two)
{	TRACE_LOG_S **Left = (TRACE_LOG_S **)One;
	TRACE_LOG_S **Right = (TRACE_LOG_S **)Two;

	return _stricmp((*Left)->Name, (*Right)->Name);
}

void SortTraceLogs(void)
{	DWORD wStat;

	if (!hmtxLogs) return;
	if ((wStat=WaitForSingleObject(hmtxLogs, 1000)) == WAIT_OBJECT_0)
	{
		Owner = GetCurrentThreadId(); if (++LockCount) LockMax = LockCount; TotalLocks++;
		qsort(Logs, LogCount, sizeof(*Logs), CompareLogNames);
		if (!--LockCount) { LastOwner = Owner; Owner = 0; }
		if (!ReleaseMutex(hmtxLogs))
		{
		}
	}
	else
	{	LockFailure("hmtxLogs", Owner, LockCount, __LINE__);
	}
}


TRACE_LOG_S *GetTraceLog(char *Name)
{	TRACE_THREAD_INFO_S *Info = GetTraceThreadInfo();
	TRACE_LOG_S *Log = NULL;
	DWORD wStat;

	if (Info && Info->LastLog && Info->LastLogName && !strcmp(Name, Info->LastLogName))
	{	Log = Info->LastLog;
	} else
	{
		if (!hmtxLogs)
		{	hmtxLogs = CreateMutex(NULL, FALSE, NULL);
			if (ActiveConfig.Enables.DebugStartup && !FirstRun) CreateLockWindow();
		}
		if ((wStat=WaitForSingleObject(hmtxLogs, 1000)) == WAIT_OBJECT_0)
		{
			Owner = GetCurrentThreadId(); if (++LockCount) LockMax = LockCount; TotalLocks++;
			{	unsigned long l;
				for (l=0; l<LogCount; l++)
				{	if (!strcmp(Name, Logs[l]->Name))
						break;
				}
				if (l >= LogCount)
				{	l = LogCount++;
					Logs = (TRACE_LOG_S **) realloc(Logs, sizeof(*Logs)*LogCount);
					Log = Logs[l] = (TRACE_LOG_S *) calloc(1,sizeof(*Logs[l]));
#ifdef DEBUG_LEAKS
					Log->Which = (unsigned short) LogCount;
#endif
					if (!Log)
					{	fprintf(stderr,"GetTraceLog:Huh, Failed to allocate Logs[%ld/%ld](%ld) for %s\n",
								(long) l, (long) LogCount, (long) sizeof(*Logs[l]), Name);
					} else
					{
					Log->hmtx = CreateMutex(NULL, FALSE, NULL);
					Log->Name = _strdup(Name);
					Log->Enabled = ActiveConfig.Enables.DebugStartup && !FirstRun;
					for (int t=0; t<sizeof(SuppressThreads)/sizeof(SuppressThreads[0]); t++)
					{	if (!_stricmp(SuppressThreads[t], Name))
						{	Log->SuppressThread = TRUE;
							break;
						}
					}
#ifndef UNDER_CE
					if (Log->Enabled)
					{	if (GetCurrentThreadId() == FirstThread)
							ShowTraceLog(Name);
					}
#endif
					if (!Log->Enabled && !FirstRun)
					{	Log->Enabled = LocateSimpleStringEntry(&ActiveConfig.AlwaysTraceLogs, Log->Name) != -1;
						if (!Log->Enabled)
							Log->Enabled = LocateSimpleStringEntry(&ActiveConfig.TraceLogs, Log->Name) != -1;
					}
					// qsort(Logs, LogCount, sizeof(*Logs), CompareLogNames);
					}
				} else Log = Logs[l];
			}
			if (!--LockCount) { LastOwner = Owner; Owner = 0; }
			if (!ReleaseMutex(hmtxLogs))
			{
#ifdef PRINTF_LOCK_FAILURES
				printf("%lX:Unlock(%s) FAILED with %ld\n", GetCurrentThreadId(), "hmtxLogs", GetLastError());
				fflush(stdout);
#endif
			}
		}
		else
		{	LockFailure("hmtxLogs", Owner, LockCount, __LINE__);
#ifdef POPUP_LOCK_FAILURES
			printf("%lX:Wait(hmtxLogs) Failed, Owner %lX %ld Locks\n",
				   (long) GetCurrentThreadId(), (long) Owner, (long) LockCount); fflush(stdout);
			MessageBox(NULL, TEXT("WaitForSingleObject Failed"), TEXT("GetTraceLog"), MB_OK);
#endif
		}
	}

	if (Info && Log)
	{	Info->LastLogName = Log->Name;
		Info->LastLog = Log;
	}

	if (Log && !LockTraceLog(Log))
	{
#ifdef PRINTF_LOCK_FAILURES
		printf("%lX:Lock(GetTraceLog(%s))\n", (long) GetCurrentThreadId(), Log->Name); fflush(stdout);
#endif
		Log = NULL;	/* Failed to lock, we didn't find the log */
	}
	return Log;
}

void PurgeTraceLog(TRACE_LOG_S *Log, long MaxMinutes, long KeepMinutes, long MinMinutes, unsigned long MaxKB, long unsigned MinKB)
{
	if (Log && !strcmp(Log->Name,"PurgeTrace")) return;

	if (Log && Log->Count)	/* Anything in there? */
	{	__int64 Now = llGetMsec();
		__int64 Last = Now - MaxMinutes*(__int64)60*1000L;	/* Purge when N minutes old */
		if (Log->FirstWhen < Last		/* Old enough first? */
		|| Log->TotalSize > MaxKB*1024)	/* Or space too big? */
		{	__int64 First = Now - MinMinutes*(__int64)60*1000L;
			Last = Now - KeepMinutes*(__int64)60*1000L;	/* Keep logs for N minutes */
			if (LockTraceLog(Log))	/* Yep, lock it down */
			{	unsigned long l;
				for (l=0; l<Log->Count; l++)
				{	if (Log->Lines[l]->When >= First	/* Too young? */
					|| (Log->Lines[l]->When <= Last		/* got them new enough? */
						&& Log->TotalSize <= MinKB*1024))	/* And small enough? */
						break;
					else
					{	Log->TotalSize -= Log->Lines[l]->Len;
						free(Log->Lines[l]);
					}
				}
				if (l)	/* Something to free? */
				{	Log->Count -= l;
					if (Log->UpdateCount > l)
						Log->UpdateCount -= l;
					else Log->UpdateCount = 0;
					if (Log->Count)
					{	memmove(&Log->Lines[0], &Log->Lines[l], Log->Count*sizeof(Log->Lines[l]));
						Log->FirstWhen = Log->Lines[0]->When;
					} else Log->FirstWhen = 0;
				}
				UnlockTraceLog(Log, __LINE__);
#ifdef OBSOLETE_DEBUG
				if (!KeepMinutes)
					TraceLogThread("PurgeTrace", TRUE, "%s-%ld:Freed=%ld Leaving=%ld First=%llu Last=%llu Left=%llu\n",
									Log->Name, (long) Log->Which,
									(long) l, (long) Log->Count,
									First, Last, Log->FirstWhen);
#endif
			}
			else
			{
#ifdef PRINTF_LOCK_FAILURES
				printf("%lX:Lock(PurgeTraceLog(%s)) Failed Owner %lX Count %ld\n", (long) GetCurrentThreadId(), Log->Name, Log->Owner, Log->LockCount); fflush(stdout);
#endif
			}
		}
#ifdef OBSOLETE_DEBUG
		else if (!KeepMinutes)
					TraceLogThread("PurgeTrace", TRUE, "%s-%ld:NOT FirstWhen %llu >= %llu and %lu < %lu\n",
									Log->Name, (long) Log->Which,
									Log->FirstWhen, Last,
									Log->TotalSize, MaxKB*1024);
#endif
	}
}

void ClearTraceLog(char *Name)
{	TRACE_LOG_S *Log = GetTraceLog(Name);
	if (Log)
	{	if (Log->hwndLog)
			SendMessage(Log->hwndLog, WM_CLOSE, 0, 0);
		PurgeTraceLog(Log, 0, 0, 0, 0, 0);
		UnlockTraceLog(Log, __LINE__);
	}
}

void PurgeTraceLogs(long MaxMinutes, long KeepMinutes, long MinMinutes, unsigned long MaxKB, long unsigned MinKB)
{	DWORD wStat;
	if (hmtxLogs)
	if ((wStat=WaitForSingleObject(hmtxLogs, 500)) == WAIT_OBJECT_0)
	{	unsigned long l;
		Owner = GetCurrentThreadId(); if (++LockCount > LockMax) LockMax = LockCount; TotalLocks++;
		for (l=0; l<LogCount; l++)
		{	if (Logs[l]->Enabled) PurgeTraceLog(Logs[l]);
			else PurgeTraceLog(Logs[l],MaxMinutes,KeepMinutes,MinMinutes,MaxKB,MinKB);	/* Reduced parameters for un-enabled logs */
		}
		if (!--LockCount) { LastOwner = Owner; Owner = 0; }
		if (!ReleaseMutex(hmtxLogs))
		{
#ifdef PRINTF_LOCK_FAILURES
			printf("%lX:Unlock(%s) FAILED with %ld\n", GetCurrentThreadId(), "hmtxLogs", GetLastError());
			fflush(stdout);
#endif
		}
	}
	else
	{	LockFailure("hmtxLogs", Owner, LockCount, __LINE__);
#ifdef POPUP_LOCK_FAILURES
		printf("%lX:Wait(hmtxLogs) Failed, Owner %lX %ld Locks\n",
			   (long) GetCurrentThreadId(), (long) Owner, (long) LockCount); fflush(stdout);
		MessageBox(NULL, TEXT("WaitForSingleObject Failed"), TEXT("PurgeTraceLogs"), MB_OK);
#endif
	}
}

BOOL IsTraceLogEnabled(char *Name)
{	TRACE_LOG_S *Log = GetTraceLog(Name);
	BOOL Result = FALSE;
	if (Log)
	{	Result = Log->Enabled;
		UnlockTraceLog(Log, __LINE__);
	}
	return Result;
}

void TraceLogActual(char *Name, BOOL ForceIt, char *Text, char *NoThreadText)
{	TRACE_LOG_S *Log = GetTraceLog(Name);

	if (Log)
	{	if (ForceIt || Log->Enabled)
		{
#ifndef UNDER_CE	/* CE always suppresses thread name */
			if (Log->SuppressThread)	/* Win32 optionally suppresses */
#endif
				Text = NoThreadText;
			size_t Len = strlen(Text)+1;	/* +1 for null */
			TRACE_LINE_S *Line = (TRACE_LINE_S *)malloc(sizeof(*Line)+sizeof(*Line->What)*Len);

			Line->When = llGetMsec();
#ifdef DEBUG_LEAKS
			Line->Which = Log->Which;
#endif
			if (Line->When < Log->LastWhen) Line->When = Log->LastWhen+1;
			Log->LastWhen = Line->When;

			Line->What = (char *) &Line[1];	/* Point to end of buffer */
			strcpy(Line->What, Text);
			Line->Len = (unsigned short) Len;
			Log->TotalSize += Len;

			unsigned long l = Log->Count++;
			if (Log->Count > Log->Size)
			{	Log->Size = Log->Count + 128;	/* Grow in chunks */
				Log->Lines = (TRACE_LINE_S **) realloc(Log->Lines, sizeof(*Log->Lines)*Log->Size);
			}
			if (!l) Log->FirstWhen = Line->When;	/* Remember oldest */
			Log->Lines[l] = Line;
			if (Len > Log->Longest) Log->Longest = Len;
			//PurgeTraceLog(Log);	/* Save time by not doing this EVERY entry */
		}
		UnlockTraceLog(Log, __LINE__);
	}
}

LRESULT CALLBACK LogWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	TRACE_LOG_S *Log;

	switch (iMsg)
	{
	case WM_CREATE:
	{	CREATESTRUCT *cs = (CREATESTRUCT *) lParam;
		Log = (TRACE_LOG_S *) cs->lpCreateParams;
		Log->UpdateWhen = 0;	/* Force first pass to fill window */
		Log->UpdateCount = 0;	/* And start from first index */
		SetWindowLong(hwnd, GWL_USERDATA, (LONG) Log);

//#ifdef DOESNT_WORK
#if defined(USING_SHELL) /* || defined(CE50) */
	SHMENUBARINFO shmbi;
	SHINITDLGINFO shidi;

	// Create our softkey bar
	ZeroMemory(&shmbi, sizeof(shmbi));
	shmbi.cbSize = sizeof(shmbi);
	shmbi.hwndParent = hwnd;
	shmbi.dwFlags = SHCMBF_HMENU;
	shmbi.nToolBarId = IDM_LOG;
	shmbi.hInstRes = g_hInstance;
	if (!SHCreateMenuBar(&shmbi))
	{
#ifdef DONT_DO_THIS
		// Failed!!
		return FALSE;
	} else
	{	//g_hwndMb          = shmbi.hwndMB;
		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize       = sizeof(tbbi);
		tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;
		SendMessage(shmbi.hwndMB, TB_GETBUTTONINFO, 1, (LPARAM)&tbbi);
		g_hMenu           = (HMENU)tbbi.lParam;
#endif
	} else
	{	//g_hwndMb          = shmbi.hwndMB;
		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize       = sizeof(tbbi);
#ifdef TBIF_BYINDEX
		tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;
		if (!SendMessage(shmbi.hwndMB, TB_GETBUTTONINFO, 1, (LPARAM)&tbbi))
			TraceError(hwnd, "TB_GETBUTTONINFO FAILED!\n");
#else
		if (!shmbi.hwndMB)
			TraceError(hwnd, "hwndMB is NULL!\n");
		tbbi.dwMask       = TBIF_LPARAM;
		if (!SendMessage(shmbi.hwndMB, TB_GETBUTTONINFO, shmbi.nToolBarId, (LPARAM)&tbbi))
		{	TraceError(hwnd, "TB_GETBUTTONINFO FAILED!\n");
			tbbi.lParam = (LPARAM) SHMenuBar_GetMenu(shmbi.hwndMB,shmbi.nToolBarId);
			if (!tbbi.lParam)
				TraceError(hwnd, "SHMenuBar_GetMenu FAILED!\n");
		}
#endif
		Log->hMenu      = (HMENU)tbbi.lParam;
	}

	// Windows Mobile applications should always display their main window
	// full-screen. We're going to let the OS do this for us by calling
	// SHInitDialog, even though technically this window isn't a dialog window.
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.hDlg = hwnd;	/* SHIDIF_SIZEDLG */
	shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_SIPDOWN;
	if (!SHInitDialog(&shidi))
	{
	}

	SHDoneButton(hwnd, SHDB_HIDE);	/* SHDB_SHOW, SHDB_HIDE, SHDB_SHOWCANCEL */
#endif
#ifdef CE50
	Log->hwndCB = CommandBar_Create(g_hInstance, hwnd, 0xBEEE);
	if (Log->hwndCB)
	{	if (!CommandBar_InsertMenubar(Log->hwndCB, g_hInstance, IDM_LOG, 0))
			TraceLogThread("Activity", TRUE, "InsertMenubar(%p) Failed with %ld\n", Log->hwndCB, (long) GetLastError());
		else
		{	TraceLogThread("Activity", TRUE, "InsertMenubar(%p) SUCCESS!\n", Log->hwndCB);
			Log->hMenu = CommandBar_GetMenu(Log->hwndCB, 0);
			Log->MenuOffset = CommandBar_Height(Log->hwndCB);
		}
		//CommandBar_AddAdornments (hwndCB, 0, 0); /*WM_HELP | CMDBAR_OK*/
	} else TraceLogThread("Activity", TRUE, "CommandBar_Create Failed with %ld\n", (long) GetLastError());

#else
#ifndef UNDER_CE
	Log->hMenu = GetMenu(hwnd);
#endif
#endif
//#endif
		Log->hwndEdit = CreateWindow(TEXT("edit"), NULL,
									WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER |
									ES_LEFT | ES_MULTILINE | ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY,
									0, 0, 0, 0, hwnd, (HMENU) 1, g_hInstance, NULL);	/* Child window ID 1 */
		SendMessage(Log->hwndEdit, EM_LIMITTEXT, 2000, 0L);

		SendMessage(hwnd, WM_USER, 0, 0);	/* Update Enable menu */
		SendMessage(hwnd, WM_TIMER, 10, 0);	/* Kickstart the display before visibility */
		SetTimer(hwnd, 10, 500, NULL);
		SetTimer(hwnd, 11, 30*1000, NULL);
		return 0;
	}
	case WM_USER:	/* Fix the enable/disable menu option */
	{	Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
#ifdef UNDER_CE
#ifdef USING_SHELL
		HWND hwndBar = SHFindMenuBar(hwnd);
		if (hwndBar)
		{	TBBUTTONINFO tbbi = {0};
		 // Change title of menu 
			tbbi.cbSize = sizeof(tbbi);
			tbbi.dwMask = TBIF_TEXT;
			tbbi.pszText = Log->Enabled?TEXT("Disable"):TEXT("Enable");
			SendMessage(hwndBar,TB_SETBUTTONINFO,ID_LOG_ENABLE,(LPARAM)&tbbi);
			DrawMenuBar(hwndBar);
		}
#endif
#endif
		//HMENU hMenu = GetMenu(hwnd);
		if (Log->hMenu)
		{	MENUITEMINFO mi = {0};
			mi.cbSize = sizeof(mi);
			mi.fMask = MIIM_TYPE;
			mi.fType = MFT_STRING;
			mi.dwTypeData = Log->Enabled?TEXT("Disable"):TEXT("Enable");
			mi.cch = 7;
			if (!SetMenuItemInfo(Log->hMenu, ID_LOG_ENABLE, FALSE, &mi))
			{	TraceLog("Menu", TRUE, hwnd, "SetMenuItemInfo(ID_LOG_ENABLE) Failed, Error %ld\n", GetLastError());
			}
#ifdef CE50
			CommandBar_DrawMenuBar(Log->hwndCB,0);
#else
			DrawMenuBar(hwnd);
#endif
		}
		return 0;
	}

	case WM_SETFOCUS:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		SetFocus(Log->hwndEdit);
#ifdef USING_SHELL
		SHFullScreen(hwnd,SHFS_HIDESIPBUTTON);
#endif
		return 0;
#ifdef USING_SHELL
	case WM_KILLFOCUS:
		SHFullScreen(hwnd,SHFS_SHOWSIPBUTTON);
		return 0;
#endif
	case WM_TIMER:
		if (wParam == 10)	/* Update timer */
		{	Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
			if (Log->LastWhen != Log->UpdateWhen)
			{	unsigned long Start, End;
				SendMessage(Log->hwndEdit, EM_GETSEL, (WPARAM) &Start, (LPARAM) &End);
				if (Start != End		/* Don't disrupt a user selection */
				|| IsIconic(hwnd))		/* but flash my icon */
				{
#ifndef UNDER_CE
					if (!Log->isFlashing)
					{	FLASHWINFO fw = {0};
						fw.cbSize = sizeof(fw);
						fw.hwnd = hwnd;
						fw.dwFlags = FLASHW_ALL | FLASHW_TIMER;
						FlashWindowEx(&fw);
						Log->isFlashing = TRUE;
					}
#endif
				} else if (LockTraceLog(Log))
				{	BOOL Inserted = FALSE;
					unsigned long l, Skipped=0;
					size_t Remaining, Original;
					TCHAR *Next, *Buff;

#ifndef UNDER_CE
					if (Log->isFlashing)
					{	FLASHWINFO fw = {0};
						fw.cbSize = sizeof(fw);
						fw.hwnd = hwnd;
						fw.dwFlags = FLASHW_STOP;	/* But stop the window flashing! */
						FlashWindowEx(&fw);
						Log->isFlashing = FALSE;
					}
#endif

					unsigned long Len = GetWindowTextLength(Log->hwndEdit);
					unsigned long CurrentSize = (unsigned long) SendMessage(Log->hwndEdit, EM_GETLIMITTEXT, 0, 0);
					unsigned long Available = CurrentSize - Len;
					unsigned long Required = 80;

					SendMessage(Log->hwndEdit, WM_SETREDRAW, FALSE, 0);

					SendMessage(Log->hwndEdit, EM_SETSEL, Len, Len);
					for (l=Log->UpdateCount; l<Log->Count; l++)
						Required += strlen(Log->Lines[l]->What)+3;
					if (Required >= Available)
					{	SendMessage(Log->hwndEdit, EM_LIMITTEXT, CurrentSize+Required+80+CurrentSize/4, 0);
					}

					Original = Remaining = max(Log->Longest+3,min(Required,16*1024));	/* no more than 16K at a time */
					Buff = Next = (TCHAR *)malloc(sizeof(*Buff)*Remaining);
					*Buff = *TEXT("");	/* Just in case there's nothing to add */

					for (l=Log->UpdateCount; l<Log->Count; l++)
					if (Log->Lines[l]->When > Log->UpdateWhen)
					{	Inserted = TRUE;
						if (Remaining < strlen(Log->Lines[l]->What)+3)
						{	SendMessage(Log->hwndEdit, EM_REPLACESEL, FALSE, (LPARAM) Buff);
							Remaining = Original;
							Next = Buff;
						}
						StringCchPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
							TEXT("%S\r\n"), Log->Lines[l]->What);
					} else Skipped++;
					if (Next != Buff) SendMessage(Log->hwndEdit, EM_REPLACESEL, FALSE, (LPARAM) Buff);
#ifdef VERBOSE
					//if (Skipped || Available <= Required)
					{	StringCchPrintf(Buff, Log->Longest+3, TEXT("%lu Remaining %ld/%ld Skipped %ld Len %ld Size %ld Available %ld Required %ld\r\n"), Log->TotalSize, (long) Remaining, (long) Original, (long) Skipped, (long) Len, (long) CurrentSize, (long) Available, (long) Required);
						SendMessage(Log->hwndEdit, EM_REPLACESEL, FALSE, (LPARAM) Buff);
					}
#endif
					Log->UpdateWhen = Log->LastWhen;
					Log->UpdateCount = Log->Count;
					UnlockTraceLog(Log, __LINE__);
					SendMessage(Log->hwndEdit, WM_SETREDRAW, TRUE, 0);
					RedrawWindow(Log->hwndEdit, NULL, NULL,
								RDW_ERASE | RDW_FRAME
								| RDW_INVALIDATE | RDW_ALLCHILDREN);
					if (Buff) free(Buff);
					if (Inserted) SendMessage(Log->hwndEdit, EM_SCROLLCARET, 0, 0);
				}
			}
		} else if (wParam == 11)	/* Purge timer */
		{	Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
			PurgeTraceLog(Log);
		}
		break;

#ifndef UNDER_CE
	case WM_MOVE:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		if (Log && !IsIconic(hwnd) && !IsZoomed(hwnd))
			GetWindowRect(hwnd, &Log->rcSize);
		break;
#endif

	case WM_SIZE:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		if (Log)
		{	MoveWindow(Log->hwndEdit, 0, Log->MenuOffset,
						LOWORD(lParam), HIWORD(lParam)-Log->MenuOffset, TRUE);
#ifndef UNDER_CE
			if (Log && !IsIconic(hwnd) && !IsZoomed(hwnd))
				GetWindowRect(hwnd, &Log->rcSize);
#endif
		}
		return 0;
	case WM_COMMAND:
//TraceLog("Menu", TRUE, hwnd, "Log:WM_COMMAND: 0x%lX 0x%lX\n", wParam, lParam);
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		if (lParam && LOWORD(wParam) == 1)
		{	switch (HIWORD(wParam))
			{
			case EN_UPDATE:	/* Flag needs Save */
				return 0;
			case EN_ERRSPACE:
			case EN_MAXTEXT:
//				MessageBox(hwnd, TEXT("Growing Edit Control"), TEXT("EN_MAXTEXT/ERRSPACE"), MB_OK);
				SendMessage(Log->hwndEdit, EM_SETLIMITTEXT, SendMessage(Log->hwndEdit, EM_GETLIMITTEXT, 0, 0)+32000, 0);
				return 0;
			}
			break;
		}
		switch (LOWORD(wParam))	/* Handle menu options here */
		{
		case ID_LOG_ENABLE:
			Log->Enabled = !Log->Enabled;
			TraceLog(Log->Name, TRUE, hwnd, "Logging %s", Log->Enabled?"Enabled":"Disabled");
			SendMessage(hwnd, WM_USER, 0, 0);	/* Update Enable menu */
			return 0;
		case ID_LOG_CLEAR:
			SetWindowText(Log->hwndEdit, TEXT(""));
			PurgeTraceLog(Log, 0, 0, 0, 0, 0);
			return 0;
		case ID_LOG_COPY:
		{	int Len = GetWindowTextLength(Log->hwndEdit);
			if (Len > 0)
			{	SendMessage(Log->hwndEdit, EM_SETSEL, 0, -1);
				SendMessage(Log->hwndEdit, WM_COPY, 0, 0);
				SendMessage(Log->hwndEdit, EM_SETSEL, Len, Len);
			}
			return 0;
		}
		default: ;
		}
		break;
	case WM_KEYDOWN:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		switch (wParam)
		{
		case VK_F1: case VK_F2: case VK_F3: case VK_F4:
		case VK_F5: case VK_F6: case VK_F7: case VK_F8:
		case VK_F9: case VK_F10: case VK_F11:
		case VK_F13: case VK_F15: case VK_F16:
		case VK_F17: case VK_F18: case VK_F19: case VK_F20:
		case VK_F21: case VK_F22: case VK_F23: case VK_F24:
	#ifndef NEW_CK30
		case VK_F12: case VK_F14:
	#endif
			break;
		case VK_ESCAPE:
		{	int Len = GetWindowTextLength(Log->hwndEdit);
			SendMessage(Log->hwndEdit, EM_SETSEL, Len, Len);
			break;
		}
		}
		break;
	case WM_CHAR:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		switch (wParam)
		{
		case 1:	/* Ctrl-A (About) */	break;
		case 3:	/* Ctrl-C Copy */		break;
		case 4:	/* Ctrl-D */			break;
		case 6:	/* Ctrl-F - conFigure */	break;
		case 14:	/* Ctrl-N - foNts */	break;
		case 16:	/* Ctrl-P */		break;
		case 18:	/* Ctrl-R */		break;
		case 20:	/* Ctrl-T */		break;
		case 22:	/* Ctrl-V Paste */	break;
		case 24:	/* Ctrl-X */		break;
		case 27:	/* Escape */
		{	int Len = GetWindowTextLength(Log->hwndEdit);
			SendMessage(Log->hwndEdit, EM_SETSEL, Len, Len);
			break;
		}
		}
		break;
	case WM_CLOSE:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		SaveLogWindow(Log);
		DestroyWindow(Log->hwndEdit);
		Log->hwndEdit = NULL;
		DestroyWindow(hwnd);
		Log->hwndEdit = Log->hwndLog = NULL;
		return 0;
	case WM_DESTROY:
		Log = (TRACE_LOG_S *) GetWindowLong(hwnd, GWL_USERDATA);
		/* Deinitialize stuff here */
		return 0;
	default:
		/* Handle registered window messages like Find/Replace */
		;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

HICON MakeMySymbolIcon(void);
static void PopulateTraceLogWindowPos(char *Name, RECT *prc);

void DisplayTrace(char *Name, int nShowCmd, BOOL Enable)
{	TRACE_LOG_S *Log = GetTraceLog(Name);
static	BOOL First = TRUE;
	if (Log)
	{	if (Enable && !Log->Enabled)
		{	Log->Enabled = TRUE;
			TraceLog(Log->Name, FALSE, NULL, "Logging Enabled");
		}
		if (!Log->hwndLog)
		{	TCHAR Title[80];
#ifndef UNDER_CE
			RECT rc;
#endif
			if (First)
#ifndef UNDER_CE
			{	WNDCLASSEX logClass = {0};
				logClass.cbSize = sizeof(logClass);
				logClass.style = CS_HREDRAW | CS_VREDRAW;
				logClass.lpfnWndProc = LogWndProc;
				logClass.hInstance = g_hInstance;
//				if (LoadIconMetric(g_hInstance, MAKEINTRESOURCE(IDI_MY_ICON), LIM_SMALL, &logClass.hIcon) != S_OK)
				logClass.hIcon = MakeMySymbolIcon();
				if (!logClass.hIcon)
					logClass.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MY_ICON));
				if (!logClass.hIcon)
					logClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
				logClass.hCursor = LoadCursor(NULL, IDC_ARROW);
				logClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
				logClass.lpszMenuName = MAKEINTRESOURCE(IDM_LOG_W32);
				logClass.lpszClassName = g_szLogName;
				RegisterClassEx(&logClass);
				First = FALSE;
			}
#else
			{	WNDCLASS logClass = {0};
				logClass.style = CS_HREDRAW | CS_VREDRAW;
				logClass.lpfnWndProc = LogWndProc;
				logClass.hInstance = g_hInstance;
#ifndef UNDER_CE
				logClass.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MY_ICON));
				if (!logClass.hIcon)
				logClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
				logClass.hCursor = LoadCursor(NULL, IDC_ARROW);
				logClass.lpszMenuName = MAKEINTRESOURCE(IDM_LOG_W32);
#endif
				logClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
				logClass.lpszClassName = g_szLogName;
				RegisterClass(&logClass);
				First = FALSE;
			}
#endif

			StringCbPrintf(Title, sizeof(Title), TEXT("Trace(%S) - %S"), Log->Name, CALLSIGN);
#ifdef UNDER_CE
#ifdef CE50
			Log->hwndLog = CreateWindow(g_szLogName, Title,
										WS_NONAVDONEBUTTON | WS_OVERLAPPED | /* WS_BORDER | */WS_DLGFRAME | WS_SYSMENU | /*WS_THICKFRAME | */WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
										CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
										NULL, NULL, g_hInstance, Log);
//			if (Log->hwndLog)
//			{	HWND hwndCB = CommandBar_Create(g_hInstance, Log->hwndLog, 0xBEEE);
//				if (hwndCB)
//				{	if (!CommandBar_InsertMenubar(hwndCB, g_hInstance, IDM_LOG, 0))
//						TraceLogThread("Activity", TRUE, "InsertMenubar(%p) Failed with %ld\n", hwndCB, (long) GetLastError());
//					else TraceLogThread("Activity", TRUE, "InsertMenubar(%p) SUCCESS!\n", hwndCB);
//					CommandBar_AddAdornments (hwndCB, 0, 0); /*WM_HELP | CMDBAR_OK*/
//				} else TraceLogThread("Activity", TRUE, "CommandBar_Create Failed with %ld\n", (long) GetLastError());
//			}
#else
			Log->hwndLog = CreateWindow(g_szLogName, Title,
										/* WS_EX_CAPTIONOKBTN*/ /*WS_OVERLAPPED |*/ WS_SYSMENU,
										CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
										NULL, NULL, g_hInstance, Log);

#endif
#else
			PopulateTraceLogWindowPos(Log->Name, &rc);
			Log->hwndLog = CreateWindow(g_szLogName, Title,
										WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
										WS_OVERLAPPED | WS_SYSMENU,
										rc.left, rc.top, rc.right, rc.bottom,
										NULL, NULL, g_hInstance, Log);
#endif
			ShowWindow(Log->hwndLog, nShowCmd);
		} else
		{	ShowWindow(Log->hwndLog, SW_RESTORE);
			PostMessage(Log->hwndLog, WM_USER, 0, 0);	/* Fix En/Disable */
		}
		SetForegroundWindow(Log->hwndLog);
		UpdateWindow(Log->hwndLog);
		UnlockTraceLog(Log, __LINE__);
	}
}

void UnDisplayTrace(char *Name)
{	TRACE_LOG_S *Log = GetTraceLog(Name);
	if (Log)
	{	if (Log->hwndLog)
		{	DestroyWindow(Log->hwndLog);
			Log->hwndLog = NULL;
		}
	}
}

#endif	/* SUPPORT_TRACE_LOGS */

FILE *BeginEndOut = NULL;
FILE *TraceOut = NULL;
long TraceHits = 0;
DWORD BeginEndCount = 0;
HANDLE hmtxLog = NULL;
DWORD logOwner, logLockCount;

static void UnlockLog(FILE *Out, BOOL ForceIt = FALSE)
{	if (Out)
	{	if (Out != TraceOut)
			fclose(Out);
		else if (ForceIt || TraceHits > 100)
		{	fclose(Out);
			TraceOut = NULL;
			TraceHits = 0;
		}
		if (Out == BeginEndOut)
			BeginEndOut = NULL;
	}
	if (!--logLockCount) logOwner = 0;
	ReleaseMutex(hmtxLog);
}

static FILE *LockLog(HWND hwnd)
{	FILE *Out = NULL;
	if (!hmtxLog) hmtxLog = CreateMutex(NULL, FALSE, NULL);
	if (WaitForSingleObject(hmtxLog, 5000) == WAIT_OBJECT_0)
	{	logOwner = GetCurrentThreadId(); logLockCount++;
		if (BeginEndOut)	/* Left over from last time? */
			Out = BeginEndOut;
		else if (TraceOut)
		{	Out = TraceOut;
			TraceHits++;
		} else
		{	char *TrcFile = GetTraceFile(hwnd);
			if (TrcFile)
			{	TraceOut = Out = fopen(TrcFile,"a+t");
				if (!Out)
				{	char *Buffer = (char*) malloc(strlen(TrcFile)+80);
					UnlockLog(NULL);
					sprintf(Buffer, "LockLog:fopen(%s)", TrcFile);
					LockFailure(Buffer, logOwner, logLockCount, __LINE__);
					free(Buffer);
				}
			} else
			{	UnlockLog(NULL);
				LockFailure("LockLog:GetTraceFile(NULL)", logOwner, logLockCount, __LINE__);
			}
		}
	}
	else LockFailure("hmtxLog", logOwner, logLockCount, __LINE__);
	return Out;
}

void FlushLog(HWND hwnd)
{	if (TraceOut)
	{	FILE *Out = LockLog(hwnd);
		if (Out) UnlockLog(Out, TRUE);
	}
}

void ClearTraceFile(HWND hwnd)
{	int i;
	int Size = sizeof(TCHAR)*MAX_PATH;
	TCHAR *uOldFile = (TCHAR*)malloc(Size);
	TCHAR *uNewFile = (TCHAR*)malloc(Size);

	FlushLog(hwnd);	/* Force the file to be closed */
	for (i=9; i>=0; i--)
	{	char Ext[4];
		StringCbPrintfA(Ext,sizeof(Ext),"%02ld",(long)i);
		char *aOldFile = _strdup(GetTraceFile(hwnd, Ext));
		StringCbPrintf(uOldFile,Size,TEXT("%S"),aOldFile);
		StringCbPrintfA(Ext,sizeof(Ext),"%02ld",(long)i-1);
		char *aNewFile = _strdup(GetTraceFile(hwnd, i?Ext:""));
		StringCbPrintf(uNewFile,Size,TEXT("%S"),aNewFile);
#ifdef UNDER_CE
		DeleteFile(uOldFile);
		if (!MoveFile(uNewFile,uOldFile))
			DeleteFile(uNewFile);
#else
		if (!MoveFileEx(uNewFile,uOldFile,MOVEFILE_REPLACE_EXISTING))
		{	printf("Move %S to %S Failed with %ld\n", uNewFile, uOldFile, GetLastError());
			DeleteFile(uNewFile);
		} else printf("Move %S to %S Succeeded!\n", uNewFile, uOldFile);
#endif
		free(aNewFile); free(aOldFile);
	}
	free(uNewFile); free(uOldFile);
}

static void cdecl TraceActual(char *Name, BOOL ForceIt, FILE *Out, BOOL IncludeTime, char *Prefix, char *Format, va_list arg_ptr)
{
	if (!Out && !Name) return;	/* Don't even need to format */

	size_t Remaining = 4096;
	char *TraceBuf = (char *) malloc(Remaining);
	char *NoThread, *Next = TraceBuf;
	char atid[33];
	char *tName = GetTraceThreadName();
	if (!tName)
	{	StringCbPrintfA(atid, sizeof(atid), "Tid(%lX)", GetCurrentThreadId());
		tName = atid;
	}

	if (!TraceBuf)
	{	MessageBox(NULL, TEXT("TraceActual malloc(4096) FAILED!"), TEXT("TraceActual"), MB_ICONERROR | MB_OK);
		return;
	}

	StringCbPrintfExA(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
					"%s:", tName);
	NoThread = Next;	/* For skipping name into window */

	if (IncludeTime)
	{	SYSTEMTIME stUTCTime;
		GetSystemTime(&stUTCTime);

		StringCbPrintfExA(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
						"%04ld-%02ld-%02ldT%02ld:%02ld:%02ld ",
						(long) stUTCTime.wYear, 
						(long) stUTCTime.wMonth, 
						(long) stUTCTime.wDay, 
						(long) stUTCTime.wHour, 
						(long) stUTCTime.wMinute, 
						(long) stUTCTime.wSecond);
#ifdef UNDER_CE
		NoThread = Next-9;	/* Skip date on WinMo */
#else
		StringCbPrintfExA(Next-1, Remaining+1, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
				".%03ld ", (long) stUTCTime.wMilliseconds);
#endif
	}
	if (Prefix)
		StringCbPrintfExA(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
						"%s", Prefix);

	StringCbVPrintfExA(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS, Format, arg_ptr);
	TrimEnd(TraceBuf);
	if (Out)
	{	if (Name)
		{	fputs(Name, Out);
			fputs(":", Out);
		}
		fputs(TraceBuf, Out);
		fputs("\n", Out);
	}
#ifdef SUPPORT_TRACE_LOGS
	if (Name) TraceLogActual(Name, ForceIt, TraceBuf, NoThread);
#endif
	free(TraceBuf);
}

void cdecl TraceActivityBegin(HWND hwnd, char *Format, ...)
{	va_list arg_ptr;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
vprintf(Format, arg_ptr);
#endif

	if (ActiveConfig.Enables.DebugGeneral)
	{	BeginEndOut = LockLog(hwnd);
		if (BeginEndOut)
		{	BeginEndCount++;	/* Count the nesting */
			TraceActual("Activity", FALSE, BeginEndOut, TRUE, NULL, Format, arg_ptr);
			UnlockLog(NULL);	/* Leave BeginEndOut open */
		}
	}
	va_end(arg_ptr);
}

void cdecl TraceActivityEnd(HWND hwnd, char *Format, ...)
{	va_list arg_ptr;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
vprintf(Format, arg_ptr);
#endif

	if (ActiveConfig.Enables.DebugGeneral)
	{	FILE *Out = LockLog(hwnd);
		if (Out)
		{	TraceActual("Activity", FALSE, Out, FALSE, "                    ", Format, arg_ptr);
			if (--BeginEndCount) UnlockLog(NULL);	/* Don't close yet */
			else UnlockLog(Out);	/* Time to close it */
		}
	}
	va_end(arg_ptr);
}

void cdecl TraceActivity(HWND hwnd, char *Format, ...)
{	va_list arg_ptr;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
vprintf(Format, arg_ptr);
#endif

	if (ActiveConfig.Enables.DebugGeneral)
	{	FILE *Out = LockLog(hwnd);
		if (Out)
		{	if (Out != BeginEndOut)
			{	TraceActual("Activity", FALSE, Out, TRUE, NULL, Format, arg_ptr);
			} else TraceActual("Activity", FALSE, Out, FALSE, "                    ", Format, arg_ptr);
			if (Out == BeginEndOut) UnlockLog(NULL);
			else UnlockLog(Out);
		}
	}
	va_end(arg_ptr);
}

void cdecl TraceError(HWND hwnd, char *Format, ...)
{	va_list arg_ptr;
	FILE *Out;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
printf("***** ");
vprintf(Format, arg_ptr);
#endif

	Out = LockLog(hwnd);
	if (Out)
	{	TraceActual("Activity", TRUE, Out, TRUE, "***** ", Format, arg_ptr);
		if (Out == BeginEndOut) UnlockLog(NULL);
		else UnlockLog(Out);
	}
	va_end(arg_ptr);
}

void cdecl TraceLog(char *Name, BOOL ForceIt, HWND hwnd, char *Format, ...)
{	va_list arg_ptr;
	FILE *Out;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
if (ForceIt) printf("***** ");
vprintf(Format, arg_ptr);
#endif

	if (!Name) Name = GetDefaultTraceName();
	if (!Name) Name = "NULL";
	TRACE_LOG_S *Log = GetTraceLog(Name);

	if (Log)
	{	Out = ForceIt || (ActiveConfig.Enables.DebugFile&&(!Log||Log->Enabled))?LockLog(hwnd):NULL;
		TraceActual(Name, ForceIt, Out, TRUE, NULL, Format, arg_ptr);
		if (Log) UnlockTraceLog(Log,__LINE__);
		if (Out)
		{	if (Out == BeginEndOut) UnlockLog(NULL);
			else UnlockLog(Out);
		}
	}
	va_end(arg_ptr);
}

void cdecl TraceLogThread(char *Name, BOOL ForceIt, char *Format, ...)
{	va_list arg_ptr;
	FILE *Out;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
if (ForceIt) printf("***** ");
vprintf(Format, arg_ptr);
#endif

	if (!Name) Name = GetDefaultTraceName();
	TRACE_LOG_S *Log = GetTraceLog(Name);

	if (Log)
	{	Out = ForceIt || (ActiveConfig.Enables.DebugFile&&(!Log||Log->Enabled))?LockLog(NULL):NULL;
		TraceActual(Name, ForceIt, Out, TRUE, NULL, Format, arg_ptr);
		if (Log) UnlockTraceLog(Log,__LINE__);
		if (Out)
		{	if (Out == BeginEndOut) UnlockLog(NULL);
			else UnlockLog(Out);
		}
	}
	va_end(arg_ptr);
}

void cdecl TraceLogThreadWithTime(char *Name, BOOL ForceIt, char *Format, ...)
{	va_list arg_ptr;
	FILE *Out;

	va_start(arg_ptr, Format);
#ifndef UNDER_CE
if (ForceIt) printf("***** ");
vprintf(Format, arg_ptr);
#endif

	if (!Name) Name = GetDefaultTraceName();
	TRACE_LOG_S *Log = GetTraceLog(Name);

	if (Log)
	{	Out = ForceIt || (ActiveConfig.Enables.DebugFile&&(!Log||Log->Enabled))?LockLog(NULL):NULL;
		TraceActual(Name, ForceIt, Out, FALSE, NULL, Format, arg_ptr);
		if (Log) UnlockTraceLog(Log,__LINE__);
		if (Out)
		{	if (Out == BeginEndOut) UnlockLog(NULL);
			else UnlockLog(Out);
		}
	}
	va_end(arg_ptr);
}

void TraceLogUpdateCallsign(void)
{	TCHAR *szTitle = (TCHAR *)malloc(128);
	unsigned long l;

	if (hwndLocks)
	{	SendMessage(hwndLocks, WM_CLOSE, 0, 0);
		if (!hwndLocks) CreateLockWindow();
	}

	for (l=0; l<LogCount; l++)
	if (Logs[l]->hwndLog)
	{	StringCbPrintf(szTitle, sizeof(*szTitle)*128, TEXT("Trace(%S) - %S"), Logs[l]->Name, CALLSIGN);
		SetWindowText(Logs[l]->hwndLog, szTitle);
	}

	free(szTitle);
}

char *GetTraceWindowName(HWND hwnd)
{
static char Buffer[80];
	if (hwnd == hwndLocks) return "Locks";
	else
	{	unsigned long i;
		for (i=0; i<LogCount; i++)
		{	if (hwnd == Logs[i]->hwndLog)
			{	sprintf(Buffer,"Log(%s)",Logs[i]->Name);
				return Buffer;
			} else if (hwnd == Logs[i]->hwndEdit)
			{	sprintf(Buffer,"Edit(%s)",Logs[i]->Name);
				return Buffer;
			}
		}
	}
	return NULL;
}

int TraceLogPopupMenuBuild(HWND hwnd, HMENU hPopup, BOOL AddSeparator)
{	int Added = 0;

	if (LogCount)		/* Don't do unless we have some */
	{	unsigned long l;
		unsigned long Enabled=0;

		if (AddSeparator)
			if (!AppendMenu(hPopup, MF_SEPARATOR, NULL, NULL))
				TraceError(hwnd,"Append Separator Failed with %ld!\n", GetLastError());
	
#ifdef OLD_WAY
		for (l=0; l<LogCount; l++)
		{	TCHAR Name[80];
			StringCbPrintf(Name, sizeof(Name), TEXT("%S"), Logs[l]->Name);
			AppendMenu(hPopup, ((Logs[l]->Enabled||Logs[l]->hwndLog)?MF_CHECKED:0) | MF_STRING, ID_LOG_ALL+(l+1), Name);
			Added++;
		}
#else

		SortTraceLogs();

		for (l=0; l<LogCount; l++)
		if (Logs[l]->Enabled || Logs[l]->hwndLog)
		{	TCHAR Name[80];
			StringCbPrintf(Name, sizeof(Name), TEXT("%S"), Logs[l]->Name);
			AppendMenu(hPopup, ((Logs[l]->Enabled||Logs[l]->hwndLog)?MF_CHECKED:0) | MF_STRING, ID_LOG_ALL+(l+1), Name);
			Added++;
		}
		if (Added < (int) LogCount)
		{	HMENU hSub = CreatePopupMenu();
			for (l=0; l<LogCount; l++)
			if (!Logs[l]->Enabled && !Logs[l]->hwndLog)
			{	TCHAR Name[80];
				StringCbPrintf(Name, sizeof(Name), TEXT("%S"), Logs[l]->Name);
				AppendMenu(hSub, ((Logs[l]->Enabled||Logs[l]->hwndLog)?MF_CHECKED:0) | MF_STRING, ID_LOG_ALL+(l+1), Name);
			}
			AppendMenu(hPopup, MF_POPUP | MF_STRING,
				(UINT_PTR) hSub, Added?TEXT("More Logs"):TEXT("Trace Logs"));
			Added++;
		}
#endif

	}
	return Added;
}

void TraceLogPopupMenuOption(unsigned long id)
{	if (id > ID_LOG_ALL && id <= ID_LOG_ALL+LogCount)
	{	id -= ID_LOG_ALL+1;
		if (id>=0 && id<LogCount)
		{	ShowTraceLog(Logs[id]->Name, FALSE);	// Was TRUE
		}
	}
}

void UpdateTraceLogViewMenu(HWND hwnd, HMENU hmenu, int cMenuItems)
{	unsigned long l;

	for (l=cMenuItems-1; l>=1; l--)	/* 1 original elements */
		DeleteMenu(hmenu, l, MF_BYPOSITION);
	cMenuItems = 1;	/* So we repopulate all of the logs */

	SortTraceLogs();

	for (l=cMenuItems-1; l<LogCount; l++)
	{	TCHAR Name[80];
#ifdef DEBUG_LEAKS
		StringCbPrintf(Name, sizeof(Name), TEXT("%S-%ld"), Logs[l]->Name, (long) Logs[l]->Which);
#else
		StringCbPrintf(Name, sizeof(Name), TEXT("%S"), Logs[l]->Name);
#endif
		AppendMenu(hmenu, MF_STRING, ID_LOG_ALL+(l+1), Name);
	}
	for (l=0; l<LogCount; l++)
	{	CheckMenuItem(hmenu, ID_LOG_ALL+(l+1), Logs[l]->hwndLog?MF_CHECKED:MF_UNCHECKED);
	}
}
void ProcessTraceLogViewMenu(UINT id)
{
	if (id > ID_LOG_ALL && id <= ID_LOG_ALL+LogCount)
	{	id -= ID_LOG_ALL+1;
		if (id>=0 && id<LogCount)
		{	ShowTraceLog(Logs[id]->Name, FALSE);	// Was TRUE
		}
	}
}

void UpdateTraceLogEnableMenu(HWND hwnd, HMENU hmenu, int cMenuItems)
{	unsigned long l;

	for (l=cMenuItems-1; l>=5; l--)	/* 5 original elements */
		DeleteMenu(hmenu, l, MF_BYPOSITION);
	cMenuItems = 5;	/* So we repopulate all of the logs */

	SortTraceLogs();

	for (l=cMenuItems-5; l<LogCount; l++)	/* Sensitive to menu pre-population */
	{	TCHAR Name[80];
#ifdef DEBUG_LEAKS
		StringCbPrintf(Name, sizeof(Name), TEXT("%S-%ld"), Logs[l]->Name, (long) Logs[l]->Which);
#else
		StringCbPrintf(Name, sizeof(Name), TEXT("%S"), Logs[l]->Name);
#endif
		AppendMenu(hmenu, MF_STRING, ID_LOG_STARTUP+(l+1), Name);
	}

	for (l=0; l<LogCount; l++)
	{	CheckMenuItem(hmenu, ID_LOG_STARTUP+(l+1), (Logs[l]->Enabled?MF_CHECKED:MF_UNCHECKED));
	}
}
void ProcessTraceLogEnableMenu(UINT id)
{
	if (id > ID_LOG_STARTUP && id <= ID_LOG_STARTUP+LogCount)
	{	id -= ID_LOG_STARTUP+1;
		if (id>=0 && id<LogCount)
		{	Logs[id]->Enabled = !Logs[id]->Enabled;
			TraceLogThread(Logs[id]->Name, TRUE, "Logging %s", Logs[id]->Enabled?"Enabled":"Disabled");
#ifndef UNDER_CE
			if (Logs[id]->Enabled)
				ShowTraceLog(Logs[id]->Name);
#endif
		}
	}
}

static void PopulateTraceLogWindowPos(char *Name, RECT *prc)
{	TIMED_STRING_LIST_S *pList = &ActiveConfig.TraceLogsPos;
	unsigned long i = LocateColonStringEntry(pList, Name);

	SetRect(prc, CW_USEDEFAULT, CW_USEDEFAULT,
					CW_USEDEFAULT, CW_USEDEFAULT);
	if (i != -1)
	{	char *string = pList->Entries[i].string;
		char *c = strchr(string,':');	/* end of name */
		if (c && *c)
		{	char *e;
			long z = strtol(c+1,&e,10);	/* And retrieve the zoom factor */
			if (e && *e=='@')
			{	RECT rc;
				rc.left = strtol(e+1,&e,10);
				if (e && *e==',')
				{	rc.top = strtol(e+1,&e,10);
					if (e && *e=='[')
					{	rc.right = rc.left + strtol(e+1,&e,10);
						if (e && *e==',')
						{	rc.bottom = rc.top + strtol(e+1,&e,10);
							if (e && *e==']')
							{	ClipOrCenterRectToMonitor(&rc,
											MONITOR_CLIP | MONITOR_WORKAREA);
								SetRect(prc, rc.left, rc.top,
											rc.right-rc.left,
											rc.bottom-rc.top);
							}
						}
					}
				}
			}
		}
	}
}

static void SaveLogWindow(TRACE_LOG_S *Log)
{
#ifndef UNDER_CE
	if (!IsRectEmpty(&Log->rcSize))
	{	TIMED_STRING_LIST_S *pList = &ActiveConfig.TraceLogsPos;
		size_t TempLen = strlen(Log->Name)+6*(1+33)+1;
		char *TempBuf = (char*)malloc(TempLen);
		StringCbPrintfA(TempBuf, TempLen,
							"%s:%ld@%ld,%ld[%ld,%ld]",
							Log->Name, Log->Enabled,
							Log->rcSize.left, Log->rcSize.top,
							Log->rcSize.right-Log->rcSize.left,
							Log->rcSize.bottom-Log->rcSize.top);
		AddOrUpdateColonStringEntry(pList, TempBuf, NULL);
		free(TempBuf);
	}
#endif
}

void CloseTraceLogs(HWND hwnd, BOOL Quiet)
{
	TraceError(hwnd, "CloseTraceLogs %ld TraceLogs\n", LogCount);

	if (LogCount)
	{	STRING_LIST_S *pList = &ActiveConfig.TraceLogs;
		size_t Count = 0;
		size_t Len = 0;

		SortTraceLogs();

		for (size_t l=0; l<LogCount; l++)
		{	unsigned long p = LocateSimpleStringEntry(&ActiveConfig.AlwaysTraceLogs, Logs[l]->Name);
			if (p != -1)
			{	if (!Logs[l]->Enabled)	/* See if we should drop this one */
					RemoveSimpleStringEntry(&ActiveConfig.AlwaysTraceLogs, p);
			} else	/* New one? */
			if (Logs[l]->Enabled)							/* And enabled? */
			{	Len += strlen(Logs[l]->Name);
				Count++;
			}
		}
		if (Count)
		{	int SaveThem = IDYES;
//#ifdef UNDER_CE
			size_t Remaining = sizeof(TCHAR) * (Len + Count*32 + 80);
			TCHAR *Buffer = (TCHAR *)malloc(Remaining);
			TCHAR *Next = Buffer;
			StringCbPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
						TEXT("Save Enable Status For:\n\n"));
			for (size_t l=0; l<LogCount; l++)
			{	if (Logs[l]->Enabled)
				{	unsigned long p = LocateSimpleStringEntry(&ActiveConfig.AlwaysTraceLogs, Logs[l]->Name);
					if (p == -1)
						StringCbPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
									TEXT("%S "), Logs[l]->Name);
				}
			}
			StringCbPrintfEx(Next, Remaining, &Next, &Remaining, STRSAFE_IGNORE_NULLS,
							TEXT("\n\nAcross Restart?"));

			BUTTONS_S *Buttons = CreateButtons(-1);
			AddButton(Buttons, "Yes", IDYES);
			AddButton(Buttons, "Always", IDRETRY);
			AddButton(Buttons, "No", IDNO);
			if (Quiet) SaveThem = IDYES;
			else SaveThem = LwdMessageBox2(hwnd, Buffer, TEXT("WM_CLOSE"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON3, Buttons);

	//		SaveThem = MessageBox(hwnd, Buffer, TEXT("WM_CLOSE"), MB_ICONQUESTION | MB_YESNO);
			free(Buffer);

			if (SaveThem == IDRETRY)	/* Upgrade to Always */
			{	STRING_LIST_S *aList = &ActiveConfig.AlwaysTraceLogs;
				EmptySimpleStringList(aList);
				for (size_t l=0; l<LogCount; l++)
				{	if (Logs[l]->Enabled)
					{	AddSimpleStringEntry(aList, Logs[l]->Name);
					}
				}
				EmptySimpleStringList(pList);	/* Not saving as pList */
				TraceError(hwnd, "WM_CLOSE Saved %ld Always Enabled TraceLogs\n", aList->Count);
			} else
//#endif
			if (SaveThem == IDYES)
			{	EmptySimpleStringList(pList);
				for (size_t l=0; l<LogCount; l++)
				{	if (Logs[l]->Enabled)
					{	unsigned long p = LocateSimpleStringEntry(&ActiveConfig.AlwaysTraceLogs, Logs[l]->Name);
						if (p == -1)
							AddSimpleStringEntry(pList, Logs[l]->Name);
					}
				}
				TraceError(hwnd, "WM_CLOSE Saved %ld Enabled TraceLogs\n", pList->Count);
			} else EmptySimpleStringList(pList);	/* Didn't say Yes */
		} else EmptySimpleStringList(pList);	/* None enabled */
#ifndef UNDER_CE
		for (size_t l=0; l<LogCount; l++)
			SaveLogWindow(Logs[l]);
#endif
	}
}
