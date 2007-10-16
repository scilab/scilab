# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>

void matmul_ui8s(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)) {
  unsigned char *u1,*u2,*y; 
  double k,C,D;
  int mu1,nu1,nu2,i,j,l,ji,jl,il;
  int *ipar;

  mu1=GetInPortRows(block,1);
  nu1=GetInPortCols(block,1);
  nu2=GetInPortCols(block,2);
  u1=Getuint8InPortPtrs(block,1);
  u2=Getuint8InPortPtrs(block,2);
  y=Getuint8OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);

     k=pow(2,8);
       for (l=0;l<nu2;l++)
	    {for(j=0;j<mu1;j++)
	        {D=0;
	        for(i=0;i<nu1;i++)
		   {ji=j+i*mu1;
		    jl=j+l*mu1;
		    il=i+l*nu1;
		    C=(double)(u1[ji])*(double)(u2[il]);
		    D=D + C;}
		    if (D>(k-1))
		        {y[jl]=(unsigned char)(k-1);}
		    else if (D<0)
			{y[jl]=0;}
		    else {y[jl]=(unsigned char)(D);}
		    }
		 }
	     }
}
