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
#include "scicos_block.h"

extern int C2F(dmmul)();
extern int C2F(dmmul1)();

void csslti4(scicos_block *block,int flag)
{
  /*  Copyright INRIA

      Scicos block simulator
      continuous state space linear system simulator
      rpar(1:nx*nx)=A
      rpar(nx*nx+1:nx*nx+nx*nu)=B
      rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
      rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D */
  
  int un=1,lb,lc,ld;
  int nx=block->nx;
  double* x=block->x;
  double* xd=block->xd;
  double* rpar=block->rpar;
  double* y=block->outptr[0];
  double* u=block->inptr[0];
  int* outsz=block->outsz;
  int* insz=block->insz;
  
  lb=nx*nx;
  lc=lb+nx*insz[0];
  
  if (flag ==1 || flag ==6){
    /* y=c*x+d*u     */
    ld=lc+nx*outsz[0];
    if (nx==0) {
      C2F(dmmul)(&rpar[ld],outsz,u,insz,y,outsz,outsz,insz,&un);
    }else{
      C2F(dmmul)(&rpar[lc],outsz,x,&nx,y,outsz,outsz,&nx,&un);
      C2F(dmmul1)(&rpar[ld],outsz,u,insz,y,outsz,outsz,insz,&un);
    }
  }

  else if (flag ==0){
    /* xd=a*x+b*u */
    C2F(dmmul)(&rpar[0],&nx,x,&nx,xd,&nx,&nx,&nx,&un);
    C2F(dmmul1)(&rpar[lb],&nx,u,insz,xd,&nx,&nx,insz,&un);
  }
}



