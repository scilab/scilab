/*------------------------------------------------------------------------
*   Graphic library for 2D and 3D plotting 
*   Copyright (C) 1998-2001 Chancelier Jean-Philippe 
*   Copyright INRIA 2006
*   Jean-Baptiste Silvy
*--------------------------------------------------------------------------*/

#ifndef _XCALL1_H_
#define _XCALL1_H_

#include "machine.h"

int C2F(dr1)( char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
              integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
              integer lx0, integer lx1) ;

int C2F(dr)(char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
            integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
            integer lx0, integer lx1) ;

#endif /* _XCALL1_H_ */
