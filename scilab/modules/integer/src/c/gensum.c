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

#include "gensum.h"

#define SUM(Typein,Typeout) {                          \
Typein *DX;\
 DX=(Typein *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) dtemp += (Typeout) DX[i];\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) dtemp += (Typeout) DX[i];\
 }\
}

int C2F(gensum)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i, dtemp;


    dtemp = 0;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      SUM(char, int);
      break;
    case 2:
      SUM(short, int);
      break;
    case 4:
      SUM(int, int) ;
      break;
    case 11:
      SUM(unsigned char, int);
      break;
    case 12:
      SUM(unsigned short, int);
      break;
    case 14:
      SUM(unsigned int, int);
      break;
    }
    return dtemp;
}

double C2F(gensumasdouble)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i;
    static double dtemp;


    dtemp = 0.0;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      SUM(char, double);
      break;
    case 2:
      SUM(short, double);
      break;
    case 4:
      SUM(int, double) ;
      break;
    case 11:
      SUM(unsigned char, double);
      break;
    case 12:
      SUM(unsigned short, double);
      break;
    case 14:
      SUM(unsigned int, double);
      break;
    }
    return dtemp;
}
