#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */
static integer c1 = 1;

extern int C2F(gendot)(integer *typ,integer *n,void *dx,integer *incx,void *dy,integer *incy); /* Defined in SCI/routines/int/gendot.c */

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

int C2F(genmmul)(typ, a, na, b, nb, c, nc, l, m, n)
integer *na, *nb, *nc, *m, *n, *l;
integer *typ;
int *a;
int *b;
int *c;
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

