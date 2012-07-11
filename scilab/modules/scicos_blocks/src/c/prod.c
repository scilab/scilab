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
#include "dynlib_scicos_blocks.h"

SCICOS_BLOCKS_IMPEXP void prod(int *flag, int *nevprt,
                               double *t, double xd[],
                               double x[], int *nx,
                               double z[], int *nz,
                               double tvec[], int *ntvec,
                               double rpar[], int *nrpar,
                               int ipar[], int *nipar,
                               double *inptr[], int insz[], int *nin,
                               double *outptr[], int outsz[], int *nout)
{
    int n = outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */
    double *y = (double *)outptr[0];
    int i = 0;

    for (i = 0; i < n; i++)
    {
        int k = 0;

        y[i] = 1.0;

        for (k = 0; k < *nin; k++)
        {
            double *u = (double *)inptr[k];
            y[i] = y[i] * u[i];
        }
    }
}
/*--------------------------------------------------------------------------*/
