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

int C2F(xsetg)(char * str,char * str1,integer lx0,integer lx1) ;
int C2F(xgetg)( char * str, char * str1, integer * len,integer  lx0,integer lx1) ;

char GetDriver( void ) ;
void GetDriver1(char *str, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6,
                integer * v7,double * dv1,double * dv2,double * dv3,double * dv4) ;

int GetDriverId( void ) ;

char * getFPF(void) ;

void C2F(SetDriver)(char *x0, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6, integer * v7,double * dv1,double * dv2,double * dv3,double * dv4);

/**
 *we draw marks in pixel
 * Returns the DPI depending on the used driver
 */
int GetDPIFromDriver( int DPI[2] ) ;

#endif /* _XCALL1_H_ */
