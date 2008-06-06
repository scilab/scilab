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
#include <stdio.h>
#include <math.h>

#include "machine.h"
#include "core_math.h"
#include "elementary_functions.h" /* C2F(dmmul) */

#include "scicos.h"

/*------------------------------------------------
 *     Scicos block simulator 
 *     A set of elementary blocks 
 *------------------------------------------------*/

/*------------------------------------------------
 *     Scicos block simulator 
 *     input to output Gain
 *     rpar=gain matrix
 *------------------------------------------------*/

void gain(flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	  ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
          integer *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
          double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  integer un = 1;
  C2F(dmmul)(rpar,ny,u,nu,y,ny,ny,nu,&un);
}
