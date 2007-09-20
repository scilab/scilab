#include "scicos_block4.h"
#include "../machine.h"

void mat_sumc(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu,i,j,ij;
  double d;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  for(j=0;j<nu;j++)
       {d=0;
	for (i=0;i<mu;i++)
	   {ij=i+j*mu;
	    d+=*(u+ij);}
	*(y+j)=d;}
}
