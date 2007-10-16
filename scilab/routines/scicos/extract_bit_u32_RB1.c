#include "scicos_block4.h"
#include <math.h>

void extract_bit_u32_RB1(scicos_block *block,int flag)
{
   int i,maxim,numb;
   unsigned long *y,*u,ref,n;
   int *ipar;
   y=Getuint32OutPortPtrs(block,1);
   u=Getuint32InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=32;
   ref=0;
   numb=*(ipar+1)-*ipar+1;
   for(i=0;i<numb;i++)
       {n=(unsigned long)pow(2,*ipar+i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>*ipar;
}
