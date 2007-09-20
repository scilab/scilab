#include "scicos_block4.h"
#include <math.h>
#include "../machine.h"
extern int C2F(wsqrt)();

void matz_sqrt(scicos_block *block,int flag)
{
 if (flag==1){
  double *ui,*ur;
  double *yi,*yr;
  double inpi,inpr;
  int nu,mu,i;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);

  for(i=0;i<mu*nu;i++)  
	{inpr=ur[i];
	 inpi=ui[i];
	 C2F(wsqrt)(&inpr,&inpi,&yr[i],&yi[i]);}
 }
}
