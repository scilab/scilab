#include "scicos_block.h"
#include <math.h>


void relationalop(scicos_block *block,int flag)
{
  int i;
  i=block->ipar[0];
  if(flag==1){
    if ((block->ng!=0)&(get_phase_simulation()==2))
      {
	block->outptr[0][0]=block->mode[0]-1.0;
      }
    else {
      switch (i)
	{
	case 0:
	  if(block->inptr[0][0]==block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;
	  
	case 1:
	  if(block->inptr[0][0]!=block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;
	case 2:
	  if(block->inptr[0][0]<block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;    
	case 3:
	  if(block->inptr[0][0]<block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;    
	case 4:
	  if(block->inptr[0][0]<=block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;   
	case 5:
	  if(block->inptr[0][0]>=block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;   
	case 6:
	  if(block->inptr[0][0]>block->inptr[1][0]) {
	    block->outptr[0][0]=1.0;
	  }else{
	    block->outptr[0][0]=0.0;
	  }
	  break;
	}
    }

  }else if(flag==9){
    block->g[0]=block->inptr[0][0]-block->inptr[1][0];
    if (get_phase_simulation()==1) {
      switch (i)
	{
	case 0:
	  if(block->inptr[0][0]==block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;
	  
	case 1:
	  if(block->inptr[0][0]!=block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;
	case 2:
	  if(block->inptr[0][0]<block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;    
	case 3:
	  if(block->inptr[0][0]<block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;    
	case 4:
	  if(block->inptr[0][0]<=block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;   
	case 5:
	  if(block->inptr[0][0]>=block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;   
	case 6:
	  if(block->inptr[0][0]>block->inptr[1][0]) {
	    block->mode[0]=(int)2.0;
	  }else{
	    block->mode[0]=(int)1.0;
	  }
	  break;
	}
    }
  }
}
