#include "scicos_block.h"
#include <math.h>

#if WIN32
extern double asinh(double x);
extern double acosh(double x);
extern double atanh(double x);
#endif

void acosh_blk(scicos_block *block,int flag)
{
  int j;
  if(flag==1){
    for (j=0;j<block->insz[0];j++) {
      block->outptr[0][j]=acosh(block->inptr[0][j]);
    }
  }
}
