#include "scicos_block.h"
#include <math.h>

void  switch2(scicos_block *block,int flag)
{
  int i,j,phase;
  if (flag == 1) {
    phase=get_phase_simulation();
    if (phase==1||block->ng==0){
      i=2;
      if (*block->ipar==0){
	if (*block->inptr[1]>=*block->rpar) i=0;
      }else if (*block->ipar==1){
	if (*block->inptr[1]>*block->rpar) i=0;
      }else {
	if (*block->inptr[1]!=*block->rpar) i=0;			     
      }
    }else{
      if(block->mode[0]==1){
	i=0;
      }else if(block->mode[0]==2){
	i=2;
      }
    }
    for (j=0;j<block->insz[0];j++) {
      block->outptr[0][j]=block->inptr[i][j];
    }
  }else if(flag == 9){
    phase=get_phase_simulation();
    block->g[0]=*block->inptr[1]-(*block->rpar);
    if (phase==1){
      i=2;
      if (*block->ipar==0){
	if (block->g[0]>=0.0) i=0;
      }else if (*block->ipar==1){
	if (block->g[0]>0.0) i=0;
      }else {
	if (block->g[0]!=0.0) i=0;			     
      }
      if(i==0) {
	block->mode[0]=1;
      }else{
	block->mode[0]=2;
      }
    }
  }
}
