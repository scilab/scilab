#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

void summation_ui8s(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    unsigned char *y;
    int *ipar;
    double v,l;
    double *rpar;
    unsigned char *u;

    y=Getuint8OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);
    l=pow(2,8);
    if (nin==1){
      v=0;
      u=Getuint8InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	v=v+(double)u[j];
      }
      if (v>=l)  v=l-1;
      else if (v<0) v=0;
      y[0]=(unsigned char)v; 
    }
    else {
      for (j=0;j<nu*mu;j++) {
        v=0;
	for (k=0;k<nin;k++) {
	  u=Getuint8InPortPtrs(block,k+1);
          if(ipar[k]>0){
	     v=v+(double)u[j];
	    }
	  else{
	     v=v-(double)u[j];}
      }
      if (v>=l)  v=l-1;
      else if (v<0) v=0;
      y[j]=(unsigned char)v;
    }
   }
  }
}
