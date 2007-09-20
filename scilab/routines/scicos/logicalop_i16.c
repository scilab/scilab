#include "scicos_block4.h"
#include <math.h>

void logicalop_i16(scicos_block *block,int flag)
{
  int i,m,n,nin,k,j,l;
  int *ipar;
  short *y,*u;
  m=GetInPortRows(block,1);
  n=GetInPortCols(block,1);
  y=Getint16OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  nin=GetNin(block);
  k=ipar[0];
  switch (k)
	{
	case 0:
	     if (nin==1)
  		 {u=Getint16InPortPtrs(block,1);
		  if (ipar[1]==0)
		      {*y=1;
                       for(i=0;i<m*n;i++)
     		       	  {if (u[i]<=0)
		       	       {*y=0;
			        break;}
     		      	  }
		      }
		  else
		      {*y=u[0];
		       for (i=1;i<m*n;i++)
			    {*y=(*y)&(u[i]);}
		      }
		 }
	     else
		 {if (ipar[1]==0)
		     {for (i=0;i<m*n;i++)
			   {y[i]=1;
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  if (u[i]<=0) 
				      {y[i]=0;
				       break;}
				 } 
			   }
		     }
		 else
		     {for (i=0;i<m*n;i++)
			   {u=Getint16InPortPtrs(block,1);
			    y[i]=u[i];
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  y[i]=(y[i])&(u[i]);
				 }
			   }
		     }
		 }
	break;
	case 1:
	     if (nin==1)
  		 {u=Getint16InPortPtrs(block,1);
		  *y=0;
		  if (ipar[1]==0)
		      {for(i=0;i<m*n;i++)
     		       	  {if (u[i]>0)
		       	       {*y=1;
			        break;}
     		      	  }
		      }
		  else
		      {for (i=0;i<m*n;i++)
			    {*y=(*y)|(u[i]);}
		      }
		 }
	     else
		 {if (ipar[1]==0)
		     {for (i=0;i<m*n;i++)
			   {y[i]=0;
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  if (u[i]>0) 
				      {y[i]=1;
				       break;}
				 }
			   }
		     }
		 else
		     {for (i=0;i<m*n;i++)
			   {y[i]=0;
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  y[i]=(y[i])|(u[i]);
				 }
			   }
		     }
		 }
	break;
	case 2:
	     if (nin==1)
  		 {u=Getint16InPortPtrs(block,1);
		  if (ipar[1]==0)
		      {*y=0;
		       for(i=0;i<m*n;i++)
     		       	  {if (u[i]<=0)
		       	       {*y=1;
			        break;}
     		      	  }
		      }
		  else
		      {*y=u[0];
		       for (i=1;i<m*n;i++)
			    {*y=((*y)&(u[i]));}
		       *y=~*y;
		      }
		 }
	     else
		 {if (ipar[1]==0)
		     {for (i=0;i<m*n;i++)
			   {y[i]=0;
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  if (u[i]<=0) 
				      {y[i]=1;
				       break;}
				 } 
			   }
		     }
		 else
		     {for (i=0;i<m*n;i++)
			   {u=Getint16InPortPtrs(block,1);
			    y[i]=u[i];
			    for (j=2;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  y[i]=((y[i])&(u[i]));
				 }
			    y[i]=~y[i];
			   }
		     }
		 }
	break;
	case 3:
	     if (nin==1)
  		 {u=Getint16InPortPtrs(block,1);
		  if (ipar[1]==0)
		      {*y=1;
		       for(i=0;i<m*n;i++)
     		       	  {if (u[i]>0)
		       	       {*y=0;
			        break;}
     		      	  }
		      }
		  else
		      {*y=0;
		       for (i=0;i<m*n;i++)
			    {*y=((*y)|(u[i]));}
		       *y=~*y;
		      }
		 }
	     else
		 {if (ipar[1]==0)
		     {for (i=0;i<m*n;i++)
			   {y[i]=1;
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  if (u[i]>0) 
				      {y[i]=0;
				       break;}
				 }
			   }
		     }
		 else
		     {for (i=0;i<m*n;i++)
			   {u=Getint16InPortPtrs(block,1);
			    y[i]=u[i];
			    for (j=2;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  y[i]=((y[i])|(u[i]));
				 }
			    y[i]=~y[i];
			   }
		     }
		 }
	break;
	case 4:
	     if (nin==1)
  		 {u=Getint16InPortPtrs(block,1);
		  if (ipar[1]==0)
		      {l=0;
		       for(i=0;i<m*n;i++)
     		       	  {if (u[i]>0)
		       	       {l=(l+1)%2;
			       }
     		      	  }
		       *y=(short)l;
		      }
		  else
		      {*y=u[0];
		       for (i=1;i<m*n;i++)
			    {*y=((*y)^(u[i]));}
		      }
		 }
	     else
		 {if (ipar[1]==0)
		     {for (i=0;i<m*n;i++)
			   {l=0;
			    for (j=1;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  if (u[i]>0) 
				      {l=(l+1)%2;}
				 }
			    y[i]=(short)l;
			   }
		     }
		 else
		     {for (i=0;i<m*n;i++)
		           {u=Getint16InPortPtrs(block,1);
		           y[i]=u[i];
			   for (j=2;j<nin+1;j++)
				 {u=Getint16InPortPtrs(block,j);
				  y[i]=((y[i])^(u[i]));
				 }
			   }
		     }
		 }
	     break;
	case 5:
	     u=Getint16InPortPtrs(block,1);
	     if (ipar[1]==0)
		 {for (i=0;i<m*n;i++)
		       {if (u[i]>0) y[i]=0;
			else y[i]=1;}
		 }
	     else
		 {for (i=0;i<m*n;i++)
		       {y[i]=~(u[i]);}
		 }
	     break;
	}
}
