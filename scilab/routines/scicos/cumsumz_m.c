#include "scicos_block4.h"
#include "../machine.h"

void cumsumz_m(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int nu,mu,j;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  yr[0]=ur[0];
  yi[0]=ui[0];
  for(j=1;j<mu*nu;j++)
       {yr[j]=ur[j]+yr[j-1];
       yi[j]=ui[j]+yi[j-1];}
}
