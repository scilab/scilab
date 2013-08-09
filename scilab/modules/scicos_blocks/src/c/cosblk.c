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
#include "scicos.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
/*------------------------------------------------
 *     Scicos block simulator
 *     A set of elementary blocks
 *------------------------------------------------*/

/*------------------------------------------------
 *     Scicos block simulator
 *
 *------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void cosblk(int *flag, int *nevprt, double *t,
                                 double *xd, double *x, int *nx, double *z,
                                 int *nz, double *tvec, int *ntvec,
                                 double *rpar, int *nrpar, int *ipar,
                                 int *nipar, double *u, int *nu, double *y,
                                 int *ny)
{
    int i  = 0;
    for (i = 0; i < *nu ; i++)
    {
        y[i] = cos(u[i]);
    }
}
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void C2F(cosblk)(int *flag, int *nevprt, double *t,
                                      double *xd, double *x, int *nx, double *z,
                                      int *nz, double *tvec, int *ntvec,
                                      double *rpar, int *nrpar, int *ipar,
                                      int *nipar, double *u, int *nu, double *y,
                                      int *ny)
{
    cosblk(flag, nevprt, t, xd, x, nx, z, nz, tvec,
           ntvec, rpar, nrpar, ipar, nipar, u, nu, y, ny);
}
/*--------------------------------------------------------------------------*/
