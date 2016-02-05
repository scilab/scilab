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
#include <string.h>
#include "sci_malloc.h"
#include "machine.h" /* C2F */
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__,
                      int *nc, int *l, int *m, int *n);
extern int C2F(dmmul1)(double *a, int *na, double *b, int *nb, double *c__,
                       int *nc, int *l, int *m, int *n);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void tcsltj4(scicos_block *block, int flag)
{
    /* Copyright INRIA

    Scicos block simulator
    continuous state space linear system simulator
    rpar(1:nx*nx)=A
    rpar(nx*nx+1:nx*nx+nx*nu)=B
    rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
    rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D */

    int un = 1, lb, lc;
    int nx = block->nx;
    double* x = block->x;
    double* xd = block->xd;
    double* rpar = block->rpar;
    double* y = block->outptr[0];
    double* u1 = block->inptr[0];
    double* u2 = block->inptr[1];
    int* outsz = block->outsz;
    int* insz = block->insz;

    lb = nx * nx;
    lc = lb + nx * insz[0];

    if (flag == 1 || flag == 6)
    {
        /* y=c*x*/
        C2F(dmmul)(&rpar[lc], outsz, x, &nx, y, outsz, outsz, &nx, &un);
    }
    else if (flag == 2 && block->nevprt == 1)
    {
        /* x+=u2 */
        memcpy(x, u2 , nx * sizeof(double));
    }
    else if (flag == 0 && block->nevprt == 0)
    {
        /* xd=a*x+b*u1*/
        C2F(dmmul)(&rpar[0], &nx, x, &nx, xd, &nx, &nx, &nx, &un);
        C2F(dmmul1)(&rpar[lb], &nx, u1, &insz[0], xd, &nx, &nx, &insz[0], &un);
    }
}
/*--------------------------------------------------------------------------*/
