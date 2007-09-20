# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>

void mat_cath(scicos_block *block,int flag)
{
 double *u;
 double *y;
 int mu,nu;
 int i,j,ij,k,bk;
 
 mu =GetInPortRows(block,1);
 y=GetRealOutPortPtrs(block,1);
if ((flag==1) || (flag==6))
   {for(j=0;j<mu;j++)
        {k=j;
	 for (bk=1;bk<GetNin(block)+1;bk++) 
   	     {u=GetRealInPortPtrs(block,bk);
	     nu=GetInPortCols(block,bk);
     	     for(i=0;i<nu;i++)
		{ij=j+i*mu;
	 	 y[k]=u[ij];
 		 k+= mu;
		}
 	     }
	}
    }
}

