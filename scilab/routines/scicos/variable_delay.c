#include "scicos_block.h"
#include <math.h>
#include "../machine.h"

#if WIN32
#define NULL    0
#endif

extern void sciprint __PARAMS((char *fmt,...));

void variable_delay(scicos_block *block,int flag)
{/*  rpar[0]=max delay, rpar[1]=init value, ipar[0]=buffer length */
  double* pw,del,t,td;
  int* iw;
  int i,j,k;
  if (flag == 4){/* the workspace is used to store previous values */
    if ((*block->work=
	 scicos_malloc(sizeof(int)+sizeof(double)* 
		       block->ipar[0]*(1+block->insz[0])))== NULL ) {
      set_block_error(-16);
      return;
    }
    pw=*block->work; 
    pw[0]=-block->rpar[0]*block->ipar[0];
    for(i=1;i< block->ipar[0];i++){
      pw[i]=pw[i-1]+block->rpar[0];
      for(j=1;j< block->insz[0]+1;j++){
	pw[i+block->ipar[0]*j]=block->rpar[1];
      }
    }
    iw=(int *) (pw+block->ipar[0]*(1+block->insz[0]));
    *iw=0;
  }else  if (flag == 5){
    scicos_free(*block->work);
  } else if (flag==1) {
    if (get_phase_simulation()==1) do_cold_restart();
    pw=*block->work; 
    iw=(int *) (pw+block->ipar[0]*(1+block->insz[0]));
    t=get_scicos_time();
    del=min(max(0,block->inptr[1][0]),block->rpar[0]);
    td=t-del;
    if(td<pw[*iw]){
      sciprint("delayed time=%f but last stored time=%f \r\n", td, pw[*iw]);
      sciprint("Consider increasing the length of buffer in variable delay block\r\n");
    }
    if (t>pw[(block->ipar[0]+*iw-1)%block->ipar[0]]){
      for(j=1;j< block->insz[0]+1;j++){
	pw[*iw +block->ipar[0]*j]=block->inptr[0][j-1];
      }
      pw[*iw]=t;
      *iw=(*iw+1)%block->ipar[0];
    }else{
      for(j=1;j< block->insz[0]+1;j++){
	pw[(block->ipar[0]+*iw-1)%block->ipar[0] +block->ipar[0]*j]=block->inptr[0][j-1];
      }
      pw[(block->ipar[0]+*iw-1)%block->ipar[0]]=t;
    }

    i=0;j= block->ipar[0]-1;

    while (j-i>1) {
      k=(i+j)/2;
      if (td<pw[(k+*iw)%block->ipar[0]]) {
	j=k;
      }else if (td>pw[(k+*iw)%block->ipar[0]]) {
	i=k;
      }else{
	i=k;
	j=k;
	break;
      }
    }
    i=(i+*iw)%block->ipar[0];
    j=(j+*iw)%block->ipar[0];
    del=pw[j]-pw[i];
    if(del!=0.0){
      for (k=1;k<block->insz[0]+1;k++){
	block->outptr[0][k-1]=((pw[j]-td)*pw[i+block->ipar[0]*k] +
			       (td-pw[i])*pw[j+block->ipar[0]*k])/del;
      }
    }else{
      for (k=1;k<block->insz[0]+1;k++){
	block->outptr[0][k-1]=pw[i+block->ipar[0]*k];
      }
    }
  }
}
