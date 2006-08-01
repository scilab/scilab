#include "scicos_block.h"
#include <math.h>

void evtvardly(scicos_block *block,int flag)
{ 
  if (flag==3){
    block->evout[0]=block->inptr[0][0];
  }
}

