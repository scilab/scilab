/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    Copyright INRIA 2006
*    Jean-Baptiste Silvy
*    Axis drawing for 2d plots (format selection) 
--------------------------------------------------------------------------*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "ObjectStructure.h"
#include "StringMatrix.h"

int C2F(graduate)(double *xmi, double *xma, double *xi, double *xa, integer *np1, integer *np2, integer *kminr, integer *kmaxr, integer *ar) ;
int ComputeC_format( sciPointObj * pobj, char * c_format ) ;
int ComputeXIntervals( sciPointObj * pobj, char xy_type, double ** vector, int * N, int checkdim ) ;
StringMatrix * computeDefaultTicsLabels( sciPointObj * pobj ) ;

#endif /* _FORMAT_H_ */