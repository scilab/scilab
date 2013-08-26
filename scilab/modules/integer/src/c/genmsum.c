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

#include "gensum.h"
#include "genmsum.h"
/*--------------------------------------------------------------------------*/
static int c__1 = 1;
/*--------------------------------------------------------------------------*/
#define MSUM(Type) {\
    Type *A;\
    Type *V;\
    A=(Type *)a;\
    V=(Type *)v;\
    iv = 0;\
    if (*job == 0) {\
	t = 0;\
	for (j = 0; j < *n; ++j) \
	    t +=  C2F(gensum)(typ,m, &A[j * (*na)], &c__1);\
	V[0] = (Type)t;}\
    else if (*job == 1) {\
	for (j = 0; j < *n; ++j) {\
	    t =  C2F(gensum)(typ,m, &A[j * (*na) ], &c__1);\
	    V[iv] = (Type)t;iv += *nv;\
	}}\
    else if (*job == 2) {\
	for (i = 0; i < *m; ++i) {\
	    t =  C2F(gensum)(typ,n, &A[i], m);\
	    V[iv] = (Type)t;iv += *nv;\
	}\
    }\
}
/*--------------------------------------------------------------------------*/
int C2F(genmsum)(int *typ, int *job, int *a, int *na, int *m, int *n, int *v, int *nv)
{
    int  i = 0, j = 0, t = 0, iv = 0;

    switch (*typ)
    {
        case 1:
            MSUM(char);
            break;
        case 2:
            MSUM(short);
            break;
        case 4:
            MSUM(int) ;
            break;
        case 11:
            MSUM(unsigned char);
            break;
        case 12:
            MSUM(unsigned short);
            break;
        case 14:
            MSUM(unsigned int);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
/* sum of int returning a double */
#define MSUM_DOUBLE(Type) {\
Type *A;\
double *V;\
    A=(Type *)a;\
    V=(Type *)v;\
    iv = 0;\
    if (*job == 0) {\
	t = 0.0;\
	for (j = 0; j < *n; ++j) \
	    t +=  C2F(gensumasdouble)(typ,m, &A[j * (*na)], &c__1);\
	v[0] = t;}\
    else if (*job == 1) {\
	for (j = 0; j < *n; ++j) {\
	    t =  C2F(gensumasdouble)(typ,m, &A[j * (*na) ], &c__1);\
	    v[iv] = t;iv += *nv;\
	}}\
    else if (*job == 2) {\
	for (i = 0; i < *m; ++i) {\
	    t =  C2F(gensumasdouble)(typ,n, &A[i], m);\
	    v[iv] = t;iv += *nv;\
	}\
    }\
}
/*--------------------------------------------------------------------------*/
int C2F(genmsumasdouble)(int *typ, int *job, int *a, int *na, int *m, int *n, double *v, int *nv)
{
    int  i = 0, j = 0, iv = 0;
    double t = 0.;
    switch (*typ)
    {
        case 1:
            MSUM_DOUBLE(char);
            break;
        case 2:
            MSUM_DOUBLE(short);
            break;
        case 4:
            MSUM_DOUBLE(int) ;
            break;
        case 11:
            MSUM_DOUBLE(unsigned char);
            break;
        case 12:
            MSUM_DOUBLE(unsigned short);
            break;
        case 14:
            MSUM_DOUBLE(unsigned int);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
