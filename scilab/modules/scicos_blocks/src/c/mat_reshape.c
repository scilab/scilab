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

void mat_reshape(scicos_block *block,int flag)
{
  double *u1;
  double *y;

  int i,xu,vu;

  xu=GetOutPortRows(block,1);
  vu=GetOutPortCols(block,1);
  u1=GetRealInPortPtrs(block,1);
  y=GetRealOutPortPtrs(block,1);

  for (i=0;i<xu*vu;i++) *(y+i)=*(u1+i);
}
