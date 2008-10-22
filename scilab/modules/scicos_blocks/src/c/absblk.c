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

#include "core_math.h"

#include "scicos.h"
#include "elementary_functions.h" /* C2F(dmmul) */
/*------------------------------------------------
 *     Scicos block simulator 
 *     A set of elementary blocks 
 *------------------------------------------------*/
#ifdef Abs
	#undef Abs
#endif
#define Abs(x) ( (x) > 0) ? (x) : -(x)

/*------------------------------------------------
 *     Scicos block simulator 
 *     returns Absolute value of the input 
 *------------------------------------------------*/


void absblk( flag, nevprt, t, xd, x, nx, z, nz, tvec, 
	     ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny)
             int *flag, *nevprt,*nx,*nz,*nrpar, *ipar, *nipar,*ntvec,*nu,*ny;
             double *t, *xd, *x, *z, *tvec, *rpar, *u, *y;
{
  int i;
  for (i = 0 ; i <  *nu ; ++i ) y[i] = Abs(u[i]);
} 

