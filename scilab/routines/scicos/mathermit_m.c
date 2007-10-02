#include "scicos_block4.h"
#include "../machine.h"
extern int C2F(mtran)();
void mathermit_m(scicos_block *block,int flag)
{
  double *ur;
  double *yr;
  double *ui;
  double *yi;
  int nu,mu,i;

  nu=GetInPortRows(block,1);
  mu=GetInPortCols(block,1);

  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  C2F(mtran)(ur,&nu,yr,&mu,&nu,&mu);
  C2F(mtran)(ui,&nu,yi,&mu,&nu,&mu);
  for(i=0;i<mu*nu;i++) *(yi+i)=-(*(yi+i));
}

