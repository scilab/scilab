#include "scicos_block4.h"
#include "../machine.h"

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

void extdiagz(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;

  int mu,nu,i,ii;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  for (i=0;i<mu*nu;i++) 
	{*(yr+i)=0;
	 *(yi+i)=0;}
  for (i=0;i<min(mu,nu);i++)     
	{ii=i+i*mu;
 	 *(yr+ii)=*(ur+ii);
	 *(yi+ii)=*(ui+ii);}
}
