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
#include "gendot.h"
#include "genmmul.h"
static integer c1 = 1;

#define MMUL(Type) {\
Type *A;\
Type *B;\
Type *C;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    C=(Type *)c;\
    --C;\
    for (j = 1; j <= i1; ++j) {\
	i2 = *l;\
	for (i = 1; i <= i2; ++i) \
	    C[ic + i] = C2F(gendot)(typ,m, &A[i], na, &B[ib], &c1);\
	ic += *nc;ib += *nb;\
    }\
}

int C2F(genmmul)(integer *typ, int *a, integer *na, int *b, integer *nb, int *c, integer *nc, integer *l, integer *m, integer *n)
{
  static integer i1, i2;
  static integer i, j, ib, ic;
  ib = 1;
  ic = 0;
  i1 = *n;


  switch (*typ) {
  case 1:
    MMUL(integer1);
    break;
  case 2:
    MMUL(integer2);
    break;
  case 4:
    MMUL(integer);
    break;
  case 11:
    MMUL(unsigned char);
    break;
  case 12:
    MMUL(unsigned short);
    break;
  case 14:
    MMUL(unsigned int);
    break;
  }
  return 0;
}

