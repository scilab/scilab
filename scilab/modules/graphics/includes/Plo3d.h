/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    jpc@cermics.enpc.fr 
*    Copyright 2006 INRIA
*    Jean-Baptiste Silvy
--------------------------------------------------------------------------*/

#ifndef _PLO3D_H_
#define _PLO3D_H_

int C2F(plot3d)(double *x, double *y, double *z, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(plot3d1)(double *x, double *y, double *z, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(fac3d)(double *x, double *y, double *z, integer *cvect, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(fac3d1)(double *x, double *y, double *z, integer *cvect, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(fac3d2)(double *x, double *y, double *z, integer *cvect, integer *p, integer *q, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(param3d)(double *x, double *y, double *z, integer *n, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(param3d1)(double *x, double *y, double *z, integer *m, integer *n, integer *iflag, integer *colors, double *teta, double *alpha, char *legend, integer *flag, double *bbox, integer lstr) ;

int C2F(box3d)(double *xbox, double *ybox, double *zbox) ;

int C2F(geom3d)(double *x, double *y, double *z, integer *n) ;

void SetEch3d(double *xbox, double *ybox, double *zbox, double *bbox, double *teta, double *alpha) ;

void DrawAxis(double *xbox, double *ybox, integer *Indices, integer style) ;

void Convex_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox) ;

void AxesStrings(integer axflag, integer *ixbox, integer *iybox, integer *xind, char *legend, double *bbox) ;

int  triangleSort(integer *polyxin, integer *polyyin, integer *fillin, integer *polyx, integer *polyy, integer *fill);

int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag) ;

#endif /* _PLO3D_H_ */
