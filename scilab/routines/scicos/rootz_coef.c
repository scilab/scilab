#include "scicos_block4.h"
#include "../machine.h"
extern int C2F(wprxc)();
void rootz_coef(scicos_block *block,int flag)
{
  double *ur,*ui;
  double *yr,*yi;
  int mu;
  mu=GetInPortRows(block,1);

  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  if (flag==1||flag==6)
  C2F(wprxc)(&mu,ur,ui,yr,yi);
}
