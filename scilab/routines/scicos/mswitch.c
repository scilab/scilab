#include "scicos_block.h"
#include <math.h>

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))

void  mswitch(scicos_block *block,int flag)
{
  int i,j;
  i=block->ipar[1];
  if (i==0) {
    if (*block->inptr[0]>0) {
      j=(int)floor(*block->inptr[0]);
    }else{
      j=(int)ceil(*block->inptr[0]);
    }
  }else if (i==1) {
    if (*block->inptr[0]>0) {
      j=(int)floor(*block->inptr[0]+.5);
    }else{
      j=(int)ceil(*block->inptr[0]-.5);
    }
  }else if (i==2) {
    j=(int)ceil(*block->inptr[0]);
  }else if (i==3) {
    j=(int)floor(*block->inptr[0]);
  }
  j=j+1-block->ipar[0];
  j=max(j,1);
  j=min(j,block->nin-1);
  for (i=0;i<block->insz[j];i++) {
    block->outptr[0][i]=block->inptr[j][i];
  }
}
