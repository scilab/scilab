#include "scicos_block4.h"
#include "../machine.h"

void matmul2_m(scicos_block *block,int flag)
{
  double *u1;
  double *u2;
  double *y;

  int nu,mu,i;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u1=GetRealInPortPtrs(block,1);
  u2=GetRealInPortPtrs(block,2);
  y=GetRealOutPortPtrs(block,1);

  for(i=0;i<mu*nu;i++) y[i]=u1[i]*u2[i];
}
