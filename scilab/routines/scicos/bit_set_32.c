#include "scicos_block4.h"
#include <math.h>

void bit_set_32(scicos_block *block,int flag)
{
  int n,m,i;
  long *opar;
  long *u,*y;
  opar=Getint32OparPtrs(block,1);
  u=Getint32InPortPtrs(block,1);
  y=Getint32OutPortPtrs(block,1);
  n=GetInPortCols(block,1);
  m=GetInPortRows(block,1);
  for (i=0;i<m*n;i++) *(y+i)=((*(u+i))|(*opar));
}
