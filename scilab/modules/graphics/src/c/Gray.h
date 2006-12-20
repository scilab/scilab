/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr
 *    Copyright INRIA 2006
 *    Jean-Baptiste Silvy
 *
 *    Modified 2002 Djalel Abdemouche INRIA for entity mode (NG)
 --------------------------------------------------------------------------*/

#ifndef _GRAY_H_
#define _GRAY_H_

void GraySquareDirect(integer *x, integer *y, double *z, integer n1, integer n2) ;

void GraySquareScaled(integer *x, integer *y, double *z, integer n1, integer n2) ;

void GraySquare1(integer *x, integer *y, double *z, integer n1, integer n2) ;

void GraySquare1_NGreverse(integer * x, integer *y, double *z, integer n1, integer n2, sciPointObj * psubwin) ;

#endif /* _GRAY_H_ */
