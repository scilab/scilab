#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */
static integer c__1 = 1;
#define MCUPROD(Type) {\
Type *A;\
    A=(Type *)a;\
    if (*job == 0) {\
	 C2F(gencuprod)(typ,&mn, A, &c__1);\
    }\
    else if (*job == 1) {\
	for (j = 0; j < *n; ++j) {\
	    C2F(gencuprod)(typ,m, &A[j * (*na) ], &c__1);\
	}}\
    else if (*job == 2) {\
	for (i = 0; i < *m; ++i) {\
	    C2F(gencuprod)(typ,n, &A[i], na);\
	}\
    }\
}

int C2F(genmcuprod)(typ,job, a, na, m, n)
     integer *typ, *job, *a, *na, *m, *n;
{
  extern integer  C2F(gencuprod)();
  static integer  i, j, mn;

  mn=(*m)*(*n);

  switch (*typ) {
  case 1:
    MCUPROD(integer1);
    break;
  case 2:
    MCUPROD(integer2);
    break;
  case 4:
    MCUPROD(integer);
    break;
  case 11:
    MCUPROD(unsigned char);
    break;
  case 12:
    MCUPROD(unsigned short);
    break;
  case 14:
    MCUPROD(unsigned int);
    break;
  }
  return 0;
}
