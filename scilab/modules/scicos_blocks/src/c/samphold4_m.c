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
#include "memory.h"

void samphold4_m(scicos_block *block,int flag)
{
  /* c     Copyright INRIA
     
  Scicos block simulator
  returns sample and hold  of the input */
  
if ((flag ==1)) {
  void *u,*y;
  int m,n,sz;
  m=GetInPortRows(block,1);
  n=GetInPortCols(block,1);
  u=GetInPortPtrs(block,1);
  y=GetOutPortPtrs(block,1);
  sz=GetSizeOfOut(block,1);
  memcpy(y,u,m*n*sz);
  }
}
