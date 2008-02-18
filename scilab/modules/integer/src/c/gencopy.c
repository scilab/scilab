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
#include "gencopy.h"

#define COPYINT(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return 0;\
    if (*incx == 1 && *incy == 1)\
       for (i = 1; i <= *n; ++i) \
	 DY[i] = DX[i];\
    else {\
      ix = 1;\
      iy = 1;\
      if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
      if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
      for (i = 1; i <= *n; ++i) {\
	DY[iy] = DX[ix];\
	ix += *incx;\
	iy += *incy;\
      }\
	 };}



int C2F(gencopy)(integer *typ,integer *n, int *dx, integer *incx, int *dy, integer *incy)
{
  static integer i, ix, iy;
  switch (*typ) {
  case 1:
    COPYINT(integer1);
    break;
  case 2:
    COPYINT(integer2);
    break;
  case 4:
    COPYINT(integer);
    break;
  case 11:
    COPYINT(unsigned char);
    break;
  case 12:
    COPYINT(unsigned short);
    break;
  case 14:
    COPYINT(unsigned int);
    break;
  }
  return 0;
}

