#include "scicos_block.h"
#include <math.h>

extern void sciprint();

void edgetrig(scicos_block *block,int flag)
{
  double z= block->z[0],u=block->inptr[0][0];
  if(flag==2||flag==6){
    block->z[0]=u;
  }else if(flag==1){
    if(block->ipar[0]!=0){
      z=z*block->ipar[0];
      u=u*block->ipar[0];
      if(((z<=0)&(u>0))||((z<0)&(u>=0))) {
	block->outptr[0][0]=1.;
      }else{
	block->outptr[0][0]=0.;
      }
    } else{ /* rising and falling edge */
      if(((z<=0)&(u>0))||((z<0)&(u>=0))||((z>0)&(u<=0))||((z>=0)&(u<0))){
	block->outptr[0][0]=1.;
      }else{
	block->outptr[0][0]=0.;
      }
    }
  }else if (flag==4) {
    if (block->ng>0){
      set_block_error(-1);
      sciprint("Trigger block must have discrete time input.");
      return;
    }
  }
}
