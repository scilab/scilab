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
#include "machine.h"
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
#include "scicos_free.h"
#include "scicos_malloc.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__,
                      int *nc, int *l, int *m, int *n);
extern int C2F(dmmul1)(double *a, int *na, double *b, int *nb, double *c__,
                       int *nc, int *l, int *m, int *n);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void dsslti4(scicos_block *block, int flag)
{
    /* Copyright INRIA

       Scicos block simulator
       discrete state space linear system simulator
       rpar(1:nx*nx)=A
       rpar(nx*nx+1:nx*nx+nx*nu)=B
       rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C */

    int un = 1, lb = 0, lc = 0, ld = 0;
    int nz = block->nz;
    int nout = block->nout;
    int nin = block->nin;
    double* z = NULL;
    double* rpar = block->rpar;
    double* y = NULL;
    double* u = NULL;
    int empty[2] = {0, 0};
    int* outsz = empty;
    int* insz = empty;
    double *w = NULL;

    if (nout > 0)
    {
        outsz = block->outsz;
        y = (double*) block->outptr[0];
    }
    if (nin > 0)
    {
        insz = block->insz;
        u = (double *) block->inptr[0];
    }
    if (nz > 0)
    {
        z = block->z;
    }

    lb = nz * nz;

    if (flag == 1 || flag == 6)
    {
        /* y=c*x+d*u */
        if (nout > 0)
        {
            lc = lb + nz * insz[0];
            ld = lc + nz * outsz[0];
            if (nz == 0)
            {
                if (nin > 0)
                {
                    C2F(dmmul)(&rpar[ld], outsz, u, insz, y, outsz, outsz, insz, &un);
                }
            }
            else
            {
                C2F(dmmul)(&rpar[lc], outsz, z, &nz, y, outsz, outsz, &nz, &un);
                if (nin > 0)
                {
                    C2F(dmmul1)(&rpar[ld], outsz, u, insz, y, outsz, outsz, insz, &un);
                }
            }
        }
    }
    else if (flag == 2)
    {
        /* x+=a*x+b*u */
        if (nz > 0)
        {
            w = (double*) *block->work;
            memcpy(w, z, nz * sizeof(double));
            C2F(dmmul)(&rpar[0], &nz, w, &nz, z, &nz, &nz, &nz, &un);
            if (nin > 0)
            {
                C2F(dmmul1)(&rpar[lb], &nz, u, insz, z, &nz, &nz, insz, &un);
            }
        }
    }
    else if (flag == 4)
    {
        /* the workspace for temp storage */
        if (nz > 0)
        {
            *block->work = scicos_malloc(sizeof(double) * nz);
            if (*block->work == NULL)
            {
                set_block_error(-16);
                return;
            }
        }
    }
    else if (flag == 5)
    {
        if (nz > 0)
        {
            scicos_free(*block->work);
        }
    }
}
/*--------------------------------------------------------------------------*/

