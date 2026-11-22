/* Prototypes generated Mon Oct 24 21:21:48 2016 */
#ifdef __cplusplus
extern "c"
{
#endif
BOOL SyslogGotPacket(int len, unsigned char *buf);
char *SyslogFormatReceive(int Len, unsigned char *Pkt, int *rLen);
char *SyslogFormatTransmit(char *String, long *rLen, void *Where);
BOOL SyslogTweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo);
BOOL SyslogNewConnection(void *What, BOOL (*Write)(void *What, int Len, unsigned char *Buf));
#ifdef __cplusplus
}
#endif
