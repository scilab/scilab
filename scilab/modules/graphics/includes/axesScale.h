/*------------------------------------------------------------------------*/
/* file: axesScale.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions to compute scale changes in a specific       */
/*        subwindow.                                                      */
/*------------------------------------------------------------------------*/

#ifndef _AXES_SCALE_H_
#define _AXES_SCALE_H_

#include "ObjectStructure.h"

/*------------------------------------------------------------------------------*/
double InvAxis( double min, double max, double u ) ;
int ReverseDataFor3DXonly( sciPointObj * psubwin, double xvect[], int n1 ) ;
int ReverseDataFor3DYonly( sciPointObj * psubwin, double yvect[], int n1 ) ;
int ReverseDataFor3DZonly( sciPointObj * psubwin, double zvect[], int n1 ) ;
int ReverseDataFor3D(      sciPointObj * psubwin, double xvect[], double yvect[], double zvect[], int n1 ) ;
/*------------------------------------------------------------------------------*/
int trans3d( sciPointObj * pobj,
             integer       n   ,
             integer       xm[],
             integer       ym[],
             double        x[] ,
             double        y[] ,
             double        z[]  ) ;
/*------------------------------------------------------------------------------*/

#endif /* _AXES_SCALE_H_ */
