# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>

void matz_catv(scicos_block *block,int flag)
{
 double *ur,*ui;
 double *yr,*yi;
 int mu,nu;
 int i,j,ij,k,bk;
 
 nu =GetInPortCols(block,1); 
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
if ((flag==1) || (flag==6))
{k=0;
 for(j=0;j<nu;j++)
    {for (bk=1;bk<GetNin(block)+1;bk++) 
   	  {ur=GetRealInPortPtrs(block,bk);
	   ui=GetImagInPortPtrs(block,bk);
	   mu=GetInPortRows(block,bk);
           for(i=0;i<mu;i++)
	       {ij=i+j*mu;
	        yr[k]=ur[ij];
	        yi[k]=ui[ij];
 	        k++;
	       }
	  }
     }
}
}
