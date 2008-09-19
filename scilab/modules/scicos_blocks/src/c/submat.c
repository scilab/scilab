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

void submat(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int *r;
  int nu,mu,i,j,ij,k;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  r=GetIparPtrs(block);
  u=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);
  k=0;
  for (j=r[2]-1;j<r[3];j++)
	{for (i=r[0]-1;i<r[1];i++)
		{ij=i+j*mu;
		 *(y+k)=*(u+ij);
		 k++;}}
}
