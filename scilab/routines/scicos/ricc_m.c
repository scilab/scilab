# include "scicos_block4.h"
# include "../machine.h"
extern int C2F(riccsl)();
extern int C2F(riccms)();
extern int C2F(ricdsl)();
extern int C2F(ricdmf)();

#if WIN32
#define NULL    0
#endif

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{         int *bwork;
          int *iwork;
          double *dwork;
	  double *LX;
	  double *LWI;
	  double *LWR;
	  double *Rcond;
	  double *Ferr;
} ricc_struct ;
void ricc_m(scicos_block *block,int flag)
{
 double *u1;
 double *u2;
 double *u3;
 double *y;
 int *ipar;
 int nu;
 int info,i;
 int lw;
 ricc_struct *ptr;

 nu =GetInPortCols(block,1);
 u1=GetRealInPortPtrs(block,1);
 u2=GetRealInPortPtrs(block,2);
 u3=GetRealInPortPtrs(block,3);
 y=GetRealOutPortPtrs(block,1);
 ipar=GetIparPtrs(block);
 if (ipar[0]==1)
    {if (ipar[1]==1)
	 {lw=9*nu*nu+4*nu+max(1,6*nu);}
     else
	 {lw=9*nu*nu+7*nu+1;}}
 else
     {if (ipar[1]==1)
	 {lw=12*nu*nu+22*nu+max(21,4*nu);}
     else
	 {lw=28*nu*nu+2*nu+max(1,2*nu);}}

             /*init : initialization*/
if (flag==4)
   {if((*(block->work)=(ricc_struct*) scicos_malloc(sizeof(ricc_struct)))==NULL)
	{set_block_error(-16);
	 return;}
    ptr=*(block->work);
    if((ptr->bwork=(int*) scicos_malloc(sizeof(int)*2*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr);
	 return;}
    if((ptr->iwork=(int*) scicos_malloc(sizeof(int)*max(2*nu,nu*nu)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
    if((ptr->dwork=(double*) scicos_malloc(sizeof(double)*lw))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
    if((ptr->LWR=(double*) scicos_malloc(sizeof(double)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
    if((ptr->LWI=(double*) scicos_malloc(sizeof(double)*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LWR);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
    if((ptr->Rcond=(double*) scicos_malloc(sizeof(double)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LWI);
	 scicos_free(ptr->LWR);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
    if((ptr->Ferr=(double*) scicos_malloc(sizeof(double)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->Rcond);
	 scicos_free(ptr->LWI);
	 scicos_free(ptr->LWR);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
    if((ptr->LX=(double*) scicos_malloc(sizeof(double)*nu*nu))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->Ferr);
	 scicos_free(ptr->Rcond);
	 scicos_free(ptr->LWI);
	 scicos_free(ptr->LWR);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->bwork);
	 scicos_free(ptr);
	 return;}
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    if((ptr->LX)!=NULL){
    	scicos_free(ptr->bwork);
    	scicos_free(ptr->Ferr);
    	scicos_free(ptr->Rcond);
    	scicos_free(ptr->iwork);
    	scicos_free(ptr->LWR);
    	scicos_free(ptr->LWI);
    	scicos_free(ptr->LX);
    	scicos_free(ptr->dwork);
    	scicos_free(ptr);
    	return;}
   }

else
   {
    ptr=*(block->work);
    if (ipar[0]==1)
       {if (ipar[1]==1)
            {C2F(riccsl)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,ptr->bwork,&info);}
         else
	    {C2F(riccms)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,&info);}}
     else
         {if (ipar[1]==1)
	     {C2F(ricdsl)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,ptr->bwork,&info);}
          else
	     {C2F(ricdmf)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,&info);}}
     if (info!=0)
	{if (flag!=6)
	    {set_block_error(-7);
             return;
	    }
	}
     for(i=0;i<nu*nu;i++) *(y+i)=*(ptr->LX+i);
    }
}
