/*
 * Copyright © 2010 Guido Trentalancia IZ6RDB
 * This program is freeware, however it is provided as is, without
 * any warranty.
 */

/* Calculate Sun rise and set time */

/* This program is the C implementation of the algorithm presented by
 * Ed Williams http://williams.best.vwh.net/sunrise_sunset_algorithm.htm
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

#ifdef FUTURE
void main (int argc, char *argv[]) {
//	double longitude = 13.3217;
//	double latitude = 43.3630;
//	double zenith = 90.5;
	double longitude = -80.659083;
	double latitude = 27.996683;
	int day;
	int month;
	int year;
#endif

#include "sun.h"

int SunRiseSet(double latitude, double longitude, int year, int month, int day,
				double *zHoursRise, double *zHoursSet)
{
	double zenith = 90.0+50.0/60.0;
//	double localOffset = 0;
	int leap_year;
	double lngHour, Lquadrant_rise, Lquadrant_set, RAquadrant_rise, RAquadrant_set;
	double sinDec_rise, sinDec_set, cosDec_rise, cosDec_set, cosH_rise, cosH_set;
	double t_rise, t_set, M_rise, M_set, L_rise, L_set, RA_rise, RA_set;
	double H_rise, H_set, T_rise, T_set, UT_rise, UT_set;
//	double localT_rise, localT_set;
//	int localH_rise, localH_set, localM_rise, localM_set;
	int N1, N2, N3, N;

#ifdef OLD_STUFF
	if (argc < 3) {
		printf("sun rise and set time: usage %s day month year\n", argv[0]);
		return FALSE;
	}
	day = atoi(argv[1]);
	month = atoi(argv[2]);
	year = atoi(argv[3]);
#endif

	if (day > 31 || day <= 0) {
		printf("Invalid day !\n");
		return FALSE;
	}
	if (month > 12 || month <= 0) {
		printf("Invalid month !\n");
		return FALSE;
	}

	// check for leap-year
	if (month == 2) {
		if (year%400 == 0 || (year%100 != 0 && year%4 == 0))
			leap_year = 1;
		else
			leap_year = 0;
	}

	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
		printf("Invalid day !\n");
		return FALSE;
	}
	if (month == 2 && leap_year && day > 29) {
		printf("Invalid day !\n");
		return FALSE;
	}
	if (month == 2 && !leap_year && day > 28) {
		printf("Invalid day !\n");
		return FALSE;
	}
	if (day > 31) {
		printf("Invalid day !\n");
		return FALSE;
	}

	N1 = (int) floor(275 * month / 9);
	N2 = (int) floor((month + 9) / 12);
	N3 = (int) (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
	N = N1 - (N2 * N3) + day - 30;

	if (N > 365) {
		printf("Invalid date !\n");
		return FALSE;
	}

	lngHour = longitude / 15;
	
	t_rise = N + ((6 - lngHour) / 24);
	t_set = N + ((18 - lngHour) / 24);

	M_rise = (0.9856 * t_rise) - 3.289;
	M_set = (0.9856 * t_set) - 3.289;

	L_rise = (M_rise + (1.916 * sin(M_rise * (M_PI/180))) + (0.020 * sin(2 * M_rise * (M_PI/180))) + 282.634);
	L_set = (M_set + (1.916 * sin(M_set * (M_PI/180))) + (0.020 * sin(2 * M_set * (M_PI/180))) + 282.634);
	while (L_rise >= 360) {
		L_rise = L_rise - 360;
	}
	while (L_rise < 0) {
		L_rise = L_rise + 360;
	}
	while (L_set >= 360) {
                L_set = L_set - 360;
        }
        while (L_set < 0) {
                L_set = L_set + 360;
        }

	RA_rise = (180/M_PI)*atan(0.91764 * tan((M_PI/180)*L_rise));
	RA_set = (180/M_PI)*atan(0.91764 * tan((M_PI/180)*L_set));
	while (RA_rise >= 360) {
                RA_rise = RA_rise - 360;
        }
        while (RA_rise < 0) {
                RA_rise = RA_rise + 360;
        }
	while (RA_set >= 360) {
                RA_set = RA_set - 360;
        }
        while (RA_set < 0) {
                RA_set = RA_set + 360;
        }

	Lquadrant_rise  = (floor(L_rise/90)) * 90;
	RAquadrant_rise = (floor(RA_rise/90)) * 90;
	RA_rise = RA_rise + (Lquadrant_rise - RAquadrant_rise);
	RA_rise = RA_rise / 15;

        Lquadrant_set  = (floor(L_set/90)) * 90;
        RAquadrant_set = (floor(RA_set/90)) * 90;
        RA_set = RA_set + (Lquadrant_set - RAquadrant_set);
        RA_set = RA_set / 15;

	sinDec_rise = 0.39782 * sin((M_PI/180)*L_rise);
	cosDec_rise = cos(asin(sinDec_rise));
        sinDec_set = 0.39782 * sin((M_PI/180)*L_set);
        cosDec_set = cos(asin(sinDec_set));

	cosH_rise = (cos((M_PI/180)*zenith) - (sinDec_rise * sin((M_PI/180)*latitude))) / (cosDec_rise * cos((M_PI/180)*latitude));
	cosH_set = (cos((M_PI/180)*zenith) - (sinDec_set * sin((M_PI/180)*latitude))) / (cosDec_set * cos((M_PI/180)*latitude));
	
	if (cosH_rise >  1) {
	  printf("The sun never rises on this location (on the specified date)\n");
		return FALSE;
	}
	if (cosH_set < -1) {
	  printf("The sun never sets on this location (on the specified date)\n");
		return FALSE;
	}

	H_rise = 360 - (180/M_PI)*acos(cosH_rise);
	H_set = (180/M_PI)*acos(cosH_set);
	
	H_rise = H_rise / 15;
	H_set = H_set / 15;

	T_rise = H_rise + RA_rise - (0.06571 * t_rise) - 6.622;
	T_set = H_set + RA_set - (0.06571 * t_set) - 6.622;

	UT_rise = T_rise - lngHour;
	UT_set = T_set - lngHour;
	while (UT_rise >= 24) {
		UT_rise = UT_rise - 24;
	}
	while (UT_rise < 0) {
		UT_rise = UT_rise + 24;
	}
	while (UT_set >= 24) {
                UT_set = UT_set - 24;
        }
        while (UT_set < 0) {
                UT_set = UT_set + 24;
        }
	*zHoursRise = UT_rise;
	*zHoursSet = UT_set;
	return TRUE;
#ifdef OLD_STUFF
	localT_rise = UT_rise + localOffset;
	localT_set = UT_set + localOffset;
	localH_rise = (int) floor(localT_rise);
	localH_set = (int) floor(localT_set);
//	localM_rise = (int) rint((localT_rise - localH_rise)*60);
//	localM_set = (int) rint((localT_set - localH_set)*60);
	localM_rise = (int) ((localT_rise - localH_rise)*60+0.5);
	localM_set = (int) ((localT_set - localH_set)*60+0.5);
	while (localM_rise >= 60 && localH_rise <= 22) {
		localH_rise += 1;
		localM_rise -= 60;
	}		
	while (localM_set >= 60 & localH_set <= 22) {
		localH_set += 1;
		localM_set -= 60;
	}

	if (localH_rise >= 0 && localH_rise < 24 && localM_rise >= 0 && localM_rise < 60 &&
	    localH_set >= 0 && localH_set < 24 && localM_set >= 0 && localM_set < 60)
	{
		printf("Sun rises at %02d:%02d and sets at %02d:%02d\n", localH_rise, localM_rise, localH_set, localM_set);
		*hRise = localH_rise
		int *hRise, int *mRise, int *hSet, int *mSet)
		return TRUE;
	} else return FALSE;
#endif
}
