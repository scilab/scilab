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
/*--------------------------------------------------------------------------*/
#include "gencusum.h"
/*--------------------------------------------------------------------------*/
#define CUSUM(Type) {                 \
   Type *IN;                          \
   Type *OUT;                         \
   IN=(Type *)in;                     \
   OUT=(Type *)out;                   \
   kk = 0;                            \
    for (k = 0; k < *n; ++k) {        \
      t += (int)IN[kk];               \
    OUT[kk] =(Type)t;kk += (*iw);     \
    }                                 \
}
/*--------------------------------------------------------------------------*/
int C2F(gencusum)(int *typ, int *n, void *in, void *out, int *iw)
{
    int k, kk;
    int t;
    t = 0;

    switch (*typ)
    {
        case 1:
            CUSUM(integer1);
            break;
        case 2:
            CUSUM(integer2);
            break;
        case 4:
            CUSUM(int) ;
            break;
        case 11:
            CUSUM(unsigned char);
            break;
        case 12:
            CUSUM(unsigned short);
            break;
        case 14:
            CUSUM(unsigned int);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
#define CUSUM_DOUBLE(Type) {                        \
Type *IN;\
   IN=(Type *)in;\
   kk = 0;                                      \
    for (k = 0; k < *n; ++k) {\
      t += (double)IN[kk];                      \
    out[kk] =t;kk += (*iw);\
    }\
}
/*--------------------------------------------------------------------------*/
int C2F(gencusumasdouble)(int *typ, int *n, void *in, double *out, int *iw)
{
    int k = 0, kk = 0;
    double t = 0.0;

    switch (*typ)
    {
        case 1:
            CUSUM_DOUBLE(integer1);
            break;
        case 2:
            CUSUM_DOUBLE(integer2);
            break;
        case 4:
            CUSUM_DOUBLE(int) ;
            break;
        case 11:
            CUSUM_DOUBLE(unsigned char);
            break;
        case 12:
            CUSUM_DOUBLE(unsigned short);
            break;
        case 14:
            CUSUM_DOUBLE(unsigned int);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
