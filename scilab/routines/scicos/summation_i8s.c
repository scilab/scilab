#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

void summation_i8s(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    char *y;
    int *ipar;
    double v,l;
    double *rpar;
    char *u;

    y=Getint8OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);
    l=pow(2,8)/2;
    if (nin==1){
      v=0;
      u=Getint8InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	v=v+(double)u[j];
      }
      if (v>=l)  v=l-1;
      else if (v<-l) v=-l;
      y[0]=(char)v; 
    }
    else {
      for (j=0;j<nu*mu;j++) {
        v=0;
	for (k=0;k<nin;k++) {
	  u=Getint8InPortPtrs(block,k+1);
          if(ipar[k]>0){
	     v=v+(double)u[j];
	    }
	  else{
	     v=v-(double)u[j];}
      }
      if (v>=l)  v=l-1;
      else if (v<-l) v=-l;
      y[j]=(char)v;
    }
   }
  }
}
