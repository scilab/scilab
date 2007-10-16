#include <math.h>
#include "../machine.h"
#include "scicos_block4.h"

void gainblk_ui8n(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)){
  int i,j,l,ji,jl,il;
  unsigned char *u,*y;
  int mu,ny,my,mo,no;
  unsigned char *opar;
  double k,D,C,t;

  mo=GetOparSize(block,1,1);
  no=GetOparSize(block,1,2);
  mu=GetInPortRows(block,1);
  my=GetOutPortRows(block,1);
  ny=GetOutPortCols(block,1);
  u=Getuint8InPortPtrs(block,1);
  y=Getuint8OutPortPtrs(block,1);
  opar=Getuint8OparPtrs(block,1);

  k=pow(2,8);
  if (mo*no==1){
    for (i=0;i<ny*mu;++i){
     D=(double)(opar[0])*(double)(u[i]);
     t=D-(double)((int)(D/(k)))*((k));
     if ((t>=k/2)|(-(t)>=k/2))
       {if (t>=0) (t)=(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));
	else (t)=-(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));}
     y[i]=(unsigned char)t;
    }
  }else{
     for (l=0;l<ny;l++)
	 {for (j=0;j<my;j++)
	      {D=0;
	       for (i=0;i<mu;i++)
		   {ji=j+i*my;
		    jl=j+l*my;
		    il=i+l*mu;
		    C=(double)(opar[ji])*(double)(u[il]);
		    D=D + C;}
		    t=D-(double)((int)(D/(k)))*((k));
		    if ((t>=k/2)|(-(t)>=k/2))
		       {if (t>=0) (t)=(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));
			else (t)=-(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));}
		    y[jl]=(unsigned char)t;
		  }
	     }
  }
 }
}
