#include "scicos_block4.h"
#include "../machine.h"

void cumsum_r(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu,i,j,ij;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  for(j=0;j<nu;j++)
       {i=0;
	ij=i+j*mu;
	y[ij]=u[ij];
	for (i=1;i<mu;i++)
	   {ij=i+j*mu;
	    y[ij]=u[ij]+y[ij-1];}}
}

