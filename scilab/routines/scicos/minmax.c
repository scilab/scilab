#include "scicos_block.h"
#include <math.h>

void minmax(scicos_block *block,int flag)
{ /*ipar[0]=1 -> min,  ipar[0]=2 -> max */
  
  int i,phase;
  double maxmin;
  phase=get_phase_simulation();
  if (flag == 1) {
    if(block->nin==1){
      if((block->ng==0)|(phase==1)){
	maxmin=block->inptr[0][0];
	for (i=1;i<block->insz[0];++i){
	  if(block->ipar[0]==1){
	    if(block->inptr[0][i]<maxmin)  maxmin=block->inptr[0][i];
	  } else {
	    if(block->inptr[0][i]>maxmin)  maxmin=block->inptr[0][i];
	  }
	}
      }else{
	maxmin=block->inptr[0][block->mode[0]-1];
      }
      block->outptr[0][0]=maxmin;

    }else if (block->nin==2){
      for (i=0;i<block->insz[0];++i){
	if((block->ng==0)|(phase==1)){
	  if(block->ipar[0]==1){
	    block->outptr[0][i]=min(block->inptr[0][i],block->inptr[1][i]);
	  } else {
	    block->outptr[0][i]=max(block->inptr[0][i],block->inptr[1][i]);
	  }
	}else{
	  block->outptr[0][i]=block->inptr[block->mode[0]-1][i];
	}
      }
    }
  } else if(flag == 9){
    if(block->nin==1){
      if(block->nin==1){
	if (phase==2){
	  for (i=0;i<block->insz[0];++i){
	    if(i!=block->mode[0]-1){
	      block->g[i]=block->inptr[0][i]-block->inptr[0][block->mode[0]-1];
	    }else{
	      block->g[i]=1.0;
	    }
	  }
	} else if (phase==1){
	  maxmin=block->inptr[0][0];
	  for (i=1;i<block->insz[0];++i){
	    block->mode[0]=1;
	    if(block->ipar[0]==1){
	      if(block->inptr[0][i]<maxmin) {
		maxmin=block->inptr[0][i];
		block->mode[0]=i+1;
	      }
	    } else {
	      if(block->inptr[0][i]>maxmin) {
		maxmin=block->inptr[0][i];
		block->mode[0]=i+1;
	      }
	    }
	  }
	}
      }
    }else if(block->nin==2){
      for (i=0;i<block->insz[0];++i){
	block->g[i]=block->inptr[0][i]-block->inptr[1][i];
	if(phase==1){
	  if(block->ipar[0]==1){
	    if(block->g[i]>0) {
	      block->mode[i]=2;
	    }else{
	      block->mode[i]=1;
	    }
	  }else{
	    if(block->g[i]<0) {
	      block->mode[i]=2;
	    }else{
	      block->mode[i]=1;
	    }
	  }
	}
      }
    }
  }
}

