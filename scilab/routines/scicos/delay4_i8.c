#include "scicos_block4.h"

void delay4_i8(scicos_block *block,int flag)
{
  /* Copyright INRIA
   
     Scicos block simulator
     Ouputs nx*dt delayed input */

  char *y,*u,*oz;
  int nz,mz;
  int i;
  
  u=Getint8InPortPtrs(block,1);
  y=Getint8OutPortPtrs(block,1);
  oz=Getint8OzPtrs(block,1);
  nz=GetOzSize(block,1,2);
  mz=GetOzSize(block,1,1);

  if ((flag==1)||(flag==6)||(flag==4)){
    y[0]=oz[0];
  }else if (flag == 2){
    /*  shift buffer */
    for (i=0; i<=(mz*nz)-2; i++){
      oz[i]=oz[i+1];
    }
    /* add new point to the buffer */
    oz[(mz*nz)-1]=u[0];
  }
}
