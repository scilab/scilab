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

void matz_reimc(scicos_block *block,int flag)
{
  double *u1;
  double *u2;
  double *y1r, *y1i;


  int nu,mu,i;

  mu=GetOutPortRows(block,1);
  nu=GetOutPortCols(block,1);
  u1=GetRealInPortPtrs(block,1);
  u2=GetRealInPortPtrs(block,2);
  y1r=GetRealOutPortPtrs(block,1);
  y1i=GetImagOutPortPtrs(block,1);
  
  for (i=0;i<mu*nu;i++) 
	{*(y1r+i)=*(u1+i);
	 *(y1i+i)=*(u2+i);}
}

