/*

		    Earth Viewer  --  Definitions

	       by John Walker  --  kelvin@fourmilab.ch
		 <http://www.fourmilab.ch/index.html>

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "bitmap.h"
#include "elements.h"
#include "linalg.h"

#define FALSE	0
#define TRUE	1


#define TERMINC  100		      /* Circle segments for terminator */

#define PI 3.14159265358979323846  /* Assume not near black hole nor in
				      Tennessee */
#define V	(void)

#define EOS     '\0'

/*  Frequently used astronomical constants */

#define J2000		    2451545.0	    /* Julian day of J2000 epoch */
#define JulianCentury	    36525.0	    /* Days in Julian century */
#define AstronomicalUnit    149597870.0     /* Astronomical unit in kilometres */
#define SunSMAX  (AstronomicalUnit * 1.000001018) /* Semi-major axis of Earth's orbit */
#define EarthRad            6378.14         /* Earth's equatorial radius, km (IAU 1976) */
#define LunatBase           2423436.0       /* Base date for E. W. Brown's numbered
					       series of lunations (1923 January 16) */
#define SynMonth	    29.53058868     /* Synodic month (mean time from new Moon to new Moon) */

#define max(x, y) (((x) > (y)) ? (x) : (y))		  /* Maximum */
#define min(x, y) (((x) < (y)) ? (x) : (y))		  /* Maximum */
#define sgn(x) (((x) < 0) ? -1 : ((x) > 0 ? 1 : 0))	  /* Extract sign */
#define abs(x) ((x) < 0 ? (-(x)) : (x)) 		  /* Absolute val */
#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))  /* Fix angle */
#define fixangr(a)  ((a) - (PI*2) * (floor((a) / (PI*2))))  /* Fix angle in radians */
#define dtr(x) ((x) * (PI / 180.0))			  /* Degree->Radian */
#define rtd(x) ((x) / (PI / 180.0))			  /* Radian->Degree */

/* From astro.c  */

extern double ucttoj(long year, int mon, int mday,
		     int hour, int min, int sec);
extern double jtime(struct tm *t);
extern void jyear(double td, long *yy, int *mm, int *dd);
extern double gmst(double jd);
extern void sunpos(double jd, int apparent,
		   double *ra, double *dec, double *rv, double *slong);
extern double phase(
		    double  pdate, double  *pphase, double  *mage, double  *dist,
		    double  *angdia, double  *sudist, double  *suangdia);
extern void highmoon(double jd, double *l, double *b, double *r);
extern void ecliptoeq(double jd, double Lambda, double Beta, double *Ra, double *Dec);

/* From elements.c */

extern void insat(FILE *in, void (*dosat)(), int *err, int *lineerr);
extern int extractel(struct satellite *sat, char *sname, char *line1, char *line2);

/* From gifout.c */

extern int gifout(int cols, int rows, int rowlen, RGBQUAD *colmap, int colors,
		  unsigned char *bitmap, FILE *outfile);

/* From orbitcalc.c */

extern void InitOrbitRoutines(double EpochDay);
extern long GetDayNum(int Year, int Month, int Day);
extern void GetPrecession(double SemiMajorAxis, double Eccentricity,
			  double Inclination,
			  double *RAANPrecession, double *PerigeePrecession);
extern void GetSatPosition(double EpochTime, double EpochRAAN,
			   double EpochArgPerigee, double SemiMajorAxis,
			   double Inclination, double Eccentricity,
			   double RAANPrecession, double PerigeePrecession,
			   double Time, double TrueAnomaly,
			   double *X_a, double *Y_a, double *Z_a,
			   double *Radius, double *VX, double *VY,
			   double *VZ);
extern void GetSubSatPoint(double SatX, double SatY, double SatZ, double Time,
			   double *Latitude, double *Longitude, double *Height);
extern int Eclipsed(double SatX, double SatY, double SatZ,
		    double SatRadius, double CurrentTime);
extern double Kepler(double MeanAnomaly, double Eccentricity);
