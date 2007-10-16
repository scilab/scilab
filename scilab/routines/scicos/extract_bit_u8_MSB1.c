#include "scicos_block4.h"
#include <math.h>

void extract_bit_u8_MSB1(scicos_block *block,int flag)
{
   int i,maxim;
   unsigned char *y,*u,ref,n;
   int *ipar;
   y=Getuint8OutPortPtrs(block,1);
   u=Getuint8InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=8;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=(unsigned char)pow(2,maxim-1-i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>(maxim-*ipar);
}
