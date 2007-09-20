#include "scicos_block4.h"
#include "../machine.h"

void matz_diag(scicos_block *block,int flag)
{
  double *u1r;
  double *u1i;
  double *yr;
  double *yi;
  int mu,i,ii;

  mu=GetInPortRows(block,1);

  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  for (i=0;i<mu*mu;i++) 
	{*(yr+i)=0;
	 *(yi+i)=0;}
  for (i=0;i<mu;i++)     
	{ii=i+i*mu;
	 *(yr+ii)=*(u1r+i);
	 *(yi+ii)=*(u1i+i);}
}
