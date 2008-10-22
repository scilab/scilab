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
#include <memory.h>
#include "scicos_block4.h"
#include<stdio.h>

void cstblk4_m(scicos_block *block,int flag)
{ 
  /* Copyright INRIA

  Scicos block simulator
  output a vector of constants out(i)=opar(i)
  opar(1:nopar) : given constants */
  int nopar,mo,no,so;
  void *y;
  void *opar;
  nopar = GetNopar(block);
  y=GetOutPortPtrs(block,1);
  opar=GetOparPtrs(block,1);
  mo=GetOparSize(block,1,1);
  no=GetOparSize(block,1,2);
  so=GetSizeOfOpar(block,1);
  memcpy(y,opar,mo*no*so);
}
