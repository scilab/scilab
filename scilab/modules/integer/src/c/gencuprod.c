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

