#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define CHECK(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    if (*op == equal) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] == B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == less + great) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] != B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia; i2 += *ib;\
	}\
      }\
      else if (*op == less) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] < B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == great) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] > B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == less + equal) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] <= B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == great + equal) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] >= B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
    }\
}




int C2F(gencheck)(typ, op, n, a, ia, b, ib, r)
integer *typ;
integer *op, *n, *ia, *ib;
int *a;
int *b;
int *r;
{
  static integer i, i1, i2, ii1;
  static integer less = 59;
  static integer great = 60;
  static integer equal = 50;

  --r;
  i1 = 1;i2 = 1;
  ii1 = *n; 
  switch (*typ) {
  case 0:
    CHECK(double);
    break;
  case 1:
    CHECK(integer1);
    break;
  case 2:
    CHECK(integer2);
    break;
  case 4:
    CHECK(integer);
    break;
  case 11:
    CHECK(unsigned char);
    break;
  case 12:
    CHECK(unsigned short);
    break;
  case 14:
    CHECK(unsigned int);
    break;
  }
  return 0;
}
