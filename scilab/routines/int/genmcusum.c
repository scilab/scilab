#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */
static integer c__1 = 1;
#define MCUSUM(Type) {\
Type *A;\
    A=(Type *)a;\
    if (*job == 0) {\
	 C2F(gencusum)(typ,&mn, A, &c__1);\
    }\
    else if (*job == 1) {\
	for (j = 0; j < *n; ++j) {\
	    C2F(gencusum)(typ,m, &A[j * (*na) ], &c__1);\
	}}\
    else if (*job == 2) {\
	for (i = 0; i < *m; ++i) {\
	    C2F(gencusum)(typ,n, &A[i], na);\
	}\
    }\
}

int C2F(genmcusum)(typ,job, a, na, m, n)
     integer *typ, *job, *a, *na, *m, *n;
{
  extern integer  C2F(gencusum)();
  static integer  i, j, mn;

  mn=(*m)*(*n);

  switch (*typ) {
  case 1:
    MCUSUM(integer1);
    break;
  case 2:
    MCUSUM(integer2);
    break;
  case 4:
    MCUSUM(integer);
    break;
  case 11:
    MCUSUM(unsigned char);
    break;
  case 12:
    MCUSUM(unsigned short);
    break;
  case 14:
    MCUSUM(unsigned int);
    break;
  }
  return 0;
}
