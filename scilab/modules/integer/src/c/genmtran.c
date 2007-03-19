#include "machine.h"
#include "genmtran.h"
/*     Copyright INRIA */

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


int C2F(genmtran)(integer *typ,int *a, integer *na, int *b, integer *nb, integer *m, integer *n)
{
  integer i1, i2;
  static integer i, j, ia, ib;
  switch (*typ) {
  case 1:
    MTRAN(integer1);
    break;
  case 2:
    MTRAN(integer2);
    break;
  case 4:
    MTRAN(integer);
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

