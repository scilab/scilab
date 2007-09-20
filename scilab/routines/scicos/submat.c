#include "scicos_block4.h"
#include "../machine.h"

void submat(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int *r;
  int nu,mu,i,j,ij,k;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  r=GetIparPtrs(block);
  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  k=0;
  for (j=r[2]-1;j<r[3];j++)
	{for (i=r[0]-1;i<r[1];i++)
		{ij=i+j*mu;
		 *(y+k)=*(u+ij);
		 k++;}}
}
