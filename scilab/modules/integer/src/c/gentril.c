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

#include "gentril.h"


#define TRIL(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    kk = *k;\
    nn = *n;\
    l = 0;\
    if (kk>=0) {\
     l = l + (*m)*(kk+1);\
     nn = nn - kk - 1;\
     kk = -1;\
    }\
    ls = l;\
    ll = -kk;\
    for (j = 1; j <= nn; ++j) {\
      if (ll > *m) ll = *m;\
      for (i = 0; i < ll; ++i) \
        DX[ls+i] = 0;\
      ll++;\
      ls = ls + *m;\
    }\
 }

int C2F(gentril)(int *typ,int *m,int *n,int *k,int *dx) 
{
  static int kk, l, ls, ll, j, i, nn;

  switch (*typ) {
  case 1:
    TRIL(integer1);
    break;
  case 2:
    TRIL(integer2);
    break;
  case 4:
    TRIL(int) ;
    break;
  case 11:
    TRIL(unsigned char);
    break;
  case 12:
    TRIL(unsigned short);
    break;
  case 14:
    TRIL(unsigned int);
    break;
  }
  return 0;
}

