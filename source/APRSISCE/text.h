#ifndef GOT_TEXT_H
#define GOT_TEXT_H

#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif
void cdecl TraceLog(char *Name, BOOL ForceIt, HWND hwnd, char *Format, ...);
void cdecl TraceLogThread(char *Name, BOOL ForceIt, char *Format, ...);
void SetTraceThreadName(char *Name);

char *TEXTFormatTransmit(char *String, long *rLen, void *Where);
char *TEXTFormatReceive(int Len, unsigned char *Pkt, int *rLen);
BOOL TEXTGotPacket(int len, unsigned char *buf);

BOOL ConverseGotPacket(int len, unsigned char *buf);
char *ConverseFormatReceive(int Len, unsigned char *Pkt, int *rLen);
BOOL ConverseTweakConfigPacket(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);
char *ConverseFormatTransmitPacket(char *String, long *rLen, void *Where);

BOOL KenwoodGotPacket(int len, unsigned char *buf);
char *KenwoodFormatReceive(int Len, unsigned char *Pkt, int *rLen);
BOOL KenwoodTweakConfigAPRS(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);
char *KenwoodFormatTransmitAPRS(char *String, long *rLen, void *Where);
BOOL KenwoodTweakConfigPacket(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);
char *KenwoodFormatTransmitPacket(char *String, long *rLen, void *Where);

BOOL FTM350GotPacket(int len, unsigned char *buf);
char *FTM350FormatReceive(int Len, unsigned char *Pkt, int *rLen);
BOOL FTM350TweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);
char *FTM350FormatTransmit(char *String, long *rLen, void *Where);

BOOL D700TEXTTweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);
char *D700TEXTFormatTransmit(char *String, long *rLen, void *Where);

BOOL NMEANewConnection(void *What, BOOL (*Write)(void *What, int Len, unsigned char *Buf));
char *NMEAFormatReceive(int Len, unsigned char *Pkt, int *rLen);
BOOL NMEATweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);

DWORD MyGPSGetPosition(
  HANDLE        hGPSDevice,
  GPS_POSITION *pGPSPosition,
  DWORD         dwMaximumAge,
  DWORD         dwFlags
);

#ifndef UNDER_CE
#define GPSGetPosition MyGPSGetPosition
#endif

#ifdef __cplusplus
}
#endif

#endif	/* GOT_TEXT_H */
