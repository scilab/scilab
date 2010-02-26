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
#include <stdio.h>
#include "machine.h" /* C2F */
#include "MALLOC.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(dgesvd)();
extern int C2F(dlaset)();
extern int C2F(dlacpy)();
/*--------------------------------------------------------------------------*/ 
typedef struct
{	  
	double *LA;
	double *LU;
	double *LVT;
	double *dwork;
} mat_sing_struct ;
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void mat_sing(scicos_block *block,int flag)
{
 double *u = NULL;
 double *y = NULL;
 int nu = 0, mu = 0;
 int info = 0;
 int lwork = 0;
 mat_sing_struct *ptr = NULL;

 mu = GetInPortRows(block,1);
 nu = GetInPortCols(block,1);
 u = GetRealInPortPtrs(block,1);
 y = GetRealOutPortPtrs(block,1);
 /* for lapack 3.1 (2006)*/
 lwork=max(3*min(mu,nu)+max(mu,nu),5*min(mu,nu));
 lwork=max(1,lwork); 
             /*init : initialization*/
if (flag==4)
   {if((*(block->work)=(mat_sing_struct*) scicos_malloc(sizeof(mat_sing_struct)))==NULL)
	{set_block_error(-16);
	 return;}
    ptr=*(block->work);
    if((ptr->LA=(double*) scicos_malloc(sizeof(double)*(mu*nu)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr);
	 return;}
    if((ptr->LU=(double*) scicos_malloc(sizeof(double)*(mu*mu)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LA);
	 scicos_free(ptr);
	 return;}
    if((ptr->LVT=(double*) scicos_malloc(sizeof(double)*(nu*nu)))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LU);
	 scicos_free(ptr->LA);
	 scicos_free(ptr);
	 return;}
    if((ptr->dwork=(double*) scicos_malloc(sizeof(double)*lwork))==NULL)
	{set_block_error(-16);
	 scicos_free(ptr->LVT);
	 scicos_free(ptr->LU);
	 scicos_free(ptr->LA);
	 scicos_free(ptr);
	 return;}
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    if(ptr->dwork!=0) {
    	scicos_free(ptr->LA);
    	scicos_free(ptr->LU);
    	scicos_free(ptr->LVT);
    	scicos_free(ptr->dwork);
    	scicos_free(ptr);
    	return;}
   }

else
   {
    ptr=*(block->work);
    C2F(dlacpy)("F",&mu,&nu,u,&mu,ptr->LA,&mu);
    C2F(dgesvd)("A","A",&mu,&nu,ptr->LA,&mu,y,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
         return;}}
   }
}
/*--------------------------------------------------------------------------*/ 
