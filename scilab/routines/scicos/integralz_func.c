#include "scicos_block4.h"
#include <math.h>

void  integralz_func(scicos_block *block,int flag)
{int i;
 double *ur,*ui;
 double *yr,*yi;
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
 if (flag==0){
    if(block->ng>0){
      for(i=0;i<(block->nx)/2;++i) {
	if(block->mode[i]==3){
	  block->xd[i]=ur[i];
	  block->xd[i+(block->nx)/2]=ui[i];
	}else{
	  block->xd[i]=0.0;
	  block->xd[i+(block->nx)/2]=0.0;
	}
      }
    }else{
      for(i=0;i<(block->nx)/2;++i) {
	block->xd[i]=ur[i];
	block->xd[i+(block->nx)/2]=ui[i];
      }
    }
  }else if (flag==1||flag==6){
    for(i=0;i<(block->nx)/2;++i) {
      yr[i]=block->x[i];
      yi[i]=block->x[i+(block->nx)/2];
    }
  }else if (flag==2&&block->nevprt==1){
    for(i=0;i<(block->nx)/2;++i) {
      block->x[i]=ur[i];
      block->x[i+(block->nx)/2]=ui[i];
    }
  } else if (flag==9){
    for(i=0;i<(block->nx)/2;++i) {
      if (block->mode[i]==3){
	block->g[i]=(block->x[i]-(block->rpar[i]))*(block->x[i]-(block->rpar[(block->nx)/2+i]));
	block->g[i+(block->nx)/2]=(block->x[i+(block->nx)/2]-(block->rpar[i+(block->nx)]))*(block->x[i+(block->nx)/2]-(block->rpar[3*(block->nx)/2+i]));
      } else {
	block->g[i]=ur[i];
	block->g[i+(block->nx)/2]=ui[i];
      } 
      if (get_phase_simulation()==1) {
	if (ur[i]>=0&&block->x[i]>=block->rpar[i]&&ui[i>=0]&&block->x[i+(block->nx)/2]>=block->rpar[i+(block->nx)]){
	  block->mode[i]=1;
	}else if (ur[i]<=0&&block->x[i]<=block->rpar[(block->nx)/2+i]&&ui[i]<=0&&block->x[i+(block->nx)/2]<=block->rpar[3*(block->nx)/2+i]){
	  block->mode[i]=2;
	}else {
	  block->mode[i]=3;
	}
      }
    }
  }
}
