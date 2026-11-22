#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>

#include "config.h"
#include "port.h"
#include "tcputil.h"
#include "tracelog.h"

#include "text.h"

#pragma warning(disable : 4995)	/* Disable deprecated warnings */

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

static GPS_POSITION GPS = {0};
static __int64 LastRead = 0;
static DWORD GPGSVCount;
static struct	/* Buffer structure to collect all sentences before displaying */
{	DWORD PRNs;
	DWORD Elevation;
	DWORD Azimuth;
	DWORD SignalToNoiseRatio;
} GPGSVs[GPS_MAX_SATELLITES];

static WORD FromHex(char *Hex, int Len)
{
static	char HexChar[] = "0123456789ABCDEF";
	WORD Result = 0;
	int i;

	for (i=0; i<Len; i++)
	{	char *x = strchr(HexChar,toupper(Hex[i]));
		if (!x) return 0;
		Result = Result*16 + (x-HexChar);
	}
	return Result;
}

static WORD FromDecimal(char *Digits, int Len)
{
	WORD Result = 0;
	int i;

	for (i=0; i<Len; i++)
	{	if (!isdigit(Digits[i])) return 0;
		Result = Result*10 + (Digits[i]-'0');
	}
	return Result;
}

static char *ParseNMEA(char *Input, PGPS_POSITION pGPS)
{	BOOL First = TRUE;
	if (!Input) return Input;
	if (!*Input) return Input;

	//TraceLogThread("NMEA", FALSE, "Read %s\n", Input);

	if (pGPS->dwSize != sizeof(*pGPS))	/* Valid structure size? */
	{	TraceLogThread("NMEA", TRUE, "Invalid GPS Size %ld != %ld\n", pGPS->dwSize, sizeof(*pGPS));
		return Input+strlen(Input);	/* Skip the whole buffer */
	}

	if (*Input != '$')
	{	char *Start = strchr(Input,'$');
#ifndef UNDER_CE
		pGPS->dwBadBuffers++;
#endif
		if (!Start)
		{	if (*Input) PortDumpHex2("NMEA", FALSE, "NO Starting $", -1, Input);
			return Input+strlen(Input);	/* No sentance at all! */
		}
		Input = Start;
	}

	while (*Input == '$')
	{	char *Next;	/* Next packet after delimiter */
		char checksum=0, *e, *p, *eol = strchr(Input,'*');
		char *Commas[20];	/* Hopefully not more than 20! */
		int CommaCount=0;

		if (!eol)
		{	if (First) PortDumpHex("NMEA", "Missing *", -1, Input);
			break;	/* Need the * */
		}
		eol += 3;	/* Move up past the *xx (theoretically at \r\n) */
		if ((eol-Input) > (int)strlen(Input))
		{	if (First) PortDumpHex("NMEA", "Missing *xx", -1, Input);
			break;	/* Need the *checksum */
		}

		if ((eol-Input) < 9)	/* $GPxxx*XX\0 minimum */
		{
#ifndef UNDER_CE
			pGPS->dwBadBuffers++;
#endif
			PortDumpHex("NMEA", "Short Sentence", (int) (eol-Input), Input);
			while (*eol=='\n' || *eol=='\r') eol++;	/* skip delimiters */
			Input = eol;	/* And pick it up after which may be \0 */
			continue;
		}

#ifdef NOT_ANY_MORE
		if (eol[-4] != '*')
		{
#ifndef UNDER_CE
			pGPS->dwBadBuffers++;
#endif
			PortDumpHex("NMEA", "Missing [-4]*", (int) (eol-Input), Input);
			Input = eol+1;
			continue;
		}
#endif

		PortDumpHex("NMEA", "Processing", (int) (eol-Input), Input);

		for (p=Input+1; p<eol-3; p++)
		{	checksum ^= *p;
			if (*p == ',')
			{	if (CommaCount < sizeof(Commas)/sizeof(Commas[0]))
					Commas[CommaCount++] = p+1;
				*p = '\0';	/* Null terminate previous string */
			}
		}
		eol[-3] = '\0';	/* Null terminate last piece */

		if (checksum != FromHex(eol-2,2))
		{
#ifndef UNDER_CE
			pGPS->dwBadChecksum++;
#endif
			TraceLogThread("NMEA", FALSE, "Bad Checksum 0x%02lX != 0x%02lX (%.2s) In %.*s\n",
						(long) checksum, (long) FromHex(eol-2,2), eol-2, (int) (eol-Input), Input);
			while (*eol=='\n' || *eol=='\r') eol++;	/* skip delimiters */
			Input = eol;
			continue;
		}
		while (*eol=='\n' || *eol=='\r') eol++;	/* skip delimiters */
		Next = eol;	/* for next loop */


{
	static char *TalkerIDs[] = {	// Common Talker IDs from http://www.catb.org/gpsd/NMEA.html#_talker_ids
								"GP",	//Global Positioning System receiver
								"LC",	// Loran-C receiver
								"II",	// Integrated Instrumentation
								"IN",	// Integrated Navigation
								"EC",	// Electronic Chart Display & Information System (ECDIS)
								"CD",	// Digital Selective Calling (DSC)
								"GA",	// Galileo Positioning System
								"GL",	// GLONASS, according to IEIC 61162-1
								"GN",	// Mixed GPS and GLONASS data, according to IEIC 61162-1
								"GB",	// BeiDou (China)
								"BD",	// BeiDou (China)
								"QZ",	// QZSS regional GPS augmentation system (Japan)
								"PK",	// Kenwood GPS passthrough "talker"
							};
	static int TalkerCount = sizeof(TalkerIDs)/sizeof(TalkerIDs[0]);
	int t;
	for (t=0; t<TalkerCount; t++)
		if (!strncmp(Input+1,TalkerIDs[t],2))
			break;
	if (t >= TalkerCount)
	{
#ifndef UNDER_CE
		pGPS->dwBadBuffers++;
#endif
		PortDumpHex("NMEA", "Unrecognized Talker", (int) (eol-Input), Input);
		while (*eol=='\n' || *eol=='\r') eol++;	/* skip delimiters */
		Input = eol;	/* And pick it up after which may be \0 */
		continue;
	}
}
		
		
/*	See NMEA Sentences at: http://aprs.gids.nl/nmea/ */
/*	See also NMEA.txt from http://gpsd.berlios.de/NMEA.txt */

#ifdef DOCUMENTATION
	/* $GPRMC,032500.067,A,2759.8030,N,08039.5431,W,0.00,66.87,081209,,*2A */
	/* $GPRMC,003149,A,4259.6701,N,07130.3863,W,0.0,326.0,040310,,*00 */
	/* $GPRMC  ,003149,A,4259.6701,N,07130.3863,W,0.0 ,326.0,040310,   ,*00 */
	/* $PKWDPOS,130430,A,2759.8045,N,08039.5386,W,0.29,20   ,290510,6.6 *15 */
	Commas[0];	/* Timestamp HHMMSS */
	Commas[1];	/* Validity, A=ok, V=invalid */
	Commas[2];	/* Latitude ddmm.mm */
	Commas[3];	/* North/South */
	Commas[4];	/* Longitude dddmm.mm */
	Commas[5];	/* East/West */
	Commas[6];	/* Speed in knots */
	Commas[7];	/* True course */
	Commas[8];	/* Date Stamp ddmmyy */
	Commas[9];	/* Magnetic Variation */
	Commas[10];	/* East/West */
#endif
		if (!strcmp(Input+3,"RMC") || !strcmp(Input,"$PKWDPOS"))	/* Kenwood */
		{	if (CommaCount != 10 && CommaCount != 11 && CommaCount != 12)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need 10, 11 or 12\n", Input, (long) CommaCount);
			} else
			{	if ((strlen(Commas[0]) == 6 || (strlen(Commas[0]) > 6 && Commas[0][6]=='.'))
				&& strlen(Commas[8]) == 6)
				{	pGPS->stUTCTime.wHour = FromDecimal(Commas[0],2);
					pGPS->stUTCTime.wMinute = FromDecimal(Commas[0]+2,2);
					pGPS->stUTCTime.wSecond = FromDecimal(Commas[0]+4,2);

					pGPS->stUTCTime.wDay = FromDecimal(Commas[8],2);
					pGPS->stUTCTime.wMonth = FromDecimal(Commas[8]+2,2);
					pGPS->stUTCTime.wYear = FromDecimal(Commas[8]+4,2)+2000;
					pGPS->dwValidFields |= GPS_VALID_UTC_TIME;
				}

				if (*Commas[1] == 'A')	/* Valid! */
				{	if (pGPS->FixQuality == GPS_FIX_QUALITY_UNKNOWN)
						pGPS->FixQuality = GPS_FIX_QUALITY_GPS;
					pGPS->dblLatitude = FromDecimal(Commas[2],2)+strtod(Commas[2]+2,&e)/60.0;
					if (!*e) pGPS->dwValidFields |= GPS_VALID_LATITUDE;
					pGPS->dblLongitude = FromDecimal(Commas[4],3)+strtod(Commas[4]+3,&e)/60.0;
					if (!*e) pGPS->dwValidFields |= GPS_VALID_LONGITUDE;
					if (toupper(*Commas[3]) == 'S') pGPS->dblLatitude = -pGPS->dblLatitude;
					if (toupper(*Commas[5]) == 'W') pGPS->dblLongitude = -pGPS->dblLongitude;

					pGPS->flSpeed = (float) strtod(Commas[6],&e);
					if (!*e && *Commas[6]) pGPS->dwValidFields |= GPS_VALID_SPEED;
					pGPS->flHeading = (float) strtod(Commas[7],&e);
					if (!*e && *Commas[7]) pGPS->dwValidFields |= GPS_VALID_HEADING;

					pGPS->dblMagneticVariation = strtod(Commas[9],&e);
					if (!*e && *Commas[9]) pGPS->dwValidFields |= GPS_VALID_MAGNETIC_VARIATION;
					if (toupper(*Commas[5]) == 'E') pGPS->dblMagneticVariation = -pGPS->dblMagneticVariation;
				} else pGPS->FixQuality = GPS_FIX_QUALITY_UNKNOWN;
			}
		}
#ifdef DOCUMENTATION
ZDA - Data and Time

  $GPZDA,hhmmss.ss,dd,mm,yyyy,xx,yy*CC
  $GPZDA,201530.00,04,07,2002,00,00*60

where:
 
	hhmmss    HrMinSec(UTC)
	dd,mm,yyy Day,Month,Year
    xx        local zone hours -13..13
    yy        local zone minutes 0..59
    *CC       checksum
	Commas[0];	/* Timestamp HHMMSS (UTC) */
	Commas[1];	/* Day dd */
	Commas[2];	/* Month mm */
	Commas[3];	/* Year yyyy */
	Commas[4];	/* Local zone hours xx (-13..13) */
	Commas[5];	/* Local zone minutes yy (0..59) */

mine (G6UIM) reports 00 for xx yy but then it probably
#endif
		else if (!strcmp(Input+3,"ZDA"))
		{	if (CommaCount != 6)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need 6\n", Input, (long) CommaCount);
			} else
			{
static BOOL FirstTime = TRUE;
				if ((strlen(Commas[0]) == 6 || (strlen(Commas[0]) > 6 && Commas[0][6]=='.'))
				&& strlen(Commas[1]) == 2
				&& strlen(Commas[2]) == 2
				&& strlen(Commas[3]) == 4)
				{	pGPS->stUTCTime.wHour = FromDecimal(Commas[0],2);
					pGPS->stUTCTime.wMinute = FromDecimal(Commas[0]+2,2);
					pGPS->stUTCTime.wSecond = FromDecimal(Commas[0]+4,2);

					pGPS->stUTCTime.wDay = FromDecimal(Commas[1],2);
					pGPS->stUTCTime.wMonth = FromDecimal(Commas[2],2);
					pGPS->stUTCTime.wYear = FromDecimal(Commas[3],4);
					pGPS->dwValidFields |= GPS_VALID_UTC_TIME;
				}
TraceLogThread("NMEA", FirstTime, "$GPZDA Local Zone Hours=%s Minute=%s\n", Commas[4], Commas[5]);
FirstTime = FALSE;
			}
		}
#ifdef DOCUMENTATION
	/* $GPGLL,4229.3770,N,07116.2909,W,175255,A*34 */
	Commas[0];	/* Latitude ddmm.mm */
	Commas[1];	/* North/South */
	Commas[2];	/* Longitude dddmm.mm */
	Commas[3];	/* East/West */
	Commas[4];	/* Timestamp HHMMSS *//* But we DON'T use this due to no date */
	Commas[5];	/* Validity, A=ok, V=invalid */
#endif
		else if (!strcmp(Input+3,"GLL"))
		{	if (CommaCount != 6 && CommaCount != 7)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need 6 or 7\n", Input, (long) CommaCount);
			} else
			{	if (*Commas[5] == 'A')	/* Valid! */
				{	pGPS->dblLatitude = FromDecimal(Commas[0],2)+strtod(Commas[0]+2,&e)/60.0;
					if (!*e) pGPS->dwValidFields |= GPS_VALID_LATITUDE;
					pGPS->dblLongitude = FromDecimal(Commas[2],3)+strtod(Commas[2]+3,&e)/60.0;
					if (!*e) pGPS->dwValidFields |= GPS_VALID_LONGITUDE;
					if (toupper(*Commas[1]) == 'S') pGPS->dblLatitude = -pGPS->dblLatitude;
					if (toupper(*Commas[3]) == 'W') pGPS->dblLongitude = -pGPS->dblLongitude;
				}
			}
		}
#ifdef DOCUMENTATION
	/* $GPGGA,032500.067,2759.8030,N,08039.5431,W,2,05,2.5,-10.1,M,-30.7,M,1.7,0000*55 */

	Commas[0];	/* Timestamp HHMMSS */
	Commas[1];	/* Latitude ddmm.mm */
	Commas[2];	/* North/South */
	Commas[3];	/* Longitude dddmm.mm */
	Commas[4];	/* East/West */
	Commas[5];	/* GPS Quality (0=No, 1=GPS, 2=DGPS) */
	Commas[6];	/* Number of Satellites in Use */
	Commas[7];	/* HDOP */
	Commas[8];	/* Altitude (Meters above sea level) */
	Commas[9];	/* M=Meters (altitude units) */
	Commas[10];	/* Geoidal separation (diff between earth ellipsoid and sea lavel, -=geoid is below ellipsoid) */
	Commas[11];	/* M=Meters (geoidal separation units) */
	Commas[12];	/* Age in seconds since last diff station update */
	Commas[13];	/* Diff reference station ID */
#endif
		else if (!strcmp(Input+3,"GGA"))
		{	if (CommaCount != 14)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need 14\n", Input, (long) CommaCount);
			} else
			{	if ((strlen(Commas[0]) == 6 || (strlen(Commas[0]) > 6 && Commas[0][6]=='.'))
				&& strlen(Commas[8]) == 6)
				{	pGPS->stUTCTime.wHour = FromDecimal(Commas[0],2);
					pGPS->stUTCTime.wMinute = FromDecimal(Commas[0]+2,2);
					pGPS->stUTCTime.wSecond = FromDecimal(Commas[0]+4,2);
					pGPS->dwValidFields |= GPS_VALID_UTC_TIME;
				}

				switch (*Commas[5])
				{
				case '1': pGPS->FixQuality = GPS_FIX_QUALITY_GPS; break;
				case '2': pGPS->FixQuality = GPS_FIX_QUALITY_DGPS; break;
				case '3': pGPS->FixQuality = GPS_FIX_QUALITY_PPS; break;
				case '4': pGPS->FixQuality = GPS_FIX_QUALITY_RTK; break;
				case '5': pGPS->FixQuality = GPS_FIX_QUALITY_FLOAT; break;
				case '0': pGPS->FixQuality = GPS_FIX_QUALITY_UNKNOWN; break;
				default: pGPS->FixQuality = GPS_FIX_QUALITY_UNKNOWN;
					TraceLogThread("NMEA", TRUE, "%s:FixQuality=0x%lX from %s (0x%lX)\n",
								Input, (long) pGPS->FixQuality, Commas[5], (long) *Commas[5]);
				}

				if (pGPS->FixQuality != GPS_FIX_QUALITY_UNKNOWN)
				{	pGPS->dblLatitude = FromDecimal(Commas[1],2)+strtod(Commas[1]+2,&e)/60.0;
					if (!*e) pGPS->dwValidFields |= GPS_VALID_LATITUDE;
					pGPS->dblLongitude = FromDecimal(Commas[3],3)+strtod(Commas[3]+3,&e)/60.0;
					if (!*e) pGPS->dwValidFields |= GPS_VALID_LONGITUDE;
					if (toupper(*Commas[2]) == 'S') pGPS->dblLatitude = -pGPS->dblLatitude;
					if (toupper(*Commas[4]) == 'W') pGPS->dblLongitude = -pGPS->dblLongitude;

					pGPS->dwSatelliteCount = strtol(Commas[6],&e,10);
					if (!*e && *Commas[6]) pGPS->dwValidFields |= GPS_VALID_SATELLITE_COUNT;

					pGPS->flHorizontalDilutionOfPrecision = (float) strtod(Commas[7],&e);
					if (!*e && *Commas[7]) pGPS->dwValidFields |= GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION;

					pGPS->flAltitudeWRTSeaLevel = (float) strtod(Commas[8],&e);
					if (!*e && *Commas[8] && toupper(*Commas[9]) == 'M') pGPS->dwValidFields |= GPS_VALID_ALTITUDE_WRT_SEA_LEVEL;

					pGPS->flAltitudeWRTEllipsoid = (float) strtod(Commas[10],&e);
					if (!*e && *Commas[10] && toupper(*Commas[11]) == 'M')
					{	pGPS->dwValidFields |= GPS_VALID_ALTITUDE_WRT_ELLIPSOID;
						pGPS->flAltitudeWRTEllipsoid += pGPS->flAltitudeWRTSeaLevel;
					}
				}
			}
		}
#ifdef DOCUMENTATION
	/* $GPGSA,A,3,31,18,14,26,22,,,,,,,,6.9,2.5,6.5*33 */

	Commas[0];	/* Mode: M=Manual, A=Automatic (2/3D) */
	Commas[1];	/* Mode: 1=No Fix, 2=2D, 3=3D */
	Commas[2-13];	/* 3-14 - IDs of SVs used in position */
	Commas[14];	/* PDOP */
	Commas[15];	/* HDOP */
	Commas[16];	/* VDOP */
#endif
		else if (!strcmp(Input,"$GPGSA"))
		{	if (CommaCount != 17 && CommaCount != 18)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need 17 or 18\n", Input, (long) CommaCount);
			} else
			{	switch (*Commas[0])
				{
				case 'A': pGPS->SelectionType = GPS_FIX_SELECTION_AUTO; break;
				case 'M': pGPS->SelectionType = GPS_FIX_SELECTION_MANUAL; break;
				default: pGPS->SelectionType = GPS_FIX_SELECTION_UNKNOWN;
				}

				switch (*Commas[1])
				{
				case '2': pGPS->FixType = GPS_FIX_2D; break;
				case '3': pGPS->FixType = GPS_FIX_3D; break;
				default: pGPS->FixType = GPS_FIX_UNKNOWN;
				}

				if (pGPS->FixType != GPS_FIX_UNKNOWN)
				{	int i;

					for (i=0; i<GPS_MAX_SATELLITES; i++)
						pGPS->rgdwSatellitesUsedPRNs[i] = 255;
					for (i=0; i<min(12,GPS_MAX_SATELLITES); i++)
					{	if (*Commas[i+2]) pGPS->rgdwSatellitesUsedPRNs[i] = strtol(Commas[i+2],&e,10);
						else pGPS->rgdwSatellitesUsedPRNs[i] = 255;
					}
					pGPS->dwValidFields |= GPS_VALID_SATELLITES_USED_PRNS;

					pGPS->flPositionDilutionOfPrecision = (float) strtod(Commas[14],&e);
					if (!*e && *Commas[14]) pGPS->dwValidFields |= GPS_VALID_POSITION_DILUTION_OF_PRECISION;

					pGPS->flHorizontalDilutionOfPrecision = (float) strtod(Commas[15],&e);
					if (!*e && *Commas[15]) pGPS->dwValidFields |= GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION;

					pGPS->flVerticalDilutionOfPrecision = (float) strtod(Commas[16],&e);
					if (!*e && *Commas[16]) pGPS->dwValidFields |= GPS_VALID_VERTICAL_DILUTION_OF_PRECISION;
				}
			}
		}

#ifdef DOCUMENTATION
	$GPGSV,3,1,10,22,70,009,36,18,54,099,29,26,52,007,37,14,52,318,37*7E
	$GPGSV,3,2,10,30,29,123,00,31,27,214,31,12,23,083,00,21,23,174,00*7A
	$GPGSV,3,3,10,09,18,039,00,27,10,039,00*7C

	Commas[0];	/* Total messages of this type */
	Commas[1];	/* Message number */
	Commas[2];	/* Total number of SVs in view */
	Commas[3];	/* SV PRN number */
	Commas[4];	/* Elevation in degrees, 90 maximum */
	Commas[5];	/* Azimuth, degrees from true north, 000 to 359 */
	Commas[6];	/* SNR, 00-99 db (null when not tracking) */
	Commas[7-10];	/* Second SV */
	Commas[11-14];	/* Third SV */
	Commas[15-18];	/* Fourth SV */
#endif
		else if (!strcmp(Input,"$GPGSV"))
		{	if (CommaCount < 3)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need at least 3\n", Input, (long) CommaCount);
			} else
			{	int total = strtol(Commas[0],&e,10);
				int number = strtol(Commas[1],&e,10);

#ifdef OLD_WAY
				pGPS->dwSatellitesInView = strtol(Commas[2],&e,10);
				if (!*e && *Commas[2]) pGPS->dwValidFields |= GPS_VALID_SATELLITES_IN_VIEW;

				if (number > 0 && number <= total)
				{	int c, i;

					if (number == 1)	/* First one of the set? */
					{	for (i=0; i<GPS_MAX_SATELLITES; i++)
						{	pGPS->rgdwSatellitesInViewPRNs[i] = 255;
							pGPS->rgdwSatellitesInViewElevation[i] = -1;
							pGPS->rgdwSatellitesInViewAzimuth[i] = -1;
							pGPS->rgdwSatellitesInViewSignalToNoiseRatio[i] = 0;
						}
					}
					for (c=3, i=(number-1)*4; c<CommaCount; c+=4, i++)
					{	if (i < GPS_MAX_SATELLITES)
						{	if (*Commas[c])
							{	pGPS->rgdwSatellitesInViewPRNs[i] = strtol(Commas[c],&e,10);
								pGPS->rgdwSatellitesInViewElevation[i] = strtol(Commas[c+1],&e,10);
								pGPS->rgdwSatellitesInViewAzimuth[i] = strtol(Commas[c+2],&e,10);
								pGPS->rgdwSatellitesInViewSignalToNoiseRatio[i] = strtol(Commas[c+3],&e,10);
							} else
							{	pGPS->rgdwSatellitesInViewPRNs[i] = 255;
								pGPS->rgdwSatellitesInViewElevation[i] = -1;
								pGPS->rgdwSatellitesInViewAzimuth[i] = -1;
								pGPS->rgdwSatellitesInViewSignalToNoiseRatio[i] = 0;
							}
						}
					}
					pGPS->dwValidFields |= GPS_VALID_SATELLITES_IN_VIEW_PRNS
							| GPS_VALID_SATELLITES_IN_VIEW_ELEVATION
							| GPS_VALID_SATELLITES_IN_VIEW_AZIMUTH
							| GPS_VALID_SATELLITES_IN_VIEW_SIGNAL_TO_NOISE_RATIO;
				}
#endif
				GPGSVCount = strtol(Commas[2],&e,10);
				if (*e || !*Commas[2]) GPGSVCount = 0;

				if (number > 0 && number <= total)
				{	int c, i;

					if (number == 1)	/* First one of the set? */
					{	TraceLogThread("NMEA", FALSE, "Clearing SatellitesInView for %ld/%ld\n", (long) number, (long) total);
						for (i=0; i<GPS_MAX_SATELLITES; i++)
						{	GPGSVs[i].PRNs = 255;
							GPGSVs[i].Elevation = -1;
							GPGSVs[i].Azimuth = -1;
							GPGSVs[i].SignalToNoiseRatio = 0;
						}
					}
					for (c=3, i=(number-1)*4; c+3<CommaCount; c+=4, i++)
					{	if (i < GPS_MAX_SATELLITES)
						{	if (*Commas[c])
							{	GPGSVs[i].PRNs = strtol(Commas[c],&e,10);
								GPGSVs[i].Elevation = strtol(Commas[c+1],&e,10);
								GPGSVs[i].Azimuth = strtol(Commas[c+2],&e,10);
								GPGSVs[i].SignalToNoiseRatio = strtol(Commas[c+3],&e,10);
							} else
							{	GPGSVs[i].PRNs = 255;
								GPGSVs[i].Elevation = -1;
								GPGSVs[i].Azimuth = -1;
								GPGSVs[i].SignalToNoiseRatio = 0;
							}
						}
					}
					if (number>=total)	/* Got the last sentence? */
					{
					static char Buffer[256];
						Buffer[0] = 0;
						for (i=0; i<(int)GPGSVCount; i++)
							sprintf(strchr(Buffer,'\0'),"%ld ",(long) GPGSVs[i].PRNs);
						TraceLogThread("NMEA", FALSE, "Remembering %ld SatellitesInView at %ld/%ld (%s)\n", (long) GPGSVCount, (long) number, (long) total, Buffer);
						for (i=0; i<GPS_MAX_SATELLITES; i++)
						{	pGPS->rgdwSatellitesInViewPRNs[i] = GPGSVs[i].PRNs;
							pGPS->rgdwSatellitesInViewElevation[i] = GPGSVs[i].Elevation;
							pGPS->rgdwSatellitesInViewAzimuth[i] = GPGSVs[i].Azimuth;
							pGPS->rgdwSatellitesInViewSignalToNoiseRatio[i] = GPGSVs[i].SignalToNoiseRatio;
						}
						pGPS->dwSatellitesInView = GPGSVCount;
						if (GPGSVCount)	/* Any there? */
						{	pGPS->dwValidFields |= 
							GPS_VALID_SATELLITES_IN_VIEW
							| GPS_VALID_SATELLITES_IN_VIEW_PRNS
							| GPS_VALID_SATELLITES_IN_VIEW_ELEVATION
							| GPS_VALID_SATELLITES_IN_VIEW_AZIMUTH
							| GPS_VALID_SATELLITES_IN_VIEW_SIGNAL_TO_NOISE_RATIO;
						}
					}
				} else TraceLogThread("NMEA", FALSE, "Bad GPGSV number(%ld) or > %ld\n", (long) number, (long) total);
			}
		}
#ifdef DOCUMENTATION
	/*        $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43 */
	/*        $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K */
	/* BT-20: $GPVTG,142.07,T,,M,0.00,N,0.0,K*60 */
	/* M-1200 $GPVTG,3.09,T,,M,0.01,N,0.01,K,D*32 */
	/* G6UIM: Holux M-1200 BT GPS using I believe the MTX MT3318 chipset */
Commas[0]    = Track made good
Commas[1]    = Fixed text 'T' indicates that track made good is relative to true north
Commas[2]    = not used
Commas[3]    = not used
Commas[4]    = Speed over ground in knots
Commas[5]    = Fixed text 'N' indicates that speed over ground in in knots
Commas[6]    = Speed over ground in kilometers/hour
Commas[7]    = Fixed text 'K' indicates that speed over ground is in kilometers/hour
#endif
		else if (!strcmp(Input+3,"VTG"))
		{	if (CommaCount != 8 && CommaCount != 9)
			{
#ifndef UNDER_CE
				pGPS->dwBadCommas++;
#endif
TraceLogThread("NMEA", TRUE, "Comma(%s) Got %ld Need 8 or 9\n", Input, (long) CommaCount);
			} else
			{	if (*Commas[5] == 'N')
				{	pGPS->flSpeed = (float) strtod(Commas[4],&e);
					if (!*e && *Commas[4]) pGPS->dwValidFields |= GPS_VALID_SPEED;
				}
			}
		}
#ifdef DOCUMENTATION
From: http://forum.delorme.com/viewtopic.php?t=7185
Postby Kurt » Wed Jan 31, 2007 3:04 pm
$PSTMECH is a DeLorme specific message. We generally don't provide documentation or support for this message since it is not needed as part of the standard NMEA sentence sequence.
$PSTMECH is the message that provides the channel specific status information in our applications.

The general structure is:
$PSTMECH,00,0,00,0,00,0,00,0,00,0,00,0,00,0,00,0,00,0,00,0,00,0,00,0*54
so
$PSTMECH,Chan1 SatId,Chan1 Status, Chan2 SatId, Chan2 Status, ......*checksum

For each repeated channel first is the Sat Id followed by the Channel Status. The Channel Status is a hex value that is bit defined in the following sequence:
Sat used in navigation solution: 0x01
Sat ephemeris available: 0x02
Sat on channel is in track: 0x04
Sat has differential data available: 0x08

The values above are read together to represent the status of the respective satellite in each channel.

#endif
		else if (!strcmp(Input+3,"GSA"))	/* Other source of active satellites? */
		{ }	/* Quietly ignore this one */
		else if (!strcmp(Input+3,"GSV"))	/* Other source of in-view satellites? */
		{ }	/* Quietly ignore this one */
		else if (!strcmp(Input,"$PSTMECH"))
		{ }	/* Quietly ignore this one */
		else
		{
#ifndef UNDER_CE
			pGPS->dwUnsupported++;
#endif
			TraceLogThread("NMEA", FALSE, "Unsupported(%s)\n", Input);
		}
		First = FALSE;
		Input = Next;
	}

	return Input;
}

#ifdef FUTURE
/*	This needs to happen when the GPS is disabled */
	memset(&GPS, 0, sizeof(GPS));
	GPS.dwSize = sizeof(GPS);
#endif

DWORD MyGPSGetPosition(
  HANDLE        hGPSDevice,
  GPS_POSITION *pGPSPosition,
  DWORD         dwMaximumAge,
  DWORD         dwFlags
)
{	__int64 Now = llGetMsec();

	if (Now-LastRead > dwMaximumAge)
	{	if (GPS.dwValidFields)
			TraceLogThread("NMEA", FALSE, "Clearing Stale GPS Data (0x%lX)\n",
							(long) GPS.dwValidFields);
		memset(&GPS, 0, sizeof(GPS));
		GPS.dwSize = sizeof(GPS);
		LastRead = Now;
	}
	*pGPSPosition = GPS;
//	TraceLogThread("NMEA", FALSE, "Returning valid(0x%lX)\n", pGPSPosition->dwValidFields);
	return ERROR_SUCCESS;
}

BOOL NMEANewConnection(void *What, BOOL (*Write)(void *What, int Len, unsigned char *Buf))
{	char GPSdWatch[] = "?WATCH={\"enable\":true,\"nmea\":true}\r\n";
	return Write(What, strlen(GPSdWatch), GPSdWatch);
}

char *NMEAFormatReceive(int Len, unsigned char *Pkt, int *rLen)
{	char *e;
	char safe = Pkt[Len];
	if (safe)
	{	if (safe != '\r' && safe != '\n' && (safe&0xff) != (0xC0&0xff))
			TraceLogThread("NMEA", TRUE, "Missing final NULL, found 0x%lX\n", safe&0xff);
		Pkt[Len] = 0;	/* Force single packet parse */
	}

	GPS.dwSize = sizeof(GPS);
	e = ParseNMEA(Pkt, &GPS);
	if (e)
	{	if (e != Pkt) LastRead = llGetMsec();
		*rLen = e-Pkt;
	} else *rLen = 0;	/* Didn't use anything */
	Pkt[Len] = safe;
	return NULL;
}

BOOL NMEATweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	pcInfo->RfBaud = 0;	/* Not really RF */
	pcInfo->XmitEnabled = FALSE;
	pcInfo->RFtoISEnabled = FALSE;
	pcInfo->IStoRFEnabled = FALSE;
	pcInfo->BeaconingEnabled = FALSE;
	pcInfo->TelemetryEnabled = FALSE;
	return TRUE;
}

static unsigned char *MyStrnChr(size_t len, unsigned char *src, unsigned char c)
{	size_t i;
	for (i=0; i<len; i++)
		if (src[i] == c)
			return &src[i];
	return NULL;
}

static unsigned char *MyStrnStr(size_t len, unsigned char *src, unsigned char *s)
{	size_t i, l=strlen(s);
	if (l > len) return NULL;
	if (!src || !s) return NULL;
	for (i=0; i<=len-l; i++)
		if (src[i] == *s	/* Match the first character? */
		&& !strncmp(&src[i],s,l))	/* Check the string */
			return &src[i];
	return NULL;
}

BOOL TEXTGotPacket(int len, unsigned char *buf)
{	if (len > 1 && buf[len-1] == '\n')	/* End of TEXT Packet? */
	{
		return TRUE;
	} else
	{	//TraceLog(NULL, FALSE, NULL,"TEXT:Got %ld Bytes starting 0x%02lX ending in 0x%02lX\n", (long) len, (unsigned long) *buf, (unsigned long) buf[len-1]);

		if (MyStrnChr(len, buf, '\n'))
		{	//TraceLog(NULL, FALSE, NULL,"TEXT:Got %ld Bytes starting 0x%02lX ending in 0x%02lX - Recovered\n", (long) len, (unsigned long) *buf, (unsigned long) buf[len-1]);
			return TRUE;
		}
	}
	return FALSE;
}

char *TEXTFormatReceive(int Len, unsigned char *Pkt, int *rLen)
{	char *Result;
	int OrgLen = Len;
	unsigned char *p = Pkt, *e;

	if (!Len)
	{	*rLen = 0;
		return NULL;
	}
	e = MyStrnChr(Len, Pkt, '\n');	/* Do we have the ending <LF>? */
	if (!e)
	{	*rLen = 0;	/* Nope, keep them all and wait for <LF> */
		return NULL;
	}
	Len = (e-p);
	while (Len && Pkt[Len-1] == '\r')
		Len--;
	if (!Len)
	{	*rLen = (e-Pkt)+1;	/* only \r and \n?  eat them all! */
		return NULL;
	}

	if (Pkt[0] == '[')	/* Smells like a javAPRSSrvr monitor port */
	{	char *e = MyStrnChr(Len, Pkt, ']');
		if (e++)	/* Skip over ] if found */
		{	Len -= (e-Pkt);	/* Actual packet length */
			if (Len<=0)
			{	*rLen = (e-Pkt)+1;	/* only []?  eat them all! */
				return NULL;
			}
			memmove(Pkt, e, Len);	/* Move the data down */
		}
	}
/*
KG4CWC-9>2VTV6V,K4PKT-3,W4OT-3*,WIDE2-1 <UI>:`l*ql?sk/"3y}
K4PKT-3>APNP50,W4OT-3*,WIDE2-1 <UI C>:!2648.72NP08012.82W#PHG6530/ WIDEn-n Digi
On The Web @ pbpg.org
KJ4ERJ-7>APTT4,WIDE1-1,WIDE2-1 <UI>:T#416,135,159,255,087,079,01001000
KJ4ERJ-7>APTT4,WX4MLB-3*,WIDE2-1 <UI>:T#416,135,159,255,087,079,01001000
KJ4ERJ-7>APTT4,WIDE1-1,WIDE2-1 <UI>:>Lynn's TT4 Beacon
KJ4ERJ-7>APTT4,WX4MLB-3*,WIDE2-1 <UI>:>Lynn's TT4 Beacon
KJ4ERJ-12>APZLWD,WX4MLB-3* <UI R>:>status
KJ4ERJ-12>APZLWD,KJ4ERJ-1,WIDE1* <UI C>:>status
WI8H-9>R8PT6W,WIDE1-1,WIDE2-1 <UI R>:`lF6l#Wk/`"3z}146.850MHz FTM-350_"
WI8H-9>R8PT6W,KJ4ERJ-1,WIDE1*,WIDE2-1 <UI C>:`lF6l#Wk/`"3z}146.850MHz FTM-350_"
KJ4ERJ-12>APZLWD,WX4MLB-3* <UI R>:
*/
	{	char *s = MyStrnStr(Len,Pkt," <UI");	/* From a D710 */
		if (s)
		{	char *e = MyStrnStr(Len-(s-Pkt),s,">:");	/* End of the <UI xxx>: */
			if (e)
			{	memmove(s,e+1,Len-(e-Pkt)-1);	/* Copy the : to end down */
				Len -= (e-s)+1;	/* Take out b<UI> but keep the : */
			}
		}
	}

	if (*Pkt=='$' && Len>=3 && Pkt[Len-3]=='*')	/* $....*cc */
	{	//int tLen;
		TraceLog("NMEA", FALSE, NULL, "Parsing TEXT NMEA(%.*s)\n", (int)Len,Pkt);
		NMEAFormatReceive(Len, Pkt, rLen);	/* Was &tLen */
		return NULL;	/* got nothing from this one as far as APRS */
	}

{	BOOL HasNULL = MyStrnChr(Len, Pkt, '\0') != NULL;
	if (HasNULL)
		PortDumpHex("Packets(NULL)", "TEXT:NULL-Truncated", Len, Pkt);
	TraceLog(NULL, FALSE, NULL, "TEXT:%.*s\n",(int)Len, Pkt);
}

	Result = (char*)malloc(Len+1);
	memcpy(Result, Pkt, Len);
	Result[Len] = '\0';
/*
	Skip over the remainging line breaks and return eaten length
*/
	Len = (e-Pkt)+1;
	while (Len < OrgLen)
	{	if (Pkt[Len] != '\n' && Pkt[Len] != '\r')
			break;
		Len++;
	}
	*rLen = Len;

	return Result;
}

char *TEXTFormatTransmit(char *String, long *rLen, void *Where)
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

BOOL KenwoodTweakConfigPacket(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	size_t i = 0;

	pcInfo->NoUsedPath = TRUE;

	if (!pcInfo->OpenCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->OpenCmds;
#ifdef USE_TIMED_STRINGS
#define DOIT(s) AddSimpleStringEntry(pList,s);
#else
		pcInfo->OpenCmds.Count = 29;
		pcInfo->OpenCmds.Strings = (char **) malloc(sizeof(*pcInfo->OpenCmds.Strings)*pcInfo->OpenCmds.Count);
#define DOIT(s) if (i < pcInfo->OpenCmds.Count) pcInfo->OpenCmds.Strings[i++] = _strdup(s); else TraceLog(pcInfo->Name, TRUE, NULL, "KenwoodTweakConfig Overflowed OpenCmds!");
#endif

DOIT("^C^C^C~!!0")
DOIT("TC 1!?")
DOIT("TC 1!TS 1")
DOIT("TN 2,0!TN 2,0")
DOIT("^M!cmd:!5")
DOIT("DIG OFF")
DOIT("GB 4800")
DOIT("HB 1200")
DOIT("GPST $GPRMC")
DOIT("LTM 10")
DOIT("LTMH OFF")
DOIT("LOC E 0")
DOIT("M ON")
DOIT("MCOM ON")
DOIT("MS OFF")
DOIT("CONO OFF")
DOIT("P 250")
DOIT("UI OFF")
DOIT("UIF NOID")
DOIT("UIT %")
DOIT("X OFF")
DOIT("HEADER OFF")
DOIT("NE ON")
DOIT("NO OFF")
DOIT("ECHO OFF!cmd:!1.0")
DOIT("BEACON EVERY 0!cmd:!1.0")
DOIT("ECHO OFF!cmd:!1.00")
DOIT("BEACON EVERY 0!cmd:!1.00")
DOIT("^C^C^C~!!0")

#undef DOIT
#ifndef USE_TIMED_STRINGS
		if (i != pcInfo->OpenCmds.Count)
		{	TraceLog(pcInfo->Name, TRUE, NULL, "KenwoodTweakConfig Resetting %ld OpenCmds to %ld\n",
					(long) pcInfo->OpenCmds.Count, (long) i);
			pcInfo->OpenCmds.Count = i;
		}
#endif
	}

	if (!pcInfo->CloseCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->CloseCmds;
#ifdef USE_TIMED_STRINGS
		AddSimpleStringEntry(pList,"^M!cmd:!1");
		AddSimpleStringEntry(pList,"LTM 0");
		AddSimpleStringEntry(pList,"TC 1!TS 1");
		AddSimpleStringEntry(pList,"TN 0,0!TN 0,0");
#else
		pcInfo->CloseCmds.Count = 4;
		pcInfo->CloseCmds.Strings = (char **) malloc(sizeof(*pcInfo->CloseCmds.Strings)*pcInfo->CloseCmds.Count);
		pcInfo->CloseCmds.Strings[0] = _strdup("^M!cmd:!1");
		pcInfo->CloseCmds.Strings[1] = _strdup("LTM 0");
		pcInfo->CloseCmds.Strings[2] = _strdup("TC 1!TS 1");
		pcInfo->CloseCmds.Strings[3] = _strdup("TN 0,0!TN 0,0");
#endif
	}

	return TRUE;
}
BOOL KenwoodTweakConfigAPRS(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	size_t i = 0;

	pcInfo->NoUsedPath = TRUE;

	if (!pcInfo->OpenCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->OpenCmds;
#ifdef USE_TIMED_STRINGS
#define DOIT(s) AddSimpleStringEntry(pList,s);
#else
		pcInfo->OpenCmds.Count = 31;
		pcInfo->OpenCmds.Strings = (char **) malloc(sizeof(*pcInfo->OpenCmds.Strings)*pcInfo->OpenCmds.Count);

#define DOIT(s) if (i < pcInfo->OpenCmds.Count) pcInfo->OpenCmds.Strings[i++] = _strdup(s); else TraceLog(pcInfo->Name, TRUE, NULL, "KenwoodTweakConfig Overflowed OpenCmds!");
#endif

		DOIT("^C^C^C~!!0")
		DOIT("TC 1!?!1.0")
		DOIT("TC 1!TS 1!1.0")
		DOIT("TN 1,0!TN 1,0")
		DOIT("TC 0!!0.050")
		DOIT("^M!cmd:!1")
		DOIT("DIG OFF")
		DOIT("GB 4800")
		DOIT("HB 1200")
		DOIT("GPST $GPRMC")
		DOIT("LTM 0")
		DOIT("LTMH OFF")
		DOIT("LOC E 0")
		DOIT("M ON")
		DOIT("MCOM ON")
		DOIT("MS OFF")
		DOIT("CONO OFF")
		DOIT("P 250")
		DOIT("UI OFF")
		DOIT("UIF NOID")
		DOIT("UIT %")
		DOIT("X OFF")
		DOIT("HEADER OFF")
		DOIT("NE ON")
		DOIT("NO OFF")
		DOIT("ECHO OFF!cmd:!1.0")
		DOIT("BEACON EVERY 0!cmd:!1.0")
		DOIT("ECHO OFF!cmd:!1.00")
		DOIT("BEACON EVERY 0!cmd:!1.00")
		DOIT("TC 1!?EH!1")
		DOIT("TC 1!TS 1!1.00")

#undef DOIT
#ifndef USE_TIMED_STRINGS
		if (i != pcInfo->OpenCmds.Count)
		{	TraceLog(pcInfo->Name, TRUE, NULL, "KenwoodTweakConfig Resetting %ld OpenCmds to %ld\n",
					(long) pcInfo->OpenCmds.Count, (long) i);
			pcInfo->OpenCmds.Count = i;
		}
#endif
	}

	if (!pcInfo->CloseCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->CloseCmds;
#ifdef USE_TIMED_STRINGS
		AddSimpleStringEntry(pList,"TC 0!!0.050");
		AddSimpleStringEntry(pList,"^M!cmd:!1");
		AddSimpleStringEntry(pList,"LTM 0");
		AddSimpleStringEntry(pList,"TC 1!?EH!1");
		AddSimpleStringEntry(pList,"TC 1!TS 1");
		AddSimpleStringEntry(pList,"TN 0,0!TN 0,0");
#else
		pcInfo->CloseCmds.Count = 6;
		pcInfo->CloseCmds.Strings = (char **) malloc(sizeof(*pcInfo->CloseCmds.Strings)*pcInfo->CloseCmds.Count);
		pcInfo->CloseCmds.Strings[0] = _strdup("TC 0!!0.050");
		pcInfo->CloseCmds.Strings[1] = _strdup("^M!cmd:!1");
		pcInfo->CloseCmds.Strings[2] = _strdup("LTM 0");
		pcInfo->CloseCmds.Strings[3] = _strdup("TC 1!?EH!1");
		pcInfo->CloseCmds.Strings[4] = _strdup("TC 1!TS 1");
		pcInfo->CloseCmds.Strings[5] = _strdup("TN 0,0!TN 0,0");
#endif
	}

	return TRUE;
}



BOOL KenwoodGotPacket(int len, unsigned char *buf)
{	if (len && (buf[len] == '\n' || buf[len] == '\r'))
		return TRUE;
	else if (len > 1 && (buf[len-1] == '\n' || buf[len-1] == '\r'))	/* End of TEXT Packet? */
		return TRUE;
	else if (len >= 4 && !strncmp(buf,"cmd:",4))
		return TRUE;
	else
	{	//TraceLog(NULL, FALSE, NULL,"TEXT:Got %ld Bytes starting 0x%02lX ending in 0x%02lX\n", (long) len, (unsigned long) *buf, (unsigned long) buf[len-1]);

		if (MyStrnChr(len, buf, '\r') || MyStrnChr(len, buf, '\n') )
		{	//TraceLog(NULL, FALSE, NULL,"TEXT:Got %ld Bytes starting 0x%02lX ending in 0x%02lX - Recovered\n", (long) len, (unsigned long) *buf, (unsigned long) buf[len-1]);
			return TRUE;
		}
	}
	return FALSE;
}

char *KenwoodFormatReceive(int Len, unsigned char *Pkt, int *rLen)
{	char *Result;
	int OrgLen = Len;
	unsigned char *p = Pkt, *e;

	if (!Len)
	{	*rLen = 0;
		return NULL;
	}
	if (*p == '\n' || *p == '\r')
	{	*rLen = 1;	/* Eat the left-over delimiter */
		return NULL;
	}
	e = MyStrnChr(Len, Pkt, '\n');	/* Do we have the ending <LF>? */
	if (!e) e = MyStrnChr(Len, Pkt, '\r');	/* Or <CR> */
	if (!e)
	{	if (Len >= 4 && !strncmp(Pkt, "cmd:", 4)) *rLen = 4;	/* Eat lone cmd: */
		else *rLen = 0;	/* Nope, keep them all and wait for <CR> or <LF> */
		return NULL;
	}
	Len = (e-Pkt);
	while ((e-Pkt)<=OrgLen && (*e=='\n' || *e=='\r')) e++;
	*rLen = (e-Pkt);	/* Maximum to eat, including delimiter(s) */

	while (Len && (Pkt[Len-1] == '\r' || Pkt[Len-1] == '\n'))
		Len--;	/* Shorten the considered length */

/*
	Take out out cmd: prompts that are lurking
*/
	{	char *s;
		while ((s=MyStrnStr(Len,Pkt,"cmd:")) != NULL)
		{	Len -= 4;
			memmove(s,s+4,Len-(s-Pkt));
		}
	}

	if (!Len) return NULL;	/* Empty packets can now go away */

	Pkt[Len] = '\0';	/* Null terminate the packet, stomping earliest delimeter */
	
	if (*Pkt=='$')	/* Look like an NMEA lead-in? */
	{	if (Len>=3 && Pkt[Len-3]=='*')	/* $....*cc */
		{	int tLen;
			TraceLog("NMEA", FALSE, NULL, "Parsing Kenwood NMEA(%.*s)\n", (int)Len,Pkt);
			NMEAFormatReceive(Len, Pkt, &tLen);
		} else 
		{	TraceLog("NMEA", FALSE, NULL, "Dumping Kenwood Corrupted NMEA(%.*s)\n", (int)Len,Pkt);
			PortDumpHex2(NULL, TRUE, "Corrupted NMEA:", Len, Pkt);
		}
		return NULL;	/* Nothing for APRS here */
	}

/*
KJ4ERJ-14>GPS <UI R>:$GPRMC,011934.000,A,2759.7929,N,08039.540
*/

	if (!strncmp(Pkt,"KJ4ERJ-14",9)	/* Needs to be MyCall */
		&& !strncmp(Pkt+9,">GPS <UI R>:$", 13)	/* Kenwood Menu 604 echo */
		&& Len>=3 && Pkt[Len-3]=='*')	/* $....*cc */
	{	int tLen;
		TraceLog("NMEA", FALSE, NULL, "Parsing Kenwood NMEA(%.*s)\n", (int)Len,Pkt);
		NMEAFormatReceive(Len-(12+9), Pkt+(12+9), &tLen);	/* 9s are strlen(MyCall) */
		return NULL;	/* Got nothing from this one as far as APRS! */
	}

/*
KG4CWC-9>2VTV6V,K4PKT-3,W4OT-3*,WIDE2-1 <UI>:`l*ql?sk/"3y}
K4PKT-3>APNP50,W4OT-3*,WIDE2-1 <UI C>:!2648.72NP08012.82W#PHG6530/ WIDEn-n Digi
On The Web @ pbpg.org
KJ4ERJ-7>APTT4,WIDE1-1,WIDE2-1 <UI>:T#416,135,159,255,087,079,01001000
KJ4ERJ-7>APTT4,WX4MLB-3*,WIDE2-1 <UI>:T#416,135,159,255,087,079,01001000
KJ4ERJ-7>APTT4,WIDE1-1,WIDE2-1 <UI>:>Lynn's TT4 Beacon
KJ4ERJ-7>APTT4,WX4MLB-3*,WIDE2-1 <UI>:>Lynn's TT4 Beacon
KJ4ERJ-12>APZLWD,WX4MLB-3* <UI R>:>status
KJ4ERJ-12>APZLWD,KJ4ERJ-1,WIDE1* <UI C>:>status
WI8H-9>R8PT6W,WIDE1-1,WIDE2-1 <UI R>:`lF6l#Wk/`"3z}146.850MHz FTM-350_"
WI8H-9>R8PT6W,KJ4ERJ-1,WIDE1*,WIDE2-1 <UI C>:`lF6l#Wk/`"3z}146.850MHz FTM-350_"
KJ4ERJ-12>APZLWD,WX4MLB-3* <UI R>:
*/
	{	char *s = MyStrnStr(Len,Pkt," <UI");	/* From a D710 */
		if (s)
		{	char *e = MyStrnStr(Len-(s-Pkt),s,">:");	/* End of the <UI xxx>: */
			if (e)
			{	memmove(s,e+1,Len-(e-Pkt)-1);	/* Copy the : to end down */
				Len -= (e-s)+1;	/* Take out b<UI> but keep the : */
			} else
			{	PortDumpHex(NULL, "Missing <UI...>:", (e-Pkt), Pkt);
				Result = NULL;
				Len = 0;
			}
		}
#ifdef COMPLAIN_ABOUT_KENWOOD_APRS
		else
		{	PortDumpHex(NULL, "Missing <UI...", (e-Pkt), Pkt);
//			Result = NULL;
//			Len = 0;
		}
#endif
	}

	if (Len)
	{
{	BOOL HasNULL = MyStrnChr(Len, Pkt, '\0') != NULL;
	if (HasNULL)
		PortDumpHex("Packets(NULL)", "Kenwood:NULL-Truncated", Len, Pkt);
	PortDumpHex(NULL, "Kenwood", Len, Pkt);
}
		Result = (char*)malloc(Len+1);
		memcpy(Result, Pkt, Len);
		Result[Len] = '\0';
	} else Result = NULL;

	return Result;
}

char *KenwoodFormatTransmitAPRS(char *String, long *rLen, void *Where)
{	char *Response = NULL;
	size_t len = strlen(String);
	size_t Available = 1 + 7+10+1 + 8+10+5+8*10+1 + 4+1 + len*4+1 + 1 + 1;
	char *Next, *Output = (char*)malloc(Available);
	char *eCall, *eDest, *ePath;
/*
	KJ4ERJ>APWWnn,path1,path2,path3:<payload>
	KJ4ERJ>APWWnn:<payload>
*/
	eCall = strchr(String,'>');
	if (!eCall) return NULL;
	eDest = strchr(eCall,',');
	ePath = strchr(eCall,':');
	if (!ePath) return NULL;
	if (!eDest || eDest > ePath) eDest = ePath;

/*
	TC 0
	MYCALL whatever		(This should be remembered on only sent on change)
	UNPROTO whatever VIA path		(This should only be sent on change)
	CONV
	<APRS Payload>
	^C
	TC 1
	TC 1
*/
	if (!PortCommand(Where, "TC 0!!0.050", NULL)
	&& !PortCommand(Where, "^M~!cmd:!1", NULL))
	{
		StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
						"MYCALL %.*s!cmd:!1", (int)(eCall-String), String);
		if (!PortCommand(Where, Output, &Response))
		{	StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
							"UNPROTO %.*s", (int)(eDest-eCall)-1, eCall+1);
			if (eDest != ePath)
			{	char *u, *UnUsed = eDest;
				while ((u=strstr(UnUsed,"*,")) != NULL && u < ePath)
				{	UnUsed = u+1;
				}
				if (UnUsed != eDest)
					TraceLogThread(NULL, TRUE, "Suppressing Used(%.*s) From(%.*s)\n",
									(int) (UnUsed-eDest)-1, eDest+1,
									(int) (ePath-eDest)-1, eDest+1);
				if (UnUsed < ePath)
					StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
								" VIA %.*s", (int)(ePath-UnUsed)-1, UnUsed+1);
			}
			StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
							"!cmd!1", (int)(ePath-eDest)-1, eDest+1);
			if (!PortCommand(Where, Output, NULL))
			{	if (!PortCommand(Where, "CONV!!0.050", NULL))
				{	char *p;
					Next = Output;
					for (p=ePath+1; *p; p++)
					{	if (*p == '^')
						{	*Next++ = '^'; *Next++='^';	/* ^^ gives ^ */
						} else if (*p == '!')
						{	StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
												"^%03ld", (long) '!');	/* ^nnn gives char */
						} else *Next++ = *p;
					}
					strcpy(Next, "!!0.050");	/* terminate the output command including null */
					PortCommand(Where, Output, NULL);
					PortCommand(Where, "^003~!cmd:!1", NULL);
				}
			}
		}
/*
	See if we need to reset MYCALL
*/
#ifdef KEEP_MYCALL
		if (Response)
		{	char *m = strstr(Response, "MYCALL ");
			if (m)
			{	char *w = strstr(m,"was ");
				if (w)
				{	char *c = w+4, *e;
					while (isspace(*c&0xff)) c++;
					e = c; while (isprint(*e&0xff)) e++;
					if (((e-c)!=(eCall-String) || strncmp(c,String,(e-c)))
					&& strncmp(c,"NOCALL",(e-c)))
					{	StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
						"MYCALL %.*s!cmd:!1", (int)(e-c), c);
TraceLog(NULL,TRUE,NULL,"Reset call from %.*s to %.*s\n", (int)(eCall-String), String, (int)(e-c), c);
						if (PortCommand(Where, Output, &Response))
							TraceLog(NULL,TRUE,NULL,"Failed To Reset MYCALL to %.*s\n", (int)(e-c),c);
					}
else TraceLog(NULL,TRUE,NULL,"Call Reset Not Necessary, was %.*s\n", (int)(e-c),c);
				} else PortDumpHex(NULL, "Missing was", -1, Response);
			} else PortDumpHex(NULL, "Missing MYCALL", -1, Response);
			free(Response);	/* From the MYCALL command */
		}
#endif
		PortCommand(Where, "TC 1!?EH!1", NULL);
		PortCommand(Where, "TC 1!TS 1!1", NULL);
	}
	*rLen = 0;
	return NULL;
}

char *KenwoodFormatTransmitPacket(char *String, long *rLen, void *Where)
{	char *Response = NULL;
	size_t len = strlen(String);
	size_t Available = 1 + 7+10+1 + 8+10+5+8*10+1 + 4+1 + len*4+1 + 1 + 1;
	char *Next, *Output = (char*)malloc(Available);
	char *eCall, *eDest, *ePath;
/*
	KJ4ERJ>APWWnn,path1,path2,path3:<payload>
	KJ4ERJ>APWWnn:<payload>
*/
	eCall = strchr(String,'>');
	if (!eCall) return NULL;
	eDest = strchr(eCall,',');
	ePath = strchr(eCall,':');
	if (!eDest || eDest > ePath) eDest = ePath;

/*
	^C	 (Why is this not sent?)
	MYCALL whatever		(This should be remembered on only sent on change)
	UNPROTO whatever VIA path		(This should only be sent on change)
	CONV
	<APRS Payload>
	^C
*/
	StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
					"MYCALL %.*s!cmd:!1", (int)(eCall-String), String);
	if (!PortCommand(Where, Output, &Response))
	{	StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
						"UNPROTO %.*s", (int)(eDest-eCall)-1, eCall+1);
		if (eDest != ePath)
		{	char *u, *UnUsed = eDest;
			while ((u=strstr(UnUsed,"*,")) != NULL && u < ePath)
			{	UnUsed = u+1;
			}
			if (UnUsed != eDest)
				TraceLogThread(NULL, TRUE, "Suppressing Used(%.*s) From(%.*s)\n",
								(int) (UnUsed-eDest)-1, eDest+1,
								(int) (ePath-eDest)-1, eDest+1);
			if (UnUsed < ePath)
				StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
							" VIA %.*s", (int)(ePath-UnUsed)-1, UnUsed+1);
		}
		StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
						"!cmd!1", (int)(ePath-eDest)-1, eDest+1);
		if (!PortCommand(Where, Output, NULL))
		{	if (!PortCommand(Where, "CONV!!0.050", NULL))
			{	char *p;
				Next = Output;
				for (p=ePath+1; *p; p++)
				{	if (*p == '^')
					{	*Next++ = '^'; *Next++='^';	/* ^^ gives ^ */
					} else if (*p == '!')
					{	StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
											"^%03ld", (long) '!');	/* ^nnn gives char */
					} else *Next++ = *p;
				}
				strcpy(Next, "!!0.050");	/* terminate the output command including null */
				PortCommand(Where, Output, NULL);
				PortCommand(Where, "^003~!cmd:!1", NULL);
			}
		}
	}

	*rLen = 0;
	return NULL;
}

/******************************************************************/
BOOL FTM350TweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	pcInfo->XmitEnabled = FALSE;
	pcInfo->IStoRFEnabled = FALSE;
	EmptySimpleStringList(&pcInfo->OpenCmds);	/* Not supported */
	EmptySimpleStringList(&pcInfo->CloseCmds);	/* Not supported */

//	ShowTraceLog(pcInfo->Name, TRUE);	/* Force it visible and enabled */

	return TRUE;
}

BOOL FTM350GotPacket(int len, unsigned char *buf)
{	char *cr = buf;
	char *lf = buf;

	if (*buf == '\r' || *buf == '\n') return TRUE;
	do
	{	cr = MyStrnChr(len-(lf-buf), lf==buf?lf:lf+1, '\r');
		lf = MyStrnChr(len-(lf-buf), lf==buf?lf:lf+1, '\n');
		if (!cr && lf) cr = lf;
		if (cr && !lf) lf = cr;
		if (!cr && !lf) return FALSE;	/* Not even a first line */
	} while (cr == buf || cr[-1] != ':');	/* Find end of first line */

	cr = MyStrnChr(len-(lf-buf), lf, '\r');	/* End of next line */
	lf = MyStrnChr(len-(lf-buf), lf, '\n');
	if (!cr && lf) cr = lf;
	if (cr && !lf) lf = cr;
	if (!cr && !lf) return FALSE;	/* No following line */

	return TRUE;	/* Got a first and following line */
}

char *FTM350FormatReceive(int Len, unsigned char *Pkt, int *rLen)
{	char *cr = Pkt, *cr2;
	char *lf = Pkt, *lf2;
	char *Result;
	int Len1, Len2;

	if (!Len)
	{	*rLen = 0;
		return NULL;
	}
	if (*Pkt == '\n' || *Pkt == '\r')
	{	*rLen = 1;	/* Eat the left-over delimiter */
		return NULL;
	}

	cr = MyStrnChr(Len-(lf-Pkt), lf, '\r');
	lf = MyStrnChr(Len-(lf-Pkt), lf, '\n');
	if (!cr && lf) cr = lf;
	if (cr && !lf) lf = cr;
	if (!cr && !lf)
	{	*rLen = 0;
		return NULL;	/* Not even a first line yet */
	}
	if (cr == Pkt || cr[-1] != ':')	/* Not a first line, eat it */
	{	*rLen = (lf-Pkt)+1;
		return NULL;
	}
	if (cr > lf) PortDumpHex(NULL, "reverse(lf/cr)", cr-lf+1, lf);

	cr2 = MyStrnChr(Len-(lf-Pkt)-1, lf+1, '\r');	/* End of next line */
	lf2 = MyStrnChr(Len-(lf-Pkt)-1, lf+1, '\n');
	if (!cr2 && lf2) cr2 = lf2;
	if (cr2 && !lf2) lf2 = cr2;
	if (!cr2 && !lf2)
	{	*rLen = 0;
		return NULL;	/* No second line yet */
	}
	if (cr2 > lf2) PortDumpHex(NULL, "reverse2(lf/cr)", cr2-lf2+1, lf2);

	*rLen = (lf2-Pkt)+1;	/* No matter what, eat all of this */

	PortDumpHex(NULL, "2-Lines", (lf2-Pkt)+1, Pkt);

	Len1 = cr-Pkt;		/* Length of line 1 */
	Len2 = (cr2-lf)-1;	/* Length of line 2 */

	if (Len1+Len2+1 > *rLen) PortDumpHex(NULL, "TooSmall(*rLen)", Len, Pkt);

	Result = (char*)malloc(max(*rLen,Len1+Len2+1));	/* Max length buffer */
	memcpy(Result, Pkt, Len1);	/* First line */
	memcpy(Result+Len1, lf+1, Len2);	/* Second line */
	Result[Len1+Len2] = '\0';	/* Null terminate it */

{	BOOL HasNULL = MyStrnChr(Len1+Len2, Result, '\0') != NULL;
	if (HasNULL)
		PortDumpHex("Packets(NULL)", "FTM350:NULL-Truncated", (int)Len1+Len2, Result);
	PortDumpHex(NULL, "1-Line", strlen(Result), Result);
}

/*
AE4XO>APRS,GA2-2 [12/19/10 13:32:01] <UI>:
;147.300/r*120346z3237.51N/08341.68WrPL107.2

AE4XO>APRS,AE4XO-2*,GA2-1 [12/19/10 13:32:02] <UI>:
;147.300/r*120346z3237.51N/08341.68WrPL107.2

K9QV>SS4S9P,AE4XO-2,WIDE2* [12/19/10 13:32:13] <UI>:
`q&9'_Q'/"E0}
WA4ORT>APTW01,AE4XO-2*,WIDE3-2 [12/19/10 13:32:19] <UI>:
_12191331c353s002g006t047r000p027P005h..b10124tU2k
WA4ORT>APTW01,WIDE3-2 [12/19/10 13:32:21] <UI>:
_12191331c353s002g006t047r000p027P005h..b10124tU2k
WA4ORT>APTW01,WIDE3-3 [12/19/10 13:32:21] <UI>:
!3238.94N/08339.34W_CGARC
N4FGA>APU25N,AB4KN-2,AE4XO-2*,WIDE3-1 [12/19/10 13:32:40] <UI C>:
=3325.80N/08428.25W-N4FGA-David, Fayetteville, Ga.!SN! {UIV32N}

K9QV>SS4U8S,AE4XO-2,WIDE2* [12/19/10 13:33:13] <UI>:
`q&S'KQ'/"GG}
AB4KN>APU25N,AB4KN-2,AE4XO-2*,WIDE3-2 [12/19/10 13:33:36] <UI R>:
@191832z3320.70N/08426.74W_359/006g010t047r000P000p000h49b10163

WM4B-9>SR3Q8R,WM4B,WIDE1*,WIDE2-1 [12/19/10 13:33:41] <UI C>:
`oC"l!yk/`"4d}146.850MHZ_"
WD4JKH-2>S2UU5P,AE4XO-2,WIDE1*,WIDE2-1 [12/19/10 13:33:44] <UI>:
`oFpl >/"45}*/

	{	char *bs = MyStrnStr(Len1,Result," [");	/* From an FTM-350 */
		if (bs)
		{	char *be = MyStrnStr(Len1-(bs-Result),bs,"] ");	/* End of the [mm/dd/yy hh:mm:ss] */
			if (be)
			{	memmove(bs,be+2,Len1-(be-Result)-2+Len2+1);	/* Copy the : to end down */
				Len1 -= (be-bs)+2;	/* Take out [...] */
			} else
			{	PortDumpHex(NULL, "Missing ...hh:mm:ss]", Len1+Len2, Result);
				Result = NULL;
			}
		}
		else
		{	PortDumpHex(NULL, "Missing [mm/dd/yy...", Len1+Len2, Result);
			Result = NULL;
		}
	}

	{	char *s = MyStrnStr(Len1,Result,"<UI");	/* From an FTM-350 */
		if (s)
		{	char *e = MyStrnStr(Len1-(s-Result),s,">:");	/* End of the <UI xxx>: */
			if (e)
			{	while (s>Result && s[-1] == ' ') s--;
				memmove(s,e+1,Len1-(e-Result)-1+Len2+1);	/* Copy the : to end down */
				Len1 -= (e-s)+1;	/* Take out b<UI> but keep the : */
			} else
			{	PortDumpHex(NULL, "Missing UI...>:", Len1+Len2, Result);
				Result = NULL;
			}
		}
		else
		{	PortDumpHex(NULL, "Missing <UI...", Len1+Len2, Result);
			Result = NULL;
		}
	}

	if (Result) PortDumpHex(NULL, "FTM-350", strlen(Result), Result);

	return Result;
}

char *FTM350FormatTransmit(char *String, long *rLen, void *Where)
{	*rLen = 0;
	return NULL;
}

/*	FTM-350 from WM4B in ???? mode
AE4XO>APRS,GA2-2 [12/19/10 13:32:01] <UI>:
;147.300/r*120346z3237.51N/08341.68WrPL107.2

AE4XO>APRS,AE4XO-2*,GA2-1 [12/19/10 13:32:02] <UI>:
;147.300/r*120346z3237.51N/08341.68WrPL107.2

K9QV>SS4S9P,AE4XO-2,WIDE2* [12/19/10 13:32:13] <UI>:
`q&9'_Q'/"E0}
WA4ORT>APTW01,AE4XO-2*,WIDE3-2 [12/19/10 13:32:19] <UI>:
_12191331c353s002g006t047r000p027P005h..b10124tU2k
WA4ORT>APTW01,WIDE3-2 [12/19/10 13:32:21] <UI>:
_12191331c353s002g006t047r000p027P005h..b10124tU2k
WA4ORT>APTW01,WIDE3-3 [12/19/10 13:32:21] <UI>:
!3238.94N/08339.34W_CGARC
N4FGA>APU25N,AB4KN-2,AE4XO-2*,WIDE3-1 [12/19/10 13:32:40] <UI C>:
=3325.80N/08428.25W-N4FGA-David, Fayetteville, Ga.!SN! {UIV32N}

K9QV>SS4U8S,AE4XO-2,WIDE2* [12/19/10 13:33:13] <UI>:
`q&S'KQ'/"GG}
AB4KN>APU25N,AB4KN-2,AE4XO-2*,WIDE3-2 [12/19/10 13:33:36] <UI R>:
@191832z3320.70N/08426.74W_359/006g010t047r000P000p000h49b10163

WM4B-9>SR3Q8R,WM4B,WIDE1*,WIDE2-1 [12/19/10 13:33:41] <UI C>:
`oC"l!yk/`"4d}146.850MHZ_"
WD4JKH-2>S2UU5P,AE4XO-2,WIDE1*,WIDE2-1 [12/19/10 13:33:44] <UI>:
`oFpl >/"45}
*/


/*	FTM-350 from WM4B in Waypoint mode
$GPWPL,3237.68,N,08339.43,W,KI4GCA-2*38
$GPWPL,3237.68,N,08339.43,W,KI4GCA-2*38
$GPWPL,3231.84,N,08339.06,W,WM4B-9*29
$GPWPL,3231.85,N,08339.06,W,WM4B-9*28
$GPWPL,3237.12,N,08341.56,W,AF1G*23
$GPWPL,3237.12,N,08341.56,W,AF1G*23
$GPWPL,3235.99,N,08337.96,W,KF4QVB*22
$GPWPL,3231.82,N,08339.06,W,WM4B-9*2F
$GPWPL,3231.82,N,08339.06,W,WM4B-9*2F
*/


/*	VX8-GR from G4ILO in Waypoint mode
$GPWPL,5445.68,N,00301.91,W,GB3AS*79
$GPWPL,5439.60,N,00322.67,W,G4ILO*6F
$GPWPL,5445.68,N,00301.91,W,GB3AS*79
$GPWPL,5439.60,N,00322.67,W,G4ILO*6F
$GPWPL,5445.68,N,00301.91,W,GB3AS*79
$GPWPL,5439.60,N,00322.67,W,G4ILO*6F
$GPWPL,5439.60,N,00322.67,W,G4ILO-L*0E
$GPWPL,5439.60,N,00322.66,W,G4ILO-12*40
$GPWPL,5445.68,N,00301.91,W,GB3AS*79
$GPWPL,5439.60,N,00322.67,W,G4ILO*6F
$GPWPL,5445.68,N,00301.91,W,GB3AS*79
$GPWPL,5439.60,N,00322.67,W,G4ILO*6F
*/
/*	VX8-GR from G4ILO in GPS mode
$GPGGA,162917.402,5439.5976,N,00322.6638,W,0,00,99.9,00083.4,M,0049.9,M,000.0,0000*51
$GPGGA,162918.402,5439.5976,N,00322.6638,W,0,00,99.9,00083.4,M,0049.9,M,000.0,0000*5E
$GPRMC,162918.402,V,5439.5976,N,00322.6638,W,9999.99,999.99,291210,,*3B
$GPGGA,162919.403,5439.5976,N,00322.6638,W,0,00,99.9,00083.4,M,0049.9,M,000.0,0000*5E
$GPGGA,162920.402,5439.5976,N,00322.6638,W,0,00,99.9,00083.4,M,0049.9,M,000.0,0000*55
$GPRMC,162920.402,V,5439.5976,N,00322.6638,W,9999.99,999.99,291210,,*30
*/


BOOL D700TEXTTweakConfig(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	pcInfo->XmitEnabled = FALSE;
	pcInfo->IStoRFEnabled = FALSE;

	if (!pcInfo->OpenCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->OpenCmds;
		AddSimpleStringEntry(pList, "^M~");
		AddSimpleStringEntry(pList, "^M~");
		AddSimpleStringEntry(pList, "LOC E 0");
		AddSimpleStringEntry(pList, "LTMON 5");
		AddSimpleStringEntry(pList, "LTMHEAD OFF");
		AddSimpleStringEntry(pList, "GPSTEXT $GPRMC");
		AddSimpleStringEntry(pList, "XFLOW OFF");
	}

	if (!pcInfo->CloseCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->CloseCmds;
		AddSimpleStringEntry(pList, "~!!0");
	}

	return TRUE;
}

char *D700TEXTFormatTransmit(char *String, long *rLen, void *Where)
{	*rLen = 0;
	return NULL;
}

BOOL ConverseTweakConfigPacket(HWND hwnd, PORT_CONFIG_INFO_S *pcInfo)
{	size_t i = 0;

	pcInfo->NoUsedPath = TRUE;

	if (!pcInfo->OpenCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->OpenCmds;
		AddSimpleStringEntry(pList,"^C!cmd:!1");
	}

	if (!pcInfo->CloseCmds.Count)
	{	STRING_LIST_S *pList = &pcInfo->CloseCmds;
		AddSimpleStringEntry(pList,"^C!cmd:!1");
	}

	return TRUE;
}

char *ConverseFormatTransmitPacket(char *String, long *rLen, void *Where)
{	char *Response = NULL;
	size_t len = strlen(String);
	size_t Available = 1 + 7+10+1 + 8+10+5+8*10+1 + 4+1 + len*4+1 + 1 + 1;
	char *Next, *Output = (char*)malloc(Available);
	char *eCall, *eDest, *ePath;
/*
	KJ4ERJ>APWWnn,path1,path2,path3:<payload>
	KJ4ERJ>APWWnn:<payload>
*/
	eCall = strchr(String,'>');
	if (!eCall) return NULL;
	eDest = strchr(eCall,',');
	ePath = strchr(eCall,':');
	if (!eDest || eDest > ePath) eDest = ePath;

/*
	^C	 (Why is this not sent?)
	MYCALL whatever		(This should be remembered on only sent on change)
	UNPROTO whatever VIA path		(This should only be sent on change)
	CONV
	<APRS Payload>
	^C
*/
	StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
					"MYCALL %.*s!cmd:!1", (int)(eCall-String), String);
	if (!PortCommand(Where, Output, &Response))
	{	StringCbPrintfExA(Output, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
						"UNPROTO %.*s", (int)(eDest-eCall)-1, eCall+1);
		if (eDest != ePath)
		{	char *u, *UnUsed = eDest;
			while ((u=strstr(UnUsed,"*,")) != NULL && u < ePath)
			{	UnUsed = u+1;
			}
			if (UnUsed != eDest)
				TraceLogThread(NULL, TRUE, "Suppressing Used(%.*s) From(%.*s)\n",
								(int) (UnUsed-eDest)-1, eDest+1,
								(int) (ePath-eDest)-1, eDest+1);
			if (UnUsed < ePath)
				StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
							" VIA %.*s", (int)(ePath-UnUsed)-1, UnUsed+1);
		}
		StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
						"!cmd!1", (int)(ePath-eDest)-1, eDest+1);
		if (!PortCommand(Where, Output, NULL))
		{	if (!PortCommand(Where, "CONV!!0.050", NULL))
			{	char *p;
				Next = Output;
				for (p=ePath+1; *p; p++)
				{	if (*p == '^')
					{	*Next++ = '^'; *Next++='^';	/* ^^ gives ^ */
					} else if (*p == '!')
					{	StringCbPrintfExA(Next, Available, &Next, &Available, STRSAFE_IGNORE_NULLS,
											"^%03ld", (long) '!');	/* ^nnn gives char */
					} else *Next++ = *p;
				}
				strcpy(Next, "!!0.050");	/* terminate the output command including null */
				PortCommand(Where, Output, NULL);
				PortCommand(Where, "^003~!cmd:!1", NULL);
			}
		}
	}

	*rLen = 0;
	return NULL;
}

char *ConverseFormatReceive(int Len, unsigned char *Pkt, int *rLen)
{	char *Result;
	int OrgLen = Len;
	unsigned char *p = Pkt, *e;

	if (!Len)
	{	*rLen = 0;
		return NULL;
	}
	if (*p == '\n' || *p == '\r')
	{	*rLen = 1;	/* Eat the left-over delimiter */
		return NULL;
	}
	e = MyStrnChr(Len, Pkt, '\n');	/* Do we have the ending <LF>? */
	if (!e) e = MyStrnChr(Len, Pkt, '\r');	/* Or <CR> */
	if (!e)
	{	if (Len >= 4 && !strncmp(Pkt, "cmd:", 4)) *rLen = 4;	/* Eat lone cmd: */
		else *rLen = 0;	/* Nope, keep them all and wait for <CR> or <LF> */
		return NULL;
	}
	Len = (e-Pkt);
	while ((e-Pkt)<=OrgLen && (*e=='\n' || *e=='\r')) e++;
	*rLen = (e-Pkt);	/* Maximum to eat, including delimiter(s) */

	while (Len && (Pkt[Len-1] == '\r' || Pkt[Len-1] == '\n'))
		Len--;	/* Shorten the considered length */

/*
	Take out out cmd: prompts that are lurking
*/
	{	char *s;
		while ((s=MyStrnStr(Len,Pkt,"cmd:")) != NULL)
		{	Len -= 4;
			memmove(s,s+4,Len-(s-Pkt));
		}
	}

	if (!Len) return NULL;	/* Empty packets can now go away */

	Pkt[Len] = '\0';	/* Null terminate the packet, stomping earliest delimeter */
	
	if (*Pkt=='$' && Len>=3 && Pkt[Len-3]=='*')	/* $....*cc */
	{	int tLen;
		TraceLog("NMEA", FALSE, NULL, "Parsing Converse NMEA(%.*s)\n", (int)Len,Pkt);
		NMEAFormatReceive(Len, Pkt, &tLen);
		return NULL;	/* got nothing from this one as far as APRS */
	}

/*
KG4CWC-9>2VTV6V,K4PKT-3,W4OT-3*,WIDE2-1 <UI>:`l*ql?sk/"3y}
K4PKT-3>APNP50,W4OT-3*,WIDE2-1 <UI C>:!2648.72NP08012.82W#PHG6530/ WIDEn-n Digi
On The Web @ pbpg.org
KJ4ERJ-7>APTT4,WIDE1-1,WIDE2-1 <UI>:T#416,135,159,255,087,079,01001000
KJ4ERJ-7>APTT4,WX4MLB-3*,WIDE2-1 <UI>:T#416,135,159,255,087,079,01001000
KJ4ERJ-7>APTT4,WIDE1-1,WIDE2-1 <UI>:>Lynn's TT4 Beacon
KJ4ERJ-7>APTT4,WX4MLB-3*,WIDE2-1 <UI>:>Lynn's TT4 Beacon
KJ4ERJ-12>APZLWD,WX4MLB-3* <UI R>:>status
KJ4ERJ-12>APZLWD,KJ4ERJ-1,WIDE1* <UI C>:>status
WI8H-9>R8PT6W,WIDE1-1,WIDE2-1 <UI R>:`lF6l#Wk/`"3z}146.850MHz FTM-350_"
WI8H-9>R8PT6W,KJ4ERJ-1,WIDE1*,WIDE2-1 <UI C>:`lF6l#Wk/`"3z}146.850MHz FTM-350_"
KJ4ERJ-12>APZLWD,WX4MLB-3* <UI R>:
*/
	{	char *s = MyStrnStr(Len,Pkt," <UI");	/* From a D710 */
		if (s)
		{	char *e = MyStrnStr(Len-(s-Pkt),s,">:");	/* End of the <UI xxx>: */
			if (e)
			{	memmove(s,e+1,Len-(e-Pkt)-1);	/* Copy the : to end down */
				Len -= (e-s)+1;	/* Take out b<UI> but keep the : */
			} else
			{	PortDumpHex(NULL, "Missing <UI...>:", (e-Pkt), Pkt);
				Result = NULL;
				Len = 0;
			}
		}
	}

	if (Len)
	{
{	BOOL HasNULL = MyStrnChr(Len, Pkt, '\0') != NULL;
	if (HasNULL)
		PortDumpHex("Packets(NULL)", "Kenwood:NULL-Truncated", Len, Pkt);
	PortDumpHex(NULL, "Converse", Len, Pkt);
}
		Result = (char*)malloc(Len+1);
		memcpy(Result, Pkt, Len);
		Result[Len] = '\0';
	} else Result = NULL;

	return Result;
}

BOOL ConverseGotPacket(int len, unsigned char *buf)
{	if (len && (buf[len] == '\n' || buf[len] == '\r'))
		return TRUE;
	else if (len > 1 && (buf[len-1] == '\n' || buf[len-1] == '\r'))	/* End of TEXT Packet? */
		return TRUE;
	else if (len >= 4 && !strncmp(buf,"cmd:",4))
		return TRUE;
	else
	{	//TraceLog(NULL, FALSE, NULL,"TEXT:Got %ld Bytes starting 0x%02lX ending in 0x%02lX\n", (long) len, (unsigned long) *buf, (unsigned long) buf[len-1]);

		if (MyStrnChr(len, buf, '\r') || MyStrnChr(len, buf, '\n') )
		{	//TraceLog(NULL, FALSE, NULL,"TEXT:Got %ld Bytes starting 0x%02lX ending in 0x%02lX - Recovered\n", (long) len, (unsigned long) *buf, (unsigned long) buf[len-1]);
			return TRUE;
		}
	}
	return FALSE;
}

