#include "scicos_block4.h"
#include "../machine.h"

void mat_reshape(scicos_block *block,int flag)
{
  double *u1;
  double *y;

  int i,xu,vu;

  xu=GetOutPortRows(block,1);
  vu=GetOutPortCols(block,1);
  u1=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  for (i=0;i<xu*vu;i++) *(y+i)=*(u1+i);
}
