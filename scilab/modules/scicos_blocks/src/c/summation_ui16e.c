#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>
extern int sciprint();
void summation_ui16e(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    unsigned short *y;
    int *ipar;
    double v,l;
    double *rpar;
    unsigned short *u;

    y=Getuint16OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);
    l=pow(2,16);
    if (nin==1){
      v=0;
      u=Getuint16InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	v=v+(double)u[j];
      }
      if ((v>=l)|(v<0)) 
	 {sciprint("overflow error");
	  set_block_error(-4);
	  return;}
      else y[0]=(unsigned short)v; 
    }
    else {
      for (j=0;j<nu*mu;j++) {
        v=0;
	for (k=0;k<nin;k++) {
	  u=Getuint16InPortPtrs(block,k+1);
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
      else y[j]=(unsigned short)v;
    }
   }
  }
}
