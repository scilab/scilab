#include "scicos_block4.h"
#include <math.h>

void counter(scicos_block *block,int flag)
{
  double *y,*z;
  int *ipar;
  y=GetRealOutPortPtrs(block,1);
  z=GetDstate(block);
  ipar=GetIparPtrs(block);
  if (flag==1)
    {if (*ipar==1) *y=*z+*(ipar+2);
     else *y=*(ipar+1)-*z;}
  else if (flag==2) 
    {*z=(1+(int)*z)%(*(ipar+1)-*(ipar+2)+1);}
} 
