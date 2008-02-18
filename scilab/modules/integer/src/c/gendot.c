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
#include "gendot.h"

#define DOT(Type) {\
Type *DX;\
Type *DY;\
Type C;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return (Type)itemp;\
    ix = 1;iy = 1;\
    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= i1; ++i) {\
	itemp += DX[ix] * DY[iy];\
	ix += *incx;\
	iy += *incy;\
    }\
    C = (Type) itemp;\
    return C;\
}

int C2F(gendot)(integer *typ,integer *n, void *dx, integer *incx, void *dy, integer *incy)
{
  integer i1;
  static integer i, itemp, ix, iy;

  itemp = 0;
  i1 = *n;

  switch (*typ) {
  case 1:
    DOT(integer1);
    break;
  case 2:
    DOT(integer2);
    break;
  case 4:
    DOT(integer);
    break;
  case 11:
    DOT(unsigned char);
    break;
  case 12:
    DOT(unsigned short);
    break;
  case 14:
    DOT(unsigned int);
    break;
  }
  return 0;
}

