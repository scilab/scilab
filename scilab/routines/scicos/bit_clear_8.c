#include "scicos_block4.h"
#include <math.h>

void bit_clear_8(scicos_block *block,int flag)
{
  int n,m,i;
  char *opar;
  char *u,*y;
  opar=Getint8OparPtrs(block,1);
  u=Getint8InPortPtrs(block,1);
  y=Getint8OutPortPtrs(block,1);
  n=GetInPortCols(block,1);
  m=GetInPortRows(block,1);
  for (i=0;i<m*n;i++) *(y+i)=((*(u+i))&(*opar));
}
