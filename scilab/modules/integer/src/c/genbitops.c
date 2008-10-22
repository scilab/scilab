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

#include "genbitops.h"

#define BIT2(Type,Op) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1)\
       for (i = 1; i <= nv; ++i) \
	 DY[i] = DX[i] Op DY[i];\
    else {\
      ix = 1; iy = 1;\
      if (*incx < 0) ix = (-nv + 1) * *incx + 1;\
      if (*incy < 0) iy = (-nv + 1) * *incy + 1;\
      for (i = 1; i <= nv; ++i) {\
	DY[iy] = DX[ix] Op DY[iy];\
	ix += *incx; iy += *incy;\
      }\
    }\
}

#define BIT1(Type,Op) {\
Type *DX;\
    DX=(Type *)dx;\
    --DX;\
    if (*incx == 1 && *incy == 1)\
       for (i = 1; i <= nv; ++i) \
	 DX[i] = Op DX[i];\
    else {\
      ix = 1;\
      if (*incx < 0) ix = (-nv + 1) * *incx + 1;\
      for (i = 1; i <= nv; ++i) {\
	DX[iy] = Op DX[ix];\
	ix += *incx;\
      }\
    }\
}

int C2F(genbitops)(int *typ,int *op,int * n,int * dx,int * incx,int * dy,int * incy)
{
  static int i, ix, iy, nv;
  nv=*n;
  if (nv <= 0) return 0;

  switch (*op) {
  case 57:
    switch (*typ) {
    case 1:
      BIT2(integer1,|);
      break;
    case 2:
      BIT2(integer2,|);
      break;
    case 4:
      BIT2(integer,|);
      break;
    case 11:
      BIT2(unsigned char,|);
      break;
    case 12:
      BIT2(unsigned short,|);
      break;
    case 14:
      BIT2(unsigned int,|);
      break;
    }
    break;
  case 58:
    switch (*typ) {
    case 1:
      BIT2(integer1,&);
      break;
    case 2:
      BIT2(integer2,&);
      break;
    case 4:
      BIT2(integer,&);
      break;
    case 11:
      BIT2(unsigned char,&);
      break;
    case 12:
      BIT2(unsigned short,&);
      break;
    case 14:
      BIT2(unsigned int,&);
      break;
    }
    break;
  case 61:
    switch (*typ) {
    case 1:
      BIT1(integer1,~);
      break;
    case 2:
      BIT1(integer2,~);
      break;
    case 4:
      BIT1(integer,~);
      break;
    case 11:
      BIT1(unsigned char,~);
      break;
    case 12:
      BIT1(unsigned short,~);
      break;
    case 14:
      BIT1(unsigned int,~);
      break;
    }
    break;
  }
  return 0;
}


