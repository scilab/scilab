#include "scicos_block.h"
#include <math.h>
void deadband(scicos_block *block,int flag)
{/* rpar[0]:upper limit,  rpar[1]:lower limit */
  if (flag==1){
    if (get_phase_simulation()==1||block->ng==0) {
      if (*block->inptr[0]>=block->rpar[0]){
	block->outptr[0][0]=*block->inptr[0]-block->rpar[0];
      }else if (*block->inptr[0]<=block->rpar[1]){
	block->outptr[0][0]=*block->inptr[0]-block->rpar[1];
      }else {
	block->outptr[0][0]=0.0;
      }
    }else{
      if (block->mode[0]==1){
	block->outptr[0][0]=*block->inptr[0]-block->rpar[0];
      }else if(block->mode[0]==2) {
	block->outptr[0][0]=*block->inptr[0]-block->rpar[1];
      }else{
	block->outptr[0][0]=0.0;
      } 
    }
  } else if (flag==9){
    block->g[0]=*block->inptr[0]-(block->rpar[0]);
    block->g[1]=*block->inptr[0]-(block->rpar[1]);
    if (get_phase_simulation()==1) {
      if (block->g[0]>=0){
	block->mode[0]=1;
      }else if (block->g[1]<=0){
	block->mode[0]=2;
      }else {
	block->mode[0]=3;
      }
    }
  }
}

