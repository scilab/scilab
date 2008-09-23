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

#include "genmtran.h"

#define MTRAN(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    ia = 0;i1 = *n;\
    i2 = *m;\
    for (j = 1; j <= i1; ++j) {\
	ib = j;\
	for (i = 1; i <= i2; ++i) {\
	    B[ib] = A[ia + i];\
	    ib += *nb;\
	}\
	ia += *na;\
    }\
}


int C2F(genmtran)(int *typ,int *a, int *na, int *b, int *nb, int *m, int *n)
{
  int i1, i2;
  static int i, j, ia, ib;
  switch (*typ) {
  case 1:
    MTRAN(integer1);
    break;
  case 2:
    MTRAN(integer2);
    break;
  case 4:
    MTRAN(int) ;
    break;
  case 11:
    MTRAN(unsigned char);
    break;
  case 12:
    MTRAN(unsigned short);
    break;
  case 14:
    MTRAN(unsigned int);
    break;
  }
  return 0;
}

