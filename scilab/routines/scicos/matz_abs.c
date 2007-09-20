#include "scicos_block4.h"
#include "../machine.h"
#include <math.h>
void matz_abs(scicos_block *block,int flag)
{
  double *u1r;
  double *u1i;
  double *y1,*y2;

  int nu,mu,i;

  mu=GetOutPortRows(block,1);
  nu=GetOutPortCols(block,1);
  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  y1=GetRealOutPortPtrs(block,1);
  y2=GetRealOutPortPtrs(block,2);
  for (i=0;i<mu*nu;i++) 
	{*(y1+i)=pow((pow(*(u1r+i),2)+pow(*(u1i+i),2)),0.5);
	 *(y2+i)=atan2(*(u1i+i),*(u1r+i));}
}

