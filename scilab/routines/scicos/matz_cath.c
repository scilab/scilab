# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>

void matz_cath(scicos_block *block,int flag)
{
 double *ur,*ui;
 double *yr,*yi;
 int mu,nu;
 int i,j,ij,k,bk;
 
 mu =GetInPortRows(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
if ((flag==1) || (flag==6))
   {for(j=0;j<mu;j++)
        {k=j;
	 for (bk=1;bk<GetNin(block)+1;bk++) 
   	     {ur=GetRealInPortPtrs(block,bk);
              ui=GetImagInPortPtrs(block,bk);
	      nu=GetInPortCols(block,bk);
     	      for(i=0;i<nu;i++)
		  {ij=j+i*mu;
	 	   yr[k]=ur[ij];
	 	   yi[k]=ui[ij];
 		   k+= mu;
		  }
 	     }
	}
    }
}

