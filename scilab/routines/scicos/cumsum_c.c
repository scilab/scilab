#include "scicos_block4.h"
#include "../machine.h"

void cumsum_c(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu,i,j,ij;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  
  for (i=0;i<mu;i++) y[i]=u[i];
  for (j=1;j<nu;j++)
       {
	for (i=0;i<mu;i++)
	   {ij=i+j*mu;
	    y[ij]=u[ij]+y[ij-mu];}}
}

