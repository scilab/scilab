#include "scicos_block.h"
#include <math.h>

void asin_blk(scicos_block *block,int flag)
{
  int j;
  if(flag==1){
    for (j=0;j<block->insz[0];j++) {
      block->outptr[0][j]=asin(block->inptr[0][j]);
    }
  }
}
