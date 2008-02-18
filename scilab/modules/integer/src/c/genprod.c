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
#include "genprod.h"

#define PROD(Type) {\
Type *DX;\
 DX=(Type *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) dtemp *= (integer)DX[i];\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) dtemp *= (integer)DX[i];\
 }\
}

integer C2F(genprod)(integer *typ, integer *n, integer *dx, integer *incx)
{
    integer  i1, i2;
    static integer i, dtemp;


    dtemp = 1;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      PROD(integer1);
      break;
    case 2:
      PROD(integer2);
      break;
    case 4:
      PROD(integer);
      break;
    case 11:
      PROD(unsigned char);
      break;
    case 12:
      PROD(unsigned short);
      break;
    case 14:
      PROD(unsigned int);
      break;
    }
    return dtemp;
}


