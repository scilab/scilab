#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define RDIV(Type) {\
Type *A;\
Type *B;\
Type *R;\
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

int C2F(genrdiv)(typ,a, ia, b, ib, r, ir, n, ierr)
integer *typ;
int *a,*b,*r;
integer *ia,*ib;
integer *ir, *n, *ierr;
{
  integer i1;
  static integer k, jb, ja, jr;

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

