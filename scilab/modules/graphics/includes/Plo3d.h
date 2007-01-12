/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    jpc@cermics.enpc.fr 
*    Copyright 2006 INRIA
*    Jean-Baptiste Silvy
--------------------------------------------------------------------------*/

#ifndef _PLO3D_H_
#define _PLO3D_H_


int C2F(box3d)(double *xbox, double *ybox, double *zbox) ;

void DrawAxis(double *xbox, double *ybox, integer *Indices, integer style) ;

void Convex_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox) ;

void AxesStrings(integer axflag, integer *ixbox, integer *iybox, integer *xind, char *legend, double *bbox) ;

int triangleSort(integer *polyxin, integer *polyyin, integer *fillin, integer *polyx, integer *polyy, integer *fill);

int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag) ;

void BBoxToval(double *x, double *y, double *z, integer ind, double bbox[6] ) ;

#endif /* _PLO3D_H_ */
