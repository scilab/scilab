#include "scicos_block.h"

void samphold4(scicos_block *block,int flag)
{
  /* c     Copyright INRIA
     
  Scicos block simulator
  returns sample and hold  of the input */
  
  int i;
  if (flag ==1){
    for (i=0;i<block->insz[0];i++) 
      block->outptr[0][i]=block->inptr[0][i];
  }
}
