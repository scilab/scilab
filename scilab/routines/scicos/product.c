#include "scicos_block.h"
#include <math.h>

void product(scicos_block *block,int flag)
{
  int j,k;
  if(flag==1){
    if (block->nin==1){
      block->outptr[0][0]=1.0;
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][0]=block->outptr[0][0]*block->inptr[0][j];
      }
    }
    else {
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][j]=1.0;
	for (k=0;k<block->nin;k++) {
	  if(block->ipar[k]>0){
	    block->outptr[0][j]=block->outptr[0][j]*block->inptr[k][j];
	  }else{
	    if(block->inptr[k][j]==0){
	      set_block_error(-2);
	      return;
	    }else{
	      block->outptr[0][j]=block->outptr[0][j]/block->inptr[k][j];
	    }
	  }
	}
      }
    }
  }
}
