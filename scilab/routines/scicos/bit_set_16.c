#include "scicos_block4.h"
#include <math.h>

void bit_set_16(scicos_block *block,int flag)
{
  int n,m,i;
  short *opar;
  short *u,*y;
  opar=Getint16OparPtrs(block,1);
  u=Getint16InPortPtrs(block,1);
  y=Getint16OutPortPtrs(block,1);
  n=GetInPortCols(block,1);
  m=GetInPortRows(block,1);
  for (i=0;i<m*n;i++) *(y+i)=((*(u+i))|(*opar));
}
