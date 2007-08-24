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

int C2F(xgray)(double *x, double *y, double *z, integer *n1, integer *n2, char *strflag, double *brect, integer *aaint, BOOL flagNax, long int l1) ;

int C2F(xgray1)(double *z, integer *n1, integer *n2, char *strflag, double *brect, integer *aaint, BOOL flagNax, long int l1) ;

#endif /* _GRAY_H_ */
