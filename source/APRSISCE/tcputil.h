/* Prototypes generated Thu Jul 31 14:17:02 2008 */
#ifdef __cplusplus
extern "C"
{
#endif

// helper macros
#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (UINT_PTR)(sizeof(a)/sizeof((a)[0]))
#endif

#ifndef STRING
#define STRING(s) sizeof(s),s
#endif

#ifdef UNDER_CE
#ifndef RDW_FRAME
#define RDW_FRAME 0
#endif
#define IsIconic(hwnd) FALSE
#ifndef MF_DISABLED
#define MF_DISABLED MF_GRAYED
#endif
#endif

void cdecl TraceActivityBegin(HWND hwnd, char *Format, ...);
void cdecl TraceActivityEnd(HWND hwnd, char *Format, ...);
void cdecl TraceActivity(HWND hwnd, char *Format, ...);
void cdecl TraceError(HWND hwnd, char *Format, ...);
void cdecl TraceLog(char *Name, BOOL ForceIt, HWND hwnd, char *Format, ...);
void cdecl TraceLogThread(char *Name, BOOL ForceIt, char *Format, ...);
#define DEBUG_TRIGGER 6000
#define MESSAGE_TRIGGER DEBUG_TRIGGER
__int64 cdecl cDebugTimer(char *Routine, char *Text, __int64 Start, __int64 *pMax, _int64 *pLast, HWND hwnd, long msTrigger);

__int64 llGetMsec(void);
__int64 llMsecSince(__int64 msStart, __int64 msNow);
#ifdef OBSOLETE
double RtGetMsec(void);
double MSecSince(double msStart, double msNow);
#endif
void settcpstatus(char *newstat, BOOL error);
char *gettcpstatus(void);
TCHAR *wgettcpstatus(void);
void tcp_disable_messages(void);
void tcp_enable_messages(void);
#ifdef OBSOLETE
void tcp_set_port(int new_port);
void tcp_set_host(char *new_host);
void tcp_set_host_port(char *new_host, int new_port);
char *tcp_get_host_port(void);
int tcp_connect(int CheckOnly);
void tcp_disconnect(void);
int tcp_transmit_buffer( unsigned int length, unsigned char *buffer, signed int timeout );
int tcp_receive_buffer( unsigned short length, unsigned char *buffer, signed short timeout, unsigned short *retlength);
#endif
int tcp_send_udp(char *To, int Port, int Len, char *Buffer, int Attempts);
void tcp_get_comm_stats(long *pAttempts, long *pConnects, long *bSent, long *bRecv);
char *Finger(HWND hWnd, char *Host, long Port, char *Username, int *pLen);
#ifdef __cplusplus
}
#endif
