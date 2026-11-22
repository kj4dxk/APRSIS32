typedef struct SHAPEFILE_POINT_S
{	double Lat, Lon;
} SHAPEFILE_POINT_S;

#ifdef OLD_WAY
typedef struct Point
{	double x, y/*, z*/;
} Point;
#else
typedef SHAPEFILE_POINT_S Point;
#endif
int poly_simplify( double tol, Point* V, int n, Point* sV );

typedef struct SHAPEFILE_OBJECT_S
{	char Code[16];
	char File[MAX_PATH];
	unsigned long Record;
	char ID[16];
	char State[16];
	char FIPS[16];
	char Zone[16];
	char Office[16];
	char County[64];
	char Name[132];
	char ShortName[64];
	double Lat, Lon, minLat, minLon, maxLat, maxLon;
	unsigned long PartCount;
	unsigned long *PartStarts;
	unsigned long PointCount;
	SHAPEFILE_POINT_S *Points;
} SHAPEFILE_OBJECT_S;

char *GetShapeIndexStats(void);
char *GetShapeCacheStats(void);
void FreeShapeObject(char *From, SHAPEFILE_OBJECT_S *Obj, BOOL ForceFree=FALSE);
SHAPEFILE_OBJECT_S *GetShapeObject( char *Code );
void FlushShapeIndexes(char *pszFilename=NULL);
void FlushShapeCache(void);

void FreeLatLonZones(SHAPEFILE_OBJECT_S **Objs);
SHAPEFILE_OBJECT_S **GetLatLonZones(double Lat, double Lon);
void FreeCWAZones(SHAPEFILE_OBJECT_S **Objs);
SHAPEFILE_OBJECT_S **GetCWAZones(char *CWA);
BOOL ProbeShapeFile(char *FileName);
size_t ShapeFileEnabledCount(void);
