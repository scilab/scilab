#include "scicos_block.h"
#include <math.h>

void 
modulo_count(scicos_block *block,int flag)
{
  if(flag == 1) {
    *block->outptr[0]=block->z[0];
  }else if (flag == 2){
    block->z[0]=(1+(int)block->z[0])%(block->ipar[0]);
   }
}

