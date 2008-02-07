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
#include <math.h>
#include "machine.h"
extern int C2F(wsqrt)();

void matz_sqrt(scicos_block *block,int flag)
{
 if (flag==1){
  double *ui,*ur;
  double *yi,*yr;
  double inpi,inpr;
  int nu,mu,i;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);

  ur=GetRealInPortPtrs(block,1);
  ui=GetImagInPortPtrs(block,1);
  yr=GetRealOutPortPtrs(block,1);
  yi=GetImagOutPortPtrs(block,1);

  for(i=0;i<mu*nu;i++)  
	{inpr=ur[i];
	 inpi=ui[i];
	 C2F(wsqrt)(&inpr,&inpi,&yr[i],&yi[i]);}
 }
}
