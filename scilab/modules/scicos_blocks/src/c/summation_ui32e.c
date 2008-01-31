#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>
extern int sciprint();
void summation_ui32e(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    unsigned long *y;
    int *ipar;
    double v,l;
    double *rpar;
    unsigned long *u;

    y=Getuint32OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);
    l=pow(2,32);
    if (nin==1){
      v=0;
      u=Getuint32InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	v=v+(double)u[j];
      }
      if ((v>=l)|(v<0)) 
	 {sciprint("overflow error");
	  set_block_error(-4);
	  return;}
      else y[0]=(unsigned long)v; 
    }
    else {
      for (j=0;j<nu*mu;j++) {
        v=0;
	for (k=0;k<nin;k++) {
	  u=Getuint32InPortPtrs(block,k+1);
          if(ipar[k]>0){
	     v=v+(double)u[j];
	    }
	  else{
	     v=v-(double)u[j];}
      }
      if ((v>=l)|(v<0)) 
	 {sciprint("overflow error");
	  set_block_error(-4);
	  return;}
      else y[j]=(unsigned long)v;
    }
   }
  }
}
