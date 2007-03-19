#include "machine.h"
#include "genconcatrow.h"
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


int C2F(genconcatrow)(integer *typ,integer *x1,integer *m1,integer *n1,integer *x2,integer *m2,integer *n2)
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
