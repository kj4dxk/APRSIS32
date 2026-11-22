#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void DoPair(double Lat, double Lon, char *Target, int Digits, char Base)
{	double fLat = floor(Lat), fLon = floor(Lon);
	Target[1] = ((int)floor(Lat))+Base;
	Target[0] = ((int)floor(Lon))+Base;
	Target[2] = '\0';

	if (Digits < 2) return;

	Lat -= fLat; Lon -= fLon;
	if (Base == 'A' || Base == 'a')
	{	Lat *= 10; Lon *= 10;
		DoPair(Lat, Lon, Target+2, Digits-2, '0');
	} else if (Base == '0')
	{	Lat *= 24; Lon *= 24;
		DoPair(Lat, Lon, Target+2, Digits-2, 'a');
	}
	return;
}

char *GridSquare(double Lat, double Lon, int Pairs)
{static char OutBuf[48];

	Lat = Lat + 90;	/* Make it 0-180 south to north */
	Lon = Lon + 180;	/* Make it 0-360 west to east */

	if (Lat < 0 || Lat >= 180.0
	|| Lon < 0 || Lon >= 360.0)
	{	strncpy(OutBuf,"Bogus",sizeof(OutBuf));
	} else
	{	DoPair(Lat/10.0, Lon/20.0, OutBuf, Pairs*2-2, 'A');
	}
	return OutBuf;
}

static void FromPair(char *Pair, char Base, double Factor, double *pLat, double *pLon)
{
printf("FromPair(%s)\n", Pair);
	if (!Pair || strlen(Pair) < 2)
	{	*pLat = *pLon = Factor/2;
	} else
	{	double Lat = Pair[1]-Base, Lon = Pair[0]-Base;
		if (Base == 'A')
		{	double dLat, dLon;
			FromPair(Pair+2, '0', 10, &dLat, &dLon);
			Lat += dLat/10; Lon += dLon/10;
		} else if (Base == '0')
		{	double dLat, dLon;
			FromPair(Pair+2, 'A', 24, &dLat, &dLon);
			Lat += dLat/24; Lon += dLon/24;
		}
		*pLat = Lat; *pLon = Lon;
	}
printf("FromPair(%s) is %.6lf %.6lf\n", Pair, *pLat, *pLon);
}

void GridSquare2LatLon(char *GridSquare, double *pLat, double *pLon)
{	double Lat, Lon;
	GridSquare = _strupr(_strdup(GridSquare));
	FromPair(GridSquare, 'A', 18, &Lat, &Lon);
	*pLat = Lat*10 - 90; *pLon = Lon*20 - 180;
	free(GridSquare);
}

void main(void)
{	double MyLat, MyLon, Lat, Lon;
	char *gs;
	MyLat = 27+59.7900/60;
	MyLon = (-80)-39.5400/60;
	gs = GridSquare(MyLat, MyLon, 4);
	GridSquare2LatLon(gs, &Lat, &Lon);
	printf("I'm at %.6lf %.6lf or %s\n", MyLat, MyLon, gs);
	printf("That's actually %.6lf %.6lf\n", Lat, Lon);
	GridSquare2LatLon("EL97qx", &Lat, &Lon);
	printf("EL97qx is %.6lf %.6lf\n", Lat, Lon);
	GridSquare2LatLon("EL97qx/E", &Lat, &Lon);
	printf("EL97qx/E is %.6lf %.6lf\n", Lat, Lon);
}
