#include "scicos_block.h"
#include <math.h>

void  step_func(scicos_block *block,int flag)
{
  int i;
  if (flag==1&&block->nevprt==1) {
    for (i=0;i<block->outsz[0];++i) {
      block->outptr[0][i]=block->rpar[block->outsz[0]+i];
    } 
  }else if (flag==4) {
    for(i=0;i<block->outsz[0];++i) {
      block->outptr[0][i]=block->rpar[i];
    }
  }
}

