#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define COLCONC(Type) {\
Type *X1;\
Type *X2;\
Type *XR;\
    X1=(Type *)x1;\
    --X1;\
    X2=(Type *)x2;\
    --X2;\
    XR=(Type *)xr;\
    --XR;\
    i1 = -(*m1);i2 = -(*m2);\
    ir1 = -m; ir2 = ir1 + (*m1);\
    for (j = 1; j <= *n1; ++j) {\
      i1 += *m1; i2 += *m2; \
      ir1 += m; ir2 += m; \
      for (i = 1; i <= *m1; ++i) XR[ir1 + i] = X1[i1 + i]; \
      for (i = 1; i <= *m2; ++i) XR[ir2 + i] = X2[i2 + i]; \
     }\
}


int C2F(genconcatcol)(typ,x1,m1,n1,x2,m2,n2,xr)
     integer *m1, *n1, *m2, *n2;
     integer *x1, *x2, *xr;
     integer *typ;
{
static integer i, j, i1, i2, ir1, ir2;
integer m;

m=(*m1)+(*m2);
  switch (*typ) {
  case 1:
    COLCONC(integer1);
    break;
  case 2:
    COLCONC(integer2);
    break;
  case 4:
    COLCONC(integer);
    break;
  case 11:
    COLCONC(unsigned char);
    break;
  case 12:
    COLCONC(unsigned short);
    break;
  case 14:
    COLCONC(unsigned int);
    break;
  }
  return 0;
}
