#include "sysheads.h"

#include "config.h"

#ifdef SUPPORT_SHAPEFILES

#include "llutil.h"
#include "OSMUtil.h"
#include "tcputil.h"
#include "TraceLog.h"

#include "ShapFile.h"

#include "..\shapelib-1.3.0b2\shapefil.h"

/* From: http://www.softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm

References

David Douglas & Thomas Peucker, "Algorithms for the reduction of the number of points required to represent a digitized line or its caricature", The Canadian Cartographer 10(2), 112-122  (1973)

John Hershberger & Jack Snoeyink, "Speeding Up the Douglas-Peucker Line-Simplification Algorithm", Proc 5th Symp on Data Handling, 134-143 (1992).  UBC Tech Report available online from NEC ResearchIndex.
 
*/

// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// Assume that classes are already given for the objects:
//    Point and Vector with
//        coordinates {float x, y, z;}    // as many as are needed
//        operators for:
//            == to test equality
//            != to test inequality
//            (Vector)0 = (0,0,0)         (null vector)
//            Point  = Point ± Vector
//            Vector = Point - Point
//            Vector = Vector ± Vector
//            Vector = Scalar * Vector    (scalar product)
//            Vector = Vector * Vector    (cross product)
//    Segment with defining endpoints {Point P0, P1;}
//===================================================================

#define SUPPORT_SIMPLIFY
#ifdef SUPPORT_SIMPLIFY

#ifdef ALREADY_DECLARED
typedef struct Point
{	double x, y, z;
} Point;
#endif

#define x Lat
#define y Lon
typedef Point Vector;
typedef struct Segment
{	Point P0, P1;
} Segment;

// dot product (3D) which allows vector operations in arguments

//#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
double dot(Point u, Point v)
{	return u.x*v.x + u.y*v.y /*+u.z*v.z*/;
}

//#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
double norm2(Point v)
{	return dot(v,v);
}

//#define norm(v)    sqrt(norm2(v))  // norm = length of vector
double norm(Point v)
{	return sqrt(norm2(v));
}

Point *add(Point u, Point v)
{static Point r;
	r.x = u.x+v.x;
	r.y = u.y+v.y;
	/*r.z = u.z+v.z;*/
	return &r;
}

Point *diff(Point u, Point v)
{static Point r;
	r.x = u.x-v.x;
	r.y = u.y-v.y;
	/*r.z = u.z-v.z;*/
	return &r;
}

Point *mult(double m, Point p)
{static Point r;
	r.x = m*p.x;
	r.y = m*p.y;
	/*r.z = m*p.z;*/
	return &r;
}

//#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
double d2(Point u, Point v)
{	return norm2(*diff(u,v));
}

//#define d(u,v)     norm(u-v)       // distance = norm of difference
double d(Point u, Point v)
{	return norm(*diff(u,v));
}
void
simplifyDP( double tol, Point* v, int j, int k, int* mk );

// poly_simplify():
//    Input:  tol = approximation tolerance
//            V[] = polyline array of vertex points
//            n   = the number of points in V[]
//    Output: sV[]= simplified polyline vertices (max is n)
//    Return: m   = the number of points in sV[]
int
poly_simplify( double tol, Point* V, int n, Point* sV )
{	if (n <= 0) return 0;	/* Basket case */
    int    i, k, m, pv;            // misc counters
    double  tol2 = tol * tol;       // tolerance squared
    //Point* vt = new Point[n];      // vertex buffer
    //int*   mk = new int[n] = {0};  // marker buffer
	Point* vt = (Point*)calloc(n,sizeof(*vt));

    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = V[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
//        if (d2(V[i], V[pv]) < tol2)
//            continue;
        vt[k++] = V[i];
        pv = i;
    }
    if (pv < n-1)
        vt[k++] = V[n-1];      // finish at the end

	// STAGE 2.  Douglas-Peucker polyline simplification
    //int*   mk = new int[n] = {0};  // marker buffer
	int *mk = (int*)calloc(k,sizeof(*mk));
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, vt, 0, k-1, mk );

    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i])
            sV[m++] = vt[i];
    }
    //delete vt;
    //delete mk;
	free(vt);
	free(mk);
//TraceLogThread("ShapeCache", FALSE, "%ld -> %ld -> %ld\n", (long) n, (long) k, (long) m);
    return m;         // m vertices in simplified polyline
}

// simplifyDP():
//  This is the Douglas-Peucker recursive simplification routine
//  It just marks vertices that are part of the simplified polyline
//  for approximating the polyline subchain v[j] to v[k].
//    Input:  tol = approximation tolerance
//            v[] = polyline array of vertex points
//            j,k = indices for the subchain v[j] to v[k]
//    Output: mk[] = array of markers matching vertex array v[]
void
simplifyDP( double tol, Point* v, int j, int k, int* mk )
{
    if (k <= j+1) // there is nothing to simplify
        return;

    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi = j;          // index of vertex farthest from S
    double  maxd2 = 0;         // distance squared of farthest vertex
    double  tol2 = tol * tol;  // tolerance squared
    Segment S;  // segment from v[j] to v[k]
	S.P0 = v[j];
	S.P1 = v[k];
    Vector  u = *diff(S.P1,S.P0);   // segment direction vector
    double  cu = dot(u,u);     // segment length squared

    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_Point_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    Vector  w;
    Point   Pb;                // base of perpendicular from v[i] to S
    double  b, cw, dv2;        // dv2 = distance v[i] to S squared

    for (int i=j+1; i<k; i++)
    {
        // compute distance squared
        w = *diff(v[i],S.P0);
        cw = dot(w,u);
        if ( cw <= 0 )
            dv2 = d2(v[i], S.P0);
        else if ( cu <= cw )
            dv2 = d2(v[i], S.P1);
        else {
            b = cw / cu;
            //Pb = S.P0 + b * u;
            Pb = *add(S.P0,*mult(b,u));
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2)
            continue;
        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}
//===================================================================
#undef x
#undef y
#endif SUPPORT_SIMPLIFY

#ifdef VB_CODE
How to Simplify Polyline and Polygon Shapefiles
Posted by: cmichaelis ()
Date: July 28, 2007 06:45PM

I've had a lot of questions lately on how to simplify polyline and polygon shapefiles. Below is a VB.Net class that will accomplish this -- see the function SimplyShapefile().

Cheers,
--Chris


' Christopher Michaelis
' cmichaelis@happysquirrel.com
' www.HappySquirrel.com
'
' This free software under the Mozilla Public License (MPL) 1.1.
' [dl.happysquirrel.com]
'
' This is a VB.NET / Shapefile adaptation of an implementation of the Douglas-Peucker algorithm
' provided by SoftSurfer at: [softsurfer.com]
' The original code bears this copyright:
'
'  Copyright 2002, softSurfer (www.softsurfer.com)
'  This code may be freely used and modified for any purpose
'  providing that this copyright notice is included with it.
'  SoftSurfer makes no warranty for this code, and cannot be held
'  liable for any real or imagined damage resulting from its use.
'  Users of this code must verify correctness for their application.
'
' The code was adapted to VB.NET and to work directly on shapefile shapes
' on May 25, 2006 by Christopher Michaelis.

Public Class clsVertexSimplifier
    Private Class Segment
        Public P0 As New MapWinGIS.Point
        Public P1 As New MapWinGIS.Point

        Public Sub New(ByVal i0 As MapWinGIS.Point, ByVal i1 As MapWinGIS.Point)
            P0.x = i0.x
            P0.y = i0.y
            P0.Z = i0.Z
            P1.x = i1.x
            P1.y = i1.y
            P1.Z = i1.Z
        End Sub
    End Class

    Public Delegate Sub ProgressUpdate(ByVal Progress As Integer)

    Public Shared Function PointMinus(ByVal P1 As MapWinGIS.Point, ByVal P2 As MapWinGIS.Point) As Vector
        Dim result As New Vector
        result.x = P1.X - P2.X
        result.y = P1.Y - P2.Y
        Return result
    End Function

    Public Shared Function PointPlus(ByVal P0 As MapWinGIS.Point, ByVal V0 As Vector) As MapWinGIS.Point
        Dim result As New MapWinGIS.Point
        result.X = P0.X + V0.x
        result.Y = P0.Y + V0.y
        result.z = P0.z + V0.z

        Return result
    End Function

    Public Class Vector
        Public x As Double
        Public y As Double
        Public z As Double

        Public Sub New()
        End Sub

        Public Sub New(ByVal V1 As Vector)
            x = V1.x
            y = V1.y
            z = V1.z
        End Sub

        Public Overrides Function Equals(ByVal obj As Object) As Boolean
            If obj.x = x And obj.y = y And obj.z = z Then Return True

            Return False
        End Function

        Public Sub Normalize()
            Dim length As Double = Math.Sqrt(Math.Pow(x, 2) + Math.Pow(y, 2) + Math.Pow(z, 2))
            If (length <= 0) Then Return

            x = x / length
            y = y / length
            z = z / length
        End Sub

        Public Shared Operator *(ByVal S1 As Double, ByVal V1 As Vector) As Vector
            Dim result As New Vector
            result.x = V1.x * S1
            result.y = V1.y * S1
            result.z = V1.z * S1

            Return result
        End Operator

        Public Shared Operator *(ByVal V1 As Vector, ByVal V2 As Vector) As Vector
            Dim result As New Vector
            result.x = V2.y * V1.z - V2.z * V1.y
            result.y = V2.z * V1.x - V2.x * V1.z
            result.z = V2.x * V1.y - V2.y * V1.x

            result.Normalize()
            Return result
        End Operator

        Public Shared Operator -(ByVal V1 As Vector, ByVal V2 As Vector) As Vector
            Dim result As New Vector
            result.x = V1.x - V2.x
            result.y = V1.y - V2.y
            result.z = V1.z - V2.z

            Return result
        End Operator
    End Class

    Private Shared Function dist(ByVal u As MapWinGIS.Point, ByVal v As MapWinGIS.Point) As Double
        Return Math.Sqrt(Math.Pow(v.x - u.x, 2) + Math.Pow(v.y - u.y, 2))
    End Function

    Public Shared Function SimplifyShapefile(ByVal origFile As String, ByVal destFile As String, ByVal tolerance As Double, Optional ByVal ProgressCallback As ProgressUpdate = Nothing) As Boolean
        Dim sf As New MapWinGIS.Shapefile
        sf.Open(origFile)

        If sf.ShapefileType = MapWinGIS.ShpfileType.SHP_MULTIPOINT Or sf.ShapefileType = MapWinGIS.ShpfileType.SHP_POINT Or sf.ShapefileType = MapWinGIS.ShpfileType.SHP_POINTM Or sf.ShapefileType = MapWinGIS.ShpfileType.SHP_POINTZ Then
            'Disallow simplification of point features for now.
            'Stage 1 could still apply, but stage 2 would be meaningless.
            sf.Close()
            Return False
        Else
            sf.Close()
        End If

        If Not origFile.ToLower() = destFile.ToLower() Then
            System.IO.File.Copy(origFile, destFile)
            System.IO.File.Copy(System.IO.Path.ChangeExtension(origFile, ".shx"), System.IO.Path.ChangeExtension(destFile, ".shx"))
            System.IO.File.Copy(System.IO.Path.ChangeExtension(origFile, ".dbf"), System.IO.Path.ChangeExtension(destFile, ".dbf"))
            If System.IO.File.Exists(System.IO.Path.ChangeExtension(origFile, ".prj")) Then System.IO.File.Copy(System.IO.Path.ChangeExtension(origFile, ".prj"), System.IO.Path.ChangeExtension(destFile, ".prj"))
        End If

        sf.Open(destFile)

        Dim prg As Integer = 0
        Dim lastPrg As Integer = -1

        sf.StartEditingShapes(False)
        For j As Integer = 0 To sf.NumShapes - 1
            Dim i, pv As Integer 'Misc Counters

            ' STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
            'Start at 1 - leave initial vertex unchanged
            i = 1
            pv = 0
            While i < sf.Shape(j).numPoints - 1
                If (dist(sf.Shape(j).Point(i), sf.Shape(j).Point(pv)) < tolerance) Then
                    'Remove that shape; do not increment counters
                    sf.Shape(j).DeletePoint(i)
                    For z As Integer = 0 To sf.Shape(j).NumParts - 1
                        If sf.Shape(j).Part(z) >= i And i > 0 Then
                            sf.Shape(j).Part(z) = i - 1
                        End If
                    Next
                Else
                    i += 1
                    pv += 1
                End If
            End While

            ' STAGE 2.  Douglas-Peucker polyline simplification
            Dim mk(sf.Shape(j).numPoints - 1) As Integer  ' marker buffer
            mk(0) = 1
            mk(mk.Length - 1) = 1       ' mark the first and last vertices
            simplifyDP(tolerance, sf.Shape(j), 1, mk.Length - 1, mk)

            ' delete unmarked vertices from the output simplified polyline
            Dim PrevDeletions As Integer = 0
            For i = 0 To mk.Length - 1
                If Not (mk(i)) = 1 Then
                    sf.Shape(j).DeletePoint(i - PrevDeletions) 'Adjust deletion index to account for previously deleted vertices
                    For z As Integer = 0 To sf.Shape(j).NumParts - 1
                        If sf.Shape(j).Part(z) >= i And i > 0 Then
                            sf.Shape(j).Part(z) = i - 1
                        End If
                    Next

                    PrevDeletions += 1
                End If
            Next

            prg = (j / sf.NumShapes) * 100
            If Not ProgressCallback Is Nothing AndAlso Not prg = lastPrg Then
                ProgressCallback(prg)
                lastPrg = prg
            End If
        Next
        sf.StopEditingShapes(True, False)
        sf.Close()
        Return True
    End Function

    ' simplifyDP():
    '  This is the Douglas-Peucker recursive simplification routine
    '  It just marks vertices that are part of the simplified polyline
    '  for approximating the polyline subchain v(j) to v(k).
    '    Input:  tol = approximation tolerance
    '            v() = polyline array of vertex points
    '            j,k = indices for the subchain v(j) to v(k)
    '    Output: mk() = array of markers matching vertex array v()
    Private Shared Sub simplifyDP(ByRef tol As Double, ByRef mwshape As MapWinGIS.Shape, ByRef j As Integer, ByRef k As Integer, ByRef mk() As Integer)
        If (k <= j + 1) Then Return ' there is nothing to simplify

        ' check for adequate approximation by segment S from v(j) to v(k)
        Dim maxi As Integer = j          ' index of vertex farthest from S
        Dim maxd2 As Double = 0          ' distance squared of farthest vertex
        Dim tol2 As Double = tol * tol   ' tolerance squared
        Dim S As New Segment(mwshape.Point(j), mwshape.Point(k - 1)) ' segment from v(j) to v(k)
        Dim u As New Vector(PointMinus(S.P1, S.P0)) ' segment direction vector
        Dim cu As Double = ((u).x * (u).x + (u).y * (u).y + (u).z * (u).z)     ' segment length squared

        ' test each vertex v(i) for max distance from S
        ' compute using the Feb 2001 Algorithm's dist_Point_to_Segment()
        ' Note: this works in any dimension (2D, 3D, ...)
        Dim w As New Vector
        Dim Pb As New MapWinGIS.Point()                ' base of perpendicular from v(i) to S
        Dim b, cw, dv2 As Double       ' dv2 = distance v(i) to S squared

        Dim i As Integer = j + 1
        While i < k
            ' compute distance squared
            w = PointMinus(mwshape.Point(i), S.P0)
            cw = ((w).x * (u).x + (w).y * (u).y + (w).z * (u).z)
            If (cw <= 0) Then
                dv2 = dist(mwshape.Point(i), S.P0)
            ElseIf (cu <= cw) Then
                dv2 = dist(mwshape.Point(i), S.P1)
            Else
                b = cw / cu
                Pb = PointPlus(S.P0, b * u)
                dv2 = dist(mwshape.Point(i), Pb)
            End If

            ' test with current max distance squared
            If Not (dv2 <= maxd2) Then
                ' v(i) is a new max vertex
                maxi = i
                maxd2 = dv2
            End If

            i += 1
        End While

        If (maxd2 > tol2) Then        ' error is worse than the tolerance
            ' split the polyline at the farthest vertex from S
            mk(maxi) = 1      ' mark v(maxi) for the simplified polyline
            ' recursively simplify the two subpolylines at v(maxi)
            simplifyDP(tol, mwshape, j, maxi, mk)  ' polyline v(j) to v(maxi)
            simplifyDP(tol, mwshape, maxi, k, mk)  ' polyline v(maxi) to v(k)
        End If
        ' else the approximation is OK, so ignore intermediate vertices
    End Sub
End Class
#endif

static BOOL LoadShape(char *FileName, int ObjNum, SHAPEFILE_OBJECT_S *Obj)
{
    SHPHandle	hSHP;
    int		nShapeType, nEntities, i, iPart, bValidate = 0,nInvalidCount=0;
    int         bHeaderOnly = 0;
    const char 	*pszPlus;
    double 	adfMinBound[4], adfMaxBound[4];

/* -------------------------------------------------------------------- */
/*      Open the passed shapefile.                                      */
/* -------------------------------------------------------------------- */
    hSHP = SHPOpen( FileName, "rb" );

    if( hSHP == NULL )
    {
        TraceLogThread("ShapeFile", TRUE,  "Unable to open:%s\n", FileName );
        return FALSE;
    }

/* -------------------------------------------------------------------- */
/*      Print out the file bounds.                                      */
/* -------------------------------------------------------------------- */
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

	if (nShapeType != SHPT_POLYGON)
	TraceLogThread("ShapeFile", TRUE,  "Shapefile Type: %s   # of Shapes: %d\n\n",
            SHPTypeName( nShapeType ), nEntities );
    
#ifdef VERBOSE
	TraceLogThread("ShapeFile", TRUE,  "File Bounds: (%12.3f,%12.3f,%g,%g)\n"
            "         to  (%12.3f,%12.3f,%g,%g)\n",
            adfMinBound[0], 
            adfMinBound[1], 
            adfMinBound[2], 
            adfMinBound[3], 
            adfMaxBound[0], 
            adfMaxBound[1], 
            adfMaxBound[2], 
            adfMaxBound[3] );
#endif

/* -------------------------------------------------------------------- */
/*	Skim over the list of shapes, printing all the vertices.	*/
/* -------------------------------------------------------------------- */
//    for( i = 0; i < nEntities && !bHeaderOnly; i++ )
    i = ObjNum;
    {
        int		j;
        SHPObject	*psShape;

        psShape = SHPReadObject( hSHP, i );

        if( psShape == NULL )
        {
            TraceLogThread("ShapeFile", TRUE,
                     "Unable to read shape %d, terminating object reading.\n",
                    i );
        } else
	{
		Obj->minLat = psShape->dfYMin;
		Obj->minLon = psShape->dfXMin;
		Obj->maxLat = psShape->dfYMax;
		Obj->maxLon = psShape->dfXMax;

		Obj->PartCount = psShape->nParts;
		Obj->PointCount = psShape->nVertices;
		Obj->PartStarts = (unsigned long *) malloc(sizeof(*Obj->PartStarts)*(Obj->PartCount+1));
		Obj->Points = (SHAPEFILE_POINT_S *) malloc(sizeof(*Obj->Points)*Obj->PointCount);

        if( psShape->bMeasureIsUsed )
            TraceLogThread("ShapeFile", FALSE,  "Shape:%d (%s) nVertices=%d, nParts=%d"
                    " Bounds:(%.3f,%.3f, %g, %g)"
                    " to (%.3f,%.3f, %g, %g)\n",
                    i, SHPTypeName(psShape->nSHPType),
                    psShape->nVertices, psShape->nParts,
                    psShape->dfXMin, psShape->dfYMin,
                    psShape->dfZMin, psShape->dfMMin,
                    psShape->dfXMax, psShape->dfYMax,
                    psShape->dfZMax, psShape->dfMMax );
        else
            TraceLogThread("ShapeFile", FALSE,  "Shape:%d (%s)  nVertices=%d, nParts=%d"
                    " Bounds:(%.3f,%.3f, %g)"
                    " to (%.3f,%.3f, %g)\n",
                    i, SHPTypeName(psShape->nSHPType),
                    psShape->nVertices, psShape->nParts,
                    psShape->dfXMin, psShape->dfYMin,
                    psShape->dfZMin,
                    psShape->dfXMax, psShape->dfYMax,
                    psShape->dfZMax );

        if( psShape->nParts > 0 && psShape->panPartStart[0] != 0 )
        {
            TraceLogThread("ShapeFile", TRUE,  "panPartStart[0] = %d, not zero as expected.\n",
                     psShape->panPartStart[0] );
        }

		Obj->PartStarts[0] = 0;
		Obj->PartStarts[Obj->PartCount] = Obj->PointCount;

        for( j = 0, iPart = 1; j < psShape->nVertices; j++ )
        {
            const char	*pszPartType = "";

            if( j == 0 && psShape->nParts > 0 )
                pszPartType = SHPPartTypeName( psShape->panPartType[0] );
            
            if( iPart < psShape->nParts
                && psShape->panPartStart[iPart] == j )
            {
				Obj->PartStarts[iPart] = j;

				pszPartType = SHPPartTypeName( psShape->panPartType[iPart] );
                iPart++;
                pszPlus = "+";
            }
            else
                pszPlus = " ";

			Obj->Points[j].Lat = psShape->padfY[j];
			Obj->Points[j].Lon = psShape->padfX[j];

#ifdef VERBOSE
			if( psShape->bMeasureIsUsed )
                TraceLogThread("ShapeFile", TRUE, "   %s (%12.3f,%12.3f, %g, %g) %s \n",
                       pszPlus,
                       psShape->padfX[j],
                       psShape->padfY[j],
                       psShape->padfZ[j],
                       psShape->padfM[j],
                       pszPartType );
            else
                TraceLogThread("ShapeFile", TRUE, "   %s (%12.3f,%12.3f, %g) %s \n",
                       pszPlus,
                       psShape->padfX[j],
                       psShape->padfY[j],
                       psShape->padfZ[j],
                       pszPartType );
#endif
		}

        {
            int nAltered = SHPRewindObject( hSHP, psShape );

            if( nAltered > 0 )
            {
                TraceLogThread("ShapeFile", TRUE,  "  %d rings wound in the wrong direction.\n",
                        nAltered );
            }
        }
        
        SHPDestroyObject( psShape );
	}
    }

    SHPClose( hSHP );
	return TRUE;
}

typedef struct SHAPE_FILE_ENTRY_S
{	int iRecord;
	char ID[8];
	char State[8];
	char FIPS[8];
	char Zone[8];
	char Office[8];
} SHAPE_FILE_ENTRY_S;

typedef struct SHAPE_FILE_INDEX_S
{	char *pszFilename;
	BOOL isCounty;
	unsigned long EntryCount;
	SHAPE_FILE_ENTRY_S *Entries;
} SHAPE_FILE_INDEX_S;

unsigned long IndexCount = 0;
SHAPE_FILE_INDEX_S *Indexes = NULL;

char *GetShapeIndexStats(void)
{
	if (!IndexCount) return NULL;

	size_t Remaining = 1024+80*IndexCount;
	char *Result = (char*)malloc(Remaining);
	char *Next = Result;
	unsigned long Count = 0;
	unsigned long Size = sizeof(*Indexes)*IndexCount;
	size_t s;

	for (s=0; s<IndexCount; s++)
	{	unsigned long ThisSize = Indexes[s].EntryCount*sizeof(*Indexes[s].Entries);
		ThisSize += strlen(Indexes[s].pszFilename)+1;
		StringCbPrintfExA(Next, Remaining, &Next, &Remaining,
							STRSAFE_IGNORE_NULLS,
							"%ld (%lu) %s\n",
							Indexes[s].EntryCount, ThisSize,
							Indexes[s].pszFilename);
		Size += ThisSize;
		Count += Indexes[s].EntryCount;
	}
	StringCbPrintfExA(Next, Remaining, &Next, &Remaining,
						STRSAFE_IGNORE_NULLS,
						"ShapeIndex[%ld] %ld Objs %lu Bytes",
						IndexCount, Count, Size);
	return Result;
}

void FlushShapeIndexes(char *pszFilename)
{	for (unsigned long i=0; i<IndexCount; i++)
	{	if (!pszFilename || !_stricmp(Indexes[i].pszFilename, pszFilename))
		{
TraceLogThread("ShapeIndex", TRUE, "Flushing ShapeIndex(%s) with %ld Entries\n",
			   Indexes[i].pszFilename, (long) Indexes[i].EntryCount);
			free(Indexes[i].pszFilename);
			free(Indexes[i].Entries);
			if (i != --IndexCount)
				Indexes[i] = Indexes[IndexCount];
			i--;	/* Check this one again */
		}
	}
TraceLogThread("ShapeIndex", TRUE, "%ld Remaining ShapeIndexes\n", (long) IndexCount);
	if (!IndexCount)
	{	if (Indexes) free(Indexes);
		Indexes = NULL;
	}
}

static BOOL BuildShapeIndex(SHAPE_FILE_INDEX_S *pIndex)
{	DBFHandle	hDBF;
    int		i, iRecord;
    int		nWidth, nDecimals;
    char	szTitle[12];

	int		fID = -1, fState = -1, fFIPS = -1, fZone = -1, fOffice = -1;
    int		fCounty = -1, fName = -1, fShortName = -1, fLat = -1, fLon = -1;

	int		FoundOne = 0;

	__int64 Start = llGetMsec();

/* -------------------------------------------------------------------- */
/*      Open the file.                                                  */
/* -------------------------------------------------------------------- */
    hDBF = DBFOpen( pIndex->pszFilename, "rb" );
    if( hDBF == NULL )
    {
	TraceLogThread("ShapeFile", TRUE,  "DBFOpen(%s,\"r\") failed.\n", pIndex->pszFilename );
	return 0;
    }
    
/* -------------------------------------------------------------------- */
/*	If there is no data in this file let the user know.		*/
/* -------------------------------------------------------------------- */
    if( DBFGetFieldCount(hDBF) == 0 )
    {
	TraceLogThread("ShapeFile", TRUE,  "There are no fields in this table!\n" );
	return 0;
	}

/* -------------------------------------------------------------------- */
/*	Dump header definitions.					*/
/* -------------------------------------------------------------------- */
        for( i = 0; i < DBFGetFieldCount(hDBF); i++ )
        {
            DBFFieldType	eType;
            const char	 	*pszTypeName;
            char chNativeType;

            chNativeType = DBFGetNativeFieldType( hDBF, i );

            eType = DBFGetFieldInfo( hDBF, i, szTitle, &nWidth, &nDecimals );
            if( eType == FTString )
                pszTypeName = "String";
            else if( eType == FTInteger )
                pszTypeName = "Integer";
            else if( eType == FTDouble )
                pszTypeName = "Double";
            else if( eType == FTInvalid )
                pszTypeName = "Invalid";

            TraceLogThread("ShapeIndex", TRUE,  "%s Field %d/%d: Type=%c/%s, Title=`%s', Width=%d, Decimals=%d\n",
				pIndex->pszFilename, i, DBFGetFieldCount(hDBF), chNativeType, pszTypeName, szTitle, nWidth, nDecimals );

		if (!_stricmp(szTitle, "ID") && eType == FTString)
			fID = i;
		else if (!_stricmp(szTitle, "STATE") && eType == FTString)
			fState = i;
		else if (!_stricmp(szTitle, "FIPS") && eType == FTString)
			fFIPS = i;
		else if (!_stricmp(szTitle, "ZONE") && eType == FTString)
			fZone = i;
		else if (!_stricmp(szTitle, "CWA") && eType == FTString)
			fOffice = i;
		else if (!_stricmp(szTitle, "WFO") && eType == FTString)
			fOffice = i;
		else if (!_stricmp(szTitle, "COUNTYNAME") && eType == FTString)
			fCounty = i;
		else if (!_stricmp(szTitle, "NAME") && eType == FTString)
			fName = i;
		else if (!_stricmp(szTitle, "SHORTNAME") && eType == FTString)
			fShortName = i;
		else if (!_stricmp(szTitle, "LAT") && eType == FTDouble)
			fLat = i;
		else if (!_stricmp(szTitle, "LON") && eType == FTDouble)
			fLon = i;
        }

	pIndex->isCounty = (fCounty != -1);
	pIndex->EntryCount = 0;
	pIndex->Entries = (SHAPE_FILE_ENTRY_S *) calloc(DBFGetRecordCount(hDBF), sizeof(*pIndex->Entries));

/* -------------------------------------------------------------------- */
/*	Read all the records 						*/
/* -------------------------------------------------------------------- */
    for( iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++ )
    {	BOOL Good = FALSE;

		if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))	/* got an ID column, check it */
		{	Good = TRUE;
		} else if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
		{	
			{	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))	/* Use state + last 3 of FIPS */
				{	Good = TRUE;
				} else if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
				{	Good = TRUE;
				}
			}
		}

		if (Good)
		{	SHAPE_FILE_ENTRY_S *Obj = &pIndex->Entries[pIndex->EntryCount++];
			Obj->iRecord = iRecord;

			if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
				strncpy(Obj->State, DBFReadStringAttribute(hDBF, iRecord, fState), sizeof(Obj->State));
			if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))
				strncpy(Obj->FIPS, DBFReadStringAttribute(hDBF, iRecord, fFIPS), sizeof(Obj->FIPS));
			if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
				strncpy(Obj->Zone, DBFReadStringAttribute(hDBF, iRecord, fZone), sizeof(Obj->Zone));
			if (fOffice != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fOffice))
				strncpy(Obj->Office, DBFReadStringAttribute(hDBF, iRecord, fOffice), sizeof(Obj->Office));
			if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))
				strncpy(Obj->ID, DBFReadStringAttribute(hDBF, iRecord, fID), sizeof(Obj->ID));
			if (fState != -1 && (fFIPS != -1 || fZone != -1) && strlen(Obj->ID) != 6)
			{	strncpy(Obj->ID, Obj->State, sizeof(Obj->ID));
				if (fCounty != -1)
					strncat(Obj->ID, "C", sizeof(Obj->ID)-strlen(Obj->ID));
				else strncat(Obj->ID, "Z", sizeof(Obj->ID)-strlen(Obj->ID));
				if (fFIPS != -1)	/* Use state + last 3 of FIPS */
				{	strncat(Obj->ID, Obj->FIPS+2, sizeof(Obj->ID)-strlen(Obj->ID));
				} else if (fZone != -1)
				{	strncat(Obj->ID, Obj->Zone, sizeof(Obj->ID)-strlen(Obj->ID));
				}
			}
#ifdef SHOW_DUPLICATES
			if (*Obj->ID && *Obj->Office)	/* Only if we have an ID */
			for (unsigned long t=0; t<pIndex->EntryCount-1; t++)
			{	if (!strncmp(pIndex->Entries[t].ID, Obj->ID, sizeof(Obj->ID)))
					TraceLogThread("ShapeIndex", TRUE, "%s[%ld] is %s's %s [%ld] is %s's %s\n",
							pIndex->pszFilename, Obj->iRecord,
							Obj->Office, Obj->ID,
							pIndex->Entries[t].iRecord,
							pIndex->Entries[t].Office,
							pIndex->Entries[t].ID);
			}
#endif
		}
#ifdef USE_COUNTIES_NOT_RESPONSIBILITIES
		else if (DBFGetRecordCount(hDBF) < 200)
		{
			TraceLogThread("ShapeIndex", FALSE, "iRecord %ld/%ld fID=%ld fState=%ld fFIPS=%ld fZone=%ld fCounty=%ld\n",
							(long) iRecord, (long)DBFGetRecordCount(hDBF), (long)fID, (long)fState, (long)fFIPS, (long)fZone, (long)fCounty);
			if (fID != -1 && DBFIsAttributeNULL(hDBF, iRecord, fID))	/* got an ID column, check it */
			{
				TraceLogThread("ShapeIndex", FALSE, "fID %ld is NULL\n", (long)fID);
			}
			if (fState != -1 && DBFIsAttributeNULL(hDBF, iRecord, fState))
			{
				TraceLogThread("ShapeIndex", FALSE, "fState %d is NULL\n", (long)fState);
			}
			if (fFIPS != -1 && DBFIsAttributeNULL(hDBF, iRecord, fFIPS))
			{
				TraceLogThread("ShapeIndex", FALSE, "fFIPS %d is NULL\n", (long)fFIPS);
			}
			if (fZone != -1 && DBFIsAttributeNULL(hDBF, iRecord, fZone))
			{
				TraceLogThread("ShapeIndex", FALSE, "fZone %d is NULL\n", (long)fZone);
			}
			if (fCounty != -1 && DBFIsAttributeNULL(hDBF, iRecord, fCounty))
			{
				TraceLogThread("ShapeIndex", FALSE, "fCounty %d is NULL\n", (long)fCounty);
			}
			if (fCounty != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fCounty))
				TraceLogThread("ShapeIndex", FALSE, "County %d is %s\n", 
							fCounty, DBFReadStringAttribute(hDBF, iRecord, fCounty));
		}
#endif
	}

	__int64 Elapsed = llMsecSince(Start,llGetMsec());

	TraceLogThread("ShapeIndex", TRUE, "Indexed %ld/%ld Records in %ldmsec from %s\n",
					pIndex->EntryCount, DBFGetRecordCount(hDBF),
					(long) Elapsed, pIndex->pszFilename);

	if (pIndex->EntryCount < (unsigned long) DBFGetRecordCount(hDBF))
		pIndex->Entries = (SHAPE_FILE_ENTRY_S *) realloc(pIndex->Entries, pIndex->EntryCount*sizeof(*pIndex->Entries));

	DBFClose( hDBF );

    return TRUE;
}

static SHAPE_FILE_INDEX_S *GetShapeFileIndex(char *pszFilename)
{	SHAPE_FILE_INDEX_S *pIndex = NULL;

	for (unsigned long i=0; i<IndexCount; i++)
	{	if (!_stricmp(pszFilename, Indexes[i].pszFilename))
		{	return &Indexes[i];
		}
	}
	Indexes = (SHAPE_FILE_INDEX_S *)realloc(Indexes, sizeof(*Indexes)*(IndexCount+1));
	pIndex = &Indexes[IndexCount++];
	memset(pIndex, 0, sizeof(*pIndex));
	pIndex->pszFilename = _strdup(pszFilename);
	BuildShapeIndex(pIndex);
	return pIndex;
}

static SHAPEFILE_OBJECT_S *ReadShapeObject( char *pszFilename, char *Code, int iRecord )
{	SHAPEFILE_OBJECT_S *Obj = NULL;
    DBFHandle	hDBF;
    int		i;
    int		nWidth, nDecimals;
    char	szTitle[12];
	char	*cState = Code;
	int		isCounty = toupper(Code[2]&0xff) == 'C';
	char	*cZone = Code+strlen(Code)-3;

    int		fID = -1, fState = -1, fFIPS = -1, fZone = -1, fOffice = -1;
    int		fCounty = -1, fName = -1, fShortName = -1, fLat = -1, fLon = -1;

	int		FoundOne = 0;

	if (strlen(Code) < 6) return 0;

/* -------------------------------------------------------------------- */
/*      Open the file.                                                  */
/* -------------------------------------------------------------------- */
    hDBF = DBFOpen( pszFilename, "rb" );
    if( hDBF == NULL )
    {
	TraceLogThread("ShapeFile", TRUE,  "DBFOpen(%s,\"r\") failed.\n", pszFilename );
	return 0;
    }
    
/* -------------------------------------------------------------------- */
/*	If there is no data in this file let the user know.		*/
/* -------------------------------------------------------------------- */
    if( DBFGetFieldCount(hDBF) == 0 )
    {
	TraceLogThread("ShapeFile", TRUE,  "There are no fields in this table!\n" );
	return 0;
	}

/* -------------------------------------------------------------------- */
/*	Dump header definitions.					*/
/* -------------------------------------------------------------------- */
        for( i = 0; i < DBFGetFieldCount(hDBF); i++ )
        {
            DBFFieldType	eType;
            const char	 	*pszTypeName;
            char chNativeType;

            chNativeType = DBFGetNativeFieldType( hDBF, i );

            eType = DBFGetFieldInfo( hDBF, i, szTitle, &nWidth, &nDecimals );
            if( eType == FTString )
                pszTypeName = "String";
            else if( eType == FTInteger )
                pszTypeName = "Integer";
            else if( eType == FTDouble )
                pszTypeName = "Double";
            else if( eType == FTInvalid )
                pszTypeName = "Invalid";

//            TraceLogThread("ShapeFile", TRUE,  "Field %d: Type=%c/%s, Title=`%s', Width=%d, Decimals=%d\n",
//                    i, chNativeType, pszTypeName, szTitle, nWidth, nDecimals );

		if (!_stricmp(szTitle, "ID") && eType == FTString)
			fID = i;
		else if (!_stricmp(szTitle, "STATE") && eType == FTString)
			fState = i;
		else if (!_stricmp(szTitle, "FIPS") && eType == FTString)
			fFIPS = i;
		else if (!_stricmp(szTitle, "ZONE") && eType == FTString)
			fZone = i;
		else if (!_stricmp(szTitle, "CWA") && eType == FTString)
			fOffice = i;
		else if (!_stricmp(szTitle, "WFO") && eType == FTString)
			fOffice = i;
		else if (!_stricmp(szTitle, "COUNTYNAME") && eType == FTString)
			fCounty = i;
		else if (!_stricmp(szTitle, "NAME") && eType == FTString)
			fName = i;
		else if (!_stricmp(szTitle, "SHORTNAME") && eType == FTString)
			fShortName = i;
		else if (!_stricmp(szTitle, "LAT") && eType == FTDouble)
			fLat = i;
		else if (!_stricmp(szTitle, "LON") && eType == FTDouble)
			fLon = i;
        }

/* -------------------------------------------------------------------- */
/*	Construct the object from iRecord's defined fields					*/
/* -------------------------------------------------------------------- */
	TraceLogThread("ShapeFile", FALSE, "Found %s At %ld in %s\n", Code, (long) iRecord, pszFilename);

	Obj = (SHAPEFILE_OBJECT_S *) calloc(1,sizeof(*Obj));
	strncpy(Obj->Code, Code, sizeof(Obj->Code));
	strncpy(Obj->File, pszFilename, sizeof(Obj->File));
	Obj->Record = iRecord;

	if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))
		strncpy(Obj->ID, DBFReadStringAttribute(hDBF, iRecord, fID), sizeof(Obj->ID));
	if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
		strncpy(Obj->State, DBFReadStringAttribute(hDBF, iRecord, fState), sizeof(Obj->State));
	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))
		strncpy(Obj->FIPS, DBFReadStringAttribute(hDBF, iRecord, fFIPS), sizeof(Obj->FIPS));
	if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
		strncpy(Obj->Zone, DBFReadStringAttribute(hDBF, iRecord, fZone), sizeof(Obj->Zone));
	if (fOffice != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fOffice))
		strncpy(Obj->Office, DBFReadStringAttribute(hDBF, iRecord, fOffice), sizeof(Obj->Office));
	if (fCounty != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fCounty))
		strncpy(Obj->County, DBFReadStringAttribute(hDBF, iRecord, fCounty), sizeof(Obj->County));
	if (fName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fName))
		strncpy(Obj->Name, DBFReadStringAttribute(hDBF, iRecord, fName), sizeof(Obj->Name));
	if (fShortName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fShortName))
		strncpy(Obj->ShortName, DBFReadStringAttribute(hDBF, iRecord, fShortName), sizeof(Obj->ShortName));
	if (fLat != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLat))
		Obj->Lat = DBFReadDoubleAttribute(hDBF, iRecord, fLat);
	if (fLon != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLon))
		Obj->Lon = DBFReadDoubleAttribute(hDBF, iRecord, fLon);

	LoadShape(pszFilename, iRecord, Obj);

	DBFClose( hDBF );

    return( Obj );
}

static SHAPEFILE_OBJECT_S *FindShapeObject( char *pszFilename, char *Code )
{	SHAPE_FILE_INDEX_S *pIndex = NULL;
	char	*cState = Code;
	int		isCounty = toupper(Code[2]&0xff) == 'C';
	char	*cZone = Code+strlen(Code)-3;
	SHAPEFILE_OBJECT_S *Obj = NULL;

	if (strlen(Code) < 6) return 0;

	pIndex = GetShapeFileIndex(pszFilename);

	if ((isCounty && pIndex->isCounty)
	|| (!isCounty && !pIndex->isCounty))
	for (unsigned long i=0; i<pIndex->EntryCount; i++)
	{	SHAPE_FILE_ENTRY_S *pEntry = &pIndex->Entries[i];
		if (*pEntry->ID)	/* got an ID column, check it */
		{	if (_strnicmp(pEntry->ID, Code, sizeof(pEntry->ID)))
				continue;
		} else if (*pEntry->State)
		{	if (!_strnicmp(pEntry->State, cState, 2))
			{	if (*pEntry->FIPS)	/* Use state + last 3 of FIPS */
				{	if (_strnicmp(pEntry->FIPS+2, cZone, 3))
						continue;
				} else if (*pEntry->Zone)
				{	if (_strnicmp(pEntry->Zone, cZone, 3))
						continue;
				} else continue;
			} else continue;
		}

#ifdef BUSTED
/*
c_01de10.shp[2497] is AKQ's VAC036 [2248] is AKQ's VAC036
c_01de10.shp[2562] is AKQ's VAC115 [2561] is AKQ's VAC115
c_01de10.shp[2572] is AKQ's VAC735 [2570] is AKQ's VAC735

c_01de10.shp[3215] is CHS's GAC179 [2897] is CHS's GAC179
c_01de10.shp[2896] is CHS's GAC191 [2541] is CHS's GAC191
c_01de10.shp[2595] is CHS's SCC013 [2537] is CHS's SCC013

c_01de10.shp[3230] is LWX's MDC005 [2544] is LWX's MDC005
c_01de10.shp[2557] is LWX's MDC017 [844] is LWX's MDC017
c_01de10.shp[3229] is LWX's MDC025 [2509] is LWX's MDC025
c_01de10.shp[2556] is LWX's VAC099 [2157] is LWX's VAC099

c_01de10.shp[2682] is OKX's NYC047 [2533] is OKX's NYC047

c_01de10.shp[3262] is TAE's FLC037 [3261] is TAE's FLC037
c_01de10.shp[3252] is TAE's FLC045 [2601] is TAE's FLC045
c_01de10.shp[2538] is TAE's FLC063 [2536] is TAE's FLC063
c_01de10.shp[3243] is TAE's FLC131 [3195] is TAE's FLC131
c_01de10.shp[2663] is TAE's GAC253 [2599] is TAE's GAC253 
*/
#endif
		if (Obj)
		{	SHAPEFILE_OBJECT_S *Obj2 = ReadShapeObject(pszFilename, Code, pEntry->iRecord);
#define MYABS(v) ((v)<0?-(v):(v))
			double a1 = MYABS((Obj->maxLat-Obj->minLat)*(Obj->maxLon-Obj->minLon));
			double a2 = MYABS((Obj2->maxLat-Obj2->minLat)*(Obj2->maxLon-Obj2->minLon));

			TraceLogThread("ShapeIndex", FALSE, "New %s Object[%ld](%ld@%ld area %.4lf) Was[%ld](%ld@%ld area %.4lf)\n",
							Code,
							i, Obj2->PartCount, Obj2->PointCount, a2,
							Obj->Record, Obj->PartCount, Obj->PointCount, a1);
			TraceLogThread("ShapeIndex", FALSE, "%s[%ld] ID(%s) State(%s) FIPS(%s) Zone(%s) Office(%s) County(%s) Short(%s) Name(%s) %ld/%ld %.4lf %.4lf %.4lf-%.4lf %.4lf-%.4lf\n",
							Obj->Code, Obj->Record,
							Obj->ID, Obj->State,
							Obj->FIPS, Obj->Zone, Obj->Office,
							Obj->County, Obj->ShortName, Obj->Name,
							Obj->PointCount, Obj->PartCount,
							Obj->Lat, Obj->Lon,
							Obj->minLat, Obj->maxLat,
							Obj->minLon, Obj->maxLon);
			TraceLogThread("ShapeIndex", FALSE, "%s[%ld] ID(%s) State(%s) FIPS(%s) Zone(%s) Office(%s) County(%s) Short(%s) Name(%s) %ld/%ld %.4lf %.4lf %.4lf-%.4lf %.4lf-%.4lf\n",
							Obj2->Code, Obj2->Record,
							Obj2->ID, Obj2->State,
							Obj2->FIPS, Obj2->Zone, Obj2->Office,
							Obj2->County, Obj2->ShortName, Obj2->Name,
							Obj2->PointCount, Obj2->PartCount,
							Obj2->Lat, Obj2->Lon,
							Obj2->minLat, Obj2->maxLat,
							Obj2->minLon, Obj2->maxLon);
#define MERGE_DUPLICATE_SHAPES
#ifndef MERGE_DUPLICATE_SHAPES
			if (a1 > a2)
			{	FreeShapeObject(Obj2);
			} else
			{	FreeShapeObject(Obj);
				Obj = Obj2;
			}
#else
			Obj->Record = -1;	/* Invalid! */
			if (strncmp(Obj->ID, Obj2->ID, sizeof(Obj->ID)))
				strncpy(Obj->ID, "*", sizeof(Obj->ID));
			if (strncmp(Obj->State, Obj2->State, sizeof(Obj->State)))
				strncpy(Obj->State, "*", sizeof(Obj->State));
			if (strncmp(Obj->FIPS, Obj2->FIPS, sizeof(Obj->FIPS)))
				strncpy(Obj->FIPS, "*", sizeof(Obj->FIPS));
			if (strncmp(Obj->Zone, Obj2->Zone, sizeof(Obj->Zone)))
				strncpy(Obj->Zone, "*", sizeof(Obj->Zone));
			if (strncmp(Obj->Office, Obj2->Office, sizeof(Obj->Office)))
				strncpy(Obj->Office, "*", sizeof(Obj->Office));
			if (strncmp(Obj->County, Obj2->County, sizeof(Obj->County)))
				strncpy(Obj->County, "*", sizeof(Obj->County));
			if (strncmp(Obj->Name, Obj2->Name, sizeof(Obj->Name)))
				strncpy(Obj->Name, "*", sizeof(Obj->Name));
			if (strncmp(Obj->ShortName, Obj2->ShortName, sizeof(Obj->ShortName)))
				strncpy(Obj->ShortName, "*", sizeof(Obj->ShortName));
			if (a1+a2)	/* Must not be zero */
			{	Obj->Lat = (Obj->Lat*a1+Obj2->Lat*a2)/(a1+a2);
				Obj->Lon = (Obj->Lon*a1+Obj2->Lon*a2)/(a1+a2);
			}
			if (Obj2->minLat < Obj->minLat) Obj->minLat = Obj2->minLat;
			if (Obj2->minLon < Obj->minLon) Obj->minLon = Obj2->minLon;
			if (Obj2->maxLat > Obj->maxLat) Obj->maxLat = Obj2->maxLat;
			if (Obj2->maxLon > Obj->maxLon) Obj->maxLon = Obj2->maxLon;

			{	unsigned long oParts = Obj->PartCount;
				unsigned long oPoints = Obj->PointCount;

				Obj->PartCount += Obj2->PartCount;
				Obj->PartStarts = (unsigned long *) realloc(Obj->PartStarts, sizeof(*Obj->PartStarts)*Obj->PartCount);
				for (unsigned long p=0; p<Obj2->PartCount; p++)
				{	Obj->PartStarts[oParts+p] = Obj2->PartStarts[p]+oPoints;
				}
				Obj->PointCount += Obj2->PointCount;
				Obj->Points = (SHAPEFILE_POINT_S *) realloc(Obj->Points, sizeof(*Obj->Points)*Obj->PointCount);
				memcpy(&Obj->Points[oPoints], Obj2->Points, sizeof(*Obj2->Points)*Obj2->PointCount);
			}
			FreeShapeObject("FindShapeObject", Obj2);

			TraceLogThread("ShapeIndex", TRUE, "%s[MERGE] ID(%s) State(%s) FIPS(%s) Zone(%s) Office(%s) County(%s) Short(%s) Name(%s) %ld/%ld %.4lf %.4lf %.4lf-%.4lf %.4lf-%.4lf\n",
							Obj->Code,
							Obj->ID, Obj->State,
							Obj->FIPS, Obj->Zone, Obj->Office,
							Obj->County, Obj->ShortName, Obj->Name,
							Obj->PointCount, Obj->PartCount,
							Obj->Lat, Obj->Lon,
							Obj->minLat, Obj->maxLat,
							Obj->minLon, Obj->maxLon);
#endif
		} else
		{	Obj = ReadShapeObject(pszFilename, Code, pEntry->iRecord);
		}
	}
	if (Obj)
	{		TraceLogThread("ShapeIndex", FALSE, "%s[%ld] ID(%s) State(%s) FIPS(%s) Zone(%s) Office(%s) County(%s) Short(%s) Name(%s) %ld/%ld %.4lf %.4lf %.4lf-%.4lf %.4lf-%.4lf\n",
							Obj->Code, Obj->Record,
							Obj->ID, Obj->State,
							Obj->FIPS, Obj->Zone, Obj->Office,
							Obj->County, Obj->ShortName, Obj->Name,
							Obj->PointCount, Obj->PartCount,
							Obj->Lat, Obj->Lon,
							Obj->minLat, Obj->maxLat,
							Obj->minLon, Obj->maxLon);
	}
	return Obj;
}

#ifdef OLD_WAY
static SHAPEFILE_OBJECT_S *FindShapeObject( char *pszFilename, char *Code )
{	SHAPEFILE_OBJECT_S *Obj = NULL;
    DBFHandle	hDBF;
    int		i, iRecord;
    int		nWidth, nDecimals;
    char	szTitle[12];
	char	*cState = Code;
	int		isCounty = toupper(Code[2]&0xff) == 'C';
	char	*cZone = Code+strlen(Code)-3;

    int		fID = -1, fState = -1, fFIPS = -1, fZone = -1, fOffice = -1;
    int		fCounty = -1, fName = -1, fShortName = -1, fLat = -1, fLon = -1;

	int		FoundOne = 0;

	if (strlen(Code) < 6) return 0;

/* -------------------------------------------------------------------- */
/*      Open the file.                                                  */
/* -------------------------------------------------------------------- */
    hDBF = DBFOpen( pszFilename, "rb" );
    if( hDBF == NULL )
    {
	TraceLogThread("ShapeFile", TRUE,  "DBFOpen(%s,\"r\") failed.\n", pszFilename );
	return 0;
    }
    
/* -------------------------------------------------------------------- */
/*	If there is no data in this file let the user know.		*/
/* -------------------------------------------------------------------- */
    if( DBFGetFieldCount(hDBF) == 0 )
    {
	TraceLogThread("ShapeFile", TRUE,  "There are no fields in this table!\n" );
	return 0;
	}

/* -------------------------------------------------------------------- */
/*	Dump header definitions.					*/
/* -------------------------------------------------------------------- */
        for( i = 0; i < DBFGetFieldCount(hDBF); i++ )
        {
            DBFFieldType	eType;
            const char	 	*pszTypeName;
            char chNativeType;

            chNativeType = DBFGetNativeFieldType( hDBF, i );

            eType = DBFGetFieldInfo( hDBF, i, szTitle, &nWidth, &nDecimals );
            if( eType == FTString )
                pszTypeName = "String";
            else if( eType == FTInteger )
                pszTypeName = "Integer";
            else if( eType == FTDouble )
                pszTypeName = "Double";
            else if( eType == FTInvalid )
                pszTypeName = "Invalid";

//            TraceLogThread("ShapeFile", TRUE,  "Field %d: Type=%c/%s, Title=`%s', Width=%d, Decimals=%d\n",
//                    i, chNativeType, pszTypeName, szTitle, nWidth, nDecimals );

		if (!_stricmp(szTitle, "ID") && eType == FTString)
			fID = i;
		else if (!_stricmp(szTitle, "STATE") && eType == FTString)
			fState = i;
		else if (!_stricmp(szTitle, "FIPS") && eType == FTString)
			fFIPS = i;
		else if (!_stricmp(szTitle, "ZONE") && eType == FTString)
			fZone = i;
		else if (!_stricmp(szTitle, "CWA") && eType == FTString)
			fOffice = i;
		else if (!_stricmp(szTitle, "WFO") && eType == FTString)
			fOffice = i;
		else if (!_stricmp(szTitle, "COUNTYNAME") && eType == FTString)
			fCounty = i;
		else if (!_stricmp(szTitle, "NAME") && eType == FTString)
			fName = i;
		else if (!_stricmp(szTitle, "SHORTNAME") && eType == FTString)
			fShortName = i;
		else if (!_stricmp(szTitle, "LAT") && eType == FTDouble)
			fLat = i;
		else if (!_stricmp(szTitle, "LON") && eType == FTDouble)
			fLon = i;
        }

	if ((isCounty && fCounty != -1)
	|| (!isCounty && fCounty == -1))
	{

/* -------------------------------------------------------------------- */
/*	Read all the records 						*/
/* -------------------------------------------------------------------- */
    for( iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++ )
    {
	if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))	/* got an ID column, check it */
	{	const char *ID = DBFReadStringAttribute(hDBF, iRecord, fID);
//TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] ID:%s\n", (long) iRecord, (long) DBFGetRecordCount(hDBF), ID);
		if (_stricmp(ID, Code))
			continue;
	} else if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
	{	const char *State = DBFReadStringAttribute(hDBF, iRecord, fState);
		if (!_strnicmp(State, cState, 2))
		{	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))	/* Use state + last 3 of FIPS */
			{const char *FIPS = DBFReadStringAttribute(hDBF, iRecord, fFIPS);
//TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] State:%s FIPS:%s\n", (long) iRecord, (long) DBFGetRecordCount(hDBF), State, FIPS);
				if (_strnicmp(FIPS+2, cZone, 3))
					continue;
			} else if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
			{const char *Zone = DBFReadStringAttribute(hDBF, iRecord, fZone);
//TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] State:%s Zone:%s\n", (long) iRecord, (long) DBFGetRecordCount(hDBF), State, Zone);
				if (_strnicmp(Zone, cZone, 3))
					continue;
			} else continue;
		} else continue;
	} else
	{
#ifdef SHOW_NULLS
		TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] Need ID(%ld)%s or STATE(%ld)%s/FIPS(%ld)%s to match code(%s), continuing...\n",
			(long) iRecord, (long) DBFGetRecordCount(hDBF),
			fID, DBFIsAttributeNULL(hDBF, iRecord, fID)?"NULL":"",
			fState, DBFIsAttributeNULL(hDBF, iRecord, fState)?"NULL":"",
			fFIPS, DBFIsAttributeNULL(hDBF, iRecord, fFIPS)?"NULL":"", Code);
#endif
		continue;
	}

	TraceLogThread("ShapeFile", FALSE, "Found %s At %ld in %s\n", Code, (long) iRecord, pszFilename);

	Obj = (SHAPEFILE_OBJECT_S *) calloc(1,sizeof(*Obj));
	strncpy(Obj->Code, Code, sizeof(Obj->Code));
	strncpy(Obj->File, pszFilename, sizeof(Obj->File));
	Obj->Record = iRecord;

	if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))
		strncpy(Obj->ID, DBFReadStringAttribute(hDBF, iRecord, fID), sizeof(Obj->ID));
	if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
		strncpy(Obj->State, DBFReadStringAttribute(hDBF, iRecord, fState), sizeof(Obj->State));
	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))
		strncpy(Obj->FIPS, DBFReadStringAttribute(hDBF, iRecord, fFIPS), sizeof(Obj->FIPS));
	if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
		strncpy(Obj->Zone, DBFReadStringAttribute(hDBF, iRecord, fZone), sizeof(Obj->Zone));
	if (fOffice != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fOffice))
		strncpy(Obj->Office, DBFReadStringAttribute(hDBF, iRecord, fOffice), sizeof(Obj->Office));
	if (fCounty != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fCounty))
		strncpy(Obj->County, DBFReadStringAttribute(hDBF, iRecord, fCounty), sizeof(Obj->County));
	if (fName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fName))
		strncpy(Obj->Name, DBFReadStringAttribute(hDBF, iRecord, fName), sizeof(Obj->Name));
	if (fShortName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fShortName))
		strncpy(Obj->ShortName, DBFReadStringAttribute(hDBF, iRecord, fShortName), sizeof(Obj->ShortName));
	if (fLat != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLat))
		Obj->Lat = DBFReadDoubleAttribute(hDBF, iRecord, fLat);
	if (fLon != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLon))
		Obj->Lon = DBFReadDoubleAttribute(hDBF, iRecord, fLon);

	ShowShape(pszFilename, iRecord, Obj);

	break;	/* got one, that's plenty! */

    }

	}

	DBFClose( hDBF );

    return( Obj );
}
#endif

typedef struct SHAPE_OBJECT_CACHE_S
{	char Code[16];
	int Hits;
	int References;
	__int64 msLastRef;
	SHAPEFILE_OBJECT_S *Obj;
} SHAPE_OBJECT_CACHE_S;

static size_t ShapeBuilds = 0;
static __int64 ShapeBuildInit = 0;
static __int64 ShapeBuildMax = 0;
static __int64 ShapeBuildTime = 0;
static size_t ShapeCacheCount = 0;
static size_t ShapeCacheSize = 0;
static SHAPE_OBJECT_CACHE_S *ShapeCache = NULL;

char *GetShapeCacheStats(void)
{
	if (!ShapeCacheCount) return NULL;

	char *Result = (char*)malloc(1024);
	int Hits = 0, InUse = 0;
	unsigned long Size = sizeof(*ShapeCache)*ShapeCacheSize;
	size_t s;
	for (s=0; s<ShapeCacheCount; s++)
	{	if (ShapeCache[s].Obj)
		{	SHAPEFILE_OBJECT_S *Obj = ShapeCache[s].Obj;
			InUse++;
			Hits += ShapeCache[s].Hits;
			if (Obj)	/* Protection */
			{	Size += sizeof(*Obj);
				Size += Obj->PartCount*sizeof(*Obj->PartStarts);
				Size += Obj->PointCount*sizeof(Obj->Points);
			}
		}
	}
	StringCbPrintfA(Result, 1024, "ShapeCache[%ld/%ld] Avg %.2lf Hits\n%ld Blds %.0lf/%.0lf/%.0lfms Av/Mx/In\n%lu Bytes",
					(long) InUse, (long) ShapeCacheSize,
					(double) (InUse?((double)Hits/InUse):0.0),
					(long) ShapeBuilds,
					(double) (ShapeBuilds?ShapeBuildTime/ShapeBuilds:0.0),
					(double) ShapeBuildMax, (double) ShapeBuildInit,
					Size);
	return Result;
}

static void FreeShapeObjectIndex(size_t s)
{
	if (s < ShapeCacheCount)
	{
		if (ShapeCache[s].References)
		{	TraceLogThread("ShapeCache", TRUE, "FreeShapeObject:%s Has %ld References!\n", ShapeCache[s].Code, ShapeCache[s].References);
			return;
		}
		if (ShapeCache[s].Obj->PartStarts) free(ShapeCache[s].Obj->PartStarts);
		if (ShapeCache[s].Obj->Points) free(ShapeCache[s].Obj->Points);
		free(ShapeCache[s].Obj);
		if (s < --ShapeCacheCount)
			ShapeCache[s] = ShapeCache[ShapeCacheCount];
	}
}

void FreeShapeObject(char *From, SHAPEFILE_OBJECT_S *Obj, BOOL ForceFree/*=FALSE*/)
{
	if (Obj)
	{	size_t s;
		for (s=0; s<ShapeCacheCount; s++)
		{	if (ShapeCache[s].Obj == Obj)
			{	if (ShapeCache[s].References-- <= 0)
				{	TraceLogThread("ShapeCache", TRUE, "%s:FreeShapeObject:%s(%p) Has %ld References!\n", From, Obj->Code, Obj, ShapeCache[s].References);
				}
				break;
			}
		}
		if (s >= ShapeCacheCount)
		{	TraceLogThread("ShapeCache", TRUE, "%s:FreeShapeObject:Failed To Find %s(%p) In Cache!\n", From, Obj->Code, Obj);
			if (Obj->PartStarts) free(Obj->PartStarts);
			if (Obj->Points) free(Obj->Points);
			free(Obj);
		} else if (ForceFree)
		{	FreeShapeObjectIndex(s);
		}
	}
}

void FlushShapeCache(void)
{
	TraceLogThread("ShapeCache", TRUE, "Flushing ShapeCache\n");
	for (size_t s=0; s<ShapeCacheCount; s++)
	{	SHAPEFILE_OBJECT_S *Obj = ShapeCache[s].Obj;
		if (Obj->PartStarts) free(Obj->PartStarts);
		if (Obj->Points) free(Obj->Points);
		free(Obj);
	}
	ShapeCacheCount = ShapeCacheSize = 0;
	if (ShapeCache) free(ShapeCache);
	ShapeCache = NULL;
	ShapeBuilds = 0;
	ShapeBuildMax = 0;
	ShapeBuildInit = 0;
	ShapeBuildTime = 0;
	TraceLogThread("ShapeCache", TRUE, "Flushed ShapeCache\n");
}

SHAPEFILE_OBJECT_S *GetShapeObject( char *Code )
{	SHAPEFILE_OBJECT_S *Obj = NULL;
	__int64 msOldest = llGetMsec() - 30*60*1000;	/* 30 minutes max */

	if (ActiveConfig.NWS.ShapesEnabled
	&& ActiveConfig.NWS.ShapeFiles.Count)
	{	size_t s;
		for (s=0; s<ShapeCacheCount; s++)
		{	if (ShapeCache[s].Obj)
			{	if (!strncmp(Code, ShapeCache[s].Code, sizeof(ShapeCache[s].Code)))
				{	ShapeCache[s].References++;
					ShapeCache[s].Hits++;
					ShapeCache[s].msLastRef = llGetMsec();
					Obj = ShapeCache[s].Obj;
					TraceLogThread("ShapeCache", FALSE, "Found(%s) at Cache[%ld](%p) (%ld Hits) (%ld Points in %ld Parts)\n", Code, s, Obj, ShapeCache[s].Hits, Obj->PointCount, Obj->PartCount);
					break;
				} else if (ShapeCache[s].msLastRef < msOldest)
				{	if (ShapeCache[s].References)
					{	TraceLogThread("ShapeCache", FALSE, "GetShapeObject:%s(%p) Has %ld References!  NOT Aging Out!\n",
										ShapeCache[s].Obj->Code,
										ShapeCache[s].Obj,
										ShapeCache[s].References);
					} else
					{	TraceLogThread("ShapeCache", FALSE, "GetShapeObject:%s(%p) Not Referenced in %ldms, Freeing\n",
									ShapeCache[s].Obj->Code,
									ShapeCache[s].Obj,
									(long) llMsecSince(ShapeCache[s].msLastRef,llGetMsec()));
						FreeShapeObjectIndex(s);
					}
				}
			}
		}
		if (!Obj)
		{	TIMED_STRING_LIST_S *pList = &ActiveConfig.NWS.ShapeFiles;
			__int64 Start = llGetMsec();

			for (unsigned long f=0; !Obj && f<pList->Count; f++)
			if (!pList->Entries[f].value)	/* Not disabled */
			{	Obj = FindShapeObject(pList->Entries[f].string, Code);
			}
			if (Obj)
			{	for (s=0; s<ShapeCacheCount; s++)
				{	if (!ShapeCache[s].Obj)
						break;
				}
				if (s >= ShapeCacheCount)
				{	s = ShapeCacheCount++;
					if (ShapeCacheCount > ShapeCacheSize)
					{	ShapeCacheSize += 16;	/* Grow in chunks */
						ShapeCache = (SHAPE_OBJECT_CACHE_S *)realloc(ShapeCache, sizeof(*ShapeCache)*ShapeCacheSize);
						memset(&ShapeCache[s], 0, sizeof(*ShapeCache)*(ShapeCacheSize-s));
						TraceLogThread("ShapeCache", FALSE, "Growing Shape Cache from %ld to %ld\n", s, ShapeCacheSize);
					}
				}
				__int64 Elapsed = llMsecSince(Start,llGetMsec());
				TraceLogThread("ShapeCache", FALSE, "Caching(%s) at [%ld](%p) after %s%ldmsec (%ld Points in %ld Parts)\n", Code, s, Obj, ShapeBuildInit?"":"INITIAL ", (long) Elapsed, Obj->PointCount, Obj->PartCount);
				ShapeBuilds++;
				if (!ShapeBuildInit)
					ShapeBuildInit = Elapsed;
				else
				{	ShapeBuildTime += Elapsed;
					if (Elapsed > ShapeBuildMax)
					{	ShapeBuildMax = Elapsed;
					}
				}
				strncpy(ShapeCache[s].Code, Code, sizeof(ShapeCache[s].Code));
				ShapeCache[s].Obj = Obj;
				ShapeCache[s].References++;
				ShapeCache[s].msLastRef = llGetMsec();

#ifdef SHOW_SIMPLIFICATION
#ifdef SUPPORT_SIMPLIFY
//if (Obj->PartCount == 1)
{	Point *V, *sV;
	double p;
	sV= (Point *)malloc(sizeof(*V)*Obj->PointCount);
#ifdef NO_LONGER_NECESSARY
	V = (Point *)malloc(sizeof(*V)*Obj->PointCount);
	for (unsigned long i=0; i<Obj->PointCount; i++)
	{	V[i].Lat = Obj->Points[i].Lat;
		V[i].Lon = Obj->Points[i].Lon;
		/* V[i].z = 0; */
	}
#endif
	for (p=0; p<=10; p+=1)
	{	double tol = max((Obj->maxLat-Obj->minLat)*p/100.0, (Obj->maxLon-Obj->minLon)*p/100.0);
		long New = poly_simplify(tol, Obj->Points, Obj->PointCount, sV);
		TraceLogThread("ShapeCache", FALSE, "(%s) %ld -> %ld @%.0lf%% (tol:%.4lf for %.4lf->%.4lf %.4lf->%.4lf)\n",
						Code, Obj->PointCount, New, p,
						tol, Obj->minLat, Obj->maxLat,
						Obj->minLon, Obj->maxLon);
	}
	free(sV);
#ifdef NO_LONGER_NECESSARY
	free(V);
#endif
}
#endif
#endif

			} else TraceLogThread("ShapeCache", TRUE, "GetShapeObject:FindShapeObject(%s) Failed after %ldmsec\n", Code, (long) llMsecSince(Start,llGetMsec()));
		}
	}
	return Obj;
}

void FreeCWAZones(SHAPEFILE_OBJECT_S **Objs)
{	for (int i=0; Objs[i]; i++)
	{	SHAPEFILE_OBJECT_S *Obj = Objs[i];
		if (Obj->PartStarts) free(Obj->PartStarts);
		if (Obj->Points) free(Obj->Points);
		free(Obj);
	}
	free(Objs);
}

SHAPEFILE_OBJECT_S **GetCWAZones(char *CWA)
{	int ResultCount = 0;
	SHAPEFILE_OBJECT_S **Result = NULL;

TraceLogThread("CWAZones", TRUE, "GetCWAZones(%s)\n", CWA);
	if (!ActiveConfig.NWS.ShapesEnabled) return NULL;
	if (!ActiveConfig.NWS.ShapeFiles.Count) return NULL;

	TIMED_STRING_LIST_S *pList = &ActiveConfig.NWS.ShapeFiles;
	for (unsigned long f=0; f<pList->Count; f++)
	if (!pList->Entries[f].value)	/* Not disabled */
	{	char *FileName = pList->Entries[f].string;
		SHAPE_FILE_INDEX_S *pIndex = GetShapeFileIndex(FileName);

		for (unsigned long i=0; i<pIndex->EntryCount; i++)
		if (!strcmp(pIndex->Entries[i].Office, CWA))
		{	SHAPE_FILE_ENTRY_S *pEntry = &pIndex->Entries[i];
			SHAPEFILE_OBJECT_S *Obj = (SHAPEFILE_OBJECT_S *) calloc(1,sizeof(*Obj));
			{	int o = ResultCount++;
				Result = (SHAPEFILE_OBJECT_S **)realloc(Result,sizeof(*Result)*(ResultCount+1));
				Result[o] = Obj;
				Result[o+1] = NULL;
			}
			strncpy(Obj->File, FileName, sizeof(Obj->File));
			Obj->Record = pEntry->iRecord;
			strncpy(Obj->Code, pEntry->ID, sizeof(Obj->Code));
			strncpy(Obj->ID, pEntry->ID, sizeof(Obj->ID));
			strncpy(Obj->State, pEntry->State, sizeof(Obj->State));
			strncpy(Obj->FIPS, pEntry->FIPS, sizeof(Obj->FIPS));
			strncpy(Obj->Zone, pEntry->Zone, sizeof(Obj->Zone));
			strncpy(Obj->Office, pEntry->Office, sizeof(Obj->Office));
		}

#ifdef OLD_WAY
		DBFHandle	hDBF;
	    int		iRecord;

		int		fID = -1, fState = -1, fFIPS = -1, fZone = -1, fOffice = -1;
	    int		fCounty = -1, fName = -1, fShortName = -1, fLat = -1, fLon = -1;

		// No fields until we discover them				
		fID = fState = fFIPS = fZone = fOffice = -1;
		fCounty = fName = fShortName = fLat = fLon = -1;
	
TraceLogThread("CWAZones", TRUE, "Checking(%s) in %s\n", CWA, FileName);

		hDBF = DBFOpen( FileName, "rb" );
		if( hDBF == NULL )
		{
			TraceLogThread("CWAZones", TRUE,  "DBFOpen(%s,\"r\") failed.\n", FileName );
			continue;	/* Next File */
		}
		if( DBFGetFieldCount(hDBF) == 0 )
		{
			TraceLogThread("CWAZones", TRUE,  "There are no fields in this table!\n" );
			continue;	/* Next File */
		}

		for( int i = 0; i < DBFGetFieldCount(hDBF); i++ )
		{
			DBFFieldType	eType;
			char chNativeType;
		    int		nWidth, nDecimals;
		    char	szTitle[12];

			chNativeType = DBFGetNativeFieldType( hDBF, i );

			eType = DBFGetFieldInfo( hDBF, i, szTitle, &nWidth, &nDecimals );
			if (!_stricmp(szTitle, "ID") && eType == FTString)
				fID = i;
			else if (!_stricmp(szTitle, "STATE") && eType == FTString)
				fState = i;
			else if (!_stricmp(szTitle, "FIPS") && eType == FTString)
				fFIPS = i;
			else if (!_stricmp(szTitle, "ZONE") && eType == FTString)
				fZone = i;
			else if (!_stricmp(szTitle, "CWA") && eType == FTString)
				fOffice = i;
			else if (!_stricmp(szTitle, "WFO") && eType == FTString)
				fOffice = i;
			else if (!_stricmp(szTitle, "COUNTYNAME") && eType == FTString)
				fCounty = i;
			else if (!_stricmp(szTitle, "NAME") && eType == FTString)
				fName = i;
			else if (!_stricmp(szTitle, "SHORTNAME") && eType == FTString)
				fShortName = i;
			else if (!_stricmp(szTitle, "LAT") && eType == FTDouble)
				fLat = i;
			else if (!_stricmp(szTitle, "LON") && eType == FTDouble)
				fLon = i;
		}
		if (fOffice != -1)	/* We have offices, get the shapes open */
		{
			SHPHandle	hSHP;
			int		nEntities = DBFGetRecordCount(hDBF);

			hSHP = SHPOpen( FileName, "rb" );

			if( hSHP == NULL )
			{
				TraceLogThread("CWAZones", TRUE,  "Unable to open:%s\n", FileName );
				DBFClose( hDBF );
				continue;		/* Next file */
			}

			for( iRecord = 0; iRecord < nEntities; iRecord++ )
			{
				if (fOffice != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fOffice))	/* got a CWA column, check it */
				{	const char *Office = DBFReadStringAttribute(hDBF, iRecord, fOffice);
					if (_stricmp(Office, CWA)) continue;	/* Not this one */
				} else continue;	/* Net record */

				SHPObject	*psShape;
				psShape = SHPReadObject( hSHP, iRecord );
				if( psShape == NULL )
				{
					TraceLogThread("CWAZones", TRUE,
							 "Unable to read shape %d, terminating object reading.\n",
							iRecord );
					continue;	/* Next record */
				} else
				{
					TraceLogThread("CWAZones", FALSE,  "%s:Shape:%d/%d (%s) nVertices=%d, nParts=%d"
							" Bounds:(%.3f,%.3f, %g, %g)"
							" to (%.3f,%.3f, %g, %g)\n",
							FileName,
							iRecord, nEntities, SHPTypeName(psShape->nSHPType),
							psShape->nVertices, psShape->nParts,
							psShape->dfXMin, psShape->dfYMin,
							psShape->dfZMin, psShape->dfMMin,
							psShape->dfXMax, psShape->dfYMax,
							psShape->dfZMax, psShape->dfMMax );

					SHAPEFILE_OBJECT_S *Obj = (SHAPEFILE_OBJECT_S *) calloc(1,sizeof(*Obj));
					{	int o = ResultCount++;
						Result = (SHAPEFILE_OBJECT_S **)realloc(Result,sizeof(*Result)*(ResultCount+1));
						Result[o] = Obj;
						Result[o+1] = NULL;
					}
					strncpy(Obj->File, FileName, sizeof(Obj->File));
					Obj->Record = iRecord;

					Obj->minLat = psShape->dfYMin;
					Obj->minLon = psShape->dfXMin;
					Obj->maxLat = psShape->dfYMax;
					Obj->maxLon = psShape->dfXMax;

					Obj->PartCount = psShape->nParts;
					Obj->PointCount = psShape->nVertices;

	if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))
		strncpy(Obj->ID, DBFReadStringAttribute(hDBF, iRecord, fID), sizeof(Obj->ID));
	if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
		strncpy(Obj->State, DBFReadStringAttribute(hDBF, iRecord, fState), sizeof(Obj->State));
	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))
		strncpy(Obj->FIPS, DBFReadStringAttribute(hDBF, iRecord, fFIPS), sizeof(Obj->FIPS));
	if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
		strncpy(Obj->Zone, DBFReadStringAttribute(hDBF, iRecord, fZone), sizeof(Obj->Zone));
	if (fOffice != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fOffice))
		strncpy(Obj->Office, DBFReadStringAttribute(hDBF, iRecord, fOffice), sizeof(Obj->Office));
	if (fCounty != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fCounty))
		strncpy(Obj->County, DBFReadStringAttribute(hDBF, iRecord, fCounty), sizeof(Obj->County));
	if (fName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fName))
		strncpy(Obj->Name, DBFReadStringAttribute(hDBF, iRecord, fName), sizeof(Obj->Name));
	if (fShortName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fShortName))
		strncpy(Obj->ShortName, DBFReadStringAttribute(hDBF, iRecord, fShortName), sizeof(Obj->ShortName));
	if (fLat != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLat))
		Obj->Lat = DBFReadDoubleAttribute(hDBF, iRecord, fLat);
	if (fLon != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLon))
		Obj->Lon = DBFReadDoubleAttribute(hDBF, iRecord, fLon);

	if (fID != -1 && *Obj->ID && strlen(Obj->ID)==6)
	{	strncpy(Obj->Code, Obj->ID, sizeof(Obj->Code));
	} else
	{	strncpy(Obj->Code, Obj->State, sizeof(Obj->Code));
		if (fCounty != -1)
			strncat(Obj->Code, "C", sizeof(Obj->Code)-strlen(Obj->Code));
		else strncat(Obj->Code, "Z", sizeof(Obj->Code)-strlen(Obj->Code));
		if (fFIPS != -1)	/* Use state + last 3 of FIPS */
		{	strncat(Obj->Code, Obj->FIPS+2, sizeof(Obj->Code)-strlen(Obj->Code));
		} else if (fZone != -1)
		{	strncat(Obj->Code, Obj->Zone, sizeof(Obj->Code)-strlen(Obj->Code));
		}
	}

	TCHAR *LatLon = APRSLatLon(Obj->Lat, Obj->Lon, ' ', ' ', 2);
	TraceLogThread("CWAZones", TRUE, "%s[%ld/%ld] CWA:%s Code(%s) %S @ %.5lf %.5lf ID(%.*s) State(%.*s) FIPS(%.*s) Zone(%.*s) Office(%.*s) County(%.*s) ShortName(%.*s) Name(%.*s)\n",
						FileName, (long) iRecord, (long) nEntities,
						CWA, Obj->Code,
						LatLon,
						(double) Obj->Lat, (double) Obj->Lon,
						STRING(Obj->ID), STRING(Obj->State),
						STRING(Obj->FIPS), STRING(Obj->Zone),
						STRING(Obj->Office), STRING(Obj->County),
						STRING(Obj->ShortName), STRING(Obj->Name));
	free(LatLon);

	TraceLogThread("CWAZones", FALSE, "Code(%s) Min %.5lf %.5lf Max %.5lf %.5lf (Avg: %.5lf %.5lf)\n",
						Obj->Code,
						(double) Obj->minLat, (double) Obj->minLon,
						(double) Obj->maxLat, (double) Obj->maxLon,
						(double) (Obj->minLat + Obj->maxLat) / 2.0,
						(double) (Obj->minLon + Obj->maxLon) / 2.0);
	TraceLogThread("CWAZones", FALSE, "Code(%s) has %ld Points In %ld Parts\n",
						Obj->Code, (long) Obj->PointCount,
						(long) Obj->PartCount);

	if (!*Obj->Code) ResultCount--;	/* Not a valid one */

				
			        SHPDestroyObject( psShape );
				}	/* Got the shape */
			}	/* Next iRecord */
		    SHPClose( hSHP );
		}	/* File has Offices */
else TraceLogThread("CWAZones", TRUE, "No fOffice for (%s) in %s\n", CWA, FileName);

		DBFClose( hDBF );
#endif


	}	/* Next enabled file */
	return Result;
}


void FreeLatLonZones(SHAPEFILE_OBJECT_S **Objs)
{	for (int i=0; Objs[i]; i++)
	{	SHAPEFILE_OBJECT_S *Obj = Objs[i];
		if (Obj->PartStarts) free(Obj->PartStarts);
		if (Obj->Points) free(Obj->Points);
		free(Obj);
	}
	free(Objs);
}

SHAPEFILE_OBJECT_S **GetLatLonZones(double Lat, double Lon)
{	int ResultCount = 0;
	SHAPEFILE_OBJECT_S **Result = NULL;

	if (!ActiveConfig.NWS.ShapesEnabled) return NULL;
	if (!ActiveConfig.NWS.ShapeFiles.Count) return NULL;

	TIMED_STRING_LIST_S *pList = &ActiveConfig.NWS.ShapeFiles;
	for (unsigned long f=0; f<pList->Count; f++)
	if (!pList->Entries[f].value)	/* Not disabled */
	{	char *FileName = pList->Entries[f].string;
		
    SHPHandle	hSHP;
    int		nShapeType, nEntities;
    int         bHeaderOnly = 0;
    double 	adfMinBound[4], adfMaxBound[4];

    DBFHandle	hDBF = NULL;
    int		fID = -1, fState = -1, fFIPS = -1, fZone = -1, fOffice = -1;
    int		fCounty = -1, fName = -1, fShortName = -1, fLat = -1, fLon = -1;


/* -------------------------------------------------------------------- */
/*      Open the passed shapefile.                                      */
/* -------------------------------------------------------------------- */
    hSHP = SHPOpen( FileName, "rb" );

    if( hSHP == NULL )
    {
        TraceLogThread("ShapeFile", TRUE,  "Unable to open:%s\n", FileName );
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      Print out the file bounds.                                      */
/* -------------------------------------------------------------------- */
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

	if (nShapeType != SHPT_POLYGON)
	TraceLogThread("ShapeFile", TRUE,  "Shapefile Type: %s   # of Shapes: %d\n\n",
            SHPTypeName( nShapeType ), nEntities );
    
#ifdef VERBOSE
	TraceLogThread("ShapeFile", TRUE,  "File Bounds: (%12.3f,%12.3f,%g,%g)\n"
            "         to  (%12.3f,%12.3f,%g,%g)\n",
            adfMinBound[0], 
            adfMinBound[1], 
            adfMinBound[2], 
            adfMinBound[3], 
            adfMaxBound[0], 
            adfMaxBound[1], 
            adfMaxBound[2], 
            adfMaxBound[3] );
#endif

/* -------------------------------------------------------------------- */
/*	Skim over the list of shapes, printing all the vertices.	*/
/* -------------------------------------------------------------------- */
    for(int iRecord = 0; iRecord < nEntities; iRecord++ )
    {
        SHPObject	*psShape;

        psShape = SHPReadObject( hSHP, iRecord );

        if( psShape == NULL )
        {
            TraceLogThread("ShapeFile", TRUE,
                     "Unable to read shape %d, terminating object reading.\n",
                    iRecord );
        } else
	{
		if (psShape->dfYMin <= Lat
		&& psShape->dfXMin <= Lon
		&& psShape->dfYMax >= Lat
		&& psShape->dfXMax >= Lon)
		{
			TraceLogThread("ShapeFile", FALSE,  "%s:Shape:%d/%d (%s) nVertices=%d, nParts=%d"
                    " Bounds:(%.3f,%.3f, %g, %g)"
                    " to (%.3f,%.3f, %g, %g)\n",
					FileName,
                    iRecord, nEntities, SHPTypeName(psShape->nSHPType),
                    psShape->nVertices, psShape->nParts,
                    psShape->dfXMin, psShape->dfYMin,
                    psShape->dfZMin, psShape->dfMMin,
                    psShape->dfXMax, psShape->dfYMax,
                    psShape->dfZMax, psShape->dfMMax );

	SHAPEFILE_OBJECT_S *Obj = (SHAPEFILE_OBJECT_S *) calloc(1,sizeof(*Obj));
	{	int o = ResultCount++;
		Result = (SHAPEFILE_OBJECT_S **)realloc(Result,sizeof(*Result)*(ResultCount+1));
		Result[o] = Obj;
		Result[o+1] = NULL;
	}
	strncpy(Obj->File, FileName, sizeof(Obj->File));
	Obj->Record = iRecord;

		Obj->minLat = psShape->dfYMin;
		Obj->minLon = psShape->dfXMin;
		Obj->maxLat = psShape->dfYMax;
		Obj->maxLon = psShape->dfXMax;

		Obj->PartCount = psShape->nParts;
		Obj->PointCount = psShape->nVertices;

/*
	Now go get the DBF info
*/
			if (!hDBF)
			{
				// No fields until we discover them				
				fID = fState = fFIPS = fZone = fOffice = -1;
				fCounty = fName = fShortName = fLat = fLon = -1;
			
				hDBF = DBFOpen( FileName, "rb" );
				if( hDBF == NULL )
				{
				TraceLogThread("ShapeFile", TRUE,  "DBFOpen(%s,\"r\") failed.\n", FileName );
				return 0;
				}
				if( DBFGetFieldCount(hDBF) == 0 )
				{
				TraceLogThread("ShapeFile", TRUE,  "There are no fields in this table!\n" );
				return 0;
				}

				for( int i = 0; i < DBFGetFieldCount(hDBF); i++ )
				{
					DBFFieldType	eType;
					char chNativeType;
				    int		nWidth, nDecimals;
				    char	szTitle[12];

					chNativeType = DBFGetNativeFieldType( hDBF, i );

					eType = DBFGetFieldInfo( hDBF, i, szTitle, &nWidth, &nDecimals );
					if (!_stricmp(szTitle, "ID") && eType == FTString)
						fID = i;
					else if (!_stricmp(szTitle, "STATE") && eType == FTString)
						fState = i;
					else if (!_stricmp(szTitle, "FIPS") && eType == FTString)
						fFIPS = i;
					else if (!_stricmp(szTitle, "ZONE") && eType == FTString)
						fZone = i;
					else if (!_stricmp(szTitle, "CWA") && eType == FTString)
						fOffice = i;
					else if (!_stricmp(szTitle, "WFO") && eType == FTString)
						fOffice = i;
					else if (!_stricmp(szTitle, "COUNTYNAME") && eType == FTString)
						fCounty = i;
					else if (!_stricmp(szTitle, "NAME") && eType == FTString)
						fName = i;
					else if (!_stricmp(szTitle, "SHORTNAME") && eType == FTString)
						fShortName = i;
					else if (!_stricmp(szTitle, "LAT") && eType == FTDouble)
						fLat = i;
					else if (!_stricmp(szTitle, "LON") && eType == FTDouble)
						fLon = i;
				}
			}

//    for( iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++ )
	TraceLogThread("ShapeFile", FALSE, "Reading %ld in %s\n", (long) iRecord, FileName);

	if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))	/* got an ID column, check it */
	{	const char *ID = DBFReadStringAttribute(hDBF, iRecord, fID);
TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] ID:%s\n", (long) iRecord, (long) DBFGetRecordCount(hDBF), ID);
		strncpy(Obj->Code, ID, sizeof(Obj->Code));
	}
	if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState) && strlen(Obj->Code) != 6)
	{	const char *State = DBFReadStringAttribute(hDBF, iRecord, fState);
		strncpy(Obj->Code, State, sizeof(Obj->Code));
		if (fCounty != -1)
			strncat(Obj->Code, "C", sizeof(Obj->Code)-strlen(Obj->Code));
		else strncat(Obj->Code, "Z", sizeof(Obj->Code)-strlen(Obj->Code));
		{	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))	/* Use state + last 3 of FIPS */
			{const char *FIPS = DBFReadStringAttribute(hDBF, iRecord, fFIPS);
TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] State:%s FIPS:%s\n", (long) iRecord, (long) DBFGetRecordCount(hDBF), State, FIPS);
				strncat(Obj->Code, FIPS+2, sizeof(Obj->Code)-strlen(Obj->Code));
			} else if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
			{const char *Zone = DBFReadStringAttribute(hDBF, iRecord, fZone);
TraceLogThread("ShapeFile", TRUE, "[%ld/%ld] State:%s Zone:%s\n", (long) iRecord, (long) DBFGetRecordCount(hDBF), State, Zone);
				strncat(Obj->Code, Zone, sizeof(Obj->Code)-strlen(Obj->Code));
			}
		}
	}

	if (fID != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fID))
		strncpy(Obj->ID, DBFReadStringAttribute(hDBF, iRecord, fID), sizeof(Obj->ID));
	if (fState != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fState))
		strncpy(Obj->State, DBFReadStringAttribute(hDBF, iRecord, fState), sizeof(Obj->State));
	if (fFIPS != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fFIPS))
		strncpy(Obj->FIPS, DBFReadStringAttribute(hDBF, iRecord, fFIPS), sizeof(Obj->FIPS));
	if (fZone != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fZone))
		strncpy(Obj->Zone, DBFReadStringAttribute(hDBF, iRecord, fZone), sizeof(Obj->Zone));
	if (fOffice != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fOffice))
		strncpy(Obj->Office, DBFReadStringAttribute(hDBF, iRecord, fOffice), sizeof(Obj->Office));
	if (fCounty != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fCounty))
		strncpy(Obj->County, DBFReadStringAttribute(hDBF, iRecord, fCounty), sizeof(Obj->County));
	if (fName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fName))
		strncpy(Obj->Name, DBFReadStringAttribute(hDBF, iRecord, fName), sizeof(Obj->Name));
	if (fShortName != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fShortName))
		strncpy(Obj->ShortName, DBFReadStringAttribute(hDBF, iRecord, fShortName), sizeof(Obj->ShortName));
	if (fLat != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLat))
		Obj->Lat = DBFReadDoubleAttribute(hDBF, iRecord, fLat);
	if (fLon != -1 && !DBFIsAttributeNULL(hDBF, iRecord, fLon))
		Obj->Lon = DBFReadDoubleAttribute(hDBF, iRecord, fLon);

	TCHAR *LatLon = APRSLatLon(Obj->Lat, Obj->Lon, ' ', ' ', 2);
	TraceLogThread("ShapeFile", TRUE, "Code(%s) %S @ %.5lf %.5lf ID(%.*s) State(%.*s) FIPS(%.*s) Zone(%.*s) Office(%.*s) County(%.*s) ShortName(%.*s) Name(%.*s)\n",
						Obj->Code,
						LatLon,
						(double) Obj->Lat, (double) Obj->Lon,
						STRING(Obj->ID), STRING(Obj->State),
						STRING(Obj->FIPS), STRING(Obj->Zone),
						STRING(Obj->Office), STRING(Obj->County),
						STRING(Obj->ShortName), STRING(Obj->Name));
	free(LatLon);
	TraceLogThread("ShapeFile", FALSE, "Code(%s) Min %.5lf %.5lf Max %.5lf %.5lf (Avg: %.5lf %.5lf)\n",
						Obj->Code,
						(double) Obj->minLat, (double) Obj->minLon,
						(double) Obj->maxLat, (double) Obj->maxLon,
						(double) (Obj->minLat + Obj->maxLat) / 2.0,
						(double) (Obj->minLon + Obj->maxLon) / 2.0);
	TraceLogThread("ShapeFile", FALSE, "Code(%s) has %ld Points In %ld Parts\n",
						Obj->Code, (long) Obj->PointCount,
						(long) Obj->PartCount);

		if (!*Obj->Code) ResultCount--;	/* Not a valid one */
		}
        
        SHPDestroyObject( psShape );
	}
    }

	if (hDBF)
	{	DBFClose( hDBF );
		hDBF = NULL;
	}

    SHPClose( hSHP );

	}
	return Result;
}


BOOL ProbeShapeFile(char *FileName)
{	BOOL Result;

	SHPHandle	hSHP = SHPOpen( FileName, "rb" );
    DBFHandle	hDBF = DBFOpen( FileName, "rb" );

	Result = (hSHP != NULL) && (hDBF != NULL);

	if (hSHP != NULL)
	{	int		nShapeType, nEntities;
	    double 	adfMinBound[4], adfMaxBound[4];
	    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
		if (nShapeType != SHPT_POLYGON || !nEntities)
		{	TraceLogThread("ShapeFile", TRUE,  "SHPfile(%s) Type: %s   # of Shapes: %d\n\n",
						    FileName, SHPTypeName( nShapeType ), nEntities );
			Result = FALSE;
		}
	}

	if (hDBF != NULL)
	{	if (DBFGetFieldCount(hDBF) <= 0)
		{	TraceLogThread("ShapeFile", TRUE,  "DBFfile(%s) Fields:%ld Records:%ld\n\n",
						    FileName, DBFGetFieldCount(hDBF), DBFGetRecordCount(hDBF));
			Result = FALSE;
		}
	}

	if (hDBF) DBFClose( hDBF );
	if (hSHP) SHPClose( hSHP );
	return Result;
}

size_t ShapeFileEnabledCount(void)
{	size_t Count = 0;
	TIMED_STRING_LIST_S *pList = &ActiveConfig.NWS.ShapeFiles;
	for (unsigned long f=0; f<pList->Count; f++)
	if (!pList->Entries[f].value)	/* Not disabled */
	if (ProbeShapeFile(pList->Entries[f].string))
		Count++;
	return Count;
}

#endif