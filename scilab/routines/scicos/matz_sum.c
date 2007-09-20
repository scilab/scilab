#include "scicos_block4.h"
#include "../machine.h"

void matz_sum(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int nu,mu,i;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);

  yr[0]=0.0;
  yi[0]=0.0;
  for(i=0;i<nu*mu;i++)
       {yr[0]+=ur[i];
	yi[0]+=ui[i];}
}
