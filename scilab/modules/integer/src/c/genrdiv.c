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
#include "genrdiv.h"

#define RDIV(Type) {\
Type *A = 0;\
Type *B = 0;\
Type *R = 0;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    R=(Type *)r;\
    --R;\
    jr = 1;jb = 1;ja = 1;\
    *ierr = 0;\
    if (*ia == 0) {\
	for (k = 1; k <= i1; ++k) {\
	    if (B[jb] == 0) {*ierr = k;return 0;}\
	    R[jr] = A[ja] / B[jb];\
	    jr += *ir;jb += *ib;\
	}\
    }\
    else if (*ib == 0) {\
	if (B[jb] == 0) {*ierr = 1;return 0;}\
	for (k = 1; k <= i1; ++k) {\
	    R[jr] = A[ja] / B[jb];\
	    jr += *ir;ja += *ia;\
	}\
    }\
    else {\
	for (k = 1; k <= i1; ++k) {\
	    if (B[jb] == 0) {*ierr = k;return 0;}\
	    R[jr] = A[ja] / B[jb];\
	    jr += *ir;jb += *ib;ja += *ia;\
	}\
    }\
}

int C2F(genrdiv)(integer *typ,int *a, integer *ia, int *b, integer *ib, int *r, integer *ir, integer *n, integer *ierr)
{
  integer i1 = 0;
  static integer k = 0, jb = 0, ja = 0, jr = 0;

  i1 = *n;
  switch (*typ) {
  case 1:
    RDIV(integer1);
    break;
  case 2:
    RDIV(integer2);
    break;
  case 4:
    RDIV(integer);
    break;
  case 11:
    RDIV(unsigned char);
    break;
  case 12:
    RDIV(unsigned short);
    break;
  case 14:
    RDIV(unsigned int);
    break;
  }
  return 0;
}

