#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define MCOPY(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    if (*na == *m && *nb == *m) {\
      mn = (*m) * (*n);\
      for (i = 1; i <= mn; ++i) B[i] = A[i];\
    }\
    else {\
    ia = -(*na);ib = -(*nb); \
    for (j = 1; j <= *n; ++j) {\
      ia += *na;ib += *nb;\
      for (i = 1; i <= *m; ++i) B[ib + i] = A[ia + i];\
    }\
    }\
}
int C2F(genmcopy)(typ,a, na, b, nb, m, n)
integer *typ;
int *a;
integer *na;
int *b;
integer *nb;
integer *m, *n;
{
  static integer i, j, ia, ib, mn;
  switch (*typ) {
  case 1:
    MCOPY(integer1);
    break;
  case 2:
    MCOPY(integer2);
    break;
  case 4:
    MCOPY(integer);
    break;
  case 11:
    MCOPY(unsigned char);
    break;
  case 12:
    MCOPY(unsigned short);
    break;
  case 14:
    MCOPY(unsigned int);
    break;
  }
  return 0;
}
