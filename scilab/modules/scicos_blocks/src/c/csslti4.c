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
#include "scicos_block.h"
#include "machine.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__,
                      int *nc, int *l, int *m, int *n);
extern int C2F(dmmul1)(double *a, int *na, double *b, int *nb, double *c__,
                       int *nc, int *l, int *m, int *n);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void csslti4(scicos_block *block, int flag)
{
    /*  Copyright INRIA

        Scicos block simulator
        continuous state space linear system simulator
        rpar(1:nx*nx)=A
        rpar(nx*nx+1:nx*nx+nx*nu)=B
        rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
        rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D */

    int un = 1, lb = 0, lc = 0, ld = 0;
    int nx = block->nx;
    int nout = block->nout;
    int nin = block->nin;
    double* x = NULL;
    double* xd = NULL;
    double* rpar = block->rpar;
    double* y = NULL;
    double* u = NULL;
    int empty[2] = {0, 0};
    int* outsz = empty;
    int* insz = empty;

    if (nout > 0)
    {
        outsz = block->outsz;
        y = (double*) (block->outptr[0]);
    }
    if (nin > 0)
    {
        insz = block->insz;
        u = (double*) (block->inptr[0]);
    }
    if (nx > 0)
    {
        x = block->x;
        xd = block->xd;
    }

    lb = nx * nx;
    lc = lb + nx * insz[0];

    if (flag == 1 || flag == 6)
    {
        /* y=c*x+d*u     */

        if (nout > 0)
        {
            ld = lc + nx * outsz[0];
            if (nx == 0)
            {
                if (nin > 0)
                {
                    C2F(dmmul)(&rpar[ld], outsz, u, insz, y, outsz, outsz, insz, &un);
                }
            }
            else
            {
                C2F(dmmul)(&rpar[lc], outsz, x, &nx, y, outsz, outsz, &nx, &un);
                if (nin > 0)
                {
                    C2F(dmmul1)(&rpar[ld], outsz, u, insz, y, outsz, outsz, insz, &un);
                }
            }
        }
    }

    else if (flag == 0)
    {
        /* xd=a*x+b*u */

        if (nx > 0)
        {
            C2F(dmmul)(&rpar[0], &nx, x, &nx, xd, &nx, &nx, &nx, &un);
            if (nin > 0)
            {
                C2F(dmmul1)(&rpar[lb], &nx, u, insz, xd, &nx, &nx, insz, &un);
            }
        }
    }
}
/*--------------------------------------------------------------------------*/


