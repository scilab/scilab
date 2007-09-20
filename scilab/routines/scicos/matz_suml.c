#include "scicos_block4.h"
#include "../machine.h"

void matz_suml(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int nu,mu,i,j,ij;
  double dr,di;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  for(j=0;j<mu;j++)
       {dr=0;
	di=0;
	for (i=0;i<nu;i++)
	   {ij=j+i*mu;
	    dr+=*(ur+ij);
	    di+=*(ui+ij);}
	*(yr+j)=dr;
	*(yi+j)=di;}
}
