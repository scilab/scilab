#include "scicos_block4.h"
#include "../machine.h"

void mat_suml(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu,i,j,ij;
  double d;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  for(j=0;j<mu;j++)
       {d=0;
	for (i=0;i<nu;i++)
	   {ij=j+i*mu;
	    d+=*(u+ij);}
	*(y+j)=d;}
}
