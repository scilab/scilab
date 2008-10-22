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

#include "genset.h"

#define SET(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return 0;\
    iy = 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= *n; ++i) { \
	DY[iy] = *DX;iy += *incy;}\
}




int C2F(genset)(int *typ, int *n, int *dx, int *dy, int *incy)
{
  static int i, iy;
  switch (*typ) {
  case 0:
    SET(double);
    break;
  case 1:
    SET(integer1);
    break;
  case 2:
    SET(integer2);
    break;
  case 4:
    SET(int) ;
    break;
  case 11:
    SET(unsigned char);
    break;
  case 12:
    SET(unsigned short);
    break;
  case 14:
    SET(unsigned int);
    break;
  }
  return 0;
}

