#include "scicos_block4.h"
#include <math.h>

void extract_bit_16_LH(scicos_block *block,int flag)
{
   int i,maxim;
   short *y,*u,ref,n;
   y=Getint16OutPortPtrs(block,1);
   u=Getint16InPortPtrs(block,1);
   maxim=16;
   ref=0;
   for(i=0;i<maxim/2;i++)
       {n=(short)pow(2,i);
        ref=ref+n;}
   *y=(*u)&(ref);
}
