#include "scicos_block.h"
#include "../machine.h"

extern int C2F(dmmul)();
extern int C2F(dmmul1)();


void gainblk(scicos_block *block,int flag)
{
  int i,un=1;

  if (block->nrpar==1){
    for (i=0;i<block->insz[0];++i){
      block->outptr[0][i]=block->rpar[0]*block->inptr[0][i];
    }    
  }else{   
    C2F(dmmul)(block->rpar,&block->outsz[0],block->inptr[0],
	  &block->insz[0],block->outptr[0],&block->outsz[0],
	  &block->outsz[0],&block->insz[0],&un);
  }
}
