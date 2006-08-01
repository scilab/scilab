#include "scicos_block.h"
#include <math.h>

void ramp(scicos_block *block,int flag)
{double dt;
  if (flag==1){
    dt=get_scicos_time()-block->rpar[1];
    if (get_phase_simulation()==1) {
      if(dt>0) {
	block->outptr[0][0]=block->rpar[2]+block->rpar[0]*dt;
      }else{
	block->outptr[0][0]=block->rpar[2];
      }
    }else{
      if(block->mode[0]==1) {
	block->outptr[0][0]=block->rpar[2]+block->rpar[0]*dt;
      }else {
	block->outptr[0][0]=block->rpar[2];
      }
    }
  } else if (flag==9){
    block->g[0]=get_scicos_time()-(block->rpar[1]);
    if (get_phase_simulation()==1) {
      if (block->g[0]>=0){
	block->mode[0]=1;
      }else{
	block->mode[0]=2;
      }
    }
  }
}

