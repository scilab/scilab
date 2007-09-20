#include "scicos_block4.h"
#include "../machine.h"

extern int C2F(dmmul)();
extern int C2F(dmmul1)();

void matmul_m(scicos_block *block,int flag)
{
  double *u1;
  double *u2;
  double *y;

  int nu,nu2,mu2;

  nu=GetInPortRows(block,1);
  nu2=GetInPortRows(block,2);
  mu2=GetInPortCols(block,2);

  u1=GetRealInPortPtrs(block,1);
  u2=GetRealInPortPtrs(block,2);
  y=GetRealOutPortPtrs(block,1);

  C2F(dmmul)(u1,&nu,u2,&nu2,y,&nu,&nu,&nu2,&mu2);
}
