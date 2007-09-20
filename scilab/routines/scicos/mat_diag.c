#include "scicos_block4.h"
#include "../machine.h"

void mat_diag(scicos_block *block,int flag)
{
  double *u1;
  double *y;

  int mu,i,ii;

  mu=GetInPortRows(block,1);

  u1=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  for (i=0;i<mu*mu;i++) *(y+i)=0;
  for (i=0;i<mu;i++)     
	{ii=i+i*mu;
	 *(y+ii)=*(u1+i);}
}
