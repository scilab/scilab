#include "scicos_block4.h"
#include <math.h>

void extract_bit_8_UH0(scicos_block *block,int flag)
{
   int i,maxim;
   char *y,*u,ref,n;
   y=Getint8OutPortPtrs(block,1);
   u=Getint8InPortPtrs(block,1);
   maxim=8;
   ref=0;
   for(i=0;i<maxim/2;i++)
       {n=(char)pow(2,maxim/2+i);
        ref=ref+n;}
   *y=(*u)&(ref);
}
