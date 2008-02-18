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
#include "genmprod.h"

static integer c__1 = 1;
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

int C2F(genmprod)(integer *typ,integer *job, integer *a, integer *na, integer *m, integer *n, integer *v, integer *nv)
{
  extern integer  C2F(genprod)();
  static integer  i, j, t, iv;

  switch (*typ) {
  case 1:
    MPROD(integer1);
    break;
  case 2:
    MPROD(integer2);
    break;
  case 4:
    MPROD(integer);
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
