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

#include "genmcuprod.h"
#include "gencuprod.h"
static int c__1 = 1;

#define MCUPROD(Type) {\
Type *IN;\
Type *OUT;\
    IN=(Type *)in;\
    OUT=(Type *)out;\
    if (*job == 0) {\
      C2F(gencuprod)(typ,&mn, IN, OUT, &c__1);  \
    }\
    else if (*job == 1) {\
    for (j = 0; j < *n; ++j) {\
        C2F(gencuprod)(typ,m, &IN[j * (*na) ], &OUT[j * (*na) ], &c__1);\
    }}\
    else if (*job == 2) {\
    for (i = 0; i < *m; ++i) {\
        C2F(gencuprod)(typ,n, &IN[i], &OUT[i], na);\
    }\
    }\
}

int C2F(genmcuprod)(int *typ, int *job, void * in, void* out, int * na, int * m, int * n)
{
    static int  i, j, mn;

    mn = (*m) * (*n);

    switch (*typ)
    {
        case 1:
            MCUPROD(integer1);
            break;
        case 2:
            MCUPROD(integer2);
            break;
        case 4:
            MCUPROD(int) ;
            break;
        case 11:
            MCUPROD(unsigned char);
            break;
        case 12:
            MCUPROD(unsigned short);
            break;
        case 14:
            MCUPROD(unsigned int);
            break;
    }
    return 0;
}


#define MCUPROD_DOUBLE(Typein) {                       \
Typein *IN;\
    IN=(Typein *)in;\
    if (*job == 0) {\
     C2F(gencuprodasdouble)(typ,&mn, IN, out, &c__1);\
    }\
    else if (*job == 1) {\
    for (j = 0; j < *n; ++j) {\
        C2F(gencuprodasdouble)(typ,m, &IN[j * (*na) ],&out[j * (*na) ], &c__1);\
    }}\
    else if (*job == 2) {\
    for (i = 0; i < *m; ++i) {\
        C2F(gencuprodasdouble)(typ,n, &IN[i], &out[i], na);\
    }\
    }\
}

int C2F(genmcuprodd)(int *typ, int *job, void * in, double *out, int * na, int * m, int * n)
{
    static int  i, j, mn;

    mn = (*m) * (*n);

    switch (*typ)
    {
        case 1:
            MCUPROD_DOUBLE(integer1);
            break;
        case 2:
            MCUPROD_DOUBLE(integer2);
            break;
        case 4:
            MCUPROD_DOUBLE(int) ;
            break;
        case 11:
            MCUPROD_DOUBLE(unsigned char);
            break;
        case 12:
            MCUPROD_DOUBLE(unsigned short);
            break;
        case 14:
            MCUPROD_DOUBLE(unsigned int);
            break;
    }
    return 0;
}
