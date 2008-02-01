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
#include "machine.h"
#include "scicos_block4.h"

extern int C2F(dmmul)();
extern int C2F(dmmul1)();


void gainblk(scicos_block *block,int flag)
{
  int i;

  double *u; double *y;
  int nu,ny,my;
  double *rpar;
  int nrpar;

  nu=GetInPortRows(block,1);
  ny=GetOutPortRows(block,1);
  my=GetOutPortCols(block,1);

  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  nrpar=GetNrpar(block);

  rpar=GetRparPtrs(block);

  if (nrpar==1){
    for (i=0;i<nu*my;++i){
     y[i]=rpar[0]*u[i];
    }
  }else{
    C2F(dmmul)(rpar,&ny,u,&nu,y,&ny,&ny,&nu,&my);
  }
}
