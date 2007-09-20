#include "scicos_block4.h"
#include <math.h>

void extract_bit_i8(scicos_block *block,int flag)
{
 if (flag==1){
   int i,maxim,k,numb;
   char *y,*u,ref,n;
   int *ipar;
   y=Getint8OutPortPtrs(block,1);
   u=Getint8InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   k=*ipar;
   maxim=8;
   switch(k)
	 {
	  case 0:
		ref=0;
		for(i=0;i<maxim/2;i++)
		   {n=pow(2,maxim/2+i);
		   ref=ref+n;}
		*y=(*u)&(ref);
		if (*(ipar+1)==1) *y=*y>>maxim/2;
	  break;
	  case 1:
		ref=0;
		for(i=0;i<maxim/2;i++)
		   {n=pow(2,i);
		   ref=ref+n;}
		*y=(*u)&(ref);
	  break;
	  case 2:
		ref=0;
		for(i=0;i<ipar[2];i++)
		   {n=pow(2,maxim-1-i);
		   ref=ref+n;}
		*y=(*u)&(ref);
		if (*(ipar+1)==1) *y=*y>>(maxim-ipar[2]);
	  break;
	  case 3:
		ref=0;
		for(i=0;i<ipar[2];i++)
		   {n=pow(2,i);
		   ref=ref+n;}
		*y=(*u)&(ref);
	  break;
	  case 4:
		ref=0;
		numb=ipar[3]-ipar[2]+1;
		for(i=0;i<numb;i++)
		   {n=pow(2,ipar[2]+i);
		   ref=ref+n;}
		*y=(*u)&(ref);
		if (*(ipar+1)==1) *y=*y>>ipar[2];
	  break;
	 }
 }
}
