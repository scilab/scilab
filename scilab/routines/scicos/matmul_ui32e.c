# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>
# include <stdio.h>
extern int sciprint();
void matmul_ui32e(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)) {
  unsigned long *u1,*u2,*y; 
  double k,C,D;
  int mu1,nu1,nu2,i,j,l,ji,jl,il;
  int *ipar;

  mu1=GetInPortRows(block,1);
  nu1=GetInPortCols(block,1);
  nu2=GetInPortCols(block,2);
  u1=Getuint32InPortPtrs(block,1);
  u2=Getuint32InPortPtrs(block,2);
  y=Getuint32OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);

     k=pow(2,32);
       for (l=0;l<nu2;l++)
	    {for(j=0;j<mu1;j++)
	        {D=0;
	        for(i=0;i<nu1;i++)
		   {ji=j+i*mu1;
		    jl=j+l*mu1;
		    il=i+l*nu1;
		    C=(double)(u1[ji])*(double)(u2[il]);
		    D=D + C;}
		    if ((D>(k-1)) |(D<0))
		        {sciprint("overflow error");
			 set_block_error(-4);
			 return;}
		    else {y[jl]=(unsigned long)(D);}
		    }
		 }
	     }
}
