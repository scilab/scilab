#include "scicos_block4.h"
#include <math.h>

void extract_bit_u32_MSB1(scicos_block *block,int flag)
{
   int i,maxim;
   unsigned long *y,*u,ref,n;
   int *ipar;
   y=Getuint32OutPortPtrs(block,1);
   u=Getuint32InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=32;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=(unsigned long)pow(2,maxim-1-i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>(maxim-*ipar);
}
