#include "scicos_block4.h"
#include "../machine.h"

extern int C2F(wmmul)();

void matzmul_m(scicos_block *block,int flag)
{
  double *u1r;
  double *u2r;
  double *u1i;
  double *u2i;
  double *yr;
  double *yi;
  int mu1,mu2,nu;

  mu1=GetInPortRows(block,1);
  mu2=GetInPortRows(block,2);
  nu=GetInPortCols(block,2);

  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  u2r=GetRealInPortPtrs(block,2);
  u2i=GetImagInPortPtrs(block,2);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);

  C2F(wmmul)(u1r,u1i,&mu1,u2r,u2i,&mu2,yr,yi,&mu1,&mu1,&mu2,&nu);
}
