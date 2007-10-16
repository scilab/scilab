#include "scicos_block4.h"
#include <math.h>

void extract_bit_16_RB1(scicos_block *block,int flag)
{
   int i,maxim,numb;
   short *y,*u,ref,n;
   int *ipar;
   y=Getint16OutPortPtrs(block,1);
   u=Getint16InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=16;
   ref=0;
   numb=*(ipar+1)-*ipar+1;
   for(i=0;i<numb;i++)
       {n=(short)pow(2,*ipar+i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>*ipar;
}
