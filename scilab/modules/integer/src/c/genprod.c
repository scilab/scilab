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

#include "genprod.h"

#define PROD(Typein,Typeout) {\
Typein *DX;\
 DX=(Typein *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) dtemp *= (Typeout) DX[i];\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) dtemp *= (Typeout) DX[i];\
 }\
}

int C2F(genprod)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i, dtemp;


    dtemp = 1;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      PROD(integer1,int);
      break;
    case 2:
      PROD(integer2,int);
      break;
    case 4:
      PROD(int,int) ;
      break;
    case 11:
      PROD(unsigned char,int);
      break;
    case 12:
      PROD(unsigned short,int);
      break;
    case 14:
      PROD(unsigned int,int);
      break;
    }
    return dtemp;
}

double C2F(genprodasdouble)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i;
    static double dtemp;


    dtemp = 1.0;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      PROD(integer1,double);
      break;
    case 2:
      PROD(integer2,double);
      break;
    case 4:
      PROD(int,double);
      break;
    case 11:
      PROD(unsigned char,double);
      break;
    case 12:
      PROD(unsigned short,double);
      break;
    case 14:
      PROD(unsigned int,double);
      break;
    }
    return dtemp;
}


