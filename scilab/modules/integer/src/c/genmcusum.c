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
/*--------------------------------------------------------------------------*/
#include "genmcusum.h"
#include "gencusum.h"
/*--------------------------------------------------------------------------*/
static int c__1 = 1;
/*--------------------------------------------------------------------------*/
#define MCUSUM(Type) {                          \
    Type *IN;                                   \
    Type *OUT;                                  \
    IN = (Type *)in;                            \
    OUT=(Type *)out;                            \
    if (*job == 0) {                            \
     C2F(gencusum)(typ,&mn, IN, OUT, &c__1);\
    }                                           \
    else if (*job == 1) {                       \
    for (j = 0; j < *n; ++j) {              \
        C2F(gencusum)(typ,m, &IN[j * (*na) ],&OUT[j * (*na) ], &c__1);\
    }}                                      \
    else if (*job == 2) {                       \
    for (i = 0; i < *m; ++i) {              \
        C2F(gencusum)(typ,n, &IN[i], &OUT[i], na);\
    }                                       \
    }                                           \
}
/*--------------------------------------------------------------------------*/
int C2F(genmcusum)(int *typ,int *job,void * in, void *out,int * na,int * m,int * n)
{
  int i = 0, j = 0, mn = 0;

  mn = (*m) * (*n);

  switch (*typ)
  {
  case 1:
    MCUSUM(integer1);
    break;
  case 2:
    MCUSUM(integer2);
    break;
  case 4:
    MCUSUM(int) ;
    break;
  case 11:
    MCUSUM(unsigned char);
    break;
  case 12:
    MCUSUM(unsigned short);
    break;
  case 14:
    MCUSUM(unsigned int);
    break;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
#define MCUSUM_DOUBLE(Type) {                       \
    Type *IN;\
    IN = (Type *)in;\
    if (*job == 0) {\
     C2F(gencusumasdouble)(typ,&mn, IN, out, &c__1);\
    }\
    else if (*job == 1) {\
    for (j = 0; j < *n; ++j) {\
        C2F(gencusumasdouble)(typ,m, &IN[j * (*na) ],&out[j * (*na) ], &c__1);\
    }}\
    else if (*job == 2) {\
    for (i = 0; i < *m; ++i) {\
        C2F(gencusumasdouble)(typ,n, &IN[i], &out[i], na);\
    }\
    }\
}
/*--------------------------------------------------------------------------*/
int C2F(genmcusumd)(int *typ,int *job,void * in, double *out,int * na,int * m,int * n)
{
  int i = 0, j = 0, mn = 0;

  mn = (*m) * (*n);

  switch (*typ)
  {
  case 1:
    MCUSUM_DOUBLE(integer1);
    break;
  case 2:
    MCUSUM_DOUBLE(integer2);
    break;
  case 4:
    MCUSUM_DOUBLE(int) ;
    break;
  case 11:
    MCUSUM_DOUBLE(unsigned char);
    break;
  case 12:
    MCUSUM_DOUBLE(unsigned short);
    break;
  case 14:
    MCUSUM_DOUBLE(unsigned int);
    break;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
