#include "scicos_block4.h"
#include "../machine.h"

void matz_reimc(scicos_block *block,int flag)
{
  double *u1;
  double *u2;
  double *y1r, *y1i;


  int nu,mu,i;

  mu=GetOutPortRows(block,1);
  nu=GetOutPortCols(block,1);
  u1=GetRealInPortPtrs(block,1);
  u2=GetRealInPortPtrs(block,2);
  y1r=GetRealOutPortPtrs(block,1);
  y1i=GetImagOutPortPtrs(block,1);
  
  for (i=0;i<mu*nu;i++) 
	{*(y1r+i)=*(u1+i);
	 *(y1i+i)=*(u2+i);}
}

