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
#include "machine.h"

void extractz(scicos_block *block,int flag)
{
  double *ur;
  double *ui;
  double *yr;
  double *yi;
  int *r;
  int nu,mu,nr,i,j,ij,k,nc,nl;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  nr=GetNipar(block);
  r=GetIparPtrs(block);
  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  nc=r[nr-1];
  nl=r[nr-2];
  k=0;
  for (j=0;j<nc;j++)
	{for (i=0;i<nl;i++)
		{ij=r[i]-1+(r[nl+j]-1)*mu;
		 *(yr+k)=*(ur+ij);
		 *(yi+k)=*(ui+ij);
		 k++;}}
}
