#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */
#define RCONC(Type) {\
Type *X1;\
Type *X2;\
    X1=(Type *)x1;\
    --X1;\
    X1 +=(*m1)*(*n1);\
    X2=(Type *)x2;\
    --X2;\
       for (i = 1; i <= (*m2)*(*n2); ++i) \
	 X1[i] = X2[i];}


int C2F(genconcatrow)(typ,x1,m1,n1,x2,m2,n2)
integer *m1, *n1, *m2, *n2;
integer *x1, *x2;
integer *typ;
{
  static integer i;
  switch (*typ) {
  case 1:
    RCONC(integer1);
    break;
  case 2:
    RCONC(integer2);
    break;
  case 4:
    RCONC(integer);
    break;
  case 11:
    RCONC(unsigned char);
    break;
  case 12:
    RCONC(unsigned short);
    break;
  case 14:
    RCONC(unsigned int);
    break;
  }
  return 0;
}
