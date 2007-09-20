#include "scicos_block4.h"
#include "../machine.h"
#include <math.h>

void expblk_m(scicos_block *block,int flag)
{
  double *u;
  double *y;
  double *rpar;
  int nu,mu,i;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  rpar=GetRparPtrs(block);
  if ((flag==1)|(flag>=4)) {
  for(i=0;i<mu*nu;i++) y[i]=exp(log(*rpar)*u[i]);
  }
}
