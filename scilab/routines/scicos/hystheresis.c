#include "scicos_block.h"
#include <math.h>

void  hystheresis(scicos_block *block,int flag)
{
  if (flag==1){
    if (get_phase_simulation()==1) {
      if (*block->inptr[0]>=block->rpar[0]){
	block->outptr[0][0]=block->rpar[2];
      }else if (*block->inptr[0]<=block->rpar[1]){
	block->outptr[0][0]=block->rpar[3];
      }
    }else{
      if (block->mode[0]<2){
	block->outptr[0][0]=block->rpar[3];
      }else{
	block->outptr[0][0]=block->rpar[2];
      }
    } 
  } else if (flag==9){
    block->g[0]=*block->inptr[0]-(block->rpar[0]);
    block->g[1]=*block->inptr[0]-(block->rpar[1]);
    if (get_phase_simulation()==1) {
      if (block->g[0]>=0){
	block->mode[0]=2;
      }else if (block->g[1]<=0){
	block->mode[0]=1;
      }
    }
  }
}
