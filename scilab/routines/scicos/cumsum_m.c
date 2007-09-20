#include "scicos_block4.h"
#include "../machine.h"

void cumsum_m(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu,j;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  y[0]=u[0];
  for(j=1;j<mu*nu;j++)
       {y[j]=u[j]+y[j-1];
       }
}
