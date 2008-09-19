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
#include "scicos_block4.h"

extern int C2F(wmmul)();

void matzmul_m(scicos_block *block,int flag)
{
  double *u1r;
  double *u2r;
  double *u1i;
  double *u2i;
  double *yr;
  double *yi;
  int mu1,mu2,nu;

  mu1=GetInPortRows(block,1);
  mu2=GetInPortRows(block,2);
  nu=GetInPortCols(block,2);

  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  u2r=GetRealInPortPtrs(block,2);
  u2i=GetImagInPortPtrs(block,2);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);

  C2F(wmmul)(u1r,u1i,&mu1,u2r,u2i,&mu2,yr,yi,&mu1,&mu1,&mu2,&nu);
}
