/*

		Earth Viewer  --  Main server program

	       by John Walker  --  kelvin@fourmilab.ch
		 <http://www.fourmilab.ch/index.html>

*/

#include "vplanet.h"

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

static long cctime;
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

static void dump_ppm(char *fname, struct bmih *bi, unsigned char *bits)
{
    FILE *fp;
    int i, j;

    if (fname[0] == '-') {
	fp = stdout;
    } else {
        fp = fopen(fname, "w");
    }
    assert(fp != NULL);
    fprintf(fp, "P6 %ld %ld %d\n", bi->bmiHeader.biWidth, bi->bmiHeader.biHeight, 255);
    for (i = 0; i < bi->bmiHeader.biHeight; i++) {
	unsigned char *px = bits +
	    (((bi->bmiHeader.biHeight - 1) - i) *
	    ((bi->bmiHeader.biWidth + 3) & ~3));

	for (j = 0; j < bi->bmiHeader.biWidth; j++) {
	    int p = *px++;

	    putc(bi->bmiColors[p].rgbRed, fp);
	    putc(bi->bmiColors[p].rgbGreen, fp);
	    putc(bi->bmiColors[p].rgbBlue, fp);
	}
    }
    if (fp != stdout) {
	fclose(fp);
    }
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
}
#undef PixA


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

/*  TEXTUREMAPIMAGE  --  Create texture mapped image of globe.	*/

static void textureMapImage(int isize, int tmwidth, int tmheight,
			    int diwidth, int diheight, int showterm,
			    double satlat, double satlon, double satalt)
{
    Vector satpos, v, n, u, up, ippo, raydir, iv;
    double acslat = abs(cos(satlat)), a, l, alpha, vheight, zclip,
	   vl, ui, vi, b, disc, t, tmwo2pi, tmhopi,
	   normTemp;
    int x, y, tmx, tmy, iso2, tmwo2, tmho2, tmhm1;
    BITMAPINFOHEADER *bh, *obmap;
    WORD ncol, offbits, rowlen, tmrowlen;
    unsigned char *opixel;
    unsigned char *tpixel;
    unsigned char termmask;

#define FastVecNorm(a)	    assert(VecLen(a) > 0.0); normTemp = 1.0 / VecLen(a); VecScale(normTemp, a)

    termmask = (unsigned char) (showterm ? 0x0 : 0x80);
    bh = &bmi.bmiHeader;
    ncol = (WORD) (bh->biClrUsed == 0 ? (1L << bh->biBitCount) : bh->biClrUsed);
    offbits = (WORD) (bh->biSize + ncol * sizeof(RGBQUAD));
    tpixel = bmBits;
    tmrowlen = ((tmwidth + (sizeof(LONG) - 1)) / sizeof(LONG)) * sizeof(LONG);
    tmhm1 = tmheight - 1;

    if (tmbmap != NULL) {
	free(tmbmap);
	tmbmap = NULL;
    }
    rowlen = ((diwidth + (sizeof(LONG) - 1)) / sizeof(LONG)) * sizeof(LONG);
    obmap = (BITMAPINFOHEADER *) malloc(offbits + diheight * rowlen);
    memset((char *) obmap, 0, offbits + diheight * rowlen);
    memcpy((char *) obmap, (char *) bh, offbits);
    obmap->biWidth = diwidth;
    obmap->biHeight = diheight;
    obmap->biSizeImage = ((DWORD) diheight) * rowlen;
    opixel = ((unsigned char *) (obmap)) + offbits;
    tmbmap = (struct bmih *) obmap;
    tmbits = (unsigned char *) opixel;

#define PixA(x, y) ((tpixel + ((x) + (((DWORD) (tmhm1 - (y))) * (tmrowlen)))))
#define Opix(x, y) ((opixel + ((x) + (((DWORD) ((diheight - 1) - (y))) * (rowlen)))))

    a = satalt + EarthRad;
    assert(a > 0.0);
    MakeVector(a * cos(satlon) * acslat, a * sin(satlon) * acslat, -a * sin(satlat), satpos);
    l = sqrt(a * a - EarthRad * EarthRad);
    alpha = asin(EarthRad / a);
    vheight = l * sin(alpha);
    zclip = a - (l * cos(alpha));

    VecCopy(satpos, v);
    assert(VecLen(v) > 0.0);
    vl = -1.0 / VecLen(v);
    VecScale(vl, v);		/* Invert and normalise at once */

    /* n = Unit direction vector from eye position to North pole */
    MakeVector(0, 0, EarthRad, n);
    VecSub(n, satpos, n);
    FastVecNorm(n);

    /* u = v X n -- View right vector */
    VecCross(v, n, u);
    FastVecNorm(u);

    /* up = u x v -- View up vector */
    VecCross(u, v, up);

    VecCopy(satpos, ippo);
    VecScale(zclip / a, ippo);

    /* Precompute some values we'll need repeatedly inside the big loop */

    iso2 = isize / 2;
    tmwo2 = tmwidth / 2;
    tmho2 = tmheight / 2;
    tmwo2pi = (tmwidth / 2) / PI;
    tmhopi = tmheight / PI;

    for (y = 0; y < isize; y++) {
	unsigned char *opix = Opix(0, y);
	double viXupX, viXupY, viXupZ;
	double visq;

	vi = -(((double) y) - iso2) / iso2;
	visq = vi * vi;

	viXupX = vi * up[X];
	viXupY = vi * up[Y];
	viXupZ = vi * up[Z];

	for (x = 0; x < isize; x++) {
	    ui = (((double) x) - iso2) / iso2;

	    if (((ui * ui) + visq) <= 1.0) {

		raydir[X] = (ippo[X] - (ui * u[X] + viXupX) * vheight) - satpos[X];
		raydir[Y] = (ippo[Y] - (ui * u[Y] + viXupY) * vheight) - satpos[Y];
		raydir[Z] = (ippo[Z] - (ui * u[Z] + viXupZ) * vheight) - satpos[Z];
		FastVecNorm(raydir);

		iv[X] = -satpos[X];
		iv[Y] = -satpos[Y];
		iv[Z] = -satpos[Z];
		b = VecDot(iv, raydir);
		disc = (b * b - VecDot(iv, iv)) + EarthRad * EarthRad;
		if (disc >= 0) {	    /* If disc < 0, no intersection */
		    disc = sqrt(disc);
		    t = ((b - disc) < 1e-6) ? b + disc : b - disc;

			tmx = (int) (tmwo2 - (tmwo2pi * atan2(t * raydir[Y] + satpos[Y], t * raydir[X] + satpos[X])));
			tmy = (int) (tmho2 + (tmhopi * asin((t * raydir[Z] + satpos[Z]) / EarthRad)));

			opix[x] = *(PixA(tmx, tmy)) | termmask;
		}
	    }
	}
    }
}

static double currorbit;
static int eclipsed;

/*  UPDSAT  --	Update satellite position.  */

static void updsat(ct)
  struct tm *ct;
{
    double now, smaxis, raanp, perigeep, reforbit, epday,
	   avgmotion, currmotion, meana, truea,
	   satx, saty, satz, radius, satvx, satvy, satvz;
    long iorbitnum;
    static int initialised = FALSE;

    now = GetDayNum(ct->tm_year + 1900, ct->tm_mon + 1, ct->tm_mday);
    now += (ct->tm_hour / 24.0) + (ct->tm_min / (24.0 * 60)) +
	   (ct->tm_sec / (24.0 * 60.0 * 60.0));
    if (!initialised) {
	InitOrbitRoutines(now);
	initialised = TRUE;
    }
    epday = sat.epochday + GetDayNum(sat.epochyear, 1, 0);
    smaxis = 331.25 * exp(2.0 * log((24 * 60.0) / sat.meanmotion) / 3.0);
    GetPrecession(smaxis, sat.eccentricity, dtr(sat.inclination),
	&raanp, &perigeep);
    reforbit = dtr(sat.meananomaly) / (2 * PI) + sat.revno;

    avgmotion = sat.meanmotion + (now - epday) * (sat.meanmotiond1 / 2);
    currmotion = sat.meanmotion + (now - epday) * sat.meanmotiond1;
    smaxis = 331.25 * exp(2.0 * log((24 * 60.0) / currmotion) / 3.0);
    currorbit = reforbit + (now - epday) * avgmotion;
    iorbitnum = currorbit;

    meana = (currorbit - iorbitnum) * (2 * PI);
    truea = Kepler(meana, sat.eccentricity);

    GetSatPosition(epday, dtr(sat.rascendnode), dtr(sat.argperigee),
	smaxis, dtr(sat.inclination), sat.eccentricity, raanp, perigeep,
	now, truea, &satx, &saty, &satz, &radius, &satvx, &satvy, &satvz);

    GetSubSatPoint(satx, saty, satz, now, &ssplat, &ssplong, &sspheight);
    eclipsed = Eclipsed(satx, saty, satz, radius, now);
}

/*  PARSEDEG  --  Parse latitude and longitude degree specifications.  */

static double parseDeg(char *dec)
{
    if (strchr(dec, 'd') != NULL ||
        strchr(dec, 'D') != NULL ||
        strchr(dec, '°') != NULL) {
	double dd = 0, mm = 0, ss = 0;
	char c1, c2;

        sscanf(dec, "%lf%c%lf%c%lf", &dd, &c1, &mm, &c2, &ss);
	return sgn(dd) * (abs(dd) + (mm / 60) + (ss / 3600));
    } else {
	return atof(dec);
    }
}

/*  SSEARCH  --  Call-back for looking up satellites. */

static char *ssought = NULL;          /* Satellite we're looking for */

static void ssearch(struct satellite *ssat, char *l1, char *l2, char *l3)
{
    if (strcasecmp(ssat->satname, ssought) == 0) {
DB("Found sat");
DB(ssat->satname);
	satmenuspec = TRUE;
	sat = *ssat;
	strcpy(sline1, l1);
	strcpy(sline2, l2);
	strcpy(sline3, l3);
    }
}

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

/*  EDDEG  --  Edit degrees and minutes.  */

static char *eddeg(double ds)
{
    static char buf[2][20];
    static int n = 0;
    double d = abs(ds);

    n = (n + 1) % 2;
    sprintf(buf[n], "%s%d\260", ds < 0 ? "-" : "", (int) d);
    d = 60 * (d - ((int) d));
    if (((int) d) > 0) {
        sprintf(buf[n] + strlen(buf[n]), "%d'", (int) d);
    }
    return buf[n];
}

/*  EDLAT  --  Edit latitude.  */

static char *edlat(double lat)
{
    static char slat[30];
    double ulat = fixangle(rtd(lat));

    if (ulat >= 180) {
	ulat = -(360 - ulat);
    }
    sprintf(slat, "%s%s", eddeg(abs(ulat)), ulat < 0 ? "S" : "N");
    return slat;
}

/*  EDLON  --  Edit longitude.	*/

static char *edlon(double lon)
{
    static char slon[30];
    double ulon = fixangle(rtd(lon));

    if (ulon >= 180) {
	ulon = -(360 - ulon);
    }
    sprintf(slon, "%s%s", eddeg(abs(ulon)), ulon < 0 ? "E" : "W");
    return slon;
}

/*  EDVPOS  --	Edit viewing position and return string.  */

static char *edvpos(double tlat, double tlon, double talt)
{
    static char vpos[80];

    sprintf(vpos, "%.f km above %s %s",
	talt, edlat(tlat), edlon(tlon));

    return vpos;
}

/*  WRITEPOST  --  Write balance of HTML file, including controls
		   which permit regenerating the view.	*/

static void writepost(FILE *fp, double lat, double lon, double alt,
    char *bmpname, int srcindex, int tmsize, char *satname, 
    char *line1, char *line2, char *line3, int daynight)
{

    lon = fixangle(rtd(lon));
    if (lon >= 180) {
	lon = -(360 - lon);
    }
    lat = fixangle(rtd(lat));
    if (lat >= 180) {
	lat = -(360 - lat);
    }

#define W(x)    fprintf(fp, "%s\n", x)
#ifdef TESTMODE
    W("<form method=\"post\" action=\"/cgi-bin/uncgi/Earth.t\">");
#else
    W("<form method=\"post\" action=\"/cgi-bin/uncgi/Earth\">");
#endif
    W("<center>");
    W("<input type=\"submit\" value=\"Update\"><br>");

    W("Display:");
    fprintf(fp, "<input type=\"radio\" name=\"opt\" value=\"-p\"%s> Map,\n",
        srcindex == 1 ? " checked" : 0);
    fprintf(fp, "<input type=\"radio\" name=\"opt\" value=\"-s\"%s> From Sun,\n",
        srcindex == 2 ? " checked" : 0);
    fprintf(fp, "<input type=\"radio\" name=\"opt\" value=\"-m\"%s> From Moon,\n",
        srcindex == 3 ? " checked" : 0);
    fprintf(fp, "<input type=\"radio\" name=\"opt\" value=\"-n\"%s> Night side,<br>\n",
        srcindex == 4 ? " checked" : 0);
    fprintf(fp, "<input type=\"radio\" name=\"opt\" value=\"-l\"%s>\n",
        srcindex == 5 ? " checked" : 0);

    W("<nobr>");
    W("Lat:");
    fprintf(fp, "<input type=\"text\" name=\"lat\" value=\"%s\" size=\"10\">\n",
	eddeg(abs(lat)));
    fprintf(fp, "<input type=\"radio\" name=\"ns\" value = \"North\"%s> N\n",
        lat > 0 ? " checked" : "");
    fprintf(fp, "<input type=\"radio\" name=\"ns\" value = \"South\"%s> S\n",
        lat < 0 ? " checked" : "");
    W("</nobr>");

    W("<nobr>");
    W("Long:");
    fprintf(fp, "<input type=\"text\" name=\"lon\" value=\"%s\" size=\"10\">\n",
	eddeg(abs(lon)));
    fprintf(fp, "<input type=\"radio\" name=\"ew\" value = \"East\"%s> E\n",
        lon < 0 ? " checked" : "");
    fprintf(fp, "<input type=\"radio\" name=\"ew\" value = \"West\"%s> W\n",
        lon > 0 ? " checked" : "");
    W("</nobr>");

    W("<nobr>");
    W("Alt:");
    fprintf(fp, "<input type=\"text\" name=\"alt\" value=\"%.f\" size=\"10\"> km\n",
	alt);
    W("</nobr>");
    W("<br>");

    if (srcindex == 6) {
        fprintf(fp, "<input type=\"radio\" name=\"opt\" value=\"-b\"%s> From satellite: %s\n",
            srcindex == 6 ? " checked" : 0, satname);
        W("<a href=\"/earthview/satellite.html\">New satellite</a><br>");
        W("<br>");
        fprintf(fp, "Launched: %d  Incl: %.2f  Eccen: %.2f  Rev/day: %.2f  Orbit: %.f  %s\n",
	    sat.intdes.launchyear + 1900 +
		(sat.intdes.launchyear < 57 ? 100 : 0),
	    sat.inclination, sat.eccentricity, sat.meanmotion, currorbit,
            eclipsed ? "Eclipsed" : "");

        fprintf(fp, "<textarea name=tle rows=3 cols=70>%s\n", line1);
	W(line2);
	W(line3);
        W("</textarea><br>");
    } else {
        W("<a href=\"/earthview/satellite.html\">Choose satellite</a><br>");
    }

    fprintf(fp, "Image: <input type=\"radio\" name=\"img\" value=\"earth-topo.bmp\"%s> Topo map\n",
        strcmp(bmpname, "earth-topo.bmp") == 0 ? " checked" : "");
    fprintf(fp, "<input type=\"radio\" name=\"img\" value=\"cloudy.bmp\"%s> Current cloud cover\n",
        strcmp(bmpname, "cloudy.bmp") == 0 ? " checked" : "");
    fprintf(fp, "<input type=\"radio\" name=\"img\" value=\"irsat.bmp\"%s> IR clouds\n",
        strcmp(bmpname, "irsat.bmp") == 0 ? " checked" : "");
    fprintf(fp, "Size: <input type=\"text\" name=\"imgsize\" value=\"%d\" size=\"6\">\n",
	tmsize);
    fprintf(fp, "<input type=\"checkbox\" name=\"daynight\" value=\"-d\"%s> No night\n",
        !daynight ? " checked" : "");

    W("</center>");
    W("</form>");
}

/* Main program. */

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

DB("Args");
    for (i = 1; i < argc; i++) {
	cp = argv[i];
DB(cp);
        if (*cp == '-' && !isdigit(cp[1])) {
	    opt = *(++cp);
	    if (islower(opt))
		opt = toupper(opt);
	    switch (opt) {

                case 'B':             /* Track satellite from elements */
		    sattrack = TRUE;
		    break;

                case 'D':             /* Suppress day / night ? */
		    daynight = FALSE;
		    break;

                case 'H':             /* Generate HTML output */
		    html = TRUE;
		    break;

                case 'I':             /* Globe image size */
		    tmsize = atoi(cp + 1);
		    tmsize = max(16, min(tmsize, 1024));
		    break;

                case 'L':             /* Use specified lat, long, alt */ 
		    locspec = TRUE;
		    break;

                case 'M':             /* View from Moon */
		    highmoon(jt, &moonclong, &moonclat, &mooncrv);
		    ecliptoeq(jt, moonclong, moonclat, &moonra, &moondec);
		    moonlong = (gt * 15) - moonra;
		    (void) phase(jt, &cphase, &aom, &cdist, &cangdia, &csund, &csuang);
		    if (moonlong > 180) {
			moonlong = -(360 - moonlong);
		    } else if (moonlong < -180) {
			moonlong += 360;
		    }
		    vlat = dtr(moondec);
		    vlon = dtr(moonlong);
		    valt = mooncrv - EarthRad;
                    sprintf(viewdesc, "View from Moon: %s",
			edvpos(vlat, vlon, valt));
		    srcindex = 3;
		    break;

                case 'N':             /* Night side */
		    subslong = (gt * 15) - sunra;
		    subslong = fixangle(subslong + 180);
		    if (subslong > 180) {
			subslong = -(360 - subslong);
		    } else if (subslong < -180) {
			subslong += 360;
		    }
		    vlat = dtr(-sundec);
		    vlon = dtr(subslong);
		    valt = sunrv * SunSMAX - EarthRad;
                    strcpy(viewdesc, "Night side");
		    srcindex = 4;
		    break;

                case 'O':             /* Output .GIF file */
                    ofile = fopen(cp + 1, "w");
		    if (ofile == NULL) {
			ofile = stdout;
		    }
		    break;

                case 'P':             /* Earth map */
		    outmap = TRUE;
                    sprintf(viewdesc, "%d %s %d %d:%02d UTC",
			ctim.tm_year + 1900, mname[ctim.tm_mon], ctim.tm_mday, 
			ctim.tm_hour, ctim.tm_min);
		    srcindex = 1;
		    break;

                case 'R':             /* Earth map bitmap source */
		    imagesource = cp + 1;
		    break;

                case 'S':             /* View from Sun */
		    subslong = (gt * 15) - sunra;
		    if (subslong > 180) {
			subslong = -(360 - subslong);
		    } else if (subslong < -180) {
			subslong += 360;
		    }
		    vlat = dtr(sundec);
		    vlon = dtr(subslong);
		    valt = sunrv * SunSMAX - EarthRad;
                    sprintf(viewdesc, "View from Sun: %s",
			edvpos(vlat, vlon, valt));
		    srcindex = 2;
		    break;

                case 'T':             /* View from named satellite */
                    if (strcasecmp(cp + 1, "From elements below") != 0) {
			ssought = cp + 1;
		    }
		    break;

                case 'V':             /* Specify weather map update time */
		    wxmap = cp + 1;
		    break;

                case 'W':             /* Write PPM to standard output */
		    wppm = TRUE;
		    break;

                case '?':
                case 'U':
    fprintf(stderr,"\nVPLANET.  Call");
    fprintf(stderr,
       "\n             with vplanet [options] [latitude [[longitude] [altitude]]]");
    fprintf(stderr,"\n");
    fprintf(stderr,"\n         Options:");
    fprintf(stderr,"\n              -b      Show view from satellite");
    fprintf(stderr,"\n              -d0     Don't show day and night");
    fprintf(stderr,"\n              -innn   Set Earth globe diameter to nnn pixels");
    fprintf(stderr,"\n              -l      View above location");
    fprintf(stderr,"\n              -m      View from Moon");
    fprintf(stderr,"\n              -n      Night side");
    fprintf(stderr,"\n              -p      Plot Mercator projection map");
    fprintf(stderr,"\n              -rfname Use image from file fname");
    fprintf(stderr,"\n              -s      View from Sun");
    fprintf(stderr,"\n              -tSat   Track named satellite");
    fprintf(stderr,"\n              -u      Print this message");
    fprintf(stderr,"\n              -v      Specify weather map update time");
    fprintf(stderr,"\n              -w      Write PPM, not GIF output");
    fprintf(stderr,"\n");
		    return 0;
	    }
	} else {
            if (isdigit(cp[0]) || cp[0] == '+' || cp[0] == '-') {
		switch (f) {
		    case 0:
			tlat = dtr(parseDeg(cp));
			f++;
			break;

		    case 1:
			tlon = dtr(parseDeg(cp));
			f++;
			break;

		    case 2:
			talt = atof(cp);
			f++;
			break;


		    default:
                        fprintf(stderr, "Extra coordinates ignored.\n");
		}
	    }
	}
    }

    if (locspec) {
	vlat = fixangr(tlat);
	vlon = fixangr(tlon);
	valt = talt;
        sprintf(viewdesc, "View from %s", edvpos(vlat, vlon, valt));
	srcindex = 5;
    }

    /* If a satellite was explicitly named, try to find its elements
       in the database. */

    if (sattrack && ssought != NULL) {
	int err, linerr;
	FILE *sfile;

        sfile = fopen(locfile("satellite.tle"), "r");
	if (sfile != NULL) {
	    insat(sfile, ssearch, &err, &linerr);
	    fclose(sfile);
	}
    }

    /* See if satellite elements are piped to standard input. */

    if (!satmenuspec && sattrack && fgets(line1, sizeof line1, stdin) &&
	fgets(line2, sizeof line2, stdin) &&
	fgets(line3, sizeof line3, stdin)) {
	    line1[strlen(line1) - 1] = line2[strlen(line2) - 1] =
		line3[strlen(line3) - 1] = EOS;
	    if (extractel(&sat, line1, line2, line3) != 0) {
		sattrack = FALSE;
	    } else {
		strcpy(sline1, line1);
		strcpy(sline2, line2);
		strcpy(sline3, line3);
		satmenuspec = TRUE;
	    }
    }

    if (sattrack && satmenuspec) {
	updsat(&ctim);
	vlat = ssplat;
	vlon = ssplong;
	valt = sspheight;

        sprintf(viewdesc, "View from %s<br>%s",
	    sat.satname,
	    edvpos(vlat, vlon, valt));
	srcindex = 6;
    }

    /* Kludge: if the image source is the (huge) NOAA topographic
       map and the user wants to see the day and night map, swap to
       a scaled-down version of the image that will fit on a typical
       user's screen and not clog the Net. */

    if (outmap && strcmp(imagesource, "earth-topo.bmp") == 0) {
        imagesource = "earth-topo-s.bmp";
    }

    load_bitmap(locfile(imagesource));
    assert(bmBits != NULL);

    /* Plot day and night images on the map. */

    if (daynight) {
	updimage();
    }

    if (html) {
	pid_t pid = getppid();

        printf("<html>\n<head>\n<title>\nEarth View\n</title>\n</head>\n");
        printf("<body>\n<center>\n<h2>%s</h2>\n", viewdesc);
        printf("<a href=\"ftp://www.fourmilab.ch/pub/kelvin/earthview/cache/%ld.gif\">\n",
	    pid);
	if (outmap) {
            printf("<img src=\"/earthview/cache/%ld.gif\" width=%d height=%d><br>\n",
		pid, (int) bmi.bmiHeader.biWidth, (int) bmi.bmiHeader.biHeight);
	} else {
            printf("<img src=\"/earthview/cache/%ld.gif\" width=%d height=%d><br>\n",
		pid, tmsize, tmsize);
	}
        printf("</a>\n");

        /* If we're showing cloud cover, indicate the time and date
	   the satellite imagery was made, and credit (with a link
	   to the site), the folks who compile and publish the
	   global composite image. */

        if (wxmap != NULL && strcmp(imagesource, "cloudy.bmp") == 0 ||
            strcmp(imagesource, "irsat.bmp") == 0) {
	    long year = ctim.tm_year + 1900;
	    int day, month, hour, min;
	    double jds, jdu;

            sscanf(wxmap, "%03dM%02d%02d", &day, &hour, &min);
	    day--;
	    /* Get JD at start of year. */
	    jds = ucttoj(year, 0, 1, 0, 0, 0);
	    /* JD on day image was taken. */
	    jdu = jds + day;
	    /* If image date (00:00 UTC) is greater than the current
	       date, this image is from the last day of the previous
               year, and it's just after midnight on New Year's
	       day.  Adjust the year and recompute JD. */
	    if (jdu > jt) {
		year--;
		jdu = ucttoj(year, 0, 1, 0, 0, 0) + day;
	    }
	    jyear(jdu, &year, &month, &day);
            printf("Infrared composite: %ld %s %d %d:%02d UTC<br>",
		year, mname[month - 1], day, hour, min);
            printf("Weather data from <a href=\"http://www.ssec.wisc.edu\">\
SSEC: UW-Madison</a><br>\n");
	}


	/* If the NOAA global topography image is selected, credit
	   the people who made it and provide a link to their site. */

        if (strncmp(imagesource, "earth-topo.bmp", 10) == 0) {
            printf("<a href=\"http://julius.ngdc.noaa.gov/mgg/announcements/announce_surface.html\">Global \
topographic map</a> courtesy of the\n");
            printf("<a href=\"http://www.ngdc.noaa.gov/mgg/\">Marine \
Geology and Geophysics Division</a>\n");
            printf("of the <a href=\"http://julius.ngdc.noaa.gov/\">National \
Geophysical Data Center</a>\n");
            printf("operated by the United States \
<a href=\"http://www.doc.gov/\">Department of Commerce</a>,\n");
            printf("<a href=\"http://www.noaa.gov/\">National Oceanic and \
Atmospheric Administration</a>.\n");
	}

        printf("</center>\n");
	writepost(stdout, vlat, vlon, valt, imagesource, srcindex,
	    tmsize, sat.satname, sline1, sline2, sline3, daynight);
    }

    if (outmap) {
	if (wppm) {
            dump_ppm("-", &bmi, bmBits);
	} else {
	    gifout(bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, bmRowLen,
		   bmi.bmiColors, bmi.bmiHeader.biClrUsed,
		   bmBits, ofile);
	}
    } else {

	/* Before we begin the texture map calculation, set the nice
	   priority to place this job below interactive programs on
	   the workstation.  The nice level chosen is based on the
	   size of the texture mapped image, so large image requests
	   run in the background with regard to smaller ones. */

	nice(6 + (tmsize / 100));

	valt = max(valt, 20);
/*fprintf(stderr, "View from Lat %.2f Long %.2f Alt %.2f\n", rtd(vlat), rtd(vlon), valt);*/
	textureMapImage(tmsize, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight,
			tmsize, tmsize, TRUE,
			vlat, vlon, valt);
	if (wppm) {
            dump_ppm("-", tmbmap, tmbits);
	} else {
	    int tmrowlen = (tmbmap->bmiHeader.biWidth + 3) & ~3;

	    gifout(tmbmap->bmiHeader.biWidth, tmbmap->bmiHeader.biHeight, tmrowlen,
		   tmbmap->bmiColors, tmbmap->bmiHeader.biClrUsed,
		   tmbits, ofile);
	}
    }

    free(bmBits);
    if (ofile != stdout) {
	fclose(ofile);
    }
#ifdef DBLOG
    fclose(db);
#endif
    return 0;
}
