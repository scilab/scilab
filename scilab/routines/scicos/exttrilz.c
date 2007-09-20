#include "scicos_block4.h"
#include "../machine.h"

void exttrilz(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int nu,mu,i,j,ij;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  for(i=0;i<mu*nu;i++) 
	{*(yr+i)=0;
 	 *(yi+i)=0;}
  for (j=0;j<nu;j++)
	{for (i=j;i<mu;i++)
		{ij=i+j*mu;
		 *(yr+ij)=*(ur+ij);
		 *(yi+ij)=*(ui+ij);}}
}
