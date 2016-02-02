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
#include "genprod.h"
#include "genmprod.h"

static int c__1 = 1;
#define MPROD(Type) {\
Type *A;\
Type *V;\
    A=(Type *)a;\
    V=(Type *)v;\
    iv = 0;\
    if (*job == 0) {\
    t = 1;\
    for (j = 0; j < *n; ++j) \
        t *=  C2F(genprod)(typ,m, &A[j * (*na)], &c__1);\
    V[0] = (Type)t;}\
    else if (*job == 1) {\
    for (j = 0; j < *n; ++j) {\
        t =  C2F(genprod)(typ,m, &A[j * (*na) ], &c__1);\
        V[iv] = (Type)t;iv += *nv;\
    }}\
    else if (*job == 2) {\
    for (i = 0; i < *m; ++i) {\
        t =  C2F(genprod)(typ,n, &A[i], m);\
        V[iv] = (Type)t;iv += *nv;\
    }\
    }\
}

int C2F(genmprod)(int *typ, int *job, int *a, int *na, int *m, int *n, int *v, int *nv)
{
    static int  i, j, t, iv;

    switch (*typ)
    {
        case 1:
            MPROD(integer1);
            break;
        case 2:
            MPROD(integer2);
            break;
        case 4:
            MPROD(int) ;
            break;
        case 11:
            MPROD(unsigned char);
            break;
        case 12:
            MPROD(unsigned short);
            break;
        case 14:
            MPROD(unsigned int);
            break;
    }
    return 0;
}
#define MPROD_DOUBLE(Type) {\
Type *A;\
    A=(Type *)a;\
    iv = 0;\
    if (*job == 0) {\
    t = 1.0;\
    for (j = 0; j < *n; ++j) \
        t *=  C2F(genprodasdouble)(typ,m, &A[j * (*na)], &c__1);\
    v[0] = t;}\
    else if (*job == 1) {\
    for (j = 0; j < *n; ++j) {\
        v[iv] =  C2F(genprodasdouble)(typ,m, &A[j * (*na) ], &c__1);\
        iv += *nv;\
    }}\
    else if (*job == 2) {\
    for (i = 0; i < *m; ++i) {\
        v[iv] =  C2F(genprodasdouble)(typ,n, &A[i], m);\
        iv += *nv;\
    }\
    }\
}

int C2F(genmprodasdouble)(int *typ, int *job, int *a, int *na, int *m, int *n, double *v, int *nv)
{
    static int  i, j, iv;
    static double t;

    switch (*typ)
    {
        case 1:
            MPROD_DOUBLE(integer1);
            break;
        case 2:
            MPROD_DOUBLE(integer2);
            break;
        case 4:
            MPROD_DOUBLE(int) ;
            break;
        case 11:
            MPROD_DOUBLE(unsigned char);
            break;
        case 12:
            MPROD_DOUBLE(unsigned short);
            break;
        case 14:
            MPROD_DOUBLE(unsigned int);
            break;
    }
    return 0;
}
