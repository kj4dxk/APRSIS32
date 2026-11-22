#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <windows.h>
#include <winsock.h>
#include <strsafe.h>

#include "tcputil.h"

#define sock_init() 							\
{	WORD wVersionRequested = MAKEWORD(1,1);				\
	WSADATA wsaData;						\
	int err = WSAStartup(wVersionRequested, &wsaData);		\
	if (err != 0)							\
	{	/*printf("WSAStartup Failed With %ld\n", (long) err);*/	\
		exit(-1);						\
	}								\
}
#define soclose(s) closesocket(s)
#define ioctl(s) ioctlsocket(s)
#define psock_errno(s) printf("%s errno %ld\n", s, (long) h_errno)
#define sock_errno() h_errno

char *tcpstatus = "Not Initialized";

void settcpstatus(char *newstat, BOOL Error)
{	if (tcpstatus != newstat)
	{	if (Error)
			TraceLog("TCP", TRUE, NULL, "settcpstatus(%s)\n", newstat);
		else TraceLog("TCP", FALSE, NULL, "settcpstatus(%s)\n", newstat);
	}
	tcpstatus = newstat;
}

char *gettcpstatus(void)
{	return tcpstatus;
}

TCHAR *wgettcpstatus(void)
{static	TCHAR Buffer[80];
	StringCbPrintf(Buffer, sizeof(Buffer), TEXT("%S"), gettcpstatus());
return Buffer;
}

#ifdef UNDER_CE
#define strdup(s) strncpy(malloc(strlen(s)+1),s,strlen(s)+1)
#endif

__int64 llGetMsec(void)
{
	LARGE_INTEGER Now = {0};
static	int First = 1;
static	LARGE_INTEGER Freq, Last;
static	double dFreq=0, dLast = 0;
	if (First)
	{	if (!QueryPerformanceFrequency(&Freq))
		{	MessageBox(NULL, TEXT("No High-Resolution Performance Counter"), TEXT("QueryPerformanceFrequency"), MB_OK | MB_ICONWARNING);
			Freq.QuadPart = 1;
		} else Freq.QuadPart /= 1000;
		First = 0;
//		TraceLogThread("CPU", TRUE, "Frequency %I64d\n", (__int64) Freq.QuadPart);
	}
	if (!QueryPerformanceCounter(&Now))
	{	return 0;
	}
	if (Now.QuadPart < Last.QuadPart)
	{	Now.QuadPart = ++Last.QuadPart;
	} else Last = Now;
//	TraceLogThread("CPU", TRUE, "Now %I64d ms %I64d\n", (__int64) Now.QuadPart, (__int64) (Now.QuadPart/Freq.QuadPart));
	return Now.QuadPart / Freq.QuadPart + (__int64)365*(__int64)(24*60*60*1000);
}

__int64 llMsecSince(__int64 msStart, __int64 msNow)
{	__int64 Result = msNow - msStart;
	if (Result < 0) Result = 0;
	return Result;
}
#ifdef OBSOLETE
double RtGetMsec(void)
{
#define C 4.294967296e9
#define Li2Double(x) ((double)((x).HighPart)*C+(double)((x).LowPart))
	double dNow;
	LARGE_INTEGER Now = {0};
static	int First = 1;
static	LARGE_INTEGER Freq;
static	double dFreq=0, dLast = 0;
	if (First)
	{	if (!QueryPerformanceFrequency(&Freq))
		{	MessageBox(NULL, TEXT("No High-Resolution Performance Counter"), TEXT("QueryPerformanceFrequency"), MB_OK | MB_ICONWARNING);
			dFreq = 1.0;
		} else
		{	dFreq = Li2Double(Freq) / 1000.0;
		}
		First = 0;
	}
	if (!QueryPerformanceCounter(&Now))
	{	return 0.0;
	}
	dNow = Li2Double(Now);
	if (dNow < dLast)
	{	dNow = ++dLast;
	} else dLast = dNow;
	return dNow / dFreq;
}

double MSecSince(double msStart, double msNow)
{	double Result = msNow - msStart;
	if (Result < 0) Result = 0;
	return Result;
}
#endif

#include "tcputil.h"

#define DISCONNECTED -1

#define SLOW_CONNECT	 // Define to throttle connects to every 15 seconds */

static int mysocket = DISCONNECTED;
static int changed = 1;
static char *host = NULL;
static int port = 0;
static char *actual = NULL;
static struct sockaddr_in server = {0};
static int MsgsDisabled = 1;

static unsigned char *SentBuffer = NULL;
static unsigned int SentLength = 0;
static double SentTime = 0;

static	unsigned short pendcount = 0;
static	unsigned short recvlen = 0;
static	char *recvbuf = NULL;

long ConnectAttempts=0, Connects=0, BytesSent=0, BytesRecv=0;

void tcp_get_comm_stats(long *pAttempts, long *pConnects, long *bSent, long *bRecv)
{
	if (pAttempts) *pAttempts = ConnectAttempts;
	if (pConnects) *pConnects = Connects;
	if (bSent) *bSent = BytesSent;
	if (bRecv) *bRecv = BytesRecv;
}

/*:tcp_status_msg

	This routine will place a message on the top line of the display
	after erasing the first 20 characters.  Note that messages displayed
	with this routine are truncated to 20 characters.
*/
static void tcp_status_msg(TCHAR *Msg, BOOL Error)
{
	if (Error) 	TraceLog("APRS-IS", TRUE, NULL, "tcp_status_msg(%S)\n", Msg);
	if (MsgsDisabled) return;
	MessageBox(NULL, Msg, TEXT("tcp_status"),
				MB_OK | MB_ICONINFORMATION);
}

/*:tcp_disable_messages

	This routine will disable TCP/IP status messages.  By default, these
	messages are enabled.
*/
void tcp_disable_messages(void)
{
	MsgsDisabled = 1;
}

/*:tcp_enable_messages

	This routine will re-enabled TCP/IP status messages.
*/
void tcp_enable_messages(void)
{
	MsgsDisabled = 0;
}

#ifdef OBSOLETE

/*:tcp_set_port

	This routine sets the port to which TCP/IP connections should be
	established.  This routine must be invoked prior to attempting any
	transmits or receives.
*/
void tcp_set_port(int new_port)
{	port = new_port;
	changed = 1;
}

/*:tcp_set_host

	This routine sets the host to which TCP/IP connections should be
	established.  This routine must be invoked prior to attempting any
	transmits or receives.  The host can be specified as a dot-delimited
	Internet address or hostname.
*/
void tcp_set_host(char *new_host)
{	if (host) free(host);
	host = _strdup(new_host);
	changed = 1;
}

/*:tcp_set_host_port

	This routine acts as a single insulation routine for tcp_set_port
	and tcp_set_host.
*/
void tcp_set_host_port(char *new_host, int new_port)
{
	tcp_set_host(new_host);
	tcp_set_port(new_port);
}

char *tcp_get_host_port(void)
{	return actual;
}

/*:tcp_connect

	This routine establishes a connection to the configured TCP/IP host
	and port if a connection does not already exist.  It is automatically
	invoked internally if a connection is required for transmitting or
	receiving.  TRUE is returned if a connection exists.
*/
int tcp_connect(int CheckOnly)
{	DWORD Status;
static HANDLE hmtx;
static int initialized = 0;

#ifdef SLOW_CONNECT
static	double NextConnect = 0.0;
#endif

	if (mysocket != DISCONNECTED) return 1;
	if (CheckOnly) return 0;
//	TraceLog("APRS-IS", TRUE, NULL, "tcp_connect(%ld) Socket(%ld)\n", (long) CheckOnly, (long) mysocket);
	ConnectAttempts++;
	if (!initialized)
	{	sock_init();
		hmtx = CreateMutex(NULL, TRUE, NULL);
		initialized = 1;
	} else
	do
	{	Status = WaitForSingleObject(hmtx, 10000);
		if (Status == WAIT_ABANDONED)
			tcp_status_msg(TEXT("WAIT Abandoned"), TRUE);
		else if (Status == WAIT_TIMEOUT)
			tcp_status_msg(TEXT("WAIT Timeout"), TRUE);
		else if (Status == WAIT_FAILED)
		{	DWORD e = GetLastError();
			TCHAR Text[80];
			StringCbPrintf(Text,sizeof(Text),TEXT("Wait FAILED %ld (0x%lX)"), (long) e, (long) e);
			tcp_status_msg(Text, TRUE);
		} else if (Status == WAIT_OBJECT_0)
#ifdef VERBOSE
			tcp_status_msg(TEXT("WAIT Object 0"),FALSE);
#else
			;
#endif
		else
		{	TCHAR Text[80];
			StringCbPrintf(Text,sizeof(Text),TEXT("Wait Returned %ld"), (long) Status);
			tcp_status_msg(Text, TRUE);
		}
	} while (Status == WAIT_TIMEOUT);

#ifdef SLOW_CONNECT
	if (RtGetMsec() < NextConnect)
	{	TCHAR Text[80];
		StringCbPrintf(Text,sizeof(Text),TEXT("Delay Connect For %ld mSec"), (long) (NextConnect-RtGetMsec()));
		tcp_status_msg(Text, FALSE);
		ReleaseMutex(hmtx);
		settcpstatus("Delay Connect",TRUE);
		return 0;
	}
	NextConnect = RtGetMsec();
	if (NextConnect) NextConnect += 15000; /* Only attempt every 15 seconds! */
#endif

	if (changed)
	{	if (!port || !host)
		{
			ReleaseMutex(hmtx);
			settcpstatus("No Host/Port Config",TRUE);
			return 0;
		}
tcp_status_msg(TEXT("Resolving..."),FALSE);
		server.sin_family      = AF_INET;
		server.sin_port        = htons((u_short)port);
		server.sin_addr.s_addr = inet_addr(host);	/* argv[1] = hostname */
		if (server.sin_addr.s_addr == (u_long) -1)
		{	struct hostent *hostnm = gethostbyname(host);
			if (!hostnm)
			{
				ReleaseMutex(hmtx);
				settcpstatus("No Host DNS",TRUE);
				return 0;
			}
			server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
		}
	}

	{	char Buffer[128];
		char *ipaddr = inet_ntoa(server.sin_addr);
		StringCbPrintfA(Buffer, sizeof(Buffer), "Resolved(%s)=>%s",host,ipaddr?ipaddr:"NULL");
		settcpstatus(Buffer,TRUE);
		if (actual) free(actual);
		actual = malloc(strlen(ipaddr)+33+1);
		StringCbPrintfA(actual,strlen(ipaddr)+33+1,"%s:%ld",ipaddr,(long)port);
	}

	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{	mysocket = DISCONNECTED;
		ReleaseMutex(hmtx);
		settcpstatus("No Socket",TRUE);
		return 0;
	}

tcp_status_msg(TEXT("Connecting..."),FALSE);
	if (connect(mysocket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{	soclose(mysocket);
		mysocket = DISCONNECTED;
tcp_status_msg(TEXT("Connect Failed"),FALSE);
		ReleaseMutex(hmtx);
		settcpstatus("Connect Failed",TRUE);
		return 0;
	}
tcp_status_msg(TEXT("Connected"),FALSE);
	settcpstatus("Connected",FALSE);

	Connects++;

	pendcount = 0;

	{	BOOL NoDelay = 1;	/* Disable Nagle algorithm for one-way sockets */
		if (setsockopt(mysocket, IPPROTO_TCP, TCP_NODELAY, (char*)&NoDelay, sizeof(NoDelay)))
			tcp_status_msg(TEXT("setsockopt FAIL"),TRUE);
	}

#ifdef SET_SNDBUF_TO_ZERO
{	int SndBuf;
	int SndLen;
	SndBuf = 0;
	if (setsockopt(mysocket, SOL_SOCKET, SO_SNDBUF, (char *) &SndBuf, sizeof(SndBuf)))
	{	DWORD e = WSAGetLastError();
		TCHAR Text[80];
		wsprintf(Text,TEXT("setsockopt Error %ld"), (long) e);
		tcp_status_msg(Text,TRUE);
	}
	SndLen = sizeof(SndBuf);
	if (getsockopt(mysocket, SOL_SOCKET, SO_SNDBUF, (char *) &SndBuf, &SndLen))
	{	DWORD e = WSAGetLastError();
		TCHAR Text[80];
		wsprintf(Text,TEXT("getsockopt Error %ld"), (long) e);
		tcp_status_msg(Text,TRUE);
	} else
	{	TCHAR Text[80];
		wsprintf(Text,TEXT("SndBuf = %ld"), (long) SndBuf);
		tcp_status_msg(Text, FALSE);
	}
}
#endif

if (SentBuffer)	/* We may want a time limit on this? */
	{	int n;
		TCHAR Text[4096];
		StringCbPrintf(Text,sizeof(Text),TEXT("ReSending After %ld msec"), (long) (SentTime?(RtGetMsec()-SentTime):0x7fffffff));
		tcp_status_msg(Text,FALSE);
#ifdef VERBOSE
		wsprintf(Text,TEXT("ReSending %.*hs"), (int) SentLength, SentBuffer);
		tcp_status_msg(Text,FALSE);
#endif
		BytesSent += SentLength;
		if ((n=send(mysocket, SentBuffer, SentLength, 0)) != (int)SentLength)
		{	tcp_disconnect();
tcp_status_msg(TEXT("re-send Failed"),TRUE);
			ReleaseMutex(hmtx);
			return 0;
		}
	}

	ReleaseMutex(hmtx);
	return 1;
}

/*:tcp_disconnect

	This routine disconnects any active connection.  It is automatically
	invoked in the event of any errors.  It can be invoked directly to
	force the connection to be reset at any time.
*/
void tcp_disconnect(void)
{
	if (mysocket == DISCONNECTED) return;
tcp_status_msg(TEXT("Lost Connection..."),TRUE);
	soclose(mysocket);
	mysocket = DISCONNECTED;
	pendcount = 0;
}

/*:tcp_transmit_buffer

	This routine transmits the specified buffer contents to the configured
	TCP/IP host and port.  A connection is automatically established if
	one does not exist.

	Successful completion of this routine does not necessarily mean that
	the data has been delivered to the host, only that the local TCP/IP
	stack has accepted the data for transmission.

	This routine automatically appends a newline ('\n') to the transmitted
	buffer to allow packet parsing on the receiving end of the connection.
	The buffer is also prefixed with the configured radio id in Intermec
	format (mA -> p5).

	0 is returned upon successful completion.  Other possible
	error returns are listed below.

<pre>
	 1 - Timeout
	 2 - Connection could not be established or was lost
	 3 - Buffer length is zero
	 4 - Select error
</pre>
*/ 
int tcp_transmit_buffer
(	unsigned int length,
	unsigned char *buffer,
	signed int timeout	/* In MilliSeconds */
)
{
static	unsigned short sendmax = 0;
static	char *sendbuf = NULL;
	int n, r;
	fd_set fds;
	struct timeval tv;

	if (!tcp_connect(1)) return 2;	/* protocol not active */
	if (!length) return 3;		/* zero length buffer */

	if (length+1 > sendmax)
	{	if (sendbuf) free(sendbuf);
		sendmax = length+1;
		sendbuf = malloc(sendmax);
	}
	memcpy(sendbuf,buffer,length);
	sendbuf[length] = '\n';
	length += 1;

	if (SentBuffer) free(SentBuffer);
	SentLength = length;
	SentBuffer = malloc(SentLength);
	memcpy(SentBuffer,sendbuf,SentLength);
	SentTime = RtGetMsec();

	FD_ZERO(&fds);
	FD_SET(mysocket, &fds);
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout%1000)*1000;

	r = select(mysocket+1, NULL, &fds, NULL, &tv);
	if (r == -1) return 4;
	if (r == 0) return 1;
	if (!FD_ISSET(mysocket, &fds)) return 1;

	BytesSent += length;
	if ((n=send(mysocket, sendbuf, length, 0)) != (int)length)
	{	tcp_disconnect();
tcp_status_msg(TEXT("send Failed"),TRUE);
		return 2;
	}
	return 0;	/* communications success */
}

/*:tcp_receive_buffer

	This routine receives a packet of data from the configured TCP/IP
	connection.  A packet is defined as being delimited with newlines
	('\n') which must be appended by the host.

	0 is returned upon successful completion.  Other possible
	error returns are listed below.

<pre>
	 1 - Timeout
	 2 - Connection could not be established or was lost
	 3 - Buffer length is zero
	 4 - Select error
	 5 - Buffer length is < 256 bytes
</pre>
*/
int tcp_receive_buffer
(	unsigned short length,	/* Must be at least 256 */
	unsigned char *buffer,
	signed short timeout,	/* In milliseconds */
	unsigned short *retlength
)
{
	int n=0;
	fd_set fds;
	struct timeval tv;
	int Sleeps = 0;
	double msStart = RtGetMsec();
	unsigned long Available = 0;

	if (!tcp_connect(0)) return 2;	/* protocol not active */
	if (!length) return 3;		/* zero length buffer */
	if (length < 256) return 5;	/* Need at least 256 */

/*
	Select seems to flush barcode reads on the CK30 so let's
	just poll the socket for availability and ignore the timeout.
*/
//	if (!timeout) timeout = 100;

	if (recvlen < length)
	{
TraceLog("APRS-IS", FALSE, NULL, "Growing Buffer from %ld for %ld length\n", (long) recvlen, (long) length);
		recvlen = length + 1;
		recvbuf = realloc(recvbuf,recvlen);
	}

	FD_ZERO(&fds);
	FD_SET(mysocket, &fds);
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout%1000)*1000;

/*printf("Read %ld bytes on socket %ld Timeout %ld\n", (long) length, (long) mysocket, (long) timeout);*/

	if (!timeout)
		tv.tv_usec = 50000;
	do
	{	unsigned short i;
		for (i=0; i<pendcount; i++)
		{	if (i==length || recvbuf[i] == '\n' || recvbuf[i] == '\r')
			{	memcpy(buffer,recvbuf,i);
				if (retlength) *retlength = i;
				while (i<pendcount && (recvbuf[i]=='\n' || recvbuf[i]=='\r'))
					i++;
				pendcount -= i;
				if (pendcount)
					memcpy(recvbuf, recvbuf+i, pendcount);
if (pendcount) TraceLog("APRS-IS", FALSE, NULL, "%.0lf tcp_receive_buffer returning %ld (%ld pending)\n", (double) MSecSince(msStart, RtGetMsec()), (long) *retlength, (long) pendcount);
				return 0;	/* communications success */
			}
		}
		if (pendcount >= recvlen)
		{
TraceLog("APRS-IS", TRUE, NULL, "tcp_receive_buffer %ld >= %ld??? (overflow %ld?)\n", (long) pendcount, recvlen, (long) length);
			length = pendcount;
		}
		if (pendcount >= length)
		{	memcpy(buffer,recvbuf,length);
			if (retlength) *retlength = length;
			pendcount -= length;
			if (pendcount)
				memcpy(recvbuf, recvbuf+length, pendcount);
TraceLog("APRS-IS", TRUE, NULL, "tcp_receive_buffer returning MAX %ld (%ld pending)\n", (long) length, (long) pendcount);
			return 0;
		}

	while ((n=ioctlsocket(mysocket,FIONREAD,&Available))==0 && Available <= 0 && timeout > 0)
	{	Sleep(10);
		Sleeps ++;
		timeout -= 10;
	}
//if (n || (!Sleeps && Available))
//	TraceLog("APRS-IS", FALSE, NULL, "%ld (%.0lf) ioctl Gave %ld (%ld) %ld Available Bytes\n",
//				(long) Sleeps, (double) MSecSince(msStart, RtGetMsec()),
//				(long) n, (long) WSAGetLastError(), (long) Available);
	if (n) return 4;
	else if (Available <= 0) return 1;

	if (recvlen < Available+pendcount)
	{
TraceLog("APRS-IS", TRUE, NULL, "Growing Buffer from %ld/%ld for %ld Available\n", (long) pendcount, (long) recvlen, (long) Available);
		recvlen = (unsigned short) Available+pendcount+1;
		recvbuf = realloc(recvbuf,recvlen);
	}

#ifdef OLD_WAY
	for (; timeout >= 10; timeout -= 10)
	{	Checks++;
		if ((n=ioctlsocket(mysocket,FIONREAD,&Available))==0 && Available <= 0)
		{	Sleep(10);
			Sleeps++;
		}
	}
#endif

#ifdef SELECTING
		int r = select(mysocket+1, &fds, NULL, NULL, &tv);
TraceLog("APRS-IS", FALSE, NULL, "Select Gave %ld\n", (long) r);
		if (r == -1) return 4;
		if (r == 0) return 1;
		if (!FD_ISSET(mysocket, &fds)) return 1;
#endif

		n=recv(mysocket, recvbuf+pendcount, recvlen-pendcount, 0);
if (n != Available) TraceLog("APRS-IS", FALSE, NULL, "Reading %ld into %ld/%ld Gave %ld\n", (long) recvlen-pendcount, (long) pendcount, (long) recvlen, (long) n);
		if (n <= 0)
		{	DWORD e = WSAGetLastError();
			TCHAR Text[80];
			StringCbPrintf(Text,sizeof(Text),TEXT("recv Error %ld Sent(%ld)"), (long) e, (long) (SentTime?(RtGetMsec()-SentTime):0));
			tcp_status_msg(Text,TRUE);
			tcp_disconnect();
			return 2;
		}
		pendcount += (unsigned short) n;
		BytesRecv += n;
		if (SentBuffer) free(SentBuffer);
		SentBuffer = NULL; SentLength = 0; SentTime = 0;
	} while (mysocket);
	return 2;	/* Good returns are all above! */
}
#endif

int tcp_send_udp(char *To, int Port, int Len, char *Buffer, int Attempts)
{	struct sockaddr_in si_other;
	int i, slen=sizeof(si_other);
	int delay = 40;
	DWORD Status;
static int s = 0;
static HANDLE hmtxLast = 0;
static char LastTo[80];
static unsigned long LastAddr = 0;
	__int64 Start = llGetMsec();
static __int64 msMax=0, msLast=0;

#define UDP_TRIGGER DEBUG_TRIGGER

	if (s==0 || s==-1)	/* Need to get a socket first */
	{	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		{	//TraceLog("APRS-IS", TRUE, NULL, "socket(UDP) Failed!\n");
Start = cDebugTimer("SendUDP","socketFail",Start,&msMax,&msLast,NULL,UDP_TRIGGER);
			return 0;
		} //else TraceLog("APRS-IS", TRUE, NULL, "Created UDP Socket %ld\n", (long) s);
	}
Start = cDebugTimer("SendUDP","socket",Start,&msMax,&msLast,NULL,UDP_TRIGGER);

	if (!hmtxLast) hmtxLast = CreateMutex(NULL, TRUE, NULL);
	else if ((Status=WaitForSingleObject(hmtxLast, 10000)) != WAIT_OBJECT_0)
	{	TraceError(NULL,"tcp_send_udp:WaitForSingleObject(hmtxLast) Failed With %ld (0x%lX)\n",
					(long) Status, (long) Status);
Start = cDebugTimer("SendUDP","WaitFail",Start,&msMax,&msLast,NULL,UDP_TRIGGER);
		return 0;
	}
Start = cDebugTimer("SendUDP","Lock",Start,&msMax,&msLast,NULL,UDP_TRIGGER);

	if (strncmp(LastTo, To, sizeof(LastTo)))
	{	memset((char *) &si_other, 0, sizeof(si_other));
		si_other.sin_addr.s_addr = inet_addr(To);
		if (si_other.sin_addr.s_addr == (u_long) -1)
		{	struct hostent *hostnm = gethostbyname(To);
			if (!hostnm)
			{	//TraceLog("APRS-IS", TRUE, NULL, "No Translation for %s\n", To);
	Start = cDebugTimer("SendUDP","DNSFail",Start,&msMax,&msLast,NULL,UDP_TRIGGER);
				ReleaseMutex(hmtxLast);
				return 0;
			}
			si_other.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
		}
		strncpy(LastTo, To, sizeof(LastTo));
		LastAddr = si_other.sin_addr.s_addr;
	}
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons((unsigned short)Port);
	si_other.sin_addr.s_addr = LastAddr;
Start = cDebugTimer("SendUDP","DNS",Start,&msMax,&msLast,NULL,UDP_TRIGGER);
	ReleaseMutex(hmtxLast);

	for (i=Attempts-1; i>=0; i--)
	{	//TraceLog("APRS-IS", FALSE, NULL, "Sending %ld/%ld\n", (long) Attempts-i, (long) Attempts);
		if (sendto(s, Buffer, Len, 0, (struct sockaddr *) &si_other, slen)==-1)
		{	//TraceLog("APRS-IS", TRUE, NULL, "sendto(%s:%ld) '%.*s' Failed\n", To, (long) Port, (int) Len, Buffer);
			soclose(s); s = 0;
Start = cDebugTimer("SendUDP","sendFail",Start,&msMax,&msLast,NULL,UDP_TRIGGER);
			return 0;
		}
		if (i)	/* Don't delay the final time */
		{	Sleep(delay);
			delay = delay * 2;
		}
	}
Start = cDebugTimer("SendUDP","sendto",Start,&msMax,&msLast,NULL,UDP_TRIGGER);

//	soclose(s);
	//TraceLog("APRS-IS", FALSE, NULL, "Sent(%.*s) To %s:%ld\n", (int) Len, Buffer, inet_ntoa(si_other.sin_addr), (long) ntohs(si_other.sin_port));
	return 1;
}

#ifdef UNDER_CE
void abort(void)
{
}
#endif

static double OSMGetMsec(void) { return 0.0; };

char *Finger(HWND hWnd, char *Host, long Port, char *Username, int *pLen)
{
	struct sockaddr_in lclserver = {0};
	struct sockaddr_in *pserver = &lclserver;
	int mysocket;
	long timeout;
	char *recvbuf;
	int n, recvlen, recvsize;
static int initialized = 0;

TraceLogThread("Finger", TRUE, "Finger(%s) At %s:%ld\n", Username, Host, (long) Port);

	if (!initialized)
	{	sock_init();
		initialized = 1;
	}

	pserver->sin_family      = AF_INET;
	pserver->sin_port        = htons((u_short)Port);
	pserver->sin_addr.s_addr = inet_addr(Host);	/* argv[1] = hostname */
	if (pserver->sin_addr.s_addr == (u_long) -1)
	{	struct hostent *hostnm = gethostbyname(Host);
		if (!hostnm)
		{	TraceLogThread("Finger", TRUE, "Finger:gethostbyname(%s) Failed\n", Host);
			return NULL;
		}
		pserver->sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
	}
	{	char *ipaddr = inet_ntoa(pserver->sin_addr);
		TraceLogThread("Finger", FALSE, "Resolved(%s)=>%s\n",Host, ipaddr?ipaddr:"NULL");
	}

	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{	TraceLogThread("Finger", TRUE, "Finger:socket() Failed\n");
		return NULL;
	}

	if (connect(mysocket, (struct sockaddr *)pserver, sizeof(*pserver)) < 0)
	{	soclose(mysocket);
		TraceLogThread("Finger", TRUE, "Finger:connect(%s:%ld) Failed\n", Host, (long) Port);
		return NULL;
	}

	n=send(mysocket, Username, strlen(Username), 0);

	if (n != (int)strlen(Username))
	{	soclose(mysocket);
		TraceLogThread("Finger", TRUE, "Finger:send(%ld) Failed\n", (long) strlen(Username));
		return NULL;
	}

	recvlen = 0;
	recvsize = 8192;
	recvbuf = (char *) malloc(recvsize+1);

	do
	{	unsigned long Available = 0;
		timeout = 15000;	/* 15 second timeout */

		while ((n=ioctlsocket(mysocket,FIONREAD,&Available))==0 && Available <= 0 && timeout > 0)
		{	double selectStart = OSMGetMsec();
			struct timeval tmo;
			fd_set fdRead;

			FD_ZERO(&fdRead);
			FD_SET(mysocket, &fdRead);
			tmo.tv_sec = timeout/1000; tmo.tv_usec = (timeout%1000) * 1000;	/* uSec */
			n = select(mysocket+1, &fdRead, NULL, NULL, &tmo);
			timeout -= (long) (OSMGetMsec() - selectStart);
			if (n > 0)
			{	if (FD_ISSET(mysocket, &fdRead))
				{	if ((n=ioctlsocket(mysocket,FIONREAD,&Available))!=0 || Available <= 0)
					{	TraceLogThread("Finger", TRUE, "Finger:ioctl(%ld)=%ld (%ld) or Available=%ld timeout %ld left\n",
									(long) mysocket, (long) n, (long) sock_errno(), (long) Available, (long) timeout);
						Available = 1;	/* Readable means recv() won't block */
					} else if (Available) TraceLogThread("Finger", FALSE, "Finger:readable %ld has %ld Available timeout %ld left\n", mysocket, Available, (long) timeout);
					n = 0;	/* make it look successful */
					break;
				}
			} else if (n < 0)
				TraceLogThread("Finger", TRUE, "Finger:select(%ld) return %ld timeout:%ld\n", (long) mysocket, (long) n, (long) timeout);
			else TraceLogThread("Finger", TRUE, "Finger:select(%ld) timeout %ld left\n", (long) mysocket, (long) timeout);
		}
		if (n)	/* Errors mean we're outa here */
		{	TraceLogThread("Finger", TRUE, "Finger:n=%ld, timeout=%ld GONE!\n", (long) n, (long) timeout);
			break;
		} else if (Available)
		{
TraceLogThread("Finger", FALSE, "Finger:Got %ld Available with %ldmsec left\n", (long) Available, (long) timeout);
			if ((n=recv(mysocket, recvbuf+recvlen, recvsize-recvlen, 0)) > 0)
			{
				recvlen += n;
				recvbuf[recvlen] = 0;	/* Null terminate for safety */

				if (recvlen >= recvsize)
				{	recvsize += 1024;
					recvbuf = (char *) realloc(recvbuf, recvsize+1);
				}
			} else	/* Errors mean we're done */
			{	TraceLogThread("Finger", TRUE, "Finger:recv(%ld) Error %ld (%ld), timeout %ld\n", (long) mysocket, (long) n, (long) sock_errno(), (long) timeout);
				break;
			}
		}
	} while (timeout>0);
	TraceLogThread("Finger", FALSE, "Finger:All done, timeout:%ld recvlen %ld\n", (long) timeout, (long) recvlen);
	soclose(mysocket);

	if (recvlen)
	{
#define KEEP_HEADER
#ifndef KEEP_HEADER
		for (n=0; n<=recvlen-4; n++)
		{	if (recvbuf[n] == '\r' && recvbuf[n+1] == '\n'
			&& recvbuf[n+2] == '\r' && recvbuf[n+3] == '\n')
				break;
		}
		if (n<=recvlen-4)
		{	n += 4;	/* Skip over \r\n\r\n */
			recvlen -= n;
			memmove(recvbuf,&recvbuf[n],recvlen);
			recvbuf[recvlen] = '\0';
		}
#else
		n = 0;
#endif
	} else if (recvbuf)
	{	free(recvbuf);
		recvbuf = NULL;
		recvlen = 0;	/* It's like it never happened */
	}
	if (pLen) *pLen = recvlen;

	return recvbuf;
}





