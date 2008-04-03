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
# include "scicos_block4.h"
# include "machine.h"
#include <stdio.h>

#if _MSC_VER
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
