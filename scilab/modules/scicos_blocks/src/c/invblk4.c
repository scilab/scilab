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
*/#include "scicos_block.h"
#include <stdio.h>

void invblk4(scicos_block *block,int flag)
{
  /* Copyright INRIA
     
  Scicos block simulator
  Outputs the inverse of the input */
  
  int i;
  double ww;
  if (flag == 6){
    for(i=0;i< block->insz[0];i++){
      ww=block->inptr[0][i];
      if (ww != 0.0)
	block->outptr[0][i]=1.0/ww;
    }
  }
  if (flag == 1){
    for(i=0;i< block->insz[0];i++){
      ww=block->inptr[0][i];
      if (ww != 0.0)
	block->outptr[0][i]=1.0/ww;
      else{
	set_block_error(-2);
	return;
      }
    }
  } 
}
