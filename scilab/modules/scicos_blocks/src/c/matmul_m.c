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
#include "machine.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(dmmul)();
extern int C2F(dmmul1)();
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void matmul_m(scicos_block *block,int flag)
{
  double *u1;
  double *u2;
  double *y;

  int nu,nu2,mu2;

  nu=GetInPortRows(block,1);
  nu2=GetInPortRows(block,2);
  mu2=GetInPortCols(block,2);

  u1=GetRealInPortPtrs(block,1);
  u2=GetRealInPortPtrs(block,2);
  y=GetRealOutPortPtrs(block,1);

  C2F(dmmul)(u1,&nu,u2,&nu2,y,&nu,&nu,&nu2,&mu2);
}
/*--------------------------------------------------------------------------*/ 
