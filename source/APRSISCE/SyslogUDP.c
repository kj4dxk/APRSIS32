#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "tcputil.h"
#include "TraceLog.h"

#include "aprs_udp.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

extern CONFIG_INFO_S ActiveConfig;

static unsigned char *MyStrnChr(int len, unsigned char *src, unsigned char c)
{	int i;
	for (i=0; i<len; i++)
		if (src[i] == c)
			return &src[i];
	return NULL;
}

BOOL SyslogGotPacket(int len, unsigned char *buf)
{	if (len > 1)	/* UDP is all or nothing */
	{	return TRUE;
	}
	return FALSE;
}

static char *SkipWhite(char *p)
{	while (*p && isspace(*p & 0xff)) p++;
	return p;
}

char *SyslogFormatReceive(int Len, unsigned char *Pkt, int *rLen)
{
// https://tools.ietf.org/html/rfc5424
//              0             kernel messages
//              1             user-level messages
//              2             mail system
//              3             system daemons
//              4             security/authorization messages
//              5             messages generated internally by syslogd
//              6             line printer subsystem
//              7             network news subsystem
//              8             UUCP subsystem
//              9             clock daemon
//             10             security/authorization messages
//             11             FTP daemon
//             12             NTP subsystem
//             13             log audit
//             14             log alert
//             15             clock daemon (note 2)
//             16             local use 0  (local0)
//             17             local use 1  (local1)
//             18             local use 2  (local2)
//             19             local use 3  (local3)
//             20             local use 4  (local4)
//             21             local use 5  (local5)
//             22             local use 6  (local6)
//             23             local use 7  (local7)

//              0       Emergency: system is unusable
//              1       Alert: action must be taken immediately
//              2       Critical: critical conditions
//              3       Error: error conditions
//              4       Warning: warning conditions
//              5       Notice: normal but significant condition
//              6       Informational: informational messages
//              7       Debug: debug-level messages

//	<Fac*8+Sev>1 yyyy-mm-ddThh:mm:ss Host App Proc Msg Structured Text

	int Done = FALSE;
	char *e, *t, *h, *a, *p, *m, *s, *text;
	if (Len > 33 && *Pkt == '<')
	{	char *lcl = strncpy(malloc(Len+1),Pkt,Len);
		lcl[Len] = '\0';
		e = strchr(lcl, '>');
		if (e && strlen(e)>2)	/* Fac/Sev must have >1 */
		{	*e++ = '\0';
			if (*e == '1')
			{	t = SkipWhite(e+1);	/* Start of time */
				h = strchr(t+1, ' ');	/* End of time */
				if (h)
				{	*h++ = '\0';
					h = SkipWhite(h);	/* Start of host */
					a = strchr(h, ' ');	/* End of host */
					if (a)
					{	*a++ = '\0';
						a = SkipWhite(a);	/* Start of App */
						p = strchr(a, ' ');	/* End of App */
						if (p)
						{	*p++ = '\0';
							p = SkipWhite(p);	/* Start of Proc */
							m = strchr(p, ' ');	/* End of Proc */
							if (m)
							{	*m++ = '\0';
								m = SkipWhite(m);	/* Start of Msg */
								s = strchr(m, ' ');	/* End of Msg */
								if (s)
								{	*s++ = '\0';
									s = SkipWhite(s);	/* Start of Structured */
									text = strchr(s, ' ');	/* End of Structured */
									if (text)
									{	char *LogName = malloc(strlen(a)+1+strlen(p)+1);
										sprintf(LogName,"%s:%s",a,p);
										*text++ = '\0';
										text = SkipWhite(text);	/* Start of Text */
										TraceLogThreadWithTime(LogName, FALSE, "%s %s %s\n", t, text, m);
										free(LogName);
										Done = TRUE;
									} else e = "TEXT";
								} else e = "STRUC";
							} else e = "MSG";
						} else e = "PROC";
					} else e = "APP";
				} else h = "HOST";
			} else e = "VER";
		} else e = "FACSEV";
		free(lcl);
	} else e = "";
	if (!Done) TraceLogThread("Syslog", FALSE, "%s:%ld:%.*s\n", e, (long) Len, Len, Pkt);
	*rLen = Len;
	return NULL;
}

char *SyslogFormatTransmit(char *String, long *rLen, void *Where)
{	size_t len = strlen(String);
	if (len <= 2 || String[len-2] != '\r' || String[len-1] != '\n')
	{	char *New = (char*)malloc(len+3);
		strcpy(New, String);
		New[len++] = '\r';
		New[len++] = '\n';
		String = New;
	}
	*rLen = len;
	return String;
}

BOOL SyslogTweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	pcInfo->RfBaud = 0;	/* Not really RF */
	pcInfo->NotRF = TRUE;
	pcInfo->RequiresInternet = TRUE;
	pcInfo->XmitEnabled = FALSE;
	pcInfo->RFtoISEnabled = FALSE;
	pcInfo->IStoRFEnabled = FALSE;
	pcInfo->BeaconingEnabled = FALSE;
	pcInfo->MessagesEnabled = FALSE;
	pcInfo->BulletinObjectEnabled = FALSE;
	pcInfo->TelemetryEnabled = FALSE;
	EmptySimpleStringList(&pcInfo->OpenCmds);	/* Not supported */
	EmptySimpleStringList(&pcInfo->CloseCmds);	/* Not supported */
	return TRUE;
}

char * cdecl FormatFilter(void);

BOOL SyslogNewConnection(void *What, BOOL (*Write)(void *What, int Len, unsigned char *Buf))
{	return FALSE;	// We don't do connections!
}

