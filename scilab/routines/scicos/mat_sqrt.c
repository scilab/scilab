#include "scicos_block4.h"
#include <math.h>

void mat_sqrt(scicos_block *block,int flag)
{
  double *u;
  double *y;

  int nu,mu,i;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  for(i=0;i<mu*nu;i++)  y[i]=pow(u[i],0.5);

}
