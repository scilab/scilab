#include "scicos_block.h"
#include <math.h>

void multiplex(scicos_block *block,int flag)
{
  int i,j,k;
  if (block->nin==1){
    k=0;
    for (i=0;i<block->nout;++i){
      for (j=0;j<block->outsz[i];++j){
	block->outptr[i][j]=block->inptr[0][k];
	++k;
      }
    }
  }else {
    k=0;
    for (i=0;i<block->nin;++i){
      for (j=0;j<block->insz[i];++j){
	block->outptr[0][k]=block->inptr[i][j];
	++k;
      }
    }
  }
}
