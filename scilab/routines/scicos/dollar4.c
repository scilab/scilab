#include "scicos_block.h"

void dollar4(scicos_block *block,int flag)
{
  /* c     Copyright INRIA
     
  Scicos block simulator
  Ouputs delayed input */
  
  int i;
  for(i=0;i< block->insz[0];i++){
    if (flag ==1 || flag ==6 || flag ==4)
      block->outptr[0][i]=block->z[i];
    else if (flag == 2)
      block->z[i]=block->inptr[0][i];
  }
}
