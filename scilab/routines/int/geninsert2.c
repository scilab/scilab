#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define INSERT2(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
    i1 = *mj;\
    for (j = 1; j <= i1; ++j) {\
	ljj = (indxj[j] - 1) * (*mr);\
	jj = (j - 1) * (*m3);\
	i2 = *mi;\
	for (i = 1; i <= i2; ++i)\
	    TO[indxi[i] + ljj] = FROM[(i - 1 + jj) * (*inc3) + 1];\
    }\
}

int C2F(geninsert2)(typ,mj,mi,indxj,indxi,to,mr,from,m3,inc3)
integer *typ;
integer *mi,*mj,*mr,*m3,*inc3;
integer *indxi,*indxj;
integer *from;
integer *to;
{
  integer i1, i2;
  static integer i, j, ljj,jj;

  --indxi;
  --indxj;

  switch (*typ) {
  case 1:
    INSERT2(integer1);
    break;
  case 2:
    INSERT2(integer2);
    break;
  case 4:
    INSERT2(integer);
    break;
  case 11:
    INSERT2(unsigned char);
    break;
  case 12:
    INSERT2(unsigned short);
    break;
  case 14:
    INSERT2(unsigned int);
    break;
  }
  return 0;
}

