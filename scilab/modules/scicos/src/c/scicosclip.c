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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <math.h>
#include "machine.h" /* C2F */
#include "math_graphics.h" 
#include "PloEch.h"
/*--------------------------------------------------------------------------*/
void C2F(scicosclip)(int *n)
{
  if(*n==1){
	  /* disabled */
	  /* TO DO : new graphics version */
    /*C2F(dr1)("xset","clipgrf",PI0,PI0,PI0,PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
  }
  else{
	  /* disabled */
	  /* TO DO : new graphics version */
    /*C2F(dr1)("xset","clipoff",PI0,PI0,PI0,PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
  }
}
/*--------------------------------------------------------------------------*/
