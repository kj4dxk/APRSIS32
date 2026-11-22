#include "sysheads.h"

#pragma warning(disable : 4995)	/* Don't care about deprecated functions */

//#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#ifndef UNDER_CE
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

//#include <windows.h>
//#include <winsock.h>
#include <commctrl.h>
//#include <strsafe.h>

#include "config.h"
#include "tcputil.h"
#include "tracelog.h"

#ifdef UNDER_CE
#ifdef CE50
#define USE_PNG_IMAGE
#else
#define USE_SHELL_IMAGE
#endif
#else
#define USE_PNG_IMAGE
#define USE_JPEG_IMAGE
#endif

#ifdef USE_SHELL_IMAGE
#include <Aygshell.h>
#endif
#ifdef USE_PNG_IMAGE
#include "pngUtil.h"
#endif
#ifdef USE_JPEG_IMAGE
#include "jpgUtil.h"
#endif

#include "LLUtil.h"
#include "MbTile3.h"

#define USE_MBTILES
#if defined(USE_MBTILES)
#include "sqlite3.h"
static char* stmtSelect = "select tile_data from tiles where zoom_level=?1 and tile_column=?2 and tile_row=?3;";
static char* stmtSelectDate = "select tile_data, tile_date, tile_expires from tiles where zoom_level=?1 and tile_column=?2 and tile_row=?3;";
static char* stmtCheck = "select rowid from tiles where zoom_level=?1 and tile_column=?2 and tile_row=?3;";
static char* stmtCheckDate = "select rowid, tile_date, tile_expires from tiles where zoom_level=?1 and tile_column=?2 and tile_row=?3;";
static char* stmtInsert = "INSERT INTO tiles(zoom_level, tile_column, tile_row, tile_data) VALUES(?1, ?2, ?3, ?4);";
static char* stmtInsertDate = "INSERT INTO tiles(zoom_level, tile_column, tile_row, tile_data, tile_date, tile_expires) VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
static char* stmtUpdate = "UPDATE tiles SET tile_data = ?4 WHERE zoom_level = ?1 AND tile_column = ?2 AND tile_row = ?3;";
static char* stmtUpdateDate = "UPDATE tiles SET tile_data = ?4, tile_date = ?5, tile_expires = ?6 WHERE zoom_level = ?1 AND tile_column = ?2 AND tile_row = ?3;";
static char* stmtGetMeta = "SELECT value FROM metadata WHERE name = ?1;";
static char* stmtSetMeta = "UPDATE metadata SET name=?1, value=?2;";
static char* stmtInsertMeta = "INSERT INTO metadata(name, value) VALUES(?1, ?2);";
#define MAX_SQL_LOCK 10
#define MAX_SQL_CHECK 5
#define MAX_SQL_LOAD 8
#define MAX_SQL_SAVE 50
//int sqlite3_exec(
//  sqlite3*,                                  /* An open database */
//  const char *sql,                           /* SQL to be evaluated */
//  int (*callback)(void*,int,char**,char**),  /* Callback function */
//  void *,                                    /* 1st argument to callback */
//  char **errmsg                              /* Error msg written here */
//);
static int ShowExec(void *path, int ColCount, char **ColValues, char **ColNames)
{	int i;
	TraceLogThread("MBTiles", TRUE, "%s:Got %ld Columns", path, (long) ColCount);
	for (i=0; i<ColCount; i++)
	{	TraceLogThread("MBTiles", TRUE, "%s:Col[%ld] %s=%s", path, (long) i, ColNames[i], ColValues[i]);
	}
	return 0;
}
static int GetZoomNumber(char *Value, char *Name)
{
	if (!Value)
	{	TraceLogThread("MBTiles", TRUE, "GetZoomNumber: NULL Value for %s\n", Name);
		return 0;
	}
	char *e;
	int v = strtol(Value,&e,10);
	if (*e) TraceLogThread("MBTiles", TRUE, "GetZoomNumber: Failed To Convert %s=%s, Using %ld",
							Name, Value, (long) v);
	return v;
}
static int ShowMaxZoom(void *UserArg, int ColCount, char **ColValues, char **ColNames)
{	MBTILES_INFO_S *sInfo = (MBTILES_INFO_S *) UserArg;
	ShowExec(sInfo->Path, ColCount, ColValues, ColNames);
	if (ColCount == 1)
	{	int Zooms = GetZoomNumber(ColValues[0], ColNames[0]);
		if (Zooms < 26)
		{	sInfo->Private.ZoomCount = Zooms+1;
			if (sInfo->Private.Zooms) free(sInfo->Private.Zooms);
			sInfo->Private.Zooms = (ZOOM_INFO_S *) calloc(sInfo->Private.ZoomCount, sizeof(*sInfo->Private.Zooms));
		} else TraceLogThread("MBTiles", TRUE, "%s:ShowMaxZoom: ZoomCount(%ld) Too Large, Max 25", sInfo->Path, (long) Zooms);
	} else return 1;	// Abort the query
	return 0;	
}

static int ShowZoomInfo(void *UserArg, int ColCount, char **ColValues, char **ColNames)
{	MBTILES_INFO_S *sInfo = (MBTILES_INFO_S *) UserArg;
	
	if (ColCount == 6)
	{	int z = GetZoomNumber(ColValues[0], ColNames[0]);
		if (z >= 0 && z < sInfo->Private.ZoomCount)
		{	int yOffset = 1<<z;
			sInfo->Private.Zooms[z].count = GetZoomNumber(ColValues[1],ColNames[1]);
			sInfo->Private.Zooms[z].minX = GetZoomNumber(ColValues[2],ColNames[2]);
			sInfo->Private.Zooms[z].maxX = GetZoomNumber(ColValues[3],ColNames[3]);
			sInfo->Private.Zooms[z].maxY = yOffset-GetZoomNumber(ColValues[4],ColNames[4])-1;	// Offset reverses min/max
			sInfo->Private.Zooms[z].minY = yOffset-GetZoomNumber(ColValues[5],ColNames[5])-1;	// Offset reverses min/max
			TraceLogThread("MBTiles", TRUE, "%s z[%ld] %ld Tiles x:%ld-%ld y:%ld-%ld",
							sInfo->Path,
							(long) z, (long) sInfo->Private.Zooms[z].count,
							(long) sInfo->Private.Zooms[z].minX,
							(long) sInfo->Private.Zooms[z].maxX,
							(long) sInfo->Private.Zooms[z].minY,
							(long) sInfo->Private.Zooms[z].maxY);
		} else TraceLogThread("MBTiles", TRUE, "%s:ShowZoomInfo: Zoom(%ld) Too Large, Max %ld", sInfo->Path, (long) z, (long) sInfo->Private.ZoomCount-1);
	} else
	{	ShowExec(sInfo->Path, ColCount, ColValues, ColNames);
		return 1;	// Abort the query
	}
	return 0;	
}

#ifdef FUTURE_INFO
		int ZoomCount;
		struct
		{	int count, minX, maxX, minY, maxY;
		} *Zooms;	// Index by zoom_level
#endif

char* SQLGetMeta(MBTILES_INFO_S* sInfo, char* name)
{
	char* result = NULL;
	sqlite3* pDb = (sqlite3*) sInfo->Private.pDb;
	if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
	{
		sqlite3_stmt* pGetMeta = (sqlite3_stmt*)sInfo->Private.pGetMeta;
		int status;
		if ((status = sqlite3_bind_text(pGetMeta, 1, name, -1, NULL)) != SQLITE_OK)
			TraceLogThread("MBTiles", TRUE, "sqlite3_bind_text(%s) returned %ld or %s",
				sInfo->Path, (long)result, sqlite3_errmsg(pDb));
		if ((status = sqlite3_step(pGetMeta)) == SQLITE_ROW)
		{	const unsigned char *text = sqlite3_column_text(pGetMeta, 0);
			if (text) result = strdup((const char*) text);
		}
		else if (status != SQLITE_DONE)
			TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step()2 returned %ld or %s",
				sInfo->Path, (long)status, sqlite3_errmsg(pDb));
		sqlite3_reset(pGetMeta);
		ReleaseMutex(sInfo->Private.hmtxDb);
	}
	else TraceLogThread("MBTiles", TRUE, "SQLGetMeta(%s):WaitForSingleObject(hmtxDb)(0x%lX) FAILED!\n", sInfo->Path, sInfo->Private.hmtxDb);
	return result;
}

static bool SQLGetMetaLong(MBTILES_INFO_S* sInfo, char* name, long *pValue)
{
	bool result = false;
	char* string = SQLGetMeta(sInfo, name);
	if (string)
	{
		char* e;
		*pValue = strtol(string, &e, 10);
		if (!*e) result = true;
		else TraceLogThread("MBTiles", TRUE, "%s:Invalid %s Value(%s), Non-Numeric", sInfo->Path, name, string);
		free(string);
	}
	else TraceLogThread("MBTiles", TRUE, "%s:No metadata(%s)", sInfo->Path, name);
	return result;
}
bool SQLSetMeta(MBTILES_INFO_S* sInfo, char* name, long value)
{
	char sValue[33];
	return SQLSetMeta(sInfo, name, ltoa(value, sValue, 10));
}

bool SQLSetMeta(MBTILES_INFO_S* sInfo, char* name, char* value)
{
	bool result = false;
	TraceLogThread("MBTiles", TRUE, "%s:Setting %s to %s", sInfo->Path, name, value);
	if (sInfo->DisplayOnly) return FALSE;

	if (SQLGetMeta(sInfo, name))
	{
		sqlite3* pDb = (sqlite3*) sInfo->Private.pDb;
		if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
		{
			sqlite3_stmt* pSetMeta = (sqlite3_stmt*)sInfo->Private.pSetMeta;
			int status;
			if ((status = sqlite3_bind_text(pSetMeta, 1, name, -1, NULL)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_text(%s) returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			if ((status = sqlite3_bind_text(pSetMeta, 2, value, -1, NULL)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_text(%s) returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			if ((status = sqlite3_step(pSetMeta)) == SQLITE_ROW)
			{
				TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step(SetMeta) returned SQLITE_ROW",
					sInfo->Path);
			}
			else if (status != SQLITE_DONE)
				TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step()2 returned %ld or %s",
					sInfo->Path, (long)status, sqlite3_errmsg(pDb));
			else result = true;
			sqlite3_reset(pSetMeta);
			ReleaseMutex(sInfo->Private.hmtxDb);
		}
		else TraceLogThread("MBTiles", TRUE, "SQLSetMeta(%s):WaitForSingleObject(hmtxDb)(0x%lX) FAILED!\n", sInfo->Path, sInfo->Private.hmtxDb);
	}
	else
	{
		sqlite3* pDb = (sqlite3*) sInfo->Private.pDb;
		if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
		{
			sqlite3_stmt* pInsertMeta = (sqlite3_stmt*)sInfo->Private.pInsertMeta;
			int status;
			if ((status = sqlite3_bind_text(pInsertMeta, 1, name, -1, NULL)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_text(%s) returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			if ((status = sqlite3_bind_text(pInsertMeta, 2, value, -1, NULL)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_text(%s) returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			if ((status = sqlite3_step(pInsertMeta)) == SQLITE_ROW)
			{
				TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step(InsertMeta) returned SQLITE_ROW",
					sInfo->Path);
			}
			else if (status != SQLITE_DONE)
				TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step()2 returned %ld or %s",
					sInfo->Path, (long)status, sqlite3_errmsg(pDb));
			else result = true;
			sqlite3_reset(pInsertMeta);
			ReleaseMutex(sInfo->Private.hmtxDb);
		}
		else TraceLogThread("MBTiles", TRUE, "SQLSetMeta(%s):WaitForSingleObject(hmtxDb)(0x%lX) FAILED!\n", sInfo->Path, sInfo->Private.hmtxDb);
	}
	return result;
}

static int GetString(void* UserArg, int ColCount, char** ColValues, char** ColNames)
{
	char** result = (char**)UserArg;
	if (ColCount == 1)
	{
		*result = strdup(ColValues[0]);
	}
	else
	{
		*result = NULL;
		ShowExec("GetString", ColCount, ColValues, ColNames);
		return 1;	// Abort the query
	}
	return 0;
}

static int GetLong(void* UserArg, int ColCount, char** ColValues, char** ColNames)
{
	long* result = (long*)UserArg;
	if (ColCount == 1)
	{
		char* e;
		*result = strtol(ColValues[0], &e, 10);
		if (*e)
		{
			ShowExec("GetLong", ColCount, ColValues, ColNames);
			return 1;	// Abort the query
		}
	}
	else
	{
		ShowExec("GetLong", ColCount, ColValues, ColNames);
		return 1;	// Abort the query
	}
	return 0;
}

static BOOL SQLTilesIsTable(sqlite3 *pDb)
{
	int result;
	char* errmsg;
	char* value = NULL;
	if ((result = sqlite3_exec(pDb, "select type from sqlite_master where name = 'tiles'", GetString, &value, &errmsg)) != SQLITE_OK)
	{
		TraceLogThread("MBTiles", TRUE, "sqlite3_exec type(tiles) returned %ld or %s or %s",
						(long)result, sqlite3_errmsg(pDb), errmsg ? errmsg : "NULL");
		return FALSE;
	}
	BOOL isTable = !stricmp(value, "table");
	if (value) free(value);
	return isTable;
}

typedef struct DATE_INFO_S
{
	BOOL isInvalid;
	BOOL hasValidDate;
	BOOL hasValidExpires;
} DATE_INFO_S;

static int CheckDateColumns(void* UserArg, int ColCount, char** ColValues, char** ColNames)
{
	DATE_INFO_S* info = (DATE_INFO_S*)UserArg;
	if (ColCount < 3) return 1;	// Invalid number of columns
	if (stricmp(ColNames[1], "name")) return 1;	// name should be 2nd column
	if (stricmp(ColNames[2], "type")) return 1;	// name should be 3rd column

	if (!stricmp(ColValues[1], "tile_date"))
	{
		if (!stricmp(ColValues[2], "integer"))
			info->hasValidDate = TRUE;
		else info->isInvalid = TRUE;
	}
	if (!stricmp(ColValues[1], "tile_expires"))
	{
		if (!stricmp(ColValues[2], "integer"))
			info->hasValidExpires = TRUE;
		else info->isInvalid = TRUE;
	}
	return 0;
}

static BOOL SQLCheckOrAddDateColumns(sqlite3* pDb)
{
	if (!SQLTilesIsTable(pDb)) return FALSE;
	int result;
	char* errmsg;
	DATE_INFO_S info = { 0 };

	TraceLogThread("MBTiles", TRUE, "SQLCheckOrAddDateColumns checking");
	if ((result = sqlite3_exec(pDb, "pragma table_info(tiles)", CheckDateColumns, &info, &errmsg)) != SQLITE_OK)
	{
		TraceLogThread("MBTiles", TRUE, "sqlite3_exec table_info(tiles) returned %ld or %s or %s",
			(long)result, sqlite3_errmsg(pDb), errmsg ? errmsg : "NULL");
		return FALSE;
	}
	if (info.isInvalid)
	{
		TraceLogThread("MBTiles", TRUE, "tile_date or tile_expires are NOT integer columns");
		return FALSE;
	}
	if (!info.hasValidDate)
	{
		TraceLogThread("MBTiles", TRUE, "SQLCheckOrAddDateColumns adding tile_date column");
		if ((result = sqlite3_exec(pDb, "ALTER TABLE tiles ADD COLUMN tile_date integer DEFAULT 0", ShowExec, "AddTileDate", &errmsg)) != SQLITE_OK)
		{
			TraceLogThread("MBTiles", TRUE, "sqlite3_exec add tile_date returned %ld or %s or %s",
							(long)result, sqlite3_errmsg(pDb), errmsg ? errmsg : "NULL");
			return FALSE;
		}
	}
	if (!info.hasValidExpires)
	{
		TraceLogThread("MBTiles", TRUE, "SQLCheckOrAddDateColumns adding tile_expires column");
		if ((result = sqlite3_exec(pDb, "ALTER TABLE tiles ADD COLUMN tile_expires integer DEFAULT 0", ShowExec, "AddTileExpires", &errmsg)) != SQLITE_OK)
		{
			TraceLogThread("MBTiles", TRUE, "sqlite3_exec add tile_expires returned %ld or %s or %s",
				(long)result, sqlite3_errmsg(pDb), errmsg ? errmsg : "NULL");
			return FALSE;
		}
	}
	return TRUE;
}

static HANDLE hmtxSQLOpenLock = CreateMutex(NULL, FALSE, NULL);

static MBTILES_INFO_S* SQLOpenSQLite(char* MBTilePath, BOOL ReadOnly /*= TRUE*/)
{
	int result;
	MBTILES_INFO_S* sInfo = NULL;

	if (MBTilePath[0])	// Using an MBTiles file?
	{
		sqlite3* pDb;

		FILE* testHandle = fopen(MBTilePath, "r");
		if (testHandle)
		{
			fclose(testHandle);
			testHandle = fopen(MBTilePath, "a");
			if (!testHandle)
			{
				if (!ReadOnly) TraceLogThread("MBTiles", TRUE, "SQLOpenSQLite:Cannot open %s for writing, marking ReadOnly (errno=%ld)", MBTilePath, (long)errno);
				ReadOnly = TRUE;
			}
			else fclose(testHandle);
		}
		else TraceLogThread("MBTiles", TRUE, "SQLOpenSQLite:fopen(%s) Failed with %ld", MBTilePath, (long)errno);

		TraceLogThread("MBTiles", TRUE, "SQLOpenSQLite: Opening %s %s", MBTilePath, ReadOnly ? "READONLY!" : "");

		if ((result = sqlite3_open_v2(MBTilePath, &pDb, ReadOnly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE, NULL)) != SQLITE_OK)
		{
			TraceLogThread("MBTiles", TRUE, "sqlite3_open_v2(%s) returned %ld or %s", MBTilePath, (long)result, sqlite3_errmsg(pDb));
			sqlite3_close(pDb);
			pDb = NULL;
			if (!ReadOnly)
			{
				if ((result = sqlite3_open_v2(MBTilePath, &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) != SQLITE_OK)
				{
					TraceLogThread("MBTiles", TRUE, "sqlite3_open_v2(%s) CREATE returned %ld or %s", MBTilePath, (long)result, sqlite3_errmsg(pDb));
					sqlite3_close(pDb);
					pDb = NULL;
				}
				else
				{
					// See https://github.com/mapbox/mbtiles-spec/blob/master/1.3/spec.md for full spec
					// See https://www.sqlite.org/src/artifact?ci=trunk&filename=magic.txt for application_id pragma
					char* errmsg;
					result = sqlite3_exec(pDb,
						"CREATE TABLE metadata (name text, value text);"
						"CREATE TABLE tiles (zoom_level integer, tile_column integer, tile_row integer, tile_data blob, tile_date integer, tile_expires integer);"
						"CREATE UNIQUE INDEX `tile_index` ON `tiles` (`zoom_level` ,`tile_column` ,`tile_row` );"
						"PRAGMA application_id = 0x4d504258;",	// =0x4d504258 is MBTiles tileset magic ID
						NULL, NULL, &errmsg);
					if (result != SQLITE_OK || errmsg)
					{
						TraceLogThread("MBTiles", TRUE, "sqlite3_execute(%s) TABLE/TABLE/INDEX returned %ld or %s",
							MBTilePath, (long)result,
							errmsg ? errmsg : "NULL");
						sqlite3_close(pDb);
						pDb = NULL;
					}
					if (errmsg) sqlite3_free(errmsg);
				}
			}
		}
		if (pDb)	// Got one open?
		{
			sInfo = (MBTILES_INFO_S*)calloc(1, sizeof(*sInfo));
			strncpy(sInfo->Path, MBTilePath, sizeof(sInfo->Path));
			sInfo->Private.pDb = pDb;
			TraceLogThread("MBTiles", TRUE, "%s Opened Successfully!", MBTilePath);

			if ((result = sqlite3_prepare_v2(pDb, stmtSelect, -1, (sqlite3_stmt * *)& sInfo->Private.pSelect, NULL)) != SQLITE_OK)
			{
				TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) Select returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			}
			else if ((result = sqlite3_prepare_v2(pDb, stmtCheck, -1, (sqlite3_stmt * *)& sInfo->Private.pCheck, NULL)) != SQLITE_OK)
			{
				TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) Check returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			}
			else if ((result = sqlite3_prepare_v2(pDb, stmtGetMeta, -1, (sqlite3_stmt * *)& sInfo->Private.pGetMeta, NULL)) != SQLITE_OK)
			{
				TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) GetMeta returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			}
			else if ((result = sqlite3_prepare_v2(pDb, stmtSetMeta, -1, (sqlite3_stmt * *)& sInfo->Private.pSetMeta, NULL)) != SQLITE_OK)
			{
				TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) SetMeta returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			}
			else if ((result = sqlite3_prepare_v2(pDb, stmtInsertMeta, -1, (sqlite3_stmt * *)& sInfo->Private.pInsertMeta, NULL)) != SQLITE_OK)
			{
				TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) InsertMeta returned %ld or %s",
					sInfo->Path, (long)result, sqlite3_errmsg(pDb));
			}
			else
			{
				char* errmsg = NULL;
				if (ReadOnly)
				{
					sInfo->DisplayOnly = TRUE;
				}
				else if (!SQLTilesIsTable(pDb))
				{
					TraceLogThread("MBTiles", TRUE, "tile(%s) is NOT a table, marking DisplayOnly", sInfo->Path);
					sInfo->DisplayOnly = TRUE;
				}
				else
				{
					TraceLogThread("MBTiles", TRUE, "tile(%s) IS a table!  Checking date columns", sInfo->Path);
					if (!SQLCheckOrAddDateColumns(pDb))
					{
						TraceLogThread("MBTiles", TRUE, "tile(%s) doesn't support dates", sInfo->Path);
						sInfo->Private.SupportsExpiration = FALSE;
					}
					else
					{
						sInfo->Private.SupportsExpiration = TRUE;
						if ((result = sqlite3_prepare_v2(pDb, stmtSelectDate, -1, (sqlite3_stmt * *)& sInfo->Private.pSelectDate, NULL)) != SQLITE_OK)
						{
							TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) SelectDate returned %ld or %s",
								sInfo->Path, (long)result, sqlite3_errmsg(pDb));
							sInfo->Private.SupportsExpiration = FALSE;
						}
						if ((result = sqlite3_prepare_v2(pDb, stmtCheckDate, -1, (sqlite3_stmt * *)& sInfo->Private.pCheckDate, NULL)) != SQLITE_OK)
						{
							TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) CheckDate returned %ld or %s",
								sInfo->Path, (long)result, sqlite3_errmsg(pDb));
							sInfo->Private.SupportsExpiration = FALSE;
						}
						if ((result = sqlite3_prepare_v2(pDb, stmtInsertDate, -1, (sqlite3_stmt * *)& sInfo->Private.pInsertDate, NULL)) != SQLITE_OK)
						{
							TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) InsertDate returned %ld or %s",
								sInfo->Path, (long)result, sqlite3_errmsg(pDb));
							sInfo->Private.SupportsExpiration = FALSE;
						}
						if ((result = sqlite3_prepare_v2(pDb, stmtUpdateDate, -1, (sqlite3_stmt * *)& sInfo->Private.pUpdateDate, NULL)) != SQLITE_OK)
						{
							TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) UpdateDate returned %ld or %s",
								sInfo->Path, (long)result, sqlite3_errmsg(pDb));
							sInfo->Private.SupportsExpiration = FALSE;
						}
					}

				}
				if (!sInfo->DisplayOnly)
				{
					if (sqlite3_exec(pDb, "PRAGMA journal_mode=WAL;", ShowExec, sInfo->Path, &errmsg) != SQLITE_OK)
					{
						TraceLogThread("MBTiles", TRUE, "sqlite3_exec(%s) journal=WAL returned %ld or %s or %s",
							sInfo->Path, (long)result, sqlite3_errmsg(pDb),
							errmsg ? errmsg : "NULL");
					}
					if (!sInfo->Private.SupportsExpiration)
					{
						if ((result = sqlite3_prepare_v2(pDb, stmtInsert, -1, (sqlite3_stmt * *)& sInfo->Private.pInsert, NULL)) != SQLITE_OK)
						{
							TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) Insert returned %ld or %s",
								sInfo->Path, (long)result, sqlite3_errmsg(pDb));
							sInfo->DisplayOnly = TRUE;
						}
						if ((result = sqlite3_prepare_v2(pDb, stmtUpdate, -1, (sqlite3_stmt * *)& sInfo->Private.pUpdate, NULL)) != SQLITE_OK)
						{
							TraceLogThread("MBTiles", TRUE, "sqlite3_prepare_v2(%s) Update returned %ld or %s",
								sInfo->Path, (long)result, sqlite3_errmsg(pDb));
							sInfo->DisplayOnly = TRUE;
						}
					}
				}
				if (sqlite3_exec(pDb, "SELECT MAX(zoom_level) from tiles;", ShowMaxZoom, sInfo, &errmsg) != SQLITE_OK)
				{
					TraceLogThread("MBTiles", TRUE, "sqlite3_exec(%s) MAX(zoom_level) returned %ld or %s or %s",
						sInfo->Path, (long)result, sqlite3_errmsg(pDb),
						errmsg ? errmsg : "NULL");
				}
				else
				{
					char* errmsg = NULL;
					if ((result = sqlite3_exec(pDb, "SELECT zoom_level, count(*), min(tile_column), max(tile_column), min(tile_row), max(tile_row) from tiles group by zoom_level order by zoom_level;",
						ShowZoomInfo, sInfo, &errmsg)) != SQLITE_OK)
					{
						TraceLogThread("MBTiles", TRUE, "sqlite3_exec(%s) Row/Col stats returned %ld or %s or %s",
							sInfo->Path, (long)result, sqlite3_errmsg(pDb),
							errmsg ? errmsg : "NULL");
					}
					else
					{
						sqlite3_extended_result_codes(pDb, 1);	// Turn on extended result codes
						sqlite3_busy_timeout(pDb, 2000);	// 2 second DB timeout
						sInfo->Private.pDb = pDb;
						sInfo->Private.hmtxDb = CreateMutex(NULL, FALSE, NULL);
						TraceLogThread("MBTiles", TRUE, "%s Stayed Open!", sInfo->Path);
					}
					if (errmsg) sqlite3_free(errmsg);
				}

				if (errmsg) sqlite3_free(errmsg);
			}
			if (result != SQLITE_OK)
			{
				sqlite3_close(pDb);
				free(sInfo);
				sInfo = NULL;
			}
		}
	}
	else TraceLogThread("MBTiles", TRUE, "SQLOpenSQLite:No Path!");
	return sInfo;
}

static int MBTilesCount = 0;
static MBTILES_INFO_S** MBTiles = NULL;

MBTILES_INFO_S* SQLOpenMBTiles(char *MBTilePath, BOOL ReadOnly)
{
	MBTILES_INFO_S* result = NULL;
	if (WaitForSingleObject(hmtxSQLOpenLock, 5000) == WAIT_OBJECT_0)
	{
		int m;
		for (m = 0; m < MBTilesCount; m++)
		{
			if (!strcmp(MBTilePath, MBTiles[m]->Path))
			{
				result = MBTiles[m];
				break;
			}
		}
		if (m >= MBTilesCount)
		{
			m = MBTilesCount++;
			MBTiles = (MBTILES_INFO_S**) realloc(MBTiles, sizeof(*MBTiles) * MBTilesCount);;
			result = MBTiles[m] = SQLOpenSQLite(MBTilePath, ReadOnly);
			if (!result)
			{
				TraceLogThread("MBTiles", TRUE, "Failed to open '%s'", MBTilePath);
				MBTilesCount--;	// Don't remember this entry
			}
		}
		ReleaseMutex(hmtxSQLOpenLock);
	}
	else TraceLogThread("MBTiles", TRUE, "SQLOpenSQLite(%s):WaitForSingleObject(hmtxSQLOpenLock)(0x%lX) FAILED!\n", MBTilePath, hmtxSQLOpenLock);
	return result;
}

BOOL SQLCheckTile(MBTILES_INFO_S* sInfo, int zoom, int x, int y, int edgeAdjust, time_t* tSince)
{
	if (!sInfo) { TraceLogThread("MBTiles", TRUE, "SQLCheckTile: NOT OPEN for %ld %ld %ld!", (long)zoom, (long)x, (long)y); return FALSE; }
	sqlite3* pDb = (sqlite3*) sInfo->Private.pDb;
	BOOL Exists = FALSE;
	if (tSince)* tSince = 0;
	if (pDb)
	{	sqlite3_stmt *pCheck = (sqlite3_stmt*) (sInfo->Private.SupportsExpiration?sInfo->Private.pCheckDate: sInfo->Private.pCheck);
		int result;

		if (zoom >= 0 && zoom < sInfo->Private.ZoomCount
		&& x >= sInfo->Private.Zooms[zoom].minX+edgeAdjust
		&& x <= sInfo->Private.Zooms[zoom].maxX-edgeAdjust
		&& y >= sInfo->Private.Zooms[zoom].minY+edgeAdjust
		&& y <= sInfo->Private.Zooms[zoom].maxY-edgeAdjust)
		{
			__int64 End2=0, Start2 = llGetMsec();
			if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
			{	__int64 Start = llGetMsec();
				End2 = llMsecSince(Start2,llGetMsec());
				if ((result=sqlite3_bind_int(pCheck, 1, zoom)) != SQLITE_OK)
					TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
										sInfo->Path, (long) result, sqlite3_errmsg(pDb));
				if ((result=sqlite3_bind_int(pCheck, 2, x)) != SQLITE_OK)
					TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
										sInfo->Path, (long) result, sqlite3_errmsg(pDb));
				if ((result=sqlite3_bind_int(pCheck, 3, (1<<zoom)-y-1)) != SQLITE_OK)
					TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
										sInfo->Path, (long) result, sqlite3_errmsg(pDb));
				if ((result=sqlite3_step(pCheck)) == SQLITE_ROW)
				{	Start = llMsecSince(Start,llGetMsec());
					TraceLogThread("MBTiles", Start>10, "%s:sqlite3_check(%ld %ld %ld) found in %ld msec",
									sInfo->Path, (long) zoom, (long) x, (long) y, (long) Start);
					if (tSince && sInfo->Private.SupportsExpiration)
					{
						time_t tDate = sqlite3_column_int(pCheck, 1);
						time_t tExpires = sqlite3_column_int(pCheck, 2);
						if (tExpires < time(NULL))
						{
							if (tDate)* tSince = tDate;
							else *tSince = 1;
						}
					}
					Exists = TRUE;
				} else if (result != SQLITE_DONE)
					TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step(%ld %ld %ld)2 returned %ld or %s",
									sInfo->Path, (long) zoom, (long) x, (long) y,
									(long) result, sqlite3_errmsg(pDb));
				sqlite3_reset(pCheck);
				ReleaseMutex(sInfo->Private.hmtxDb);
			} else TraceLogThread("MBTiles", TRUE, "SQLCheckTile(%s):WaitForSingleObject(hmtxDb)(0x%lX) FAILED!\n", sInfo->Path, sInfo->Private.hmtxDb);
			TraceLogThread("MBTiles", End2>MAX_SQL_LOCK||(llMsecSince(Start2,llGetMsec())-End2)>MAX_SQL_CHECK,
							"%s:SQLCheckTile(%ld %ld %ld) %s Lock %ld Total %ld",
							sInfo->Path, (long) zoom, (long) x, (long) y, Exists?"Exists":"NEED",
							(long) End2, (long) llMsecSince(Start2,llGetMsec()));
		}
	} else TraceLogThread("MBTiles", TRUE, "SQLCheckTile(%s): NOT OPEN for %ld %ld %ld!", sInfo->Path, (long) zoom, (long) x, (long) y);
	return Exists;
}

HBITMAP SQLLoadTile(MBTILES_INFO_S *sInfo, HWND hWnd, int zoom, int x, int y, time_t* tSince)
{
	if (!sInfo) { TraceLogThread("MBTiles", TRUE, "SQLLoadTile: NOT OPEN for %ld %ld %ld!", (long)zoom, (long)x, (long)y); return FALSE; }
	HBITMAP hbm = 0;
	int Suppress = FALSE;
	sqlite3* pDb = (sqlite3*)sInfo->Private.pDb;
	if (tSince)* tSince = 0;
	if (pDb)
	{	sqlite3_stmt *pSelect = (sqlite3_stmt*) (sInfo->Private.SupportsExpiration? sInfo->Private.pSelectDate: sInfo->Private.pSelect);
		int result;

		__int64 End2=0, Start2 = llGetMsec();
		if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
		{	__int64 Start = llGetMsec();
			End2 = llMsecSince(Start2,llGetMsec());
			if ((result=sqlite3_bind_int(pSelect, 1, zoom)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
									sInfo->Path, (long) result, sqlite3_errmsg(pDb));
			if ((result=sqlite3_bind_int(pSelect, 2, x)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
									sInfo->Path, (long) result, sqlite3_errmsg(pDb));
			if ((result=sqlite3_bind_int(pSelect, 3, (1<<zoom)-y-1)) != SQLITE_OK)
				TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
									sInfo->Path, (long) result, sqlite3_errmsg(pDb));
			do
			{
				if ((result=sqlite3_step(pSelect)) == SQLITE_ROW)
				{	int Length = sqlite3_column_bytes(pSelect, 0);	// Get the tile_data blob length
					const void *Buffer = sqlite3_column_blob(pSelect, 0);	// And the blob itself
					Start = llMsecSince(Start,llGetMsec());
					TraceLogThread("MBTiles", Start>10, "%s:sqlite3_blob(%ld %ld %ld) has %ld Bytes at %p in %ld msec",
									sInfo->Path, (long) zoom, (long) x, (long) y, (long) Length, Buffer, (long) Start);
					if (tSince && sInfo->Private.SupportsExpiration)
					{
						time_t tDate = sqlite3_column_int(pSelect, 1);
						time_t tExpires = sqlite3_column_int(pSelect, 2);
						if (tExpires < time(NULL))
						{
							if (tDate) *tSince = tDate;
							else *tSince = 1;
						}
					}
		#if defined(USE_PNG_IMAGE)
					{	PNG_INFO_S *Info = pngLoadFromBuffer(Buffer, Length);
						if (Info)
						{	hbm = PngToHbm(hWnd, Info, "MBTiles");
							pngDestroy(Info);
						}
					}
		#endif
		#if defined(USE_JPEG_IMAGE)
					if (!hbm)
					{	hbm = jpgLoadFromBuffer(hWnd, (char *) Buffer, Length);
					}
		#endif

				} else if (result == SQLITE_DONE)
					Suppress = TRUE;
				else
					TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step(%ld %ld %ld)1 returned %ld or %s",
									sInfo->Path, (long) zoom, (long) x, (long) y,
									(long) result, sqlite3_errmsg(pDb));
				if (result == SQLITE_BUSY || result == SQLITE_LOCKED)
					Sleep(50);
			} while (result == SQLITE_BUSY || result == SQLITE_LOCKED);
			sqlite3_reset(pSelect);
			ReleaseMutex(sInfo->Private.hmtxDb);
		} else TraceLogThread("MBTiles", TRUE, "SQLLoadTile(%s):WaitForSingleObject(hmtxDb)(0x%lX) FAILED!\n", sInfo->Path, sInfo->Private.hmtxDb);
		if (!Suppress)
			TraceLogThread("MBTiles", End2>MAX_SQL_LOCK||(llMsecSince(Start2,llGetMsec())-End2)>MAX_SQL_LOAD,
							"%s:SQLLoadTile(%ld %ld %ld) %s Lock %ld Total %ld",
							sInfo->Path, (long) zoom, (long) x, (long) y, hbm?"Good":"BAD",
							(long) End2, (long) llMsecSince(Start2,llGetMsec()));
	}
	return hbm;
}

typedef struct CHECKPOINT_INFO_S
{
	long busy;
	long log;
	long checkpointed;
} CHECKPOINT_INFO_S;

static int CaptureCheckpoint(void* UserArg, int ColCount, char** ColValues, char** ColNames)
{
	CHECKPOINT_INFO_S* cInfo = (CHECKPOINT_INFO_S*)UserArg;
	ShowExec("checkpoint", ColCount, ColValues, ColNames);
	for (int c = 0; c < ColCount; c++)
	{
		if (!strcmp(ColNames[c], "busy"))
		{
			char* e;
			cInfo->busy = strtol(ColValues[c], &e, 10);
		}
		else if (!stricmp(ColNames[c], "log"))
		{
			char* e;
			cInfo->log = strtol(ColValues[c], &e, 10);
		}
		else if (!stricmp(ColNames[c], "checkpointed"))
		{
			char* e;
			cInfo->checkpointed = strtol(ColValues[c], &e, 10);
		}
	}
	return 0;
}

#ifdef UNDER_CE
static DWORD SQLCheckpointer(LPVOID pvParam)
#else
static DWORD WINAPI SQLCheckpointer(LPVOID pvParam)
#endif
{
	MBTILES_INFO_S* sInfo = (MBTILES_INFO_S*)pvParam;
	int result = SQLITE_BUSY;

	do
	{
		Sleep(1000);
		if (!sInfo->Private.TransactionCount && time(NULL) > sInfo->Private.CheckpointTime)
		{
			if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
			{
				char* errmsg;
				__int64 Start = llGetMsec();
				CHECKPOINT_INFO_S Info = { 0 };

				if ((result = sqlite3_exec((sqlite3*)sInfo->Private.pDb, "PRAGMA wal_checkpoint;", CaptureCheckpoint, &Info, &errmsg)) != SQLITE_OK)
				{
					Start = llMsecSince(Start, llGetMsec());
					TraceLogThread("MBTiles", TRUE, "sqlite3_checkpoint(%s) %ld msec returned %ld or %s or %s",
						sInfo->Path, (long)Start, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
						errmsg ? errmsg : "NULL");
					TraceLogThread("commit", TRUE, "sqlite3_checkpoint(%s) %ld msec returned %ld or %s or %s",
						sInfo->Path, (long)Start, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
						errmsg ? errmsg : "NULL");
				}
				else
				{
					Start = llMsecSince(Start, llGetMsec());
					TraceLogThread("MBTiles", FALSE, "%s:sqlite3_checkpoint(%ld) %ld/%ld took %ld msec",
									sInfo->Path, (long)sInfo->Private.TransactionCount,
									(long) Info.checkpointed, (long) Info.log, (long)Start);
					TraceLogThread("commit", FALSE, "%s:sqlite3_checkpoint(%ld) %ld/%ld took %ld msec",
						sInfo->Path, (long)sInfo->Private.TransactionCount,
						(long)Info.checkpointed, (long)Info.log, (long)Start);
					sInfo->Private.CheckpointTime = 0;
					if (Info.log > 0 && Info.log == Info.checkpointed)	/* Checkpointed all, fire a TRUNCATE for good measure */
					{
						Start = llGetMsec();
						if ((result = sqlite3_exec((sqlite3*)sInfo->Private.pDb, "PRAGMA wal_checkpoint(TRUNCATE);", ShowExec, "TRUNCATE", &errmsg)) != SQLITE_OK)
						{
							Start = llMsecSince(Start, llGetMsec());
							TraceLogThread("MBTiles", TRUE, "sqlite3_checkpointTRUNCATE(%s) %ld msec returned %ld or %s or %s",
								sInfo->Path, (long)Start, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
								errmsg ? errmsg : "NULL");
							TraceLogThread("commit", TRUE, "sqlite3_checkpointTRUNCATE(%s) %ld msec returned %ld or %s or %s",
								sInfo->Path, (long)Start, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
								errmsg ? errmsg : "NULL");
							result = SQLITE_OK;	// Don't retry on this bad status
						}
						else
						{
							Start = llMsecSince(Start, llGetMsec());
							TraceLogThread("MBTiles", FALSE, "%s:sqlite3_checkpointTRUNCATE(%ld) took %ld msec", sInfo->Path, (long)sInfo->Private.TransactionCount, (long)Start);
							TraceLogThread("commit", TRUE, "%s:sqlite3_checkpointTRUNCATE(%ld) took %ld msec", sInfo->Path, (long)sInfo->Private.TransactionCount, (long)Start);
						}
					}
				}
				ReleaseMutex(sInfo->Private.hmtxDb);
			}
		}
	} while (result != SQLITE_OK);
	return 0;
}

#ifdef UNDER_CE
static DWORD SQLCommitter(LPVOID pvParam)
#else
static DWORD WINAPI SQLCommitter(LPVOID pvParam)
#endif
{
	MBTILES_INFO_S* sInfo = (MBTILES_INFO_S*)pvParam;
	int result = SQLITE_BUSY;

	do
	{
		Sleep(2000);
		if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
		{
			char* errmsg;
			__int64 Start = llGetMsec();

			if ((result = sqlite3_exec((sqlite3*)sInfo->Private.pDb, "COMMIT;", ShowExec, "Commit", &errmsg)) != SQLITE_OK)
			{
				Start = llMsecSince(Start, llGetMsec());
				TraceLogThread("MBTiles", TRUE, "sqlite3_commit(%s) %ld msec returned %ld or %s or %s",
					sInfo->Path, (long)Start, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
					errmsg ? errmsg : "NULL");
				TraceLogThread("commit", TRUE, "sqlite3_commit(%s) %ld msec returned %ld or %s or %s",
					sInfo->Path, (long)Start, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
					errmsg ? errmsg : "NULL");
			}
			else
			{
				Start = llMsecSince(Start, llGetMsec());
				TraceLogThread("MBTiles", FALSE, "%s:sqlite3_commit(%ld) took %ld msec", sInfo->Path, (long)sInfo->Private.TransactionCount, (long)Start);
				TraceLogThread("commit", TRUE, "%s:sqlite3_commit(%ld) took %ld msec", sInfo->Path, (long)sInfo->Private.TransactionCount, (long)Start);
				sInfo->Private.TransactionCount = 0;

				if (!sInfo->Private.CheckpointTime)
				{
					sInfo->Private.CheckpointTime = time(NULL)+30;	// Checkpoint after 30 seconds of inactivity
					CloseHandle(CreateThread(NULL, 0, SQLCheckpointer, sInfo, 0, NULL));
				} else sInfo->Private.CheckpointTime = time(NULL) + 30;	// Push the checkpoint further out
			}
			ReleaseMutex(sInfo->Private.hmtxDb);
		}
	} while (result != SQLITE_OK);
	return 0;
}

static int beginTransaction(MBTILES_INFO_S* sInfo)
{
	if (!sInfo->Private.TransactionCount)
	{
		int result;
		char* errmsg;
		if ((result = sqlite3_exec((sqlite3*)sInfo->Private.pDb, "BEGIN TRANSACTION;", ShowExec, "BeginTransaction", &errmsg)) != SQLITE_OK)
		{
			TraceLogThread("MBTiles", TRUE, "sqlite3_begin_transaction(%s) returned %ld or %s or %s",
				sInfo->Path, (long)result, sqlite3_errmsg((sqlite3*)sInfo->Private.pDb),
				errmsg ? errmsg : "NULL");
		}
		else
		{
			sInfo->Private.TransactionCount = 1;
			CloseHandle(CreateThread(NULL, 0, SQLCommitter, sInfo, 0, NULL));
			if (sInfo->Private.CheckpointTime)	// If there's a checkpointer running, delay it longer
			{
				sInfo->Private.CheckpointTime = time(NULL) + 30;
			}
		}
		return TRUE;
	}
	else sInfo->Private.TransactionCount++;
	return FALSE;
}

static int internalInsertUpdateTile(MBTILES_INFO_S* sInfo, sqlite3* pDb, sqlite3_stmt *pStatement, int zoom, int x, int y, void* Buffer, int Length, time_t Date, time_t Expires)
{
	int result;
	__int64 Start = llGetMsec();

	if ((result = sqlite3_bind_int(pStatement, 1, zoom)) != SQLITE_OK)
		TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
			sInfo->Path, (long)result, sqlite3_errmsg(pDb));
	if ((result = sqlite3_bind_int(pStatement, 2, x)) != SQLITE_OK)
		TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
			sInfo->Path, (long)result, sqlite3_errmsg(pDb));
	if ((result = sqlite3_bind_int(pStatement, 3, (1 << zoom) - y - 1)) != SQLITE_OK)
		TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
			sInfo->Path, (long)result, sqlite3_errmsg(pDb));
	if ((result = sqlite3_bind_blob(pStatement, 4, Buffer, Length, SQLITE_TRANSIENT)) != SQLITE_OK)
		TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
			sInfo->Path, (long)result, sqlite3_errmsg(pDb));
	if (sInfo->Private.SupportsExpiration)
	{
		if ((result = sqlite3_bind_int(pStatement, 5, Date)) != SQLITE_OK)
			TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
				sInfo->Path, (long)result, sqlite3_errmsg(pDb));
		if ((result = sqlite3_bind_int(pStatement, 6, Expires)) != SQLITE_OK)
			TraceLogThread("MBTiles", TRUE, "sqlite3_bind_int(%s) returned %ld or %s",
				sInfo->Path, (long)result, sqlite3_errmsg(pDb));

	}
	do
	{
		if ((result = sqlite3_step(pStatement)) == SQLITE_DONE)
		{
			Start = llMsecSince(Start, llGetMsec());
			TraceLogThread("MBTiles", Start > MAX_SQL_SAVE, "%s:insertUpdate(%ld %ld %ld) Date:%ld Expires:%ld took %ld msec",
				sInfo->Path, (long)zoom, (long)x, (long)y, (long)Date, (long)Expires, (long)Start);
		}
		else
		{
			TraceLogThread("MBTiles", TRUE, "%s:insertUpdate(%ld %ld %ld)3 returned %ld or %s",
				sInfo->Path, (long)zoom, (long)x, (long)y,
				(long)result, sqlite3_errmsg(pDb));
		}
		if (result == SQLITE_BUSY || result == SQLITE_LOCKED)
			Sleep(50);
	} while (result == SQLITE_BUSY || result == SQLITE_LOCKED);
	sqlite3_reset(pStatement);
	return result;
}

BOOL SQLSaveTile(MBTILES_INFO_S* sInfo, int zoom, int x, int y, void* Buffer, int Length, time_t Date, time_t Expires)
{
	if (!sInfo) { TraceLogThread("MBTiles", TRUE, "SQLSaveTile: NOT OPEN for %ld %ld %ld!", (long)zoom, (long)x, (long)y); return FALSE; }
	BOOL ItWorked = FALSE;
	sqlite3* pDb = (sqlite3*)sInfo->Private.pDb;

	if (sInfo->DisplayOnly) return FALSE;

	if (pDb)
	{
		sqlite3_stmt* pInsert = (sqlite3_stmt*)(sInfo->Private.SupportsExpiration ? sInfo->Private.pInsertDate : sInfo->Private.pInsert);
		int result;

		__int64 End2 = 0, Start2 = llGetMsec();
		if (WaitForSingleObject(sInfo->Private.hmtxDb, 5000) == WAIT_OBJECT_0)
		{
			__int64 Start = llGetMsec();
			End2 = llMsecSince(Start2, llGetMsec());

			beginTransaction(sInfo);
			result = internalInsertUpdateTile(sInfo, pDb, pInsert, zoom, x, y, Buffer, Length, Date, Expires);
			if (result == SQLITE_DONE)
			{
				Start = llMsecSince(Start, llGetMsec());
				TraceLogThread("MBTiles", Start > MAX_SQL_SAVE, "%s:sqlite3_insert(%ld %ld %ld) Date:%ld Expires:%ld took %ld msec",
					sInfo->Path, (long)zoom, (long)x, (long)y, (long)Date, (long)Expires, (long)Start);
				ItWorked = TRUE;
				if (zoom >= sInfo->Private.ZoomCount)
				{
					sInfo->Private.Zooms = (ZOOM_INFO_S*)realloc(sInfo->Private.Zooms, sizeof(*sInfo->Private.Zooms) * (zoom + 1));
					memset(&sInfo->Private.Zooms[sInfo->Private.ZoomCount], 0, sizeof(*sInfo->Private.Zooms) * (zoom - sInfo->Private.ZoomCount + 1));
					sInfo->Private.ZoomCount = zoom + 1;
				}
				sInfo->Private.Zooms[zoom].count++;
				if (x < sInfo->Private.Zooms[zoom].minX) sInfo->Private.Zooms[zoom].minX = x;
				if (x > sInfo->Private.Zooms[zoom].maxX) sInfo->Private.Zooms[zoom].maxX = x;
				if (y < sInfo->Private.Zooms[zoom].minY) sInfo->Private.Zooms[zoom].minY = y;
				if (y > sInfo->Private.Zooms[zoom].maxY) sInfo->Private.Zooms[zoom].maxY = y;
			}
			else if (result == SQLITE_CONSTRAINT_UNIQUE)
			{
				__int64 Start3 = llGetMsec();
				sqlite3_stmt* pUpdate = (sqlite3_stmt*)(sInfo->Private.SupportsExpiration ? sInfo->Private.pUpdateDate : sInfo->Private.pUpdate);
				result = internalInsertUpdateTile(sInfo, pDb, pUpdate, zoom, x, y, Buffer, Length, Date, Expires);
				if (result == SQLITE_DONE)
				{
					Start3 = llMsecSince(Start3, llGetMsec());
					TraceLogThread("MBTiles", Start > MAX_SQL_SAVE, "%s:sqlite3_update(%ld %ld %ld) Date:%ld Expires:%ld took %ld msec",
						sInfo->Path, (long)zoom, (long)x, (long)y, (long)Date, (long)Expires, (long)Start3);
					ItWorked = TRUE;
				}
			}
			if (result != SQLITE_DONE)
			{
				TraceLogThread("MBTiles", TRUE, "%s:sqlite3_step(%ld %ld %ld)3 returned %ld or %s",
					sInfo->Path, (long)zoom, (long)x, (long)y,
					(long)result, sqlite3_errmsg(pDb));
				ItWorked = FALSE;
			}

			ReleaseMutex(sInfo->Private.hmtxDb);
		}
		else TraceLogThread("MBTiles", TRUE, "SQLSaveTile(%s_:WaitForSingleObject(hmtxDb)(0x%lX) FAILED!\n", sInfo->Path, sInfo->Private.hmtxDb);
		TraceLogThread("MBTiles", End2 > MAX_SQL_LOCK || (llMsecSince(Start2, llGetMsec()) - End2) > MAX_SQL_SAVE,
			"%s:SQLSaveTile(%ld %ld %ld) %s Lock %ld Total %ld",
			sInfo->Path, (long)zoom, (long)x, (long)y, ItWorked ? "Worked" : "FAILED",
			(long)End2, (long)llMsecSince(Start2, llGetMsec()));
	}
	return ItWorked;
}

#endif
