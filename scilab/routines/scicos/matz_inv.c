# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>

#if WIN32
#define NULL    0
#endif

extern int C2F(zgetrf)();
extern int C2F(zgetri)();
typedef struct
{         int *ipiv;
          double *wrk;
	  double *LX;
} mat_inv_struct ;
void matz_inv(scicos_block *block,int flag)
{
 double *ur;
 double *yr;
 double *ui;
 double *yi;
 int nu;
 int info;
 int i;
 mat_inv_struct *ptr;
 
 nu =GetInPortRows(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
             /*init : initialization*/
if (flag==4)

      {if((*(block->work)=(mat_inv_struct*) scicos_malloc(sizeof(mat_inv_struct)))==NULL)
	{set_block_error(-16);
	 return;}
    ptr=*(block->work);
    if((ptr->ipiv=(int*) scicos_malloc(sizeof(int)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr);
	 return;}
    if((ptr->wrk=(double*) scicos_malloc(sizeof(double)*(2*nu*nu)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LX=(double*) scicos_malloc(sizeof(double)*(2*nu*nu)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LX);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
      }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    if((ptr->LX)!=NULL){
    	scicos_free(ptr->ipiv);
    	scicos_free(ptr->LX);
    	scicos_free(ptr->wrk);
    	scicos_free(ptr);
    	return;}
   }

else
   {
    ptr=*(block->work);
    for (i=0;i<(nu*nu);i++)
	{ptr->LX[2*i]=ur[i];
	ptr->LX[2*i+1]=ui[i];}
     C2F(zgetrf)(&nu,&nu,ptr->LX,&nu,ptr->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
    	{set_block_error(-7);
        return;}}
      C2F(zgetri)(&nu,ptr->LX,&nu,ptr->ipiv,ptr->wrk,&nu,&info);
	for (i=0;i<(nu*nu);i++)
	   {yr[i]=ptr->LX[2*i];
	    yi[i]=ptr->LX[2*i+1];}
    }
 }
