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
extern int C2F(wprxc)();
void rootz_coef(scicos_block *block,int flag)
{
  double *ur,*ui;
  double *yr,*yi;
  int mu;
  mu=GetInPortRows(block,1);

  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);
  if (flag==1||flag==6)
  C2F(wprxc)(&mu,ur,ui,yr,yi);
}
