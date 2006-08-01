#include "scicos_block.h"
#include <math.h>

void logicalop(scicos_block *block,int flag)
{
  int i,j,k,l;
  i=block->ipar[0];
  switch (i)
    {
    case 0:
      if (block->nin==1)
	{
	  block->outptr[0][0]=1.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]<=0)
	      {
		block->outptr[0][0]=0.0;
		break;
	      }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=1.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]<=0) {
	      block->outptr[0][j]=0.0;
	      break;
	    }
	  }
	}
      }
      break;
      
    case 1:
      if (block->nin==1)
	{
	  block->outptr[0][0]=0.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]>0) {
	      block->outptr[0][0]=1.0;
	      break;
	    }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=0.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]>0) {
	      block->outptr[0][j]=1.0;
	      break;
	    }
	  }
	}
      }
      break;

    case 2:
      if (block->nin==1)
	{
	  block->outptr[0][0]=0.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]<=0)
	      {
		block->outptr[0][0]=1.0;
		break;
	      }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=0.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]<=0) {
	      block->outptr[0][j]=1.0;
	      break;
	    }
	  }
	}
      }
      break;

    case 3:
      if (block->nin==1)
	{
	  block->outptr[0][0]=1.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]>0) {
	      block->outptr[0][0]=0.0;
	      break;
	    }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=1.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]>0) {
	      block->outptr[0][j]=0.0;
	      break;
	    }
	  }
	}
      }
      break;

    case 4:
      if (block->nin==1)
	{
	  l=0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]>0) {
	      l=(l+1)%2;
	    }
	  }
	  block->outptr[0][0]=(double) l;
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  l=0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]>0) {
	      l=(l+1)%2;
	    }
	  }
	  block->outptr[0][j]=(double) l;
	}
      }
      break;

    case 5:
      for (j=0;j<block->insz[0];j++) {
	if(block->inptr[0][j]>0) {
	  block->outptr[0][j]=0.0;
	}
	else{
	  block->outptr[0][j]=1.0;
	}
      }
    }
}
