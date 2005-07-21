#include "scicos_block.h"

extern int dmmul(double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n);

void gainblk(scicos_block *block,int flag)
{
  int i,un=1;

  if (block->nrpar==1){
    for (i=0;i<block->insz[0];++i){
      block->outptr[0][i]=block->rpar[0]*block->inptr[0][i];
    }    
  }else{   
    dmmul(block->rpar,&block->outsz[0],block->inptr[0],
	  &block->insz[0],block->outptr[0],&block->outsz[0],
	  &block->outsz[0],&block->insz[0],&un);
  }
}
