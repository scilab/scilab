#include "scicos_block4.h"
#include <math.h>

void logicalop_m(scicos_block *block,int flag)
{
  int i,m,n,nin,k,j,l;
  int *ipar;
  double *y,*u;
  m=GetInPortRows(block,1);
  n=GetInPortCols(block,1);
  y=GetRealOutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  nin=GetNin(block);
  k=ipar[0];
  switch (k)
	{
	case 0:
	     if (nin==1)
  		 {u=GetRealInPortPtrs(block,1);
		  *y=1;
                  for(i=0;i<m*n;i++)
     		      {if (u[i]<=0)
		       	   {*y=0;
			    break;}
     		      }
		 }
	     else
		 {for (i=0;i<m*n;i++)
		       {y[i]=1;
			for (j=1;j<nin+1;j++)
			     {u=GetRealInPortPtrs(block,j);
			      if (u[i]<=0) 
				  {y[i]=0;
				   break;}
			     } 
		       }
		 }
	break;
	case 1:
	     if (nin==1)
  		 {u=GetRealInPortPtrs(block,1);
		  *y=0;
		  for(i=0;i<m*n;i++)
     		      {if (u[i]>0)
		       	   {*y=1;
			    break;}
     		      }
		 }
	     else
		 {for (i=0;i<m*n;i++)
		       {y[i]=0;
			for (j=1;j<nin+1;j++)
			     {u=GetRealInPortPtrs(block,j);
			      if (u[i]>0) 
				  {y[i]=1;
				   break;}
			     }
		       }
		 }
	break;
	case 2:
	     if (nin==1)
  		 {u=GetRealInPortPtrs(block,1);
		  *y=0;
		  for(i=0;i<m*n;i++)
     		      {if (u[i]<=0)
		       	   {*y=1;
			    break;}
     		      }
		 }
	     else
		 {for (i=0;i<m*n;i++)
		       {y[i]=0;
			for (j=1;j<nin+1;j++)
			     {u=GetRealInPortPtrs(block,j);
			      if (u[i]<=0) 
				  {y[i]=1;
				   break;}
			     } 
		       }
		 }
	break;
	case 3:
	     if (nin==1)
  		 {u=GetRealInPortPtrs(block,1);
		  *y=1;
		  for(i=0;i<m*n;i++)
     		      {if (u[i]>0)
		       	   {*y=0;
			    break;}
     		      }
		 }
	     else
		 {for (i=0;i<m*n;i++)
		       {y[i]=1;
			for (j=1;j<nin+1;j++)
			     {u=GetRealInPortPtrs(block,j);
			      if (u[i]>0) 
				  {y[i]=0;
				   break;}
		   	     }
		       }
		 }
	break;
	case 4:
	     if (nin==1)
  		 {u=GetRealInPortPtrs(block,1);
		  l=0;
		  for(i=0;i<m*n;i++)
     		      {if (u[i]>0)
		       	   {l=(l+1)%2;
			   }
     		      }
		  *y=(double)l;
		 }
	     else
		 {for (i=0;i<m*n;i++)
		       {l=0;
			for (j=1;j<nin+1;j++)
			     {u=GetRealInPortPtrs(block,j);
			      if (u[i]>0) 
				  {l=(l+1)%2;}
		  	     }
			y[i]=(double)l;
		       }
		 }
	     break;
	case 5:
	     u=GetRealInPortPtrs(block,1);
	     for (i=0;i<m*n;i++)
		  {if (u[i]>0) y[i]=0;
		   else y[i]=1;}
	     break;
	}
}
