#include "scicos_block.h"
#include <math.h>

void summation(scicos_block *block,int flag)
{
  int j,k;
  if(flag==1){
    if (block->nin==1){
      block->outptr[0][0]=0.0;
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][0]=block->outptr[0][0]+block->inptr[0][j];
      }
    }
    else {
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][j]=0.0;
	for (k=0;k<block->nin;k++) {
	  if(block->ipar[k]>0){
	    block->outptr[0][j]=block->outptr[0][j]+block->inptr[k][j];
	  }else{
	    block->outptr[0][j]=block->outptr[0][j]-block->inptr[k][j];
	  }
	}
      }
    }
  }
}
