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
#include <stdio.h>
#include <math.h>
#include "dynlib_scicos_blocks.h"
#include "core_math.h"
#include "scicos.h"
/*--------------------------------------------------------------------------*/
/*------------------------------------------------
 *     Scicos block simulator
 *     A set of elementary blocks
 *------------------------------------------------*/

/*------------------------------------------------
 *     Scicos block simulator
 *     Logical and block
 *     if event input exists synchronuously, output is 1 else -1
 *------------------------------------------------*/

SCICOS_BLOCKS_IMPEXP void andlog(int *flag, int *nevprt, double *t, double *xd,
                                 double *x, int *nx, double *z, int *nz,
                                 double *tvec, int *ntvec, double *rpar,
                                 int *nrpar, int  *ipar, int  *nipar, double *u,
                                 int *nu, double *y, int *ny)
{
    if ( *flag == 1)
    {
        y[0] = ( *nevprt != 3 ) ? -1.00 :  1.00;
    }
}
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void C2F(andlog)(int *flag, int *nevprt, double *t, double *xd,
                                      double *x, int *nx, double *z, int *nz,
                                      double *tvec, int *ntvec, double *rpar,
                                      int *nrpar, int  *ipar, int  *nipar, double *u,
                                      int *nu, double *y, int *ny)
{
    andlog(flag, nevprt, t, xd, x, nx, z, nz, tvec, ntvec, rpar, nrpar, ipar,
           nipar, u, nu, y, ny);
}
/*--------------------------------------------------------------------------*/


