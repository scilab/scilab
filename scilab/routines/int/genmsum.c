#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */
static integer c__1 = 1;
#define MSUM(Type) {\
Type *A;\
Type *V;\
    A=(Type *)a;\
    V=(Type *)v;\
    iv = 0;\
    if (*job == 0) {\
	t = 0;\
	for (j = 0; j < *n; ++j) \
	    t +=  C2F(gensum)(typ,m, &A[j * (*na)], &c__1);\
	V[0] = (Type)t;}\
    else if (*job == 1) {\
	for (j = 0; j < *n; ++j) {\
	    t =  C2F(gensum)(typ,m, &A[j * (*na) ], &c__1);\
	    V[iv] = (Type)t;iv += *nv;\
	}}\
    else if (*job == 2) {\
	for (i = 0; i < *m; ++i) {\
	    t =  C2F(gensum)(typ,n, &A[i], m);\
	    V[iv] = (Type)t;iv += *nv;\
	}\
    }\
}

int C2F(genmsum)(typ,job, a, na, m, n, v, nv)
     integer *typ, *job, *a, *na, *m, *n, *v, *nv;
{
  extern integer  C2F(gensum)();
  static integer  i, j, t, iv;

  switch (*typ) {
  case 1:
    MSUM(integer1);
    break;
  case 2:
    MSUM(integer2);
    break;
  case 4:
    MSUM(integer);
    break;
  case 11:
    MSUM(unsigned char);
    break;
  case 12:
    MSUM(unsigned short);
    break;
  case 14:
    MSUM(unsigned int);
    break;
  }
  return 0;
}
