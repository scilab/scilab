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
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
#include "machine.h" /* C2F */
#include "MALLOC.h"
#include "scicos.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(riccsl)();
extern int C2F(riccms)();
extern int C2F(ricdsl)();
extern int C2F(ricdmf)();
/*--------------------------------------------------------------------------*/ 
typedef struct
{         
	int *bwork;
	int *iwork;
	double *dwork;
	double *LX;
	double *LWI;
	double *LWR;
	double *Rcond;
	double *Ferr;
} ricc_struct ;
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void ricc_m(scicos_block *block,int flag)
{
	double *u1 = NULL;
	double *u2 = NULL;
	double *u3 = NULL;
	double *y = NULL;
	int *ipar = NULL;
	int nu = 0;
	int info = 0,i = 0;
	int lw = 0;
	ricc_struct *ptr = NULL;

	nu = GetInPortCols(block,1);
	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);
	u3 = GetRealInPortPtrs(block,3);
	y = GetRealOutPortPtrs(block,1);
	ipar = GetIparPtrs(block);

	if (ipar[0]==1)
	{
		if (ipar[1]==1)
		{
			lw=9*nu*nu+4*nu+max(1,6*nu);
		}
		else
		{
			lw=9*nu*nu+7*nu+1;
		}
	}
	else
	{
		if (ipar[1]==1)
		{
			lw=12*nu*nu+22*nu+max(21,4*nu);
		}
		else
		{
			lw=28*nu*nu+2*nu+max(1,2*nu);
		}
	}

	/*init : initialization*/
	if (flag==4)
	{
		if((*(block->work)=(ricc_struct*) scicos_malloc(sizeof(ricc_struct)))==NULL)
		{
			set_block_error(-16);
			return;
		}
		ptr=*(block->work);
		if((ptr->bwork=(int*) scicos_malloc(sizeof(int)*2*nu))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr);
			return;
		}
		if((ptr->iwork=(int*) scicos_malloc(sizeof(int)*max(2*nu,nu*nu)))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
		if((ptr->dwork=(double*) scicos_malloc(sizeof(double)*lw))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->iwork);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
		if((ptr->LWR=(double*) scicos_malloc(sizeof(double)*nu))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->dwork);
			scicos_free(ptr->iwork);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
		if((ptr->LWI=(double*) scicos_malloc(sizeof(double)*nu))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->LWR);
			scicos_free(ptr->dwork);
			scicos_free(ptr->iwork);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
		if((ptr->Rcond=(double*) scicos_malloc(sizeof(double)))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->LWI);
			scicos_free(ptr->LWR);
			scicos_free(ptr->dwork);
			scicos_free(ptr->iwork);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
		if((ptr->Ferr=(double*) scicos_malloc(sizeof(double)))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->Rcond);
			scicos_free(ptr->LWI);
			scicos_free(ptr->LWR);
			scicos_free(ptr->dwork);
			scicos_free(ptr->iwork);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
		if((ptr->LX=(double*) scicos_malloc(sizeof(double)*nu*nu))==NULL)
		{
			set_block_error(-16);
			scicos_free(ptr->Ferr);
			scicos_free(ptr->Rcond);
			scicos_free(ptr->LWI);
			scicos_free(ptr->LWR);
			scicos_free(ptr->dwork);
			scicos_free(ptr->iwork);
			scicos_free(ptr->bwork);
			scicos_free(ptr);
			return;
		}
	}

	/* Terminaison */
	else if (flag==5)
	{
		ptr=*(block->work);
		if((ptr->LX)!=NULL)
		{
			scicos_free(ptr->bwork);
			scicos_free(ptr->Ferr);
			scicos_free(ptr->Rcond);
			scicos_free(ptr->iwork);
			scicos_free(ptr->LWR);
			scicos_free(ptr->LWI);
			scicos_free(ptr->LX);
			scicos_free(ptr->dwork);
			scicos_free(ptr);
			return;
		}
	}

	else
	{
		ptr=*(block->work);
		if (ipar[0]==1)
		{
			if (ipar[1]==1)
			{
				C2F(riccsl)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,ptr->bwork,&info);
			}
			else
			{
				C2F(riccms)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,&info);
			}
		}
		else
		{
			if (ipar[1]==1)
			{
				C2F(ricdsl)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,ptr->bwork,&info);
			}
			else
			{
				C2F(ricdmf)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,&info);
			}
		}
		if (info!=0)
		{
			if (flag!=6)
			{
				set_block_error(-7);
				return;
			}
		}
		for(i=0;i<nu*nu;i++) *(y+i)=*(ptr->LX+i);
	}
}
/*--------------------------------------------------------------------------*/ 
