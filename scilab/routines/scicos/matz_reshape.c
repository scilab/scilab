#include "scicos_block4.h"
#include "../machine.h"

void matz_reshape(scicos_block *block,int flag)
{
  double *u1r;
  double *u1i;
  double *yr;
  double *yi;

  int i,xu,vu;

  xu=GetOutPortRows(block,1);
  vu=GetOutPortCols(block,1);
  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  
  for (i=0;i<xu*vu;i++) 
	{*(yr+i)=*(u1r+i);
	 *(yi+i)=*(u1i+i);}
}

