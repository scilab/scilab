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

#include "gentriu.h"


#define TRIU(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    kk = *k;\
    nn = *n;\
    l = 0;\
    if (kk>0) {\
      for (i = 0; i < (*m)*kk; ++i) \
         DX[i] = 0;\
      l = l+ (*m)*kk;\
      nn = nn - kk;\
      kk = 0;}\
    ls = l +1 - kk;\
    ll = *m -1 + kk;\
    for (j = 1; j <= nn; ++j) {\
      if (ll > 0) {\
        for (i = 0; i < ll; ++i) \
          DX[ls+i] = 0;\
        ll--;\
        ls = ls + *m +1;\
      }\
    }\
 }

int C2F(gentriu)(int *typ,int *m,int *n,int *k,int *dx) 
{
  static int kk, l, ls, ll, j, i, nn;

  switch (*typ) {
  case 1:
    TRIU(integer1);
    break;
  case 2:
    TRIU(integer2);
    break;
  case 4:
    TRIU(int) ;
    break;
  case 11:
    TRIU(unsigned char);
    break;
  case 12:
    TRIU(unsigned short);
    break;
  case 14:
    TRIU(unsigned int);
    break;
  }
  return 0;
}

