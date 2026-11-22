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

/* Direct GIF output for Earth viewer.	Based on... */

/* ppmtogif.c - read a portable pixmap and produce a GIF file
**
** Based on GIFENCOD by David Rowley <mgardi@watdscu.waterloo.edu>.A
** Lempel-Zim compression based on "compress".
**
** Modified by Marcel Wijkstra <wijkstra@fwi.uva.nl>
**
**
** Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
**
** The Graphics Interchange Format(c) is the Copyright property of
** CompuServe Incorporated.  GIF(sm) is a Service Mark property of
** CompuServe Incorporated.
*/

#define ARGS(x) x

#define MAXCOLORS 256

/*
 * Pointer to function returning an int
 */
typedef int (* ifunptr) ARGS((int, int));

/*
 * a code_int must be able to hold 2**BITS values of type int, and also -1
 */
typedef int		code_int;

#ifdef SIGNED_COMPARE_SLOW
typedef unsigned long int count_int;
typedef unsigned short int count_short;
#else /*SIGNED_COMPARE_SLOW*/
typedef long int	  count_int;
#endif /*SIGNED_COMPARE_SLOW*/

static int GetPixel ARGS(( int x, int y ));
static void BumpPixel ARGS(( void ));
static int GIFNextPixel ARGS(( ifunptr getpixel ));
static void GIFEncode ARGS(( FILE* fp, int GWidth, int GHeight, int GInterlace, int Background, int Transparent, int BitsPerPixel, int* Red, int* Green, int* Blue, ifunptr GetPixel ));
static int Red[MAXCOLORS],Green[MAXCOLORS],Blue[MAXCOLORS];
static void Putword ARGS(( int w, FILE* fp ));
static void compress ARGS(( int init_bits, FILE* outfile, ifunptr ReadValue ));
static void output ARGS(( code_int code ));
static void cl_block ARGS(( void ));
static void cl_hash ARGS(( count_int hsize ));
static void writeerr ARGS(( void ));
static void char_init ARGS(( void ));
static void char_out ARGS(( int c ));
static void flush_char ARGS(( void ));

static int ok;

static unsigned char *tpixel;
static int tmhm1, tmrowlen;

/*  GIFOUT  --	Write a GIF file from the contents of a BMP-format
		memory image.  */

int gifout(int cols, int rows, int rowlen, RGBQUAD *colmap, int colors,
	   unsigned char *bitmap, FILE *outfile)
{
    int i, BitsPerPixel;
    int interlace, transparent;

    ok = TRUE;

    tpixel = bitmap;
    tmhm1 = rows - 1;
    tmrowlen = rowlen;

    interlace = 1;
    transparent = -1;


    /* Now turn the BMP colormap into the appropriate GIF colormap. */

    for ( i = 0; i < colors; ++i )
	{
	    Red[i] = colmap[i].rgbRed;
	    Green[i] = colmap[i].rgbGreen;
	    Blue[i] = colmap[i].rgbBlue;
	}

    BitsPerPixel = 8;

    /* All set, let's do it. */
    GIFEncode(
	outfile, cols, rows, interlace, 0, transparent, BitsPerPixel,
	Red, Green, Blue, GetPixel );

    return ok;
}

static int
GetPixel( x, y )
int x, y;
{
#define PixA(x, y) ((tpixel + ((x) + (((DWORD) (tmhm1 - (y))) * (tmrowlen)))))
    return *PixA(x, y);
#undef PixA
}


/*****************************************************************************
 *
 * GIFENCODE.C	  - GIF Image compression interface
 *
 * GIFEncode( FName, GHeight, GWidth, GInterlace, Background, Transparent,
 *	      BitsPerPixel, Red, Green, Blue, GetPixel )
 *
 *****************************************************************************/

#define TRUE 1
#define FALSE 0

static int Width, Height;
static int curx, cury;
static long CountDown;
static int Pass = 0;
static int Interlace;

/*
 * Bump the 'curx' and 'cury' to point to the next pixel
 */
static void
BumpPixel()
{
	/*
	 * Bump the current X position
	 */
	++curx;

	/*
	 * If we are at the end of a scan line, set curx back to the beginning
	 * If we are interlaced, bump the cury to the appropriate spot,
	 * otherwise, just increment it.
	 */
	if( curx == Width ) {
		curx = 0;

		if( !Interlace )
			++cury;
		else {
		     switch( Pass ) {

		       case 0:
			  cury += 8;
			  if( cury >= Height ) {
				++Pass;
				cury = 4;
			  }
			  break;

		       case 1:
			  cury += 8;
			  if( cury >= Height ) {
				++Pass;
				cury = 2;
			  }
			  break;

		       case 2:
			  cury += 4;
			  if( cury >= Height ) {
			     ++Pass;
			     cury = 1;
			  }
			  break;

		       case 3:
			  cury += 2;
			  break;
			}
		}
	}
}

/*
 * Return the next pixel from the image
 */
static int
GIFNextPixel( getpixel )
ifunptr getpixel;
{
	int r;

	if( CountDown == 0 )
		return EOF;

	--CountDown;

	r = ( * getpixel )( curx, cury );

	BumpPixel();

	return r;
}

/* public */

static void
GIFEncode( fp, GWidth, GHeight, GInterlace, Background, Transparent,
	   BitsPerPixel, Red, Green, Blue, GetPixel )

FILE* fp;
int GWidth, GHeight;
int GInterlace;
int Background;
int Transparent;
int BitsPerPixel;
int Red[], Green[], Blue[];
ifunptr GetPixel;
{
	int B;
	int RWidth, RHeight;
	int LeftOfs, TopOfs;
	int Resolution;
	int ColorMapSize;
	int InitCodeSize;
	int i;

	Interlace = GInterlace;

	ColorMapSize = 1 << BitsPerPixel;

	RWidth = Width = GWidth;
	RHeight = Height = GHeight;
	LeftOfs = TopOfs = 0;

	Resolution = BitsPerPixel;

	/*
	 * Calculate number of bits we are expecting
	 */
	CountDown = (long)Width * (long)Height;

	/*
	 * Indicate which pass we are on (if interlace)
	 */
	Pass = 0;

	/*
	 * The initial code size
	 */
	if( BitsPerPixel <= 1 )
		InitCodeSize = 2;
	else
		InitCodeSize = BitsPerPixel;

	/*
	 * Set up the current x and y position
	 */
	curx = cury = 0;

	/*
	 * Write the Magic header
	 */
        fwrite( Transparent < 0 ? "GIF87a" : "GIF89a", 1, 6, fp );

	/*
	 * Write out the screen width and height
	 */
	Putword( RWidth, fp );
	Putword( RHeight, fp );

	/*
	 * Indicate that there is a global colour map
	 */
	B = 0x80;	/* Yes, there is a color map */

	/*
	 * OR in the resolution
	 */
	B |= (Resolution - 1) << 5;

	/*
	 * OR in the Bits per Pixel
	 */
	B |= (BitsPerPixel - 1);

	/*
	 * Write it out
	 */
	fputc( B, fp );

	/*
	 * Write out the Background colour
	 */
	fputc( Background, fp );

	/*
         * Byte of 0's (future expansion)
	 */
	fputc( 0, fp );

	/*
	 * Write out the Global Colour Map
	 */
	for( i=0; i<ColorMapSize; ++i ) {
		fputc( Red[i], fp );
		fputc( Green[i], fp );
		fputc( Blue[i], fp );
	}

	/*
	 * Write out extension for transparent colour index, if necessary.
	 */
	if ( Transparent >= 0 ) {
            fputc( '!', fp );
	    fputc( 0xf9, fp );
	    fputc( 4, fp );
	    fputc( 1, fp );
	    fputc( 0, fp );
	    fputc( 0, fp );
	    fputc( Transparent, fp );
	    fputc( 0, fp );
	}

	/*
	 * Write an Image separator
	 */
        fputc( ',', fp );

	/*
	 * Write the Image header
	 */

	Putword( LeftOfs, fp );
	Putword( TopOfs, fp );
	Putword( Width, fp );
	Putword( Height, fp );

	/*
	 * Write out whether or not the image is interlaced
	 */
	if( Interlace )
		fputc( 0x40, fp );
	else
		fputc( 0x00, fp );

	/*
	 * Write out the initial code size
	 */
	fputc( InitCodeSize, fp );

	/*
	 * Go and actually compress the data
	 */
	compress( InitCodeSize+1, fp, GetPixel );

	/*
	 * Write out a Zero-length packet (to end the series)
	 */
	fputc( 0, fp );

	/*
	 * Write the GIF file terminator
	 */
        fputc( ';', fp );

	/*
	 * And close the file
	 */
	fclose( fp );
}

/*
 * Write out a word to the GIF file
 */
static void
Putword( w, fp )
int w;
FILE* fp;
{
	fputc( w & 0xff, fp );
	fputc( (w / 256) & 0xff, fp );
}


/***************************************************************************
 *
 *  GIFCOMPR.C	     - GIF Image compression routines
 *
 *  Lempel-Ziv compression based on 'compress'.  GIF modifications by
 *  David Rowley (mgardi@watdcsu.waterloo.edu)
 *
 ***************************************************************************/

/*
 * General DEFINEs
 */

#define BITS	12

#define HSIZE  5003	       /* 80% occupancy */

#ifdef NO_UCHAR
 typedef char	char_type;
#else /*NO_UCHAR*/
 typedef	unsigned char	char_type;
#endif /*NO_UCHAR*/

/*
 *
 * GIF Image compression - modified 'compress'
 *
 * Based on: compress.c - File compression ala IEEE Computer, June 1984.
 *
 * By Authors:	Spencer W. Thomas	(decvax!harpo!utah-cs!utah-gr!thomas)
 *		Jim McKie		(decvax!mcvax!jim)
 *		Steve Davies		(decvax!vax135!petsd!peora!srd)
 *		Ken Turkowski		(decvax!decwrl!turtlevax!ken)
 *		James A. Woods		(decvax!ihnp4!ames!jaw)
 *		Joe Orost		(decvax!vax135!petsd!joe)
 *
 */
#include <ctype.h>

#define ARGVAL() (*++(*argv) || (--argc && *++argv))

static int n_bits;			  /* number of bits/code */
static int maxbits = BITS;		  /* user settable max # bits/code */
static code_int maxcode;		  /* maximum code, given n_bits */
static code_int maxmaxcode = (code_int)1 << BITS; /* should NEVER generate this code */
#ifdef COMPATIBLE		/* But wrong! */
# define MAXCODE(n_bits)	((code_int) 1 << (n_bits) - 1)
#else /*COMPATIBLE*/
# define MAXCODE(n_bits)	(((code_int) 1 << (n_bits)) - 1)
#endif /*COMPATIBLE*/

static count_int htab [HSIZE];
static unsigned short codetab [HSIZE];
#define HashTabOf(i)	   htab[i]
#define CodeTabOf(i)	codetab[i]

static code_int hsize = HSIZE;		       /* for dynamic table sizing */

/*
 * To save much memory, we overlay the table used by compress() with those
 * used by decompress().  The tab_prefix table is the same size and type
 * as the codetab.  The tab_suffix table needs 2**BITS characters.  We
 * get this from the beginning of htab.  The output stack uses the rest
 * of htab, and contains characters.  There is plenty of room for any
 * possible stack (stack used to be 8000 characters).
 */

#define tab_prefixof(i) CodeTabOf(i)
#define tab_suffixof(i)        ((char_type*)(htab))[i]
#define de_stack	       ((char_type*)&tab_suffixof((code_int)1<<BITS))

static code_int free_ent = 0;		       /* first unused entry */

/*
 * block compression parameters -- after all codes are used up,
 * and compression rate changes, start over.
 */
static int clear_flg = 0;

static long int in_count = 1;		 /* length of input */
static long int out_count = 0;		 /* # of codes output (for debugging) */

/*
 * compress stdin to stdout
 *
 * Algorithm:  use open addressing double hashing (no chaining) on the
 * prefix code / next character combination.  We do a variant of Knuth's
 * algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
 * secondary probe.  Here, the modular division first probe is gives way
 * to a faster exclusive-or manipulation.  Also do block compression with
 * an adaptive reset, whereby the code table is cleared when the compression
 * ratio decreases, but after the table fills.	The variable-length output
 * codes are re-sized at this point, and a special CLEAR code is generated
 * for the decompressor.  Late addition:  construct the table according to
 * file size for noticeable speed improvement on small files.  Please direct
 * questions about this implementation to ames!jaw.
 */

static int g_init_bits;
static FILE* g_outfile;

static int ClearCode;
static int EOFCode;

static void
compress( init_bits, outfile, ReadValue )
int init_bits;
FILE* outfile;
ifunptr ReadValue;
{
    register long fcode;
    register code_int i /* = 0 */;
    register int c;
    register code_int ent;
    register code_int disp;
    register code_int hsize_reg;
    register int hshift;

    /*
     * Set up the globals:  g_init_bits - initial number of bits
     *			    g_outfile	- pointer to output file
     */
    g_init_bits = init_bits;
    g_outfile = outfile;

    /*
     * Set up the necessary values
     */
    out_count = 0;
    clear_flg = 0;
    in_count = 1;
    maxcode = MAXCODE(n_bits = g_init_bits);

    ClearCode = (1 << (init_bits - 1));
    EOFCode = ClearCode + 1;
    free_ent = ClearCode + 2;

    char_init();

    ent = GIFNextPixel( ReadValue );

    hshift = 0;
    for ( fcode = (long) hsize;  fcode < 65536L; fcode *= 2L )
	++hshift;
    hshift = 8 - hshift;		/* set hash code range bound */

    hsize_reg = hsize;
    cl_hash( (count_int) hsize_reg);		/* clear hash table */

    output( (code_int)ClearCode );

#ifdef SIGNED_COMPARE_SLOW
    while ( (c = GIFNextPixel( ReadValue )) != (unsigned) EOF ) {
#else /*SIGNED_COMPARE_SLOW*/
    while ( (c = GIFNextPixel( ReadValue )) != EOF ) {	/* } */
#endif /*SIGNED_COMPARE_SLOW*/

	++in_count;

	fcode = (long) (((long) c << maxbits) + ent);
	i = (((code_int)c << hshift) ^ ent);	/* xor hashing */

	if ( HashTabOf (i) == fcode ) {
	    ent = CodeTabOf (i);
	    continue;
	} else if ( (long)HashTabOf (i) < 0 )	   /* empty slot */
	    goto nomatch;
	disp = hsize_reg - i;		/* secondary hash (after G. Knott) */
	if ( i == 0 )
	    disp = 1;
probe:
	if ( (i -= disp) < 0 )
	    i += hsize_reg;

	if ( HashTabOf (i) == fcode ) {
	    ent = CodeTabOf (i);
	    continue;
	}
	if ( (long)HashTabOf (i) > 0 )
	    goto probe;
nomatch:
	output ( (code_int) ent );
	++out_count;
	ent = c;
#ifdef SIGNED_COMPARE_SLOW
	if ( (unsigned) free_ent < (unsigned) maxmaxcode) {
#else /*SIGNED_COMPARE_SLOW*/
	if ( free_ent < maxmaxcode ) {	/* } */
#endif /*SIGNED_COMPARE_SLOW*/
	    CodeTabOf (i) = free_ent++; /* code -> hashtable */
	    HashTabOf (i) = fcode;
	} else
		cl_block();
    }
    /*
     * Put out the final code.
     */
    output( (code_int)ent );
    ++out_count;
    output( (code_int) EOFCode );
}

/*****************************************************************
 * TAG( output )
 *
 * Output the given code.
 * Inputs:
 *	code:	A n_bits-bit integer.  If == -1, then EOF.  This assumes
 *		that n_bits =< (long)wordsize - 1.
 * Outputs:
 *	Outputs code to the file.
 * Assumptions:
 *	Chars are 8 bits long.
 * Algorithm:
 *	Maintain a BITS character long buffer (so that 8 codes will
 * fit in it exactly).	Use the VAX insv instruction to insert each
 * code in turn.  When the buffer fills up empty it and start over.
 */

static unsigned long cur_accum = 0;
static int cur_bits = 0;

static unsigned long masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
				  0x001F, 0x003F, 0x007F, 0x00FF,
				  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
				  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

static void
output( code )
code_int  code;
{
    cur_accum &= masks[ cur_bits ];

    if( cur_bits > 0 )
	cur_accum |= ((long)code << cur_bits);
    else
	cur_accum = code;

    cur_bits += n_bits;

    while( cur_bits >= 8 ) {
	char_out( (unsigned int)(cur_accum & 0xff) );
	cur_accum >>= 8;
	cur_bits -= 8;
    }

    /*
     * If the next entry is going to be too big for the code size,
     * then increase it, if possible.
     */
   if ( free_ent > maxcode || clear_flg ) {

	    if( clear_flg ) {

		maxcode = MAXCODE (n_bits = g_init_bits);
		clear_flg = 0;

	    } else {

		++n_bits;
		if ( n_bits == maxbits )
		    maxcode = maxmaxcode;
		else
		    maxcode = MAXCODE(n_bits);
	    }
	}

    if( code == EOFCode ) {
	/*
	 * At EOF, write the rest of the buffer.
	 */
	while( cur_bits > 0 ) {
		char_out( (unsigned int)(cur_accum & 0xff) );
		cur_accum >>= 8;
		cur_bits -= 8;
	}

	flush_char();

	fflush( g_outfile );

	if( ferror( g_outfile ) )
		writeerr();
    }
}

/*
 * Clear out the hash table
 */
static void
cl_block ()		/* table clear for block compress */
{

	cl_hash ( (count_int) hsize );
	free_ent = ClearCode + 2;
	clear_flg = 1;

	output( (code_int)ClearCode );
}

static void
cl_hash(hsize)		/* reset code table */
register count_int hsize;
{

	register count_int *htab_p = htab+hsize;

	register long i;
	register long m1 = -1;

	i = hsize - 16;
	do {				/* might use Sys V memset(3) here */
		*(htab_p-16) = m1;
		*(htab_p-15) = m1;
		*(htab_p-14) = m1;
		*(htab_p-13) = m1;
		*(htab_p-12) = m1;
		*(htab_p-11) = m1;
		*(htab_p-10) = m1;
		*(htab_p-9) = m1;
		*(htab_p-8) = m1;
		*(htab_p-7) = m1;
		*(htab_p-6) = m1;
		*(htab_p-5) = m1;
		*(htab_p-4) = m1;
		*(htab_p-3) = m1;
		*(htab_p-2) = m1;
		*(htab_p-1) = m1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for ( i += 16; i > 0; --i )
		*--htab_p = m1;
}

static void
writeerr()
{
	ok = FALSE;
}

/******************************************************************************
 *
 * GIF Specific routines
 *
 ******************************************************************************/

/*
 * Number of characters so far in this 'packet'
 */
static int a_count;

/*
 * Set up the 'byte output' routine
 */
static void
char_init()
{
	a_count = 0;
}

/*
 * Define the storage for the packet accumulator
 */
static char accum[ 256 ];

/*
 * Add a character to the end of the current packet, and if it is 254
 * characters, flush the packet to disk.
 */
static void
char_out( c )
int c;
{
	accum[ a_count++ ] = (char) c;
	if( a_count >= 254 )
		flush_char();
}

/*
 * Flush the packet to disk, and reset the accumulator
 */
static void
flush_char()
{
	if( a_count > 0 ) {
		fputc( a_count, g_outfile );
		fwrite( accum, 1, a_count, g_outfile );
		a_count = 0;
	}
}

/* The End */


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

//#define max(x, y) (((x) > (y)) ? (x) : (y))		  /* Maximum */
//#define min(x, y) (((x) < (y)) ? (x) : (y))		  /* Maximum */
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


/*

	 Earth Viewer  --  Astronomical calculation routines

	       by John Walker  --  kelvin@fourmilab.ch
		 <http://www.fourmilab.ch/index.html>

*/

/*  Astronomical constants  */

#define epoch	    2444238.5	   /* 1980 January 0.0 */

/*  Constants defining the Sun's apparent orbit  */

#define elonge	    278.833540	   /* Ecliptic longitude of the Sun
				      at epoch 1980.0 */
#define elongp	    282.596403	   /* Ecliptic longitude of the Sun at
				      perigee */
#define eccent      0.016718       /* Eccentricity of Earth's orbit */
#define sunangsiz   0.533128       /* Sun's angular size, degrees, at
				      semi-major axis distance */

/*  Elements of the Moon's orbit, epoch 1980.0  */

#define mmlong      64.975464      /* Moon's mean longitude at the epoch */
#define mmlongp     349.383063	   /* Mean longitude of the perigee at the
				      epoch */
#define mlnode	    151.950429	   /* Mean longitude of the node at the
				      epoch */
#define minc        5.145396       /* Inclination of the Moon's orbit */
#define mecc        0.054900       /* Eccentricity of the Moon's orbit */
#define mangsiz     0.5181         /* Moon's angular size at distance a
				      from Earth */
#define msmax       384401.0       /* Semi-major axis of Moon's orbit in km */
#define mparallax   0.9507	   /* Parallax at distance a from Earth */

#define dsin(x) (sin(dtr((x))))     /* Sin from deg */
#define dcos(x) (cos(dtr((x))))     /* Cos from deg */

static double obliqeq(double jd);

/*  UCTTOJ  --	Convert GMT date and time to astronomical
		Julian time (i.e. Julian date plus day fraction,
		expressed as a double).  */

static double ucttoj(long year, int mon, int mday,
	      int hour, int min, int sec)
{

    /* Algorithm as given in Meeus, Astronomical Algorithms, Chapter 7, page 61 */

    int a, b, m;
    long y;

    assert(mon	>= 0 && mon  < 12);
    assert(mday >  0 && mday < 32);
    assert(hour >= 0 && hour < 24);
    assert(min	>= 0 && min  < 60);
    assert(sec	>= 0 && sec  < 60);

    m = mon + 1;
    y = year;

    if (m <= 2) {
	y--;
	m += 12;
    }

    /* Determine whether date is in Julian or Gregorian calendar based on
       canonical date of calendar reform. */

    if ((year < 1582) || ((year == 1582) && ((mon < 9) || (mon == 9 && mday < 5)))) {
	b = 0;
    } else {
	a = ((int) (y / 100));
	b = 2 - a + (a / 4);
    }

    return (((long) (365.25 * (y + 4716))) + ((int) (30.6001 * (m + 1))) +
		mday + b - 1524.5) +
	    ((sec + 60L * (min + 60L * hour)) / 86400.0);
}

/*  JTIME  --  Convert a Unix date and time (tm) structure to astronomical
	       Julian time (i.e. Julian date plus day fraction,
	       expressed as a double).	*/

static double jtime(struct tm *t)
{
    return ucttoj(t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

/*  JYEAR  --  Convert	Julian	date  to  year,  month, day, which are
	       returned via integer pointers to integers (note that year is a long).  */

static void jyear(double td, long *yy, int *mm, int *dd)
{
    double z, f, a, alpha, b, c, d, e;

    td += 0.5;
    z = floor(td);
    f = td - z;

    if (z < 2299161.0) {
	a = z;
    } else {
	alpha = floor((z - 1867216.25) / 36524.25);
	a = z + 1 + alpha - floor(alpha / 4);
    }

    b = a + 1524;
    c = floor((b - 122.1) / 365.25);
    d = floor(365.25 * c);
    e = floor((b - d) / 30.6001);

    *dd = (int) (b - d - floor(30.6001 * e) + f);
    *mm = (int) ((e < 14) ? (e - 1) : (e - 13));
    *yy = (long) ((*mm > 2) ? (c - 4716) : (c - 4715));
}

/*  KEPLER  --	Solve the equation of Kepler.  */

static double kepler(double m, double ecc)
{
    double e, delta;
#define EPSILON 1E-6

    e = m = dtr(m);
    do {
	delta = e - ecc * sin(e) - m;
	e -= delta / (1 - ecc * cos(e));
    } while (abs(delta) > EPSILON);
    return e;
}

/*  SUNPOS  --	Calculate position of the Sun.	JD is the Julian  date
		of  the  instant for which the position is desired and
		APPARENT should be nonzero if  the  apparent  position
		(corrected  for  nutation  and aberration) is desired.
                The Sun's co-ordinates are returned  in  RA  and  DEC,
		both  specified  in degrees (divide RA by 15 to obtain
		hours).  The radius vector to the Sun in  astronomical
                units  is returned in RV and the Sun's longitude (true
		or apparent, as desired) is  returned  as  degrees  in
		SLONG.	*/

static void sunpos(double jd, int apparent,
	    double *ra, double *dec, double *rv, double *slong)
{
    double t, t2, t3, l, m, e, ea, v, theta, omega, eps;

    /* Time, in Julian centuries of 36525 ephemeris days,
       measured from the epoch 1900 January 0.5 ET. */

    t = (jd - 2415020.0) / JulianCentury;
    t2 = t * t;
    t3 = t2 * t;

    /* Geometric mean longitude of the Sun, referred to the
       mean equinox of the date. */

    l = fixangle(279.69668 + 36000.76892 * t + 0.0003025 * t2);

    /* Sun's mean anomaly. */

    m = fixangle(358.47583 + 35999.04975 * t - 0.000150 * t2 - 0.0000033 * t3);

    /* Eccentricity of the Earth's orbit. */

    e = 0.01675104 - 0.0000418 * t - 0.000000126 * t2;

    /* Eccentric anomaly. */

    ea = kepler(m, e);

    /* True anomaly */

    v = fixangle(2 * rtd(atan(sqrt((1 + e) / (1 - e))  * tan(ea / 2))));

    /* Sun's true longitude. */

    theta = l + v - m;

    /* Obliquity of the ecliptic. */

    eps = obliqeq(jd);

    /* Corrections for Sun's apparent longitude, if desired. */

    if (apparent) {
       omega = fixangle(259.18 - 1934.142 * t);
       theta = theta - 0.00569 - 0.00479 * sin(dtr(omega));
       eps += 0.00256 * cos(dtr(omega));
    }

    /* Return Sun's longitude and radius vector */

    *slong = theta;
    *rv = (1.0000002 * (1 - e * e)) / (1 + e * cos(dtr(v)));

    /* Determine solar co-ordinates. */

    *ra =
    fixangle(rtd(atan2(cos(dtr(eps)) * sin(dtr(theta)), cos(dtr(theta)))));
    *dec = rtd(asin(sin(dtr(eps)) * sin(dtr(theta))));
}

/*  GMST  --  Calculate Greenwich Mean Siderial Time for a given
	      instant expressed as a Julian date and fraction.	*/

static double gmst(double jd)
{
    double t, theta0;

    /* Time, in Julian centuries of 36525 ephemeris days,
       measured from the epoch 1900 January 0.5 ET. */

    t = ((floor(jd + 0.5) - 0.5) - 2415020.0) / JulianCentury;

    theta0 = 6.6460656 + 2400.051262 * t + 0.00002581 * t * t;

    t = (jd + 0.5) - (floor(jd + 0.5));

    theta0 += (t * 24.0) * 1.002737908;

    theta0 = (theta0 - 24.0 * (floor(theta0 / 24.0)));

    return theta0;
}

/*  PHASE  --  Calculate phase of moon as a fraction:

    The  argument  is  the  time  for  which  the  phase is requested,
    expressed as a Julian date and fraction.  Returns  the  terminator
    phase  angle  as a percentage of a full circle (i.e., 0 to 1), and
    stores into pointer arguments  the	illuminated  fraction  of  the
    Moon's  disc, the Moon's age in days and fraction, the distance of
    the Moon from the centre of the Earth, and	the  angular  diameter
    subtended  by the Moon as seen by an observer at the centre of the
    Earth.
*/

static double phase(
  double  pdate,	      /* Date for which to calculate phase */
  double  *pphase,	      /* Illuminated fraction */
  double  *mage,	      /* Age of moon in days */
  double  *dist,	      /* Distance in kilometres */
  double  *angdia,	      /* Angular diameter in degrees */
  double  *sudist,	      /* Distance to Sun */
  double  *suangdia)                  /* Sun's angular diameter */
{

    double Day, N, M, Ec, Lambdasun, ml, MM, MN, Ev, Ae, A3, MmP,
       mEc, A4, lP, Varia, lPP, NP, y, x, Lambdamoon,
       MoonAge, MoonPhase,
       MoonDist, MoonDFrac, MoonAng,
       F, SunDist, SunAng;

    /* Calculation of the Sun's position */

    Day = pdate - epoch;		    /* Date within epoch */
    N = fixangle((360 / 365.2422) * Day);   /* Mean anomaly of the Sun */
    M = fixangle(N + elonge - elongp);	    /* Convert from perigee
					       co-ordinates to epoch 1980.0 */
    Ec = kepler(M, eccent);		    /* Solve equation of Kepler */
    Ec = sqrt((1 + eccent) / (1 - eccent)) * tan(Ec / 2);
    Ec = 2 * rtd(atan(Ec));		    /* True anomaly */
    Lambdasun = fixangle(Ec + elongp);      /* Sun's geocentric ecliptic
					       longitude */
    /* Orbital distance factor */
    F = ((1 + eccent * cos(dtr(Ec))) / (1 - eccent * eccent));
    SunDist = SunSMAX / F;		    /* Distance to Sun in km */
    SunAng = F * sunangsiz;                 /* Sun's angular size in degrees */

    /* Calculation of the Moon's position */

    /* Moon's mean longitude */
    ml = fixangle(13.1763966 * Day + mmlong);

    /* Moon's mean anomaly */
    MM = fixangle(ml - 0.1114041 * Day - mmlongp);

    /* Moon's ascending node mean longitude */
    MN = fixangle(mlnode - 0.0529539 * Day);

    /* Evection */
    Ev = 1.2739 * sin(dtr(2 * (ml - Lambdasun) - MM));

    /* Annual equation */
    Ae = 0.1858 * sin(dtr(M));

    /* Correction term */
    A3 = 0.37 * sin(dtr(M));

    /* Corrected anomaly */
    MmP = MM + Ev - Ae - A3;

    /* Correction for the equation of the centre */
    mEc = 6.2886 * sin(dtr(MmP));

    /* Another correction term */
    A4 = 0.214 * sin(dtr(2 * MmP));

    /* Corrected longitude */
    lP = ml + Ev + mEc - Ae + A4;

    /* Variation */
    Varia = 0.6583 * sin(dtr(2 * (lP - Lambdasun)));

    /* True longitude */
    lPP = lP + Varia;

    /* Corrected longitude of the node */
    NP = MN - 0.16 * sin(dtr(M));

    /* Y inclination coordinate */
    y = sin(dtr(lPP - NP)) * cos(dtr(minc));

    /* X inclination coordinate */
    x = cos(dtr(lPP - NP));

    /* Ecliptic longitude */
    Lambdamoon = rtd(atan2(y, x));
    Lambdamoon += NP;

    /* Ecliptic latitude */
/*    BetaM = rtd(asin(sin(dtr(lPP - NP)) * sin(dtr(minc))));  */

    /* Calculation of the phase of the Moon */

    /* Age of the Moon in degrees */
    MoonAge = lPP - Lambdasun;

    /* Phase of the Moon */
    MoonPhase = (1 - cos(dtr(MoonAge))) / 2;

    /* Calculate distance of moon from the centre of the Earth */

    MoonDist = (msmax * (1 - mecc * mecc)) /
	   (1 + mecc * cos(dtr(MmP + mEc)));

    /* Calculate Moon's angular diameter */

    MoonDFrac = MoonDist / msmax;
    MoonAng = mangsiz / MoonDFrac;

    /* Calculate Moon's parallax */

/*    MoonPar = mparallax / MoonDFrac;	*/

    *pphase = MoonPhase;
    *mage = SynMonth * (fixangle(MoonAge) / 360.0);
    *dist = MoonDist;
    *angdia = MoonAng;
    *sudist = SunDist;
    *suangdia = SunAng;
    return fixangle(MoonAge) / 360.0;
}

/*  HIGHMOON  --  High precision calculation of the Moon's geocentric longitude,
		  latitude, and radius vector (distance) as given in Chapter 45
                  of Meeus' "Astronomical Algorithms".  */
		  

#define NTERMS	60		      /* Number of terms for L, B, and R */

/*  Coefficients for L and R terms.  */

static signed char lrCoeff[NTERMS][4] = {
    {0, 0, 1, 0},
    {2, 0,-1, 0},
    {2, 0, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 2},
    {2, 0,-2, 0},
    {2,-1,-1, 0},
    {2, 0, 1, 0},
    {2,-1, 0, 0},
    {0, 1,-1, 0},
    {1, 0, 0, 0},
    {0, 1, 1, 0},
    {2, 0, 0,-2},
    {0, 0, 1, 2},
    {0, 0, 1,-2},
    {4, 0,-1, 0},
    {0, 0, 3, 0},
    {4, 0,-2, 0},
    {2, 1,-1, 0},
    {2, 1, 0, 0},
    {1, 0,-1, 0},
    {1, 1, 0, 0},
    {2,-1, 1, 0},
    {2, 0, 2, 0},
    {4, 0, 0, 0},
    {2, 0,-3, 0},
    {0, 1,-2, 0},
    {2, 0,-1, 2},
    {2,-1,-2, 0},
    {1, 0, 1, 0},
    {2,-2, 0, 0},
    {0, 1, 2, 0},
    {0, 2, 0, 0},
    {2,-2,-1, 0},
    {2, 0, 1,-2},
    {2, 0, 0, 2},
    {4,-1,-1, 0},
    {0, 0, 2, 2},
    {3, 0,-1, 0},
    {2, 1, 1, 0},
    {4,-1,-2, 0},
    {0, 2,-1, 0},
    {2, 2,-1, 0},
    {2, 1,-2, 0},
    {2,-1, 0,-2},
    {4, 0, 1, 0},
    {0, 0, 4, 0},
    {4,-1, 0, 0},
    {1, 0,-2, 0},
    {2, 1, 0,-2},
    {0, 0, 2,-2},
    {1, 1, 1, 0},
    {3, 0,-2, 0},
    {4, 0,-3, 0},
    {2,-1, 2, 0},
    {0, 2, 1, 0},
    {1, 1,-1, 0},
    {2, 0, 3, 0},
    {2, 0,-1,-2}
};

/*  Coefficients for B terms.  */

static signed char bCoeff[NTERMS][4] = {
    {0, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 0, 1,-1},
    {2, 0, 0,-1},
    {2, 0,-1, 1},
    {2, 0,-1,-1},
    {2, 0, 0, 1},
    {0, 0, 2, 1},
    {2, 0, 1,-1},
    {0, 0, 2,-1},
    {2,-1, 0,-1},
    {2, 0,-2,-1},
    {2, 0, 1, 1},
    {2, 1, 0,-1},
    {2,-1,-1, 1},
    {2,-1, 0, 1},
    {2,-1,-1,-1},
    {0, 1,-1,-1},
    {4, 0,-1,-1},
    {0, 1, 0, 1},
    {0, 0, 0, 3},
    {0, 1,-1, 1},
    {1, 0, 0, 1},
    {0, 1, 1, 1},
    {0, 1, 1,-1},
    {0, 1, 0,-1},
    {1, 0, 0,-1},
    {0, 0, 3, 1},
    {4, 0, 0,-1},
    {4, 0,-1, 1},
    {0, 0, 1,-3},
    {4, 0,-2, 1},
    {2, 0, 0,-3},
    {2, 0, 2,-1},
    {2,-1, 1,-1},
    {2, 0,-2, 1},
    {0, 0, 3,-1},
    {2, 0, 2, 1},
    {2, 0,-3,-1},
    {2, 1,-1, 1},
    {2, 1, 0, 1},
    {4, 0, 0, 1},
    {2,-1, 1, 1},
    {2,-2, 0,-1},
    {0, 0, 1, 3},
    {2, 1, 1,-1},
    {1, 1, 0,-1},
    {1, 1, 0, 1},
    {0, 1,-2,-1},
    {2, 1,-1,-1},
    {1, 0, 1, 1},
    {2,-1,-2,-1},
    {0, 1, 2, 1},
    {4, 0,-2,-1},
    {4,-1,-1,-1},
    {1, 0, 1,-1},
    {4, 0, 1,-1},
    {1, 0,-1,-1},
    {4,-1, 0,-1},
    {2,-2, 0, 1}
};

static long lTerms[NTERMS] = {
     6288774,
     1274027,
      658314,
      213618,
     -185116,
     -114332,
       58793,
       57066,
       53322,
       45758,
      -40923,
      -34720,
      -30383,
       15327,
      -12528,
       10980,
       10675,
       10034,
	8548,
       -7888,
       -6766,
       -5163,
	4987,
	4036,
	3994,
	3861,
	3665,
       -2689,
       -2602,
	2390,
       -2348,
	2236,
       -2120,
       -2069,
	2048,
       -1773,
       -1595,
	1215,
       -1110,
	-892,
	-810,
	 759,
	-713,
	-700,
	 691,
	 596,
	 549,
	 537,
	 520,
	-487,
	-399,
	-381,
	 351,
	-340,
	 330,
	 327,
	-323,
	 299,
	 294,
	   0
};

static long rTerms[NTERMS] = {
   -20905355,
    -3699111,
    -2955968,
     -569925,
       48888,
       -3149,
      246158,
     -152138,
     -170733,
     -204586,
     -129620,
      108743,
      104755,
       10321,
	   0,
       79661,
      -34782,
      -23210,
      -21636,
       24208,
       30824,
       -8379,
      -16675,
      -12831,
      -10445,
      -11650,
       14403,
       -7003,
	   0,
       10056,
	6322,
       -9884,
	5751,
	   0,
       -4950,
	4130,
	   0,
       -3958,
	   0,
	3258,
	2616,
       -1897,
       -2117,
	2354,
	   0,
	   0,
       -1423,
       -1117,
       -1571,
       -1739,
	   0,
       -4421,
	   0,
	   0,
	   0,
	   0,
	1165,
	   0,
	   0,
	8752
};

static long bTerms[NTERMS] = {
     5128122,
      280602,
      277693,
      173237,
       55413,
       46271,
       32573,
       17198,
	9266,
	8822,
	8216,
	4324,
	4200,
       -3359,
	2463,
	2211,
	2065,
       -1870,
	1828,
       -1794,
       -1749,
       -1565,
       -1491,
       -1475,
       -1410,
       -1344,
       -1335,
	1107,
	1021,
	 833,
	 777,
	 671,
	 607,
	 596,
	 491,
	-451,
	 439,
	 422,
	 421,
	-366,
	-351,
	 331,
	 315,
	 302,
	-283,
	-229,
	 223,
	 223,
	-220,
	-220,
	-185,
	 181,
	-177,
	 176,
	 166,
	-164,
	 132,
	-119,
	 115,
	 107
};

static void highmoon(double jd, double *l, double *b, double *r)
{
    double t, t2, t3, t4, lprime, d, m, mprime, f, a1, a2, a3, e[3],
	   sigmaL, sigmaB, sigmaR, ang;
    int i;

    t = (jd - J2000) / JulianCentury;

    t4 = t * (t3 = t * (t2 = t * t));

    lprime = 218.3164591 + 481267.88134236 * t - 0.0013268 * t2 +
		t3 / 538841.0 - t4 / 65194000.0;

    d = 297.8502042 + 445267.1115168 * t - 0.00163 * t2 +
		t3 / 545868.0 - t4 / 113065000.0;

    m = 357.5291092 + 35999.0502909 * t - 0.0001536 * t2 +
		t3 / 24490000.0;

    mprime = 134.9634114 + 477198.8676313 * t + 0.008997 * t2 +
		t3 / 69699.0 - t4 / 14712000.0;

    f = 93.2720993 + 483202.0175273 * t - 0.0034029 * t2 -
		t3 / 3526000.0 + t4 / 863310000.0;

    a1 = 119.75 + 131.849 * t;

    a2 = 53.09 + 479264.29 * t;

    a3 = 313.45 + 481266.484 * t;

    e[0] = 1;
    e[1] = 1 - 0.002516 * t - 0.0000074 * t2;
    e[2] = e[1] * e[1];

    lprime = fixangr(dtr(lprime));
    d = fixangr(dtr(d));
    m = fixangr(dtr(m));
    mprime = fixangr(dtr(mprime));
    f = fixangr(dtr(f));
    a1 = fixangr(dtr(a1));
    a2 = fixangr(dtr(a2));
    a3 = fixangr(dtr(a3));

    sigmaL = sigmaB = sigmaR = 0;

    for (i = 0; i < NTERMS; i++) {
	ang = lrCoeff[i][0] * d + lrCoeff[i][1] * m +
	      lrCoeff[i][2] * mprime + lrCoeff[i][3] * f;
	sigmaL += lTerms[i] * sin(ang) * e[abs(lrCoeff[i][1])];
	if (rTerms[i] != 0) {
	    sigmaR += rTerms[i] * cos(ang) * e[abs(lrCoeff[i][1])];
	}

	ang = bCoeff[i][0] * d + bCoeff[i][1] * m +
	      bCoeff[i][2] * mprime + bCoeff[i][3] * f;
	sigmaB += bTerms[i] * sin(ang) * e[abs(bCoeff[i][1])];
    }

    sigmaL += 3958.0 * sin(a1) + 1962.0 * sin(lprime - f) +
	      318.0 * sin(a2);

    sigmaB += -2235.0 * sin(lprime) + 382.0 * sin(a3) + 175.0 * sin(a1 - f) +
	      175.0 * sin(a1 + f) + 127.0 * sin(lprime - mprime) -
	      115.0 * sin(lprime + mprime);

    *l = rtd(lprime) + sigmaL / 1000000.0;
    *b = sigmaB / 1000000.0;
    *r = 385000.56 + sigmaR / 1000.0;
}

/*  OBLIQEQ  --  Calculate the obliquity of the ecliptic for a given Julian
                 date.  This uses Laskar's tenth-degree polynomial fit
		 (J. Laskar, Astronomy and Astrophysics, Vol. 157, page 68 [1986])
		 which is accurate to within 0.01 arc second between AD 1000
		 and AD 3000, and within a few seconds of arc for +/-10000
                 years around AD 2000.  If we're outside the range in which
		 this fit is valid (deep time) we simply return the J2000 value
		 of the obliquity, which happens to be almost precisely the mean.  */

static double obliqeq(double jd)
{
#define Asec(x) ((x) / 3600.0)

    static double oterms[10] = {
	Asec(-4680.93),
	Asec(	-1.55),
	Asec( 1999.25),
	Asec(  -51.38),
	Asec( -249.67),
	Asec(  -39.05),
	Asec(	 7.12),
	Asec(	27.87),
	Asec(	 5.79),
	Asec(	 2.45)
    };

    double eps = 23 + (26 / 60.0) + (21.448 / 3600.0), u, v;
    int i;

    v = u = (jd - J2000) / (JulianCentury * 100);

    if (abs(u) < 1.0) {
	for (i = 0; i < 10; i++) {
	    eps += oterms[i] * v;
	    v *= u;
	}
    }
    return eps;
}
		  
/*  ECLIPTOEQ  --  Convert celestial (ecliptical) longitude and latitude into
		   right ascension (in degrees) and declination.  We must supply
		   the time of the conversion in order to compensate correctly
		   for the varying obliquity of the ecliptic over time.  */

static void ecliptoeq(double jd, double Lambda, double Beta, double *Ra, double *Dec)
{
    double eps;

    /* Obliquity of the ecliptic. */

    eps = dtr(obliqeq(jd));

    *Ra = fixangle(rtd(atan2((cos(eps) * sin(dtr(Lambda)) -
			 (tan(dtr(Beta)) * sin(eps))), cos(dtr(Lambda)))));
    *Dec = rtd(asin((sin(eps) * sin(dtr(Lambda)) * cos(dtr(Beta))) +
		 (sin(dtr(Beta)) * cos(eps))));

}

/*  PROJILLUM  --  Project illuminated area on the map.  */

static void projillum(short *wtab, int xdots, int ydots, double dec)
{
    int i, ftf = TRUE, ilon, ilat, lilon, lilat, xt;
    double m, x, y, z, th, lon, lat, s, c;

    /* Clear unoccupied cells in width table */

    for (i = 0; i < ydots; i++) {
	wtab[i] = -1;
    }

	printf("Sun dec:%.2lf\n", dec);
	
    /* Build transformation for declination */

    s = sin(-dtr(dec));
    c = cos(-dtr(dec));

    /* Increment over a semicircle of illumination */

    for (th = -(PI / 2); th <= PI / 2 + 0.001; th += PI / TERMINC) {

	/* Transform the point through the declination rotation. */

	x = -s * sin(th);
	y = cos(th);
	z = c * sin(th);

	/* Transform the resulting co-ordinate through the
	   map projection to obtain screen co-ordinates. */

	lon = (y == 0 && x == 0) ? 0.0 : rtd(atan2(y, x));
	lat = rtd(asin(z));

	printf("%.2lf,%.2lf,%.2lf,%.2lf\n", dec, th, lat, lon);
	
	ilat = (int) (ydots - (lat + 90) * (ydots / 180.0));
	ilon = (int) (lon * (xdots / 360.0));

	if (ftf) {

	    /* First time.  Just save start co-ordinate. */

	    lilon = ilon;
	    lilat = ilat;
	    ftf = FALSE;
	} else {

	    /* Trace out the line and set the width table. */

	    if (lilat == ilat) {
		wtab[(ydots - 1) - ilat] = 2 * (ilon == 0 ? 1 : ilon);
	    } else {
		m = ((double) (ilon - lilon)) / (ilat - lilat);
		for (i = lilat; i != ilat; i += sgn(ilat - lilat)) {
		    xt = (int) (lilon + floor((m * (i - lilat)) + 0.5));
		    wtab[(ydots - 1) - i] = 2 * (xt == 0 ? 1 : xt);
		}
	    }
	    lilon = ilon;
	    lilat = ilat;
	}
    }

    /* Now tweak the widths to generate full illumination for
       the correct pole. */

    if (dec < 0.0) {
	ilat = ydots - 1;
	lilat = -1;
    } else {
	ilat = 0;
	lilat = 1;
    }

    for (i = ilat; i != ydots / 2; i += lilat) {
	if (wtab[i] != -1) {
	    while (TRUE) {
		wtab[i] = (short) xdots;
		if (i == ilat) {
		    break;
		}
		i -= lilat;
	    }
	    break;
	}
    }
}

static short *wtab = NULL, *wtab1 = NULL, *wtabs;
static int wtabsize;		      /* Width table size */

static BITMAPFILEHEADER bfh;
static struct bmih {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[256];
} bmi, *tmbmap;
static int bmRowLen;
static unsigned char *bmBits = NULL, *tmbits;
static unsigned char ct[4];

static time_t cctime;
static double jt, sunra, sundec, sunrv, sunlong, gt,
	      ssplat, ssplong, sspheight;
static struct tm ctim;
static struct satellite sat;
static int satmenuspec = FALSE;
static char *progpath;
static char viewdesc[132];
static char sline1[80], sline2[80], sline3[80];

#ifdef DBLOG
static FILE *db;
#define DB(x)   fprintf(db, "%s\n", x)
#else
#define DB(x)
#endif

#define rB(x)	(x) = getc(fp)
#define rS(x)	fread(ct, 2, 1, fp); (x) = ((ct[1] << 8) | ct[0])
#define rL(x)	fread(ct, 4, 1, fp); (x) = (((long) ct[3] << 24) | ((long) ct[2] << 16) | (ct[1] << 8) | ct[0])

/*  LOCFILE  --  Expand local file name to full directory name from
                 which we're executing this program.  */

static char *locfile(char *basename)
{
    static char pn[512];

    strcpy(pn, progpath);
    if (strrchr(pn, '/') != NULL) {
        *(strrchr(pn, '/') + 1) = EOS;
    } else {
        strcpy(pn, "./");
    }
    strcat(pn, basename);
    return pn;
}

static void load_bitmap(char *bmpfile)
{
    FILE *fp = fopen(bmpfile, "r");

    if (bmBits != NULL) {
	free(bmBits);
	bmBits = NULL;
    }
    if (fp != NULL) {
	rS(bfh.bfType);
	rL(bfh.bfSize);
	rS(bfh.bfReserved1);
	rS(bfh.bfReserved2);
	rL(bfh.bfOffBits);

	rL(bmi.bmiHeader.biSize);
	rL(bmi.bmiHeader.biWidth);
	rL(bmi.bmiHeader.biHeight);
	rS(bmi.bmiHeader.biPlanes);
	rS(bmi.bmiHeader.biBitCount);
	rL(bmi.bmiHeader.biCompression);
	rL(bmi.bmiHeader.biSizeImage);
	rL(bmi.bmiHeader.biXPelsPerMeter);
	rL(bmi.bmiHeader.biYPelsPerMeter);
	rL(bmi.bmiHeader.biClrUsed);
	rL(bmi.bmiHeader.biClrImportant);

	assert(bmi.bmiHeader.biBitCount == 8);
	assert(bmi.bmiHeader.biCompression == BI_RGB);

	if (bmi.bmiHeader.biClrUsed == 0) {
	    bmi.bmiHeader.biClrUsed = 1 << bmi.bmiHeader.biBitCount;
	}

	/* Read the colour palette table. */

	fread(&bmi.bmiColors[0], sizeof(RGBQUAD), bmi.bmiHeader.biClrUsed, fp);

	/* Compute the row length and read the bitmap. */

	bmRowLen = (bmi.bmiHeader.biWidth + 3) & ~3;

	fseek(fp, bfh.bfOffBits, SEEK_SET);
	bmBits = (unsigned char *) malloc(bmRowLen * bmi.bmiHeader.biHeight);
	fread(bmBits, bmRowLen * bmi.bmiHeader.biHeight, 1, fp);

	fclose(fp);
    }
}

/*  MOVETERM  --  Update illuminated portion of the globe.  */

static void moveterm(short *wtab, int noon)
{
    int i, j, oh, nl, nh;

    BITMAPINFOHEADER *bh;
    int wid, hgt;
    unsigned char *pa;

    bh = &bmi.bmiHeader;

    wid = bh->biWidth;
    hgt = bh->biHeight;

#define PixA(x, y) (bmBits + (x) + (((DWORD) ((hgt - 1) - (y))) * bmRowLen))
    for (i = 0; i < (int) hgt; i++) {
	if (wtab[i] < 0) {
	    pa = PixA(0, i);
	    for (j = 0; j < wid; j++) {
		*pa++ &= 0x7F;
	    }
	} else {

	    pa = PixA(0, i);
	    for (j = 0; j < wid; j++) {
		*pa++ &= 0x7F;
	    }

	    nl =  ((noon - (wtab[i] / 2)) + wid) % wid;
	    nh = (nl + wtab[i]) - 1;

	    oh = (nh - nl) + 1;
	    if ((nl + oh) > wid) {
		pa = PixA(nl, i);
		for (j = nl; j < wid; j++) {
		    *pa++ |= 0x80;;
		}
		pa = PixA(0, i);
		for (j = 0; j < (((nl + oh) - wid) + 1); j++) {
		    *pa++ |= 0x80;
		}
	    } else {
		pa = PixA(nl, i);
		for (j = nl; j < ((nl + oh) + 1); j++) {
		    *pa++ |= 0x80;
		}
	    }
	}
    }
#undef PixA
}

/*  UPDIMAGE  --  Update current displayed image.  */

static void updimage(void)
{
    int i, xl;

    /* Allocate (or reallocate, if already in existence) the projected
       illumination width tables. */

    if (wtab != NULL) {
	free(wtab);
	free(wtab1);
    }
    wtabsize = bmi.bmiHeader.biHeight;
    wtab = (short *) malloc((unsigned int) wtabsize * sizeof(short));
    wtab1 = (short *) malloc((unsigned int) wtabsize * sizeof(short));
    for (i = 0; i < wtabsize; i++) {
	wtab[i] = wtab1[i] = -1;
    }

    projillum(wtab, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, sundec);
    wtabs = wtab;
    wtab = wtab1;
    wtab1 = wtabs;

    sunlong = fixangle(180.0 + (sunra - (gt * 15)));
    xl = (int) (sunlong * (bmi.bmiHeader.biWidth / 360.0));

    /* If the subsolar point has moved at least one pixel, update
       the illuminated area on the screen.  */

    moveterm(wtab1, xl);
}

int main(int argc, char *argv[])
{
    double vlat, vlon, valt, subslong,
	   tlat = 0, tlon = 0, talt = 35785,
	   moonclong, moonclat, mooncrv, moonra, moondec, moonlong,
	   cphase, aom, cdist, cangdia, csund, csuang;
    int i, f = 0, outmap = FALSE, locspec = FALSE, srcindex = 2,
	tmsize = 320, wppm = FALSE, sattrack = FALSE, html = FALSE,
	daynight = TRUE;
    char *cp, opt, *wxmap = NULL;
    char line1[80], line2[80], line3[80];
    FILE *ofile = stdout;
    static char *imagesource = "earth-topo.bmp";
    static char *mname[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
        "Nov", "Dec" };

    line1[0] = line2[0] = line3[0];
    sline1[0] = sline2[0] = sline3[0];
    strcpy(viewdesc, "Undefined location (Sun used by default)");
#ifdef DBLOG
    db = fopen("/tmp/dblog", "w");
#endif
    progpath = argv[0]; 	      /* Save path used to call us */
    time(&cctime);
    ctim = *gmtime(&cctime);
    jt = jtime(&ctim);

    sunpos(jt, FALSE, &sunra, &sundec, &sunrv, &sunlong);
    gt = gmst(jt);

    subslong = (gt * 15) - sunra;
    if (subslong > 180) {
	subslong = -(360 - subslong);
    } else if (subslong < -180) {
	subslong += 360;
    }
    vlat = dtr(sundec);
    vlon = dtr(subslong);
    valt = sunrv * SunSMAX - EarthRad;

	printf("jt:%.2lf vlat: %.2lf vlon: %.2lf valt: %.2lf\n", jt, sundec, subslong, valt);
	
    imagesource = "earth-topo-s.bmp";

    load_bitmap(locfile(imagesource));

	updimage();

	ofile = fopen("temp.gif","wb");
	    gifout(bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, bmRowLen,
		   bmi.bmiColors, bmi.bmiHeader.biClrUsed,
		   bmBits, ofile);
	fclose(ofile);

	return 0;
}