# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>

#if WIN32
#define NULL    0
#endif

extern int C2F(dgetrf)();
typedef struct
{         int *ipiv;
          double *wrk;
} mat_det_struct ;
void mat_det(scicos_block *block,int flag)
{
 double *u;
 double *y;
 int nu;
 int info;
 int i;
 double D,l;
 mat_det_struct *mdet;
 
 nu =GetInPortRows(block,1);
 u=GetRealInPortPtrs(block,1);
 y=GetRealOutPortPtrs(block,1);

             /*init : initialization*/
if (flag==4)

   {if((*(block->work)=(mat_det_struct*) scicos_malloc(sizeof(mat_det_struct)))==NULL)
	{set_block_error(-16);
	 return;}
    mdet=*(block->work);
    if((mdet->ipiv=(int*) scicos_malloc(sizeof(int)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(mdet);
	 return;}
    if((mdet->wrk=(double*) scicos_malloc(sizeof(double)*(nu*nu)))==NULL)
	{set_block_error(-16);
	 scicos_free(mdet->ipiv);
	 scicos_free(mdet);
	 return;}
    }

       /* Terminaison */
else if (flag==5)
   {mdet=*(block->work);
    if(mdet->wrk!=NULL) {
	scicos_free(mdet->ipiv);
    	scicos_free(mdet->wrk);
    	scicos_free(mdet);
    	return;}
   }

else
   {
    mdet=*(block->work);
    for (i=0;i<(nu*nu);i++)   mdet->wrk[i]=u[i];
     C2F(dgetrf)(&nu,&nu,mdet->wrk,&nu,mdet->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
    	{set_block_error(-7);
        return;}}
      D=1;
    for (i=0;i<nu;i++)
       {if((*(mdet->ipiv+i))!=i+1) D=-D;
        l=*(mdet->wrk+i*(nu+1));
        D=D*l;
        };
     *y=D;
    }
 }
