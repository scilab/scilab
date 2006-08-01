#include <memory.h>
#include "scicos_block.h"

void cstblk4(scicos_block *block,int flag)
{ 
  /* Copyright INRIA
     
  Scicos block simulator
  output a vector of constants out(i)=rpar(i)
  rpar(1:nrpar) : given constants */
  
  memcpy(block->outptr[0],block->rpar,block->nrpar*sizeof(double));    
}
