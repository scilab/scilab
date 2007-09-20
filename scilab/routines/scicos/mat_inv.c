# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>

#if WIN32
#define NULL    0
#endif

extern int C2F(dgetrf)();
extern int C2F(dgetri)();
typedef struct
{         int *ipiv;
          double *dwork;
} mat_inv_struct ;
void mat_inv(scicos_block *block,int flag)
{
 double *u;
 double *y;
 int nu;
 int info;
 int i;
 mat_inv_struct *ptr;
 
 nu =GetInPortRows(block,1);
 u=GetRealInPortPtrs(block,1);
 y=GetRealOutPortPtrs(block,1);

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
    if((ptr->dwork=(double*) scicos_malloc(sizeof(double)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    if ((ptr->dwork)!=NULL){
    	scicos_free(ptr->ipiv);
    	scicos_free(ptr->dwork);
    	scicos_free(ptr);
    	return;}
   }

else
   {
    ptr=*(block->work);
    for (i=0;i<(nu*nu);i++)   { y[i]=u[i];}
    C2F(dgetrf)(&nu,&nu,&y[0],&nu,ptr->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
    C2F (dgetri)(&nu,y,&nu,ptr->ipiv,ptr->dwork,&nu,&info);
    
   }
}
