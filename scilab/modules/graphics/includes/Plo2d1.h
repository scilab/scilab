/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    Copyright INRIA 2006
*    Jean-Baptiste Silvy
--------------------------------------------------------------------------*/

#ifndef _PLO2D1_H_
#define _PLO2D1_H_

int C2F(plot2d1)(char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer l1, integer l2, integer l3) ;
int C2F(plot2d2)(char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer l1, integer l2, integer l3) ;
int C2F(plot2d3)(char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer l1, integer l2, integer l3) ;
int C2F(plot2d4)(char *xf, double *x, double *y, integer *n1, integer *n2, integer *style, char *strflag, char *legend, double *brect, integer *aaint, integer l1, integer l2, integer l3) ;

void Plo2d2RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf) ;
void Plo2d3RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf) ;
void Plo2d4RealToPixel(integer *n1, integer *n2, double *x, double *y, integer *xm, integer *ym, char *xf) ;

#endif /* _PLO2D1_H_ */
