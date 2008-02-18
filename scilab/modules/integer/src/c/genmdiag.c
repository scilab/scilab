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
#include "genmdiag.h"
#include "core_math.h"

#define MDIAG(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    if (*k>=0)\
      ix=1+(*k)*(*m);\
    else\
      ix=1-(*k);\
    for (i = 0; i < mn; ++i) \
       DY[i] = DX[ix+i*(*m+1)];\
 }


int C2F(genmdiag)(integer *typ,integer *m,integer *n,integer *k,int *dx,int *dy) 
{
  static integer i, ix, mn;

  if (*k >= 0) 
    mn=Max(0,Min(*m,*n-*k));
  else
    mn=Max(0,Min(*m+*k,*n));

  switch (*typ) {
  case 1:
    MDIAG(integer1);
    break;
  case 2:
    MDIAG(integer2);
    break;
  case 4:
    MDIAG(integer);
    break;
  case 11:
    MDIAG(unsigned char);
    break;
  case 12:
    MDIAG(unsigned short);
    break;
  case 14:
    MDIAG(unsigned int);
    break;
  }
  return 0;
}

