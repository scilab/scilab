#include "scicos_block.h"
#include <math.h>

void  signum(scicos_block *block,int flag)
{
  int i,j;
  if (flag==1){
    for(i=0;i<block->insz[0];++i){
      if (get_phase_simulation()==1||block->ng==0) {
	if (block->inptr[0][i]<0){
	  j=2;
	} else if (block->inptr[0][i]>0){
	  j=1;
	}else{
	  j=0;
	}
      }else {
	j=block->mode[i];
      }
      if (j==1){
	block->outptr[0][i]=1.0;
      } else if(j==2){
	block->outptr[0][i]=-1.0;
      }else{
	block->outptr[0][i]=0.0;
      }
    }
  }else if (flag==9){
    for(i=0;i<block->insz[0];++i){
      block->g[i]=block->inptr[0][i];
      if (get_phase_simulation()==1) {
	if(block->g[i]<0){
	  block->mode[i]=2;
	}else{
	  block->mode[i]=1;
	}
      }
    }
  }
}

