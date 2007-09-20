#include "scicos_block4.h"
#include "../machine.h"
extern int C2F(dprxc)();
void root_coef(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int mu;
  mu=GetInPortRows(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  if (flag==1||flag==6)
  C2F(dprxc)(&mu,u,y);
}
