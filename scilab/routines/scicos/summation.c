#include <math.h>
#include "scicos_block4.h"


void summation(scicos_block *block,int flag)
{
  int j,k;

  double *u;
  int nu,mu;
  double *y;
  int *ipar;

  y=GetRealOutPortPtrs(block,1);
  nu=GetInPortRows(block,1);
  mu=GetInPortCols(block,1);
  ipar=GetIparPtrs(block);

  if(flag==1){
    if (GetNin(block)==1){
      y[0]=0.0;
      u=GetRealInPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	y[0]=y[0]+u[j];
      }
    }
    else {
      for (j=0;j<nu*mu;j++) {
	y[j]=0.0;
	for (k=0;k<GetNin(block);k++) {
	  u=GetRealInPortPtrs(block,k+1);
	  if(ipar[k]>0){
	    y[j]=y[j]+u[j];
	  }else{
	    y[j]=y[j]-u[j];
	  }
	}
      }
    }
  }
}
