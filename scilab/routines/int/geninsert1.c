#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define INSERT1(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
       for (i = 1; i <= *mi; ++i) \
	 TO[indx[i]] = FROM[1+(i-1)*(*inc2)];\
}

int C2F(geninsert1)(typ,mi,indx,to,from,inc2)
integer *typ;
integer *mi;
integer *indx;
integer *from;
integer *to;
integer *inc2;
{

  static integer i;
  --indx;
  switch (*typ) {
  case 1:
    INSERT1(integer1);
    break;
  case 2:
    INSERT1(integer2);
    break;
  case 4:
    INSERT1(integer);
    break;
  case 11:
    INSERT1(unsigned char);
    break;
  case 12:
    INSERT1(unsigned short);
    break;
  case 14:
    INSERT1(unsigned int);
    break;
  }
  return 0;
}

