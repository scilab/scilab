/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <math.h>
#include "machine.h" /* C2F */
#include "MALLOC.h"
#include "scicos.h"
#include "core_math.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(dgetrf)();
extern double C2F(dlamch)();
extern double C2F(dlange)();
extern int C2F(dlacpy)();
extern int C2F(dgecon)();
extern int C2F(dgetrs)();
extern int C2F(dgelsy1)();
/*--------------------------------------------------------------------------*/ 
typedef struct
{
	int *ipiv;
	int *rank;
	int *jpvt;
	int *iwork;
	double *dwork;
	double *LAF;
	double *LA;
	double *LXB;
} mat_bksl_struct ;
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void mat_bksl(scicos_block *block,int flag)
{
 double *u1 = NULL;
 double *u2 = NULL;
 double *y = NULL;
 int mu = 0;
 int nu1 = 0;
 int nu2 = 0;
 int info = 0;
 int i = 0,l = 0,lw = 0,lu = 0;
 mat_bksl_struct *ptr = NULL;
 double rcond = 0., ANORM = 0., EPS = 0.;

 mu =GetInPortRows(block,1);
 nu1 =GetInPortCols(block,1);
 nu2 =GetInPortCols(block,2);
 u1=GetRealInPortPtrs(block,1);
 u2=GetRealInPortPtrs(block,2);
 y=GetRealOutPortPtrs(block,1);
 l=Max(mu,nu1);
 lu=Max(4*nu1,Min(mu,nu1)+3*nu1+1);
 lw=Max(lu,2*Min(mu,nu1)+nu2);
             /*init : initialization*/
if (flag==4)
   {if((*(block->work)=(mat_bksl_struct*) scicos_malloc(sizeof(mat_bksl_struct)))==NULL)
	{set_block_error(-16);
	 return;}
    ptr=*(block->work);
    if((ptr->ipiv=(int*) scicos_malloc(sizeof(int)*nu1))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr);
	 return;}
    if((ptr->rank=(int*) scicos_malloc(sizeof(int)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->jpvt=(int*) scicos_malloc(sizeof(int)*nu1))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->iwork=(int*) scicos_malloc(sizeof(int)*nu1))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->dwork=(double*) scicos_malloc(sizeof(double)*lw))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LAF=(double*) scicos_malloc(sizeof(double)*(mu*nu1)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LA=(double*) scicos_malloc(sizeof(double)*(mu*nu1)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LAF);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    if((ptr->LXB=(double*) scicos_malloc(sizeof(double)*(l*nu2)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LA);
	 scicos_free(ptr->LAF);
	 scicos_free(ptr->dwork);
	 scicos_free(ptr->iwork);
	 scicos_free(ptr->jpvt);
	 scicos_free(ptr->rank);
	 scicos_free(ptr->ipiv);
	 scicos_free(ptr);
	 return;}
    
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    if(ptr->LXB!=NULL){
    	scicos_free(ptr->ipiv);
    	scicos_free(ptr->rank);
    	scicos_free(ptr->jpvt);
    	scicos_free(ptr->iwork);
    	scicos_free(ptr->LAF);
    	scicos_free(ptr->LA);
    	scicos_free(ptr->LXB);
    	scicos_free(ptr->dwork);
    	scicos_free(ptr);
    	return;}
   }

else
   {
    ptr=*(block->work);
    EPS=C2F(dlamch)("e",1L);
    ANORM=C2F(dlange)("1",&mu,&nu1,u1,&mu,ptr->dwork);
    C2F(dlacpy)("F",&mu,&nu1,u1,&mu,ptr->LA,&mu);
    if (mu==nu1)
	{C2F(dlacpy)("F",&mu,&nu1,ptr->LA,&mu,ptr->LAF,&mu);
	 C2F(dgetrf)(&nu1,&nu1,ptr->LAF,&nu1,ptr->ipiv,&info);
	 rcond=0;
 	 if (info==0)
	    {C2F(dgecon)("1",&nu1,ptr->LAF,&nu1,&ANORM,&rcond,ptr->dwork,ptr->iwork,&info);
	     if (rcond>pow(EPS,0.5))
		{C2F(dlacpy)("F",&nu1,&nu2,u2,&nu1,ptr->LXB,&nu1);
		 C2F(dgetrs)("N",&nu1,&nu2,ptr->LAF,&nu1,ptr->ipiv,ptr->LXB,&nu1,&info);
		 C2F(dlacpy)("F",&nu1,&nu2,ptr->LXB,&nu1,y,&nu1);
		 return;
		}
	    }
	}
    rcond=pow(EPS,0.5);
    C2F(dlacpy)("F",&mu,&nu2,u2,&mu,ptr->LXB,&l);
    for (i=0;i<nu1;i++)    *(ptr->jpvt+i)=0;
    
    C2F(dgelsy1)(&mu,&nu1,&nu2,ptr->LA,&mu,ptr->LXB,&l,ptr->jpvt,&rcond,ptr->rank,ptr->dwork,&lw,&info);
    if (info!=0)
	{if (flag!=6)
	    {set_block_error(-7);
             return;
	    }
	}
    C2F(dlacpy)("F",&nu1,&nu2,ptr->LXB,&l,y,&nu1);
    }
}
/*--------------------------------------------------------------------------*/ 
