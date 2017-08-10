/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
