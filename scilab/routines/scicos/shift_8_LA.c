# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>

void shift_8_LA(scicos_block *block,int flag)
{
  char *u,*y; 
  int *ipar;
  int mu,nu,i;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=Getint8InPortPtrs(block,1);
  y=Getint8OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  for (i=0;i<mu*nu;i++) y[i]=u[i]<<ipar[0];
}
