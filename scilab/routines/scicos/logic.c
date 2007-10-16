#include "scicos_block4.h"
#include <math.h>

void logic(scicos_block *block,int flag)
{
    char *u,*y;
    char inp,num;
    int i,nin,nout,mo;
    char *opar;
    nin=GetNin(block);
    nout=GetNout(block);
    opar=Getint8OparPtrs(block,1);
    mo=GetOparSize(block,1,1);
    if (flag==1) {
	num=0;
	for (i=0;i<nin;i++)
	{
	    u=Getint8InPortPtrs(block,i+1);
	    inp=*u;
	    if (inp>0) inp=1;
	    else inp=0;
	    inp=inp<<i;
	    num=num+inp;
	}
	for (i=0;i<nout;i++)
	{
	    y=Getint8OutPortPtrs(block,i+1);
	    *y=*(opar+num+i*mo);
	}
    }
    if (flag==6) {
	u=Getint8InPortPtrs(block,1);
       	for (i=0;i<nout;i++)
	{
	    y=Getint8OutPortPtrs(block,i+1);
	    *y=*u;
	}
    }
}
