#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */


#define TRIU(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    kk = *k;\
    nn = *n;\
    l = 0;\
    if (kk>0) {\
      for (i = 0; i < (*m)*kk; ++i) \
         DX[i] = 0;\
      l = l+ (*m)*kk;\
      nn = nn - kk;\
      kk = 0;}\
    ls = l +1 - kk;\
    ll = *m -1 + kk;\
    for (j = 1; j <= nn; ++j) {\
      if (ll > 0) {\
        for (i = 0; i < ll; ++i) \
          DX[ls+i] = 0;\
        ll--;\
        ls = ls + *m +1;\
      }\
    }\
 }

int C2F(gentriu)(typ,m,n,k,dx) 
integer *n;
integer *m;
integer *k;
integer *typ;
int *dx;
{
  static integer kk, l, ls, ll, j, i, nn;

  switch (*typ) {
  case 1:
    TRIU(integer1);
    break;
  case 2:
    TRIU(integer2);
    break;
  case 4:
    TRIU(integer);
    break;
  case 11:
    TRIU(unsigned char);
    break;
  case 12:
    TRIU(unsigned short);
    break;
  case 14:
    TRIU(unsigned int);
    break;
  }
  return 0;
}

