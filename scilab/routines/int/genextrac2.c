#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define EXTRAC2(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
       l = 1;\
       for (j = 1; j <= *nj; ++j) \
          for (i = 1; i <= *mi; ++i) {\
	    TO[l] = FROM[indxi[i]+(indxj[j]-1)*(*m)];\
	    l = l+1;}\
}


int C2F(genextrac2)(typ,mi,nj,indxi,indxj,from,m,to)
integer *typ;
integer *mi,*nj,*m;
integer *indxi,*indxj;
integer *from;
integer *to;
{
  static integer i, j, l;
  --indxi;
  --indxj;
  switch (*typ) {
  case 1:
    EXTRAC2(integer1);
    break;
  case 2:
    EXTRAC2(integer2);
    break;
  case 4:
    EXTRAC2(integer);
    break;
  case 11:
    EXTRAC2(unsigned char);
    break;
  case 12:
    EXTRAC2(unsigned short);
    break;
  case 14:
    EXTRAC2(unsigned int);
    break;
  }
  return 0;
}

