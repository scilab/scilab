#include "scicos_block4.h"
#include "../machine.h"

void submatz(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int *r;
  int nu,mu,i,j,ij,k;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  r=GetIparPtrs(block);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  k=0;
  for (j=r[2]-1;j<r[3];j++)
	{for (i=r[0]-1;i<r[1];i++)
		{ij=i+j*mu;
		 *(yr+k)=*(ur+ij);
		 *(yi+k)=*(ui+ij);
		 k++;}}
}
