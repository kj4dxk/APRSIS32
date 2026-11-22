#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UNDER_CE
#define SUPPORT_TRACE_LOGS
#else
#define SUPPORT_TRACE_LOGS
#endif

typedef struct TRACE_LINE_S
{	__int64 When;
	char *What;
	unsigned short Len;
#ifdef DEBUG_LEAKS
	unsigned short Which;
#endif
} TRACE_LINE_S;

typedef struct TRACE_LOG_S
{	char *Name;
#ifdef DEBUG_LEAKS
	unsigned short Which;
#endif
	HANDLE hmtx;
	DWORD Owner, LastOwner, LockCount, LockMax, TotalLocks;
	BOOL Enabled;
	BOOL SuppressThread;
	HWND hwndLog, hwndEdit, hwndCB;
	HMENU hMenu;
	int MenuOffset;
#ifndef UNDER_CE
	RECT rcSize;
#endif
	BOOL isFlashing;
	__int64 FirstWhen, LastWhen, UpdateWhen;
	unsigned long Count, UpdateCount, Size, Longest;
	unsigned long TotalSize;
	TRACE_LINE_S **Lines;
} TRACE_LOG_S;

typedef struct TRACE_THREAD_INFO_S
{	DWORD ThreadID;
	char *ThreadName;
	char DefaultName[80];
	char *DefaultLogName;
	char *LastLogName;
	TRACE_LOG_S *LastLog;
} TRACE_THREAD_INFO_S;

#ifndef TLS_OUT_OF_INDEXES
#define TLS_OUT_OF_INDEXES -1
#endif

char *GetTraceLogStats(BOOL Detailed);

void SetDefaultTraceName(char *Name);

void SetTraceThreadName(char *Name);
char *GetThreadNameByID(DWORD tID);
BOOL GetThreadIDs(DWORD **IDs, unsigned long *IDCount);

void cdecl TraceActivityBegin(HWND hwnd, char *Format, ...);
void cdecl TraceActivityEnd(HWND hwnd, char *Format, ...);
void cdecl TraceActivity(HWND hwnd, char *Format, ...);
void cdecl TraceError(HWND hwnd, char *Format, ...);
void cdecl TraceLog(char *Name, BOOL ForceIt, HWND hwnd, char *Format, ...);
void cdecl TraceLogThread(char *Name, BOOL ForceIt, char *Format, ...);
void cdecl TraceLogThreadWithTime(char *Name, BOOL ForceIt, char *Format, ...);

#ifndef __cplusplus
void ShowTraceLog(char *Name, BOOL Enable);	/* NULL for all */
BOOL IsTraceLogEnabled(char *Name);
#else
char *GetTraceFile(HWND hwnd, char *Ext = "");
void CreateLockWindow(void);
void ShowTraceLog(char *Name, BOOL Enable=FALSE);	/* NULL for all */
void ClearTraceLog(char *Name);
void UnShowTraceLog(char *Name);	/* NULL for all */
void PopupTraceLogs(HWND hwnd);
BOOL LockTraceLog(TRACE_LOG_S *Log);
void UnlockTraceLog(TRACE_LOG_S *Log, long Line);
void SortTraceLogs(void);
TRACE_LOG_S *GetTraceLog(char *Name);
#ifdef UNDER_CE
void PurgeTraceLog(TRACE_LOG_S *Log, long MaxMinutes=7, long KeepMinutes=5, long MinMinutes=2, unsigned long MaxKB=512, long unsigned MinKB = 128);
#else
void PurgeTraceLog(TRACE_LOG_S *Log, long MaxMinutes=15, long KeepMinutes=10, long MinMinutes=5, unsigned long MaxKB=768, long unsigned MinKB = 512);
#endif
void PurgeTraceLogs(long MaxMinutes=3, long KeepMinutes=2, long MinMinutes=1, unsigned long MaxKB=8, long unsigned MinKB = 4);
void TraceLogActual(char *Name, BOOL ForceIt, char *Text, char *NoThreadText);
void DisplayTrace(char *Name, int nShowCmd, BOOL Enable=FALSE);
void UnDisplayTrace(char *Name);

BOOL IsTraceLogEnabled(char *Name);
void FlushLog(HWND hwnd);
void ClearTraceFile(HWND hwnd);
void TraceLogUpdateCallsign(void);
char *GetTraceWindowName(HWND hwnd);
int TraceLogPopupMenuBuild(HWND hwnd, HMENU hPopup, BOOL AddSeparator);
void TraceLogPopupMenuOption(unsigned long id);
void UpdateTraceLogViewMenu(HWND hwnd, HMENU hmenu, int cMenuItems);
void UpdateTraceLogEnableMenu(HWND hwnd, HMENU hmenu, int cMenuItems);
void ProcessTraceLogEnableMenu(UINT id);
void ProcessTraceLogViewMenu(UINT id);
void CloseTraceLogs(HWND hwnd, BOOL Quiet);
#endif

#ifdef __cplusplus
}
#endif
