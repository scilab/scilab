#include "scicos_block4.h"
#include "../machine.h"

void extract(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int *r;
  int nu,mu,nr,i,j,ij,k,nc,nl;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  nr=GetNipar(block);
  r=GetIparPtrs(block);
  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  nc=r[nr-1];
  nl=r[nr-2];
  k=0;
  for (j=0;j<nc;j++)
	{for (i=0;i<nl;i++)
		{ij=r[i]-1+(r[nl+j]-1)*mu;
		 *(y+k)=*(u+ij);
		 k++;}}
}
