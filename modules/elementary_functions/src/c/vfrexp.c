/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include "machine.h"

void C2F(vfrexp)(int *n, double *x, int *ix,double *y,int *iy,double *z, int *iz);

void C2F(vfrexp)(int *n, double *x, int *ix,double *y,int *iy,double *z, int *iz)
{
  int ix1,iy1,iz1,i,j;
  ix1=0;
  iy1=0;
  iz1=0;

  if (*ix<0) ix1=-(*n-1)*(*ix);
  if (*iy<0) iy1=-(*n-1)*(*iy);
  if (*iz<0) iz1=-(*n-1)*(*iz);

  for (i=0;i<*n;i++) {
    y[iy1]=frexp(x[ix1],&j);
    z[iz1]=j;
    iy1 += *iy;
    ix1 += *ix;
    iz1 += *iz;
  }
}
