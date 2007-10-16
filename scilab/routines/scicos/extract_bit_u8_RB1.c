#include "scicos_block4.h"
#include <math.h>

void extract_bit_u8_RB1(scicos_block *block,int flag)
{
   int i,maxim,numb;
   unsigned char *y,*u,ref,n;
   int *ipar;
   y=Getuint8OutPortPtrs(block,1);
   u=Getuint8InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=8;
   ref=0;
   numb=*(ipar+1)-*ipar+1;
   for(i=0;i<numb;i++)
       {n=(unsigned char)pow(2,*ipar+i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>*ipar;
}
