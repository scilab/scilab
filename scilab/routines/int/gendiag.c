#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define Abs(x) ( ( (x) >= 0) ? (x) : -( x) )

#define DIAG(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    DY=(Type *)dy;\
    --DY;\
    for (i = 1; i <= nn*nn; ++i) {\
       DY[i] = 0;}\
    if (*k>=0) {\
      iy=1+(*k)*nn;}\
    else {\
      iy=1-(*k);}\
    for (i = 0; i < *n; ++i) {\
       DY[iy+i*(nn+1)] = DX[i];}\
 }

int C2F(gendiag)(typ,n,k,dx,dy) 
integer *n;
integer *k;
integer *typ;
int *dx;
int *dy;
{
  static integer i, iy, nn;

  nn = *n+Abs(*k);

  switch (*typ) {
  case 1:
    DIAG(integer1);
    break;
  case 2:
    DIAG(integer2);
    break;
  case 4:
    DIAG(integer);
    break;
  case 11:
    DIAG(unsigned char);
    break;
  case 12:
    DIAG(unsigned short);
    break;
  case 14:
    DIAG(unsigned int);
    break;
  }
  return 0;
}

