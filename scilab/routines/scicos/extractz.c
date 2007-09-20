#include "scicos_block4.h"
#include "../machine.h"

void extractz(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int *r;
  int nu,mu,nr,i,j,ij,k,nc,nl;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  nr=GetNipar(block);
  r=GetIparPtrs(block);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  nc=r[nr-1];
  nl=r[nr-2];
  k=0;
  for (j=0;j<nc;j++)
	{for (i=0;i<nl;i++)
		{ij=r[i]-1+(r[nl+j]-1)*mu;
		 *(yr+k)=*(ur+ij);
		 *(yi+k)=*(ui+ij);
		 k++;}}
}
