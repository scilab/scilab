#include <math.h>
#include "../machine.h"
#include "scicos_block4.h"
extern int sciprint();
void gainblk_i32e(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)){
  int i,j,l,ji,jl,il;
  long *u,*y;
  int mu,ny,my,mo,no;
  long *opar;
  double k,D,C;

  mo=GetOparSize(block,1,1);
  no=GetOparSize(block,1,2);
  mu=GetInPortRows(block,1);
  my=GetOutPortRows(block,1);
  ny=GetOutPortCols(block,1);
  u=Getint32InPortPtrs(block,1);
  y=Getint32OutPortPtrs(block,1);
  opar=Getint32OparPtrs(block,1);

  k=pow(2,32)/2;
  if (mo*no==1){
    for (i=0;i<ny*mu;++i){
     D=(double)(opar[0])*(double)(u[i]);
     if ((D>=k)|( D<-k))
	{sciprint("overflow error");
	 set_block_error(-4);
	 return;}
     else y[i]=(long)D;
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
		    if ((D>=k)|( D<-k))
			{sciprint("overflow error");
			 set_block_error(-4);
			 return;}
		    else y[jl]=(long)D;
		  }
	     }
  }
 }
}
