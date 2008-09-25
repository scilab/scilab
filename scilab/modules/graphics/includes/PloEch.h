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
  int WIRect1[4];                   /* frame bounds in pixel */
  int Waaint1[4];                   /* tics and subtics numbers: [xint,xsubint,yint,ysubint] */
  double m[3][3];                       /* 3d geometric transformation */
  double bbox1[6];                      /* 3d bounds */
  double alpha,theta;                   /* polar coordinates of visualization point */
  int metric3d;                     /* added by es - metric mode for 3d -> 2d */
  struct wcscalelist *next;             /* points to next one */
  struct wcscalelist *prev;             /* points to previous one */
}  WCScaleList ;

typedef struct scalelist 
{
  WCScaleList *scales;                  /* list of Scales for window Win */
                                        /* one scale for each subwin */
  int Win;                          /* window number */
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
                      int   l1       ) ;

int getscale2d( double WRect[4], double FRect[4], char * logscale, double ARect[4] ) ;

void set_scale( char    flag[6]        ,
                double  subwin[4]      ,
                double  frame_values[4],
                int aaint[4]       ,
                char    logflag[3]     ,
                double  axis_values[4]  ) ;

void unzoom( void ) ;


extern int XScale(double x);

extern int YScale(double y);


int get_window_scale( int i, double * subwin ) ;


void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4]);

void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4]);

#endif  /* _SCI_ECH */






