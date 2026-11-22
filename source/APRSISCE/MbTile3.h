MBTILES_INFO_S* SQLOpenMBTiles(char *MBTilePath, BOOL ReadOnly);
BOOL SQLCheckTile(MBTILES_INFO_S* sInfo, int zoom, int x, int y, int edgeAdjust, time_t *tSince=(time_t*)0);
HBITMAP SQLLoadTile(MBTILES_INFO_S* sInfo, HWND hWnd, int zoom, int x, int y, time_t *tSince=(time_t*)0);
BOOL SQLSaveTile(MBTILES_INFO_S* sInfo, int zoom, int x, int y, void* Buffer, int Length, time_t Date=0, time_t Expires=0);
char* SQLGetMeta(MBTILES_INFO_S* sInfo, char* name);
bool SQLSetMeta(MBTILES_INFO_S* sInfo, char* name, char* value);
bool SQLSetMeta(MBTILES_INFO_S* sInfo, char* name, long value);
