#include "../machine.h"
#include "scicos_block4.h"

extern int C2F(mtran)();

void mattran_m(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu;

  nu=GetInPortRows(block,1);
  mu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  C2F(mtran)(u,&nu,y,&mu,&nu,&mu);
}
