
#include "scicos_block.h"
#include <math.h>

#if WIN32
#define NULL    0
#endif

void deriv(scicos_block *block,int flag)
{ 
  double* rw;
  double t,dt;
  int i;
  if (flag == 4){/* the workspace is used to store previous values */
    if ((*block->work=
	 scicos_malloc(sizeof(double)*2*(1+block->insz[0])))== NULL ) {
      set_block_error(-16);
      return;
    }
    rw=*block->work; 
    t=get_scicos_time();
    rw[0]=t;
    rw[1]=t;
    for(i=0;i<block->insz[0];++i){
      rw[2+2*i]=0;
      rw[3+2*i]=0;
    }
  }else  if (flag == 5){
    scicos_free(*block->work);
  }else  if (flag == 1) {
    rw=*block->work;
    t=get_scicos_time();
    if(t>rw[1]) {
      rw[0]=rw[1];
      for(i=0;i<block->insz[0];++i)  {rw[2+2*i]=rw[3+2*i];}
    }
    rw[1]=t;
    for(i=0;i<block->insz[0];++i) {
      rw[3+2*i]=block->inptr[0][i];
    }
    dt=rw[1]-rw[0];

    if(dt!=0.0) {
      for(i=0;i<block->insz[0];++i)
	block->outptr[0][i]=(rw[3+2*i]-rw[2+2*i])/dt;
    }
  }
}
  
