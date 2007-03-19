#include "machine.h"
#include "genmdiag.h"
#include "core_math.h"
/*     Copyright INRIA */

#define MDIAG(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    if (*k>=0)\
      ix=1+(*k)*(*m);\
    else\
      ix=1-(*k);\
    for (i = 0; i < mn; ++i) \
       DY[i] = DX[ix+i*(*m+1)];\
 }


int C2F(genmdiag)(integer *typ,integer *m,integer *n,integer *k,int *dx,int *dy) 
{
  static integer i, ix, mn;

  if (*k >= 0) 
    mn=Max(0,Min(*m,*n-*k));
  else
    mn=Max(0,Min(*m+*k,*n));

  switch (*typ) {
  case 1:
    MDIAG(integer1);
    break;
  case 2:
    MDIAG(integer2);
    break;
  case 4:
    MDIAG(integer);
    break;
  case 11:
    MDIAG(unsigned char);
    break;
  case 12:
    MDIAG(unsigned short);
    break;
  case 14:
    MDIAG(unsigned int);
    break;
  }
  return 0;
}

