/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "machine.h"

#ifndef _MSC_VER

/* These DEFAULTNUMCOLORS colors come from Xfig */
unsigned short default_colors[] =
{
    0,   0,   0, /* Black: DEFAULTBLACK */
    0,   0, 255, /* Blue */
    0, 255,   0, /* Green */
    0, 255, 255, /* Cyan */
    255,   0,   0, /* Red */
    255,   0, 255, /* Magenta */
    255,   255,   0, /* Yellow */
    255, 255, 255, /* White: DEFAULTWHITE */
    0,   0, 144, /* Blue4 */
    0,   0, 176, /* Blue3 */
    0,   0, 208, /* Blue2 */
    135, 206, 255, /* LtBlue */
    0, 144,   0, /* Green4 */
    0, 176,   0, /* Green3 */
    0, 208,   0, /* Green2 */
    0, 144, 144, /* Cyan4 */
    0, 176, 176, /* Cyan3 */
    0, 208, 208, /* Cyan2 */
    144,   0,   0, /* Red4 */
    176,   0,   0, /* Red3 */
    208,   0,   0, /* Red2 */
    144,   0, 144, /* Magenta4 */
    176,   0, 176, /* Magenta3 */
    208,   0, 208, /* Magenta2 */
    128,  48,   0, /* Brown4 */
    160,  64,   0, /* Brown3 */
    192,  96,   0, /* Brown2 */
    255, 128, 128, /* Pink4 */
    255, 160, 160, /* Pink3 */
    255, 192, 192, /* Pink2 */
    255, 224, 224, /* Pink */
    255, 215,   0  /* Gold */
};

#endif

int WithBackingStore(void)

{
    return 0 ;
}


/*
 * Resize the Pixmap according to window size change
 * But only if there's a pixmap
 */
void CPixmapResize1( void )
{

}

void clip_line( int   x1  ,
                int   yy1 ,
                int   x2  ,
                int   y2  ,
                int * x1n ,
                int * yy1n,
                int * x2n ,
                int * y2n ,
                int * flag )
{
}

/*--------------------------------------------------------------------------*/
/* Clip the given line to drawing coords defined as xleft,xright,ybot,ytop.
 *   This routine uses the cohen & sutherland bit mapping for fast clipping -
 * see "Principles of Interactive Computer Graphics" Newman & Sproull page 65.
 return 0  : segment out
 1  : (x1,y1) changed
 2  : (x2,y2) changed
 3  : (x1,y1) and (x2,y2) changed
 4  : segment in
*/


void set_clip_box( int xxleft ,
                   int xxright,
                   int yybot  ,
                   int yytop  )
{
}


void SetWinhdc( void )
{
}
/*--------------------------------------------------------------------------*/
int  MaybeSetWinhdc()
{
    return (0);
}
/*--------------------------------------------------------------------------*/
void  ReleaseWinHdc()
{
}
/*--------------------------------------------------------------------------*/
void wininfo(char *fmt, ...)
{
}
/*--------------------------------------------------------------------------*/
void C2F(xgetmouse)(str, ibutton, x1, yy1, iflag, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *ibutton, *x1, *yy1, *iflag, *v6, *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{

}
/*--------------------------------------------------------------------------*/
void SciMouseCapture( void )
{

}
/*--------------------------------------------------------------------------*/
void SciMouseRelease( void )
{

}
/*--------------------------------------------------------------------------*/
int C2F(sedeco)( int * flag )
{
    return (0);
}
/*--------------------------------------------------------------------------*/
/**************************************************
 * loadfamily Loads a font at size  08 10 12 14 18 24
 * for example TimR08 TimR10 TimR12 TimR14 TimR18 TimR24
 * name is a string
 *  ( X11 only : if it's a string containing the char %
 *    it's suposed to be a format for a generic font in X11 string style
 *    "-adobe-times-bold-i-normal--%s-*-75-75-p-*-iso8859-1" )
 * it's supposed to be an alias for a font name
 * Ex : TimR and we shall try to load TimR08 TimR10 TimR12 TimR14 TimR18 TimR24
 **************************************************/

void C2F(loadfamily)(name, j, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *name;
int *j, *v3, *v4, *v5, *v6, *v7;
double *dv1, *dv2, *dv3, *dv4;
{
}
/*--------------------------------------------------------------------------*/
void C2F(xclick)(str, ibutton, x1, yy1, iflag, istr, v7, dv1, dv2, dv3, dv4)
char *str;
int *ibutton, *x1, *yy1, *iflag, *istr, *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(getFontMaxSize)( char * str, int * sizeMin, int * sizeMax,
                          int * v1, int * v2, int * v3, int * v4,
                          double * dx1, double * dx2, double * dx3, double * dx4 )
{

}

void C2F(queryfamily)(name, j, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *name;
int *j;
int *v3;
int *v4;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{

}

void C2F(xinfo)(message, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *message;
int *v2, *v3, *v4, *v5, *v6, *v7;
double *dv1, *dv2, *dv3, *dv4;
{
}

/** Draw a set of arrows **/
/** arrows are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/
/** n is the size of vx and vy **/
/** as is 10*arsize (arsize) the size of the arrow head in pixels **/

void C2F(drawarrows)(str, vx, vy, n, as, style, iflag, dv1, dv2, dv3, dv4)
char *str;
int *vx;
int *vy;
int *n;
int *as;
int *style;
int *iflag;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(xend)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *v1;
int *v2;
int *v3;
int *v4;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
    /** Nothing in Windows **/
}

/*----------------------
  \subsection{Circles and Ellipsis }
  ------------------------*/
/** Draw or fill a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ **/
/** <x,y,width,height> is the bounding box **/
/** angle1,angle2 specifies the portion of the ellipsis **/
/** caution : angle=degreangle*64          **/
/** if fillvect[i] is in [0,whitepattern] then  fill the ellipsis i **/
/** with pattern fillvect[i] **/
/** if fillvect[i] is > whitepattern  then only draw the ellipsis i **/
/** The drawing style is the current drawing **/

void C2F(fillarcs)(str, vects, fillvect, n, v5, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *vects;
int *fillvect;
int *n;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{

}

/** Fill a single elipsis or part of it with current pattern **/
void C2F(fillarc)(str, x, y, width, height, angle1, angle2, dv1, dv2, dv3, dv4)
char *str;
int *x, *y, *width, *height, *angle1, *angle2;
double *dv1, *dv2, *dv3, *dv4;
{
}

/** Draw a set of (*n) polylines (each of which have (*p) points) **/
/** with lines or marks **/
/** drawvect[i] >= 0 use a mark for polyline i **/
/** drawvect[i] < 0 use a line style for  i **/

void C2F(drawpolylines)(str, vectsx, vectsy, drawvect, n, p, v7, dv1, dv2, dv3, dv4)
char *str;
int *vectsx;
int *vectsy;
int *drawvect;
int *n;
int *p;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}


void C2F(drawpolyline)(str, n, vx, vy, closeflag, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *n;
int *vx;
int *vy;
int *closeflag;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}


/** fill a set of polygons each of which is defined by
    (*p) points (*n) is the number of polygons
    the polygon is closed by the routine
    fillvect[*n] :
    fillvect[*n] :
    if fillvect[i] == 0 draw the boundaries with current color
    if fillvect[i] > 0  draw the boundaries with current color
    then fill with pattern fillvect[i]
    if fillvect[i] < 0  fill with pattern - fillvect[i]
**/

void C2F(fillpolylines)(str, vectsx, vectsy, fillvect, n, p, v7, dv1, dv2, dv3, dv4)
char *str;
int *vectsx;
int *vectsy;
int *fillvect;
int *n;
int *p;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

/** Only draw one polygon  with current line style **/
/** according to *closeflag : it's a polyline or a polygon **/
/** n is the number of points of the polyline */
/** ths routine also perform clipping to avoid overflow */
void C2F(drawClippedPolyline)(str, n, vx, vy, closeflag, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *n;
int *vx;
int *vy;
int *closeflag;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void Setpopupname(char *string)
{
}

void C2F(xclick_any)(char *str, int *ibutton, int* x1, int * yy1, int *iwin, int *iflag, int *istr, double * dv1, double *dv2, double * dv3, double * dv4)
{
}

void C2F(xselgraphic)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *v1;
int *v2;
int *v3;
int *v4;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(cleararea)(str, x, y, w, h, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *x;
int *y;
int *w;
int *h;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(boundingbox)(string, x, y, rect, v5, v6, v7, dv1, dv2, dv3, dv4)
char *string;
int *x, *y, *rect, *v5, *v6, *v7;
double *dv1, *dv2, *dv3, *dv4;
{

}

void C2F(clearwindow)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *v1;
int *v2, *v3, *v4, *v5, *v6, *v7;
double *dv1, *dv2, *dv3, *dv4;
{

}


void C2F(drawaxis)(str, alpha, nsteps, v2, initpoint, v6, v7, size, dx2, dx3, dx4)
char *str;
int *alpha;
int *nsteps;
int *v2;
int *initpoint;
int *v6;
int *v7;
double *size;
double *dx2;
double *dx3;
double *dx4;
{
}

void C2F(MissileGCGetorSet)(char *str, int flag, int *verbose, int *x1, int *x2, int *x3, int *x4, int *x5, int *x6, double  *dv1)
{
}

void C2F(initgraphic)(string, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *string;
int *v2;
int *v3;
int *v4;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(drawpolymark)(str, n, vx, vy, v5, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *n;
int *vx;
int *vy;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(drawsegments)(str, vx, vy, n, style, iflag, v7, dv1, dv2, dv3, dv4)
char *str;
int *vx;
int *vy;
int *n;
int *style;
int *iflag;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(MissileGCset)(str, x1, x2, x3, x4, x5, x6, dv1, dv2, dv3, dv4)
char *str;
int *x1;
int *x2;
int *x3;
int *x4;
int *x5;
int *x6;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(drawrectangles)(str, vects, fillvect, n, v5, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *vects;
int *fillvect;
int *n;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(drawrectangle)(str, x, y, width, height, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *x;
int *y;
int *width;
int *height;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(drawarcs)(str, vects, style, n, v5, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *vects;
int *style;
int *n;
int *v5;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(drawarc)(str, x, y, width, height, angle1, angle2, dv1, dv2, dv3, dv4)
char *str;
int *x, *y, *width, *height, *angle1, *angle2;
double *dv1, *dv2, *dv3, *dv4;
{
}

int SwitchWindow(int *intnum)
{
    return (0);
}


int CheckScilabXgc()
{
    return ( 0);
}

void C2F(fillrectangle)(str, x, y, width, height, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *x;
int *y;
int *width;
int *height;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}


void C2F(setpopupname)(x0, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
char *x0;
int *v2, *v3, *v4, *v5, *v6, *v7;
double *dv1, *dv2, *dv3, *dv4;
{
}

void C2F(MissileGCget)(str, verbose, x1, x2, x3, x4, x5, dv1, dv2, dv3, dv4)
char *str;
int *verbose;
int *x1;
int *x2;
int *x3;
int *x4;
int *x5;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{

}


void C2F(displaystring)(string, x, y, v1, flag, v6, v7, angle, dv2, dv3, dv4)
char *string;
int *x, *y, *v1, *flag, *v6, *v7;
double *angle, *dv2, *dv3, *dv4;
{
}

void C2F(fillpolyline)(str, n, vx, vy, closeflag, v6, v7, dv1, dv2, dv3, dv4)
char *str;
int *n;
int *vx;
int *vy;
int *closeflag;
int *v6;
int *v7;
double *dv1;
double *dv2;
double *dv3;
double *dv4;
{
}

void C2F(displaynumbers)(str, x, y, v1, v2, n, flag, z, alpha, dx3, dx4)
char *str;
int *x;
int *y;
int *v1;
int *v2;
int *n;
int *flag;
double *z;
double *alpha;
double *dx3;
double *dx4;
{
}

void SciViewportMove (struct BCG *ScilabGC, int x, int y)
{

}

int C2F (deletewin) (int * number)
{
    return 0;
}
