/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "gencuprod.h"

#define CUPROD(Type) {         \
Type *IN;                              \
Type *OUT;                            \
   IN=(Type *)in;                      \
   OUT=(Type *)out;                   \
   kk = 0;                               \
    for (k = 0; k < *n; ++k) {           \
      t *= (int)IN[kk];                  \
    OUT[kk] =(Type)t;kk += (*iw); \
    }\
}


int C2F(gencuprod)(int *typ, int *n, void *in, void *out, int *iw)
{
    int k, kk, t;
    t = 1;

    switch (*typ)
    {
        case 1:
            CUPROD(integer1);
            break;
        case 2:
            CUPROD(integer2);
            break;
        case 4:
            CUPROD(int) ;
            break;
        case 11:
            CUPROD(unsigned char);
            break;
        case 12:
            CUPROD(unsigned short);
            break;
        case 14:
            CUPROD(unsigned int);
            break;
    }
    return 0;
}

#define CUPROD_DOUBLE(Type) {         \
Type *IN;                              \
   IN=(Type *)in;                      \
   kk = 0;                               \
    for (k = 0; k < *n; ++k) {           \
      t *= (double)IN[kk];               \
    out[kk] =t;kk += (*iw); \
    }\
}



int C2F(gencuprodasdouble)(int *typ, int *n, void *in, double *out, int *iw)
{
    int k, kk;
    double t;
    t = 1.0;
    switch (*typ)
    {
        case 1:
            CUPROD_DOUBLE(integer1);
            break;
        case 2:
            CUPROD_DOUBLE(integer2);
            break;
        case 4:
            CUPROD_DOUBLE(int) ;
            break;
        case 11:
            CUPROD_DOUBLE(unsigned char);
            break;
        case 12:
            CUPROD_DOUBLE(unsigned short);
            break;
        case 14:
            CUPROD_DOUBLE(unsigned int);
            break;
    }
    return 0;
}

