#include "scicos_block4.h"
#include "../machine.h"

void mat_sum(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu,i;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  
  y[0]=0.0;
  for(i=0;i<nu*mu;i++)
       {y[0]=u[i]+y[0];}
}
