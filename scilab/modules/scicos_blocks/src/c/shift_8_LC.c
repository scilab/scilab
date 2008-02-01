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
# include "scicos_block4.h"
# include "machine.h"
# include <math.h>

void shift_8_LC(scicos_block *block,int flag)
{
  char *u,*y,v; 
  int *ipar;
  int mu,nu,i,j;
  unsigned char k;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=Getint8InPortPtrs(block,1);
  y=Getint8OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  k=(unsigned char)pow(2,8-1);
  for (i=0;i<mu*nu;i++)
      { v=u[i];
	for(j=0;j<ipar[0];j++)
	     {
	       y[i]=v&k;
               if (y[i]==0)  y[i]=v<<1;
	       else {
	             y[i]=v<<1;
	             y[i]=(y[i])|(1);
	            }
	       v=y[i];
	      }
       }
}
