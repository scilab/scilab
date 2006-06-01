/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
#ifndef _SCI_ECH
#define _SCI_ECH

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
/*#define HLogScale(y,h) inint (Cscale.Wscy1 * (log10 (y / y - h))) */
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

#endif  /* _SCI_ECH */






