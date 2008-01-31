#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

void summation_ui32n(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    unsigned long *y;
    int *ipar;
    double *rpar;
    unsigned long *u;

    y=Getuint32OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);

    if (nin==1){
      y[0]=0;
      u=Getuint32InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	y[0]=y[0]+u[j];
      }
    }
    else {
      for (j=0;j<nu*mu;j++) {
        y[j]=0;
	for (k=0;k<nin;k++) {
	  u=Getuint32InPortPtrs(block,k+1);
          if(ipar[k]>0){
	     y[j]=y[j]+u[j];
	    }
	  else{
	     y[j]=y[j]-u[j];}
      }
    }
   }
  }
}
