#include "scicos_block.h"
#include <math.h>

#if WIN32
#define NULL    0
#endif

void ratelimiter(scicos_block *block,int flag)
{/*  rpar[0]=rising rate limit, rpar[1]=falling rate limit */
  double* pw,rate,t;
  if (flag == 4){/* the workspace is used to store previous values */
    if ((*block->work=
	 scicos_malloc(sizeof(double)*4))== NULL ) {
      set_block_error(-16);
      return;
    }
    pw=*block->work; 
    pw[0]=0.0;
    pw[1]=0.0;
    pw[2]=0.0;
    pw[3]=0.0;
  }else  if (flag == 5){
    scicos_free(*block->work);
  } else if (flag==1) {
    if (get_phase_simulation()==1) do_cold_restart();
    pw=*block->work; 
    t=get_scicos_time();
    if(t>pw[2]){
      pw[0]=pw[2];
      pw[1]=pw[3];
      rate=(block->inptr[0][0]-pw[1])/(t-pw[0]);
    }
    else if(t<=pw[2]){
      if(t>pw[0]){
	rate=(block->inptr[0][0]-pw[1])/(t-pw[0]);
      }else{
	rate=0.0;
      }
    }
    if(rate>block->rpar[0]){
      block->outptr[0][0]=(t-pw[0])*block->rpar[0]+pw[1];
    } else if(rate<block->rpar[1]){
      block->outptr[0][0]=(t-pw[0])*block->rpar[1]+pw[1];
    }else{
      block->outptr[0][0]=block->inptr[0][0];
    }
    pw[2]=t;
    pw[3]=block->outptr[0][0];
  }
}
