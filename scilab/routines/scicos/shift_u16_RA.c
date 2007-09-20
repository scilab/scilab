# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>

void shift_u16_RA(scicos_block *block,int flag)
{
  unsigned short *u,*y; 
  int *ipar;
  int mu,nu,i;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=Getuint16InPortPtrs(block,1);
  y=Getuint16OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  for (i=0;i<mu*nu;i++) y[i]=u[i]>>-ipar[0];
}
