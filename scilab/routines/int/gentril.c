#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */


#define TRIL(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    kk = *k;\
    nn = *n;\
    l = 0;\
    if (kk>=0) {\
     l = l + (*m)*(kk+1);\
     nn = nn - kk - 1;\
     kk = -1;\
    }\
    ls = l;\
    ll = -kk;\
    for (j = 1; j <= nn; ++j) {\
      if (ll > *m) ll = *m;\
      for (i = 0; i < ll; ++i) \
        DX[ls+i] = 0;\
      ll++;\
      ls = ls + *m;\
    }\
 }

int C2F(gentril)(typ,m,n,k,dx) 
integer *n;
integer *m;
integer *k;
integer *typ;
int *dx;
{
  static integer kk, l, ls, ll, j, i, nn;

  switch (*typ) {
  case 1:
    TRIL(integer1);
    break;
  case 2:
    TRIL(integer2);
    break;
  case 4:
    TRIL(integer);
    break;
  case 11:
    TRIL(unsigned char);
    break;
  case 12:
    TRIL(unsigned short);
    break;
  case 14:
    TRIL(unsigned int);
    break;
  }
  return 0;
}

