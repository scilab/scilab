#include "scicos_block4.h"
#include <math.h>

void extract_bit_16_MSB0(scicos_block *block,int flag)
{
   int i,maxim;
   short *y,*u,ref,n;
   int *ipar;
   y=Getint16OutPortPtrs(block,1);
   u=Getint16InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=16;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=(short)pow(2,maxim-1-i);
        ref=ref+n;}
   *y=(*u)&(ref);
}
