#include "scicos_block4.h"
#include "../machine.h"

void matzmul2_m(scicos_block *block,int flag)
{
  double *u1r;
  double *u2r;
  double *u1i;
  double *u2i;
  double *yr;
  double *yi;
  int mu,nu,i;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  u2r=GetRealInPortPtrs(block,2);
  u2i=GetImagInPortPtrs(block,2);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);

  for(i=0;i<mu*nu;i++)
	{yr[i]=u1r[i]*u2r[i]-u1i[i]*u2i[i];
	 yi[i]=u1r[i]*u2i[i]+u1i[i]*u2r[i];}
}
