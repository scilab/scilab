#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
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


int C2F(genmtran)(typ,a, na, b, nb, m, n)
integer *na, *nb, *m, *n;
integer *typ;
int *a;
int *b;
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

