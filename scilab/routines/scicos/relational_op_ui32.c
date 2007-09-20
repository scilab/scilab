#include "scicos_block4.h"
#include <math.h>

void relational_op_ui32(scicos_block *block,int flag)
{
  unsigned long *u1,*u2,*y;
  int *ipar;
  int k,i,m,n;
  m=GetInPortRows(block,1);
  n=GetInPortCols(block,1);
  u1=Getuint32InPortPtrs(block,1);
  u2=Getuint32InPortPtrs(block,2);
  y=Getuint32OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  if (flag==1)
  {if ((block->ng!=0)&(get_phase_simulation()==2)) 
	{for(i=0;i<m*n;i++) *(y+i)=block->mode[i]-1;}
   else{
	for(i=0;i<m*n;i++) y[i]=0;
	k=ipar[0];
   	switch(k)
		{case 0:
			for(i=0;i<m*n;i++)
			     {if (u1[i]==u2[i]) y[i]=1;}
	 	break;
		case 1:
			for(i=0;i<m*n;i++)
			     {if (u1[i]!=u2[i]) y[i]=1;}
		break;
		case 2:
			for(i=0;i<m*n;i++)
			     {if (u1[i]<u2[i]) y[i]=1;}
		break;
		case 3:
			for(i=0;i<m*n;i++)
			     {if (u1[i]<=u2[i]) y[i]=1;}
		break;
		case 4:
			for(i=0;i<m*n;i++)
			     {if (u1[i]>u2[i]) y[i]=1;}
		break;
		case 5:
			for(i=0;i<m*n;i++)
			     {if (u1[i]>=u2[i]) y[i]=1;}
		break;
		}
	}
  }
  else if (flag==9)
  {for(i=0;i<m*n;i++) block->g[i]=*(u1+i)-*(u2+i);
   if (get_phase_simulation()==1)
	{for(i=0;i<m*n;i++) block->mode[i]=(int)1;
	k=ipar[0];
   	switch(k)
		{case 0:
			for(i=0;i<m*n;i++)
			    {if (u1[i]==u2[i]) block->mode[i]=(int)2;}
	 	break;
		case 1:
			for(i=0;i<m*n;i++)
			    {if (u1[i]!=u2[i]) block->mode[i]=(int)2;}
		break;
		case 2:
			for(i=0;i<m*n;i++)
			    {if (u1[i]<u2[i]) block->mode[i]=(int)2;}
		break;
		case 3:
			for(i=0;i<m*n;i++)
			    {if (u1[i]<=u2[i]) block->mode[i]=(int)2;}
		break;
		case 4:
			for(i=0;i<m*n;i++)
			    {if (u1[i]>u2[i]) block->mode[i]=(int)2;}
		break;
		case 5:
			for(i=0;i<m*n;i++)
			    {if (u1[i]>=u2[i]) block->mode[i]=(int)2;}
		break;
		}
	}
  }

}
