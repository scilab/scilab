#include "scicos_block4.h"
#include <math.h>

void bit_clear(scicos_block *block,int flag)
{
if (flag==1){
  int n,k,m;
  int *ipar;
  ipar=GetIparPtrs(block);
  k=*(ipar+1);
  m=*(ipar+2);
  n=(pow(2,m)-1)-pow(2,*ipar);
  switch(k)
	{case 0:
		{long *u,*y;
		u=Getint32InPortPtrs(block,1);
		y=Getint32OutPortPtrs(block,1);
		*y=((*u)&(n));
	break;}
	case 1:
		{short *u,*y;
		u=Getint16InPortPtrs(block,1);
		y=Getint16OutPortPtrs(block,1);	
		*y=((*u)&(n));
	break;}
	case 2:
		{char *u,*y;
		u=Getint8InPortPtrs(block,1);
		y=Getint8OutPortPtrs(block,1);
		*y=((*u)&(n));
	break;}
	case 3:
		{unsigned long *u,*y;
		u=Getuint32InPortPtrs(block,1);
		y=Getuint32OutPortPtrs(block,1);
		*y=((*u)&(n));
	break;}
	case 4:
		{unsigned short *u,*y;
		u=Getuint16InPortPtrs(block,1);
		y=Getuint16OutPortPtrs(block,1);
		*y=((*u)&(n));
	break;}
	case 5:
		{unsigned char *u,*y;
		u=Getuint8InPortPtrs(block,1);
		y=Getuint8OutPortPtrs(block,1);	
		*y=((*u)&(n));
	break;}
	}
  }
}
