#include <memory.h>
#include "scicos_block4.h"
#include<stdio.h>

void cstblk4_m(scicos_block *block,int flag)
{ 
  /* Copyright INRIA

  Scicos block simulator
  output a vector of constants out(i)=opar(i)
  opar(1:nopar) : given constants */
  int nopar,mo,no,so;
  void *y;
  void *opar;
  nopar = GetNopar(block);
  y=GetOutPortPtrs(block,1);
  opar=GetOparPtrs(block,1);
  mo=GetOparSize(block,1,1);
  no=GetOparSize(block,1,2);
  so=GetSizeOfOpar(block,1);
  memcpy(y,opar,mo*no*so);
}
