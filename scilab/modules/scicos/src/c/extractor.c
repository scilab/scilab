#include "scicos_block.h"
#include <math.h>

void extractor(scicos_block *block,int flag)
{
  int i,j;
  if(flag==1){
    for(i=0;i<block->nipar;++i){
      j=block->ipar[i]-1;
      if(j<0) j=0;
      if(j>=block->insz[0]) j=block->insz[0]-1;
      block->outptr[0][i]=block->inptr[0][j];
    }
  }
}
