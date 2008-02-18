/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "machine.h"
#include "genkron.h"

#define KRON(Type) {\
Type *DX;\
Type *DY;\
Type *DR;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    DR=(Type *)dr;\
    --DR;\
    kx1 = 1 - ix;\
    kk1 = -*ny;\
    for (jx = 1; jx <= *nx; ++jx) {\
      ky = 1;\
      kx1 = kx1 + ix;\
      kk1 = kk1 + *ny;\
      for (jy = 1; jy <= *ny; ++jy){\
        kx = kx1;\
        kk = 1 + (jy - 1 + kk1) * ir;\
        for (i = 1; i <= *mx; ++i){\
          for (l = 0; l < *my; ++l)\
   	       DR[kk + l] = DY[ky + l] * DX[kx];\
   	     kk = kk + *my;\
   	     kx = kx + 1;}\
   	   ky = ky + iy;}}\
}


int C2F(genkron)(integer *typ,int *dx,integer *incx,integer *mx,integer *nx,int *dy,integer *incy,integer *my,integer *ny,int *dr,integer *incr)
{
  static integer i, ix, iy, ir, jx, jy, l, kk1, kx1, kk, kx, ky;
  ix = *incx;
  iy = *incy;
  ir = *incr;

  switch (*typ) {
  case 1:
    KRON(integer1);
    break;
  case 2:
    KRON(integer2);
    break;
  case 4:
    KRON(integer);
    break;
  case 11:
    KRON(unsigned char);
    break;
  case 12:
    KRON(unsigned short);
    break;
  case 14:
    KRON(unsigned int);
    break;
  }
  return 0;
}

