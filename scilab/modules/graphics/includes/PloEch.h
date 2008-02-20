/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2000 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 * --------------------------------------------------------------------------*/
#ifndef _SCI_ECH
#define _SCI_ECH

#include "ObjectStructure.h"

typedef struct wcscalelist 
{
  int flag ;                            /* zero when this is a default scale */
  int    wdim[2];                       /* currend windo dim in pixels */
  double subwin_rect[4];                /* subwindow specification */
  double frect[4];                      /* bounds in the <<real>> space: xmin,ymin,xmax,ymax */
  double axis[4];                       /* position of the axis rectangle */
                                        /* = [mfact_xl, mfact_xr,mfact_yu,mfact_yd]; */
  double xtics[4],ytics[4];             /* [xmin,ymin,nint] or [kmin,kmax,ar,nint]           */
  /* XXXX : c'est redondant avec aaint et quelquefois avec frect ? */
  double Wxofset1,Wyofset1,Wscx1,Wscy1; /* ofsets and scale factor for pixel<->double transf.*/
  char logflag[2];                      /* are we using logscale */
  integer WIRect1[4];                   /* frame bounds in pixel */
  integer Waaint1[4];                   /* tics and subtics numbers: [xint,xsubint,yint,ysubint] */
  double m[3][3];                       /* 3d geometric transformation */
  double bbox1[6];                      /* 3d bounds */
  double alpha,theta;                   /* polar coordinates of visualization point */
  integer metric3d;                     /* added by es - metric mode for 3d -> 2d */
  struct wcscalelist *next;             /* points to next one */
  struct wcscalelist *prev;             /* points to previous one */
}  WCScaleList ;

typedef struct scalelist 
{
  WCScaleList *scales;                  /* list of Scales for window Win */
                                        /* one scale for each subwin */
  integer Win;                          /* window number */
  struct scalelist *next;
} ScaleList ;

/*
 * Current scale values. 
 */

extern WCScaleList Cscale;

/*
 * Current geometric transformation : from double to pixel 
 */
#ifndef Min
#define Min(x,y)	(((x)<(y))?(x):(y))
#endif 

void ShowScales( void ) ;

int C2F(Nsetscale2d)( double    WRect[4],
                      double    ARect[4],
                      double    FRect[4],
                      char    * logscale,
                      integer   l1       ) ;

int getscale2d( double WRect[4], double FRect[4], char * logscale, double ARect[4] ) ;

void set_scale( char    flag[6]        ,
                double  subwin[4]      ,
                double  frame_values[4],
                integer aaint[4]       ,
                char    logflag[3]     ,
                double  axis_values[4]  ) ;

int zoom( void ) ;
int zoom_box( double * bbox, int * x_pixel, int * y_pixel ) ;

void unzoom( void ) ;
void unzoom_one_axes( sciPointObj * psousfen ) ;

extern void scizoom(double bbox[4], sciPointObj * pobj); /* INTERACTION */

extern int XScale(double x);
extern int XLogScale(double x);

extern int YScale(double y);
extern int YLogScale(double y);

/* #define XScale(x)    inint( Min(Cscale.Wscx1*((x) -Cscale.frect[0]) + Cscale.Wxofset1,2147483647)) */
/* #define XLogScale(x) inint( Cscale.Wscx1*(log10(x) -Cscale.frect[0]) + Cscale.Wxofset1) */
/* #define YScale(y)    inint(  Min(Cscale.Wscy1*(-(y)+Cscale.frect[3]) + Cscale.Wyofset1,2147483647)) */
/* #define YLogScale(y) inint( Cscale.Wscy1*(-log10(y)+Cscale.frect[3]) + Cscale.Wyofset1) */
#define XDouble2Pixel(x) ((Cscale.logflag[0] == 'n') ? ( XScale(x)) : ( XLogScale(x)))
#define YDouble2Pixel(y) ((Cscale.logflag[1] == 'n') ? ( YScale(y)) : ( YLogScale(y)))

/* NG beg */
/*
 * geometric transformation "for length"
 */
#define WScale(w)   inint (Cscale.Wscx1 * (w))
#define WLogScale(x,w) inint (Cscale.Wscx1 * (log10 (((x) + (w)) / (x))))
#define HScale(h)   inint (Cscale.Wscy1 * (h)) 
#define HLogScale(y,h) inint (Cscale.Wscy1 * (log10 ( ((y) + (h)) / (y) ) )) /* modif jb Silvy 05/2006 */
#define WDouble2Pixel(x,w) ((Cscale.logflag[0] == 'n') ? ( WScale(w)) : ( WLogScale(x,w)))
#define HDouble2Pixel(y,h) ((Cscale.logflag[1] == 'n') ? ( HScale(h)) : ( HLogScale(y,h)))
/* NG end */

/*
 * Current geometric transformation : from pixel to double 
 */

/* #define XPi2R(x)  Cscale.frect[0] + (1.0/Cscale.Wscx1)*((x) - Cscale.Wxofset1) */
/* #define YPi2R(y)  Cscale.frect[3] - (1.0/Cscale.Wscy1)*((y) - Cscale.Wyofset1) */


/* change of frame with integer values */ 
extern double XPi2R(int x);
extern double YPi2R(int y);

#define XPi2LogR(x)  exp10( XPi2R(x))
#define YPi2LogR(y)  exp10( YPi2R(y))
#define XPixel2Double(x)  (( Cscale.logflag[0] == 'l') ? XPi2LogR(x) : XPi2R(x))
#define YPixel2Double(y)  (( Cscale.logflag[1] == 'l') ? YPi2LogR(y) : YPi2R(y))

/* chang eof frame with double values for entries */
extern double XDPi2R( double x ) ;
extern double YDPi2R( double y ) ;

#define XDPi2LogR(x)  exp10( XDPi2R(x))
#define YDPi2LogR(y)  exp10( YDPi2R(y))
#define XDPixel2Double(x)  (( Cscale.logflag[0] == 'l') ? XDPi2LogR(x) : XDPi2R(x))
#define YDPixel2Double(y)  (( Cscale.logflag[1] == 'l') ? YDPi2LogR(y) : YDPi2R(y))

/*
 * Current geometric transformation : 3D plots 
 */

#define TRX(x1,y1,z1) ( Cscale.m[0][0]*(x1) +Cscale.m[0][1]*(y1) +Cscale.m[0][2]*(z1))
#define TRY(x1,y1,z1) ( Cscale.m[1][0]*(x1) +Cscale.m[1][1]*(y1) +Cscale.m[1][2]*(z1))
#define TRZ(x1,y1,z1) ( Cscale.m[2][0]*(x1) +Cscale.m[2][1]*(y1) +Cscale.m[2][2]*(z1))
#define GEOX(x1,y1,z1)  XScale(TRX(x1,y1,z1))
#define GEOY(x1,y1,z1)  YScale(TRY(x1,y1,z1))

#define TX3D(x1,y1,z1) Cscale.Wscx1*(TRX(x1,y1,z1)-Cscale.frect[0]) +Cscale.Wxofset1 ;
#define TY3D(x1,y1,z1) Cscale.Wscy1*(-TRY(x1,y1,z1)+Cscale.frect[3])+Cscale.Wyofset1 ;

/**
* convert the coordinates of a 2d or 3d point to its pixel coordinates.
* @param pSubWin SubWindow in which the point is, might be either in 2d or 3d.
* @param coord3d Coordinates of the point in the axes. If the subwin is in 2d,
*        only the first two coordinates are used.
*/
void getPixelCoordinates( sciPointObj * pSubWin, double coord3d[3], int pixCoord[2] ) ;

/**
* Convert a length from user coord. to pixels in 2D. This function is similar to
*         WDouble2Pixel, but also use the logmode in 3D.
* @param parentSubWin Axes in which the length is computed.
* @param posX         X coordinate of where the distance is transformed
*                     (only used in log mode).
* @param height       converted width in user coord.
* @return the corresponding length in pixels.
*/
int PixelWidth2d( sciPointObj * parentSubWin, double posX, double width ) ;

/**
 * Convert a length from user coord. to pixels in 2D. This function is similar to
 *         HDouble2Pixel, but also use the logmode in 3D.
 * @param parentSubWin Axes in which the length is computed.
 * @param posY         Y coordinate of where the distance is transformed
 *                     (only used in log mode).
 * @param height       converted height in user coord.
 * @return the corresponding length in pixels.
 */
int PixelHeight2d( sciPointObj * parentSubWin, double posY, double height ) ;

/**
 * Give the position of the 4 corners of a rectangle in pixels given the position of its
 * upper left point in user coordiantes and its size. It works in every axes mode
 * (2D/3D, log, reverse).
 * @param parentSubWin Subwindow in which the rectangle is drawn.
 * @param ulPoint      Coordinates of the upper left point of the rectangle in user coord.
 * @param userSize     Width and height of the rectangle in user coord.
 * @param edgesX       X coordinates of the resulting positions.
 * @param edgesY       Y coordinates of the resulting positions.
 */
void rectangleDouble2Pixel( sciPointObj * parentSubWin ,
                            double        ulPoint[3]   ,
                            double        userSize[2]  ,
                            int           edgesX[4]    ,
                            int           edgesY[4]     ) ;

void Plo2d2RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf) ;
void Plo2d3RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf) ;
void Plo2d4RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf) ;

int C2F(echelle2d)( double    x[]  ,
                    double    y[]  ,
                    integer   x1[] ,
                    integer   yy1[],
                    integer * n1   ,
                    integer * n2   ,
                    char      dir[],
                    integer   lstr ) ;

void C2F(echelle2dl)( double    x[]  ,
                      double    y[]  ,
                      integer   x1[] ,
                      integer   yy1[],
                      integer * n1   ,
                      integer * n2   ,
                      char    * dir   ) ;

void C2F(rect2d)( double x[], integer x1[], integer * n, char * dir ) ;
void C2F(ellipse2d)( double x[], integer x1[], integer * n, char * dir) ;
void C2F(axis2d)( double  * alpha     ,
                  double  * initpoint ,
                  double  * size      ,
                  integer * initpoint1,
                  double  * size1       ) ;

int get_window_scale( integer i, double * subwin ) ;
void del_window_scale( integer i ) ;

void Cscale2default( void ) ;

void set_window_scale_with_default( int i ) ;

void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4]);

void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4]);

#endif  /* _SCI_ECH */






