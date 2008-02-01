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
#include <math.h>
void matz_abs(scicos_block *block,int flag)
{
  double *u1r;
  double *u1i;
  double *y1,*y2;

  int nu,mu,i;

  mu=GetOutPortRows(block,1);
  nu=GetOutPortCols(block,1);
  u1r=GetRealInPortPtrs(block,1);
  u1i=GetImagInPortPtrs(block,1);
  y1=GetRealOutPortPtrs(block,1);
  y2=GetRealOutPortPtrs(block,2);
  for (i=0;i<mu*nu;i++) 
	{*(y1+i)=pow((pow(*(u1r+i),2)+pow(*(u1i+i),2)),0.5);
	 *(y2+i)=atan2(*(u1i+i),*(u1r+i));}
}

