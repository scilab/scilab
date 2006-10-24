#include "scicos_block.h"
#include <math.h>

void  integral_func(scicos_block *block,int flag)
{int i;
  if (flag==0){
    if(block->ng>0){
      for(i=0;i<block->nx;++i) {
	if(block->mode[i]==3){
	  block->xd[i]=block->inptr[0][i];
	}else{
	  block->xd[i]=0.0;
	}
      }
    }else{
      for(i=0;i<block->nx;++i) {
	block->xd[i]=block->inptr[0][i];
      }
    }
  }else if (flag==1||flag==6){
    for(i=0;i<block->nx;++i) {
      block->outptr[0][i]=block->x[i];
    }
  }else if (flag==2&&block->nevprt==1){
    for(i=0;i<block->nx;++i) {
      block->x[i]=block->inptr[1][i];
    }
  } else if (flag==9){
    for(i=0;i<block->nx;++i) {
      if (block->mode[i]==3){
	block->g[i]=(block->x[i]-(block->rpar[i]))*(block->x[i]-(block->rpar[block->nx+i]));
      } else {
	block->g[i]=block->inptr[0][i];
      } 
      if (get_phase_simulation()==1) {
	if (block->inptr[0][i]>=0&&block->x[i]>=block->rpar[i]){
	  block->mode[i]=1;
	}else if (block->inptr[0][i]<=0&&block->x[i]<=block->rpar[block->nx+i]){
	  block->mode[i]=2;
	}else {
	  block->mode[i]=3;
	}
      }
    }
  }
}
