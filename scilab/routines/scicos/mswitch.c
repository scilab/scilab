#include "scicos_block.h"
#include <math.h>

void * scicos_malloc(size_t );
void scicos_free(void *p);

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))

void  mswitch(scicos_block *block,int flag)
{
  int i,j;
  i=block->ipar[1];
  if (i==0) {
    if (*block->inptr[0]>0) {
      j=floor(*block->inptr[0]);
    }else{
      j=ceil(*block->inptr[0]);
    }
  }else if (i==1) {
    if (*block->inptr[0]>0) {
      j=floor(*block->inptr[0]+.5);
    }else{
      j=ceil(*block->inptr[0]-.5);
    }
  }else if (i==2) {
    j=ceil(*block->inptr[0]);
  }else if (i==3) {
    j=floor(*block->inptr[0]);
  }
  j=j+1-block->ipar[0];
  j=max(j,1);
  j=min(j,block->nin-1);
  for (i=0;i<block->insz[j];i++) {
    block->outptr[0][i]=block->inptr[j][i];
  }
}
