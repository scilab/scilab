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
#include "sci_malloc.h"
#include "scicos_block.h"
#include "scicos_free.h"
#include "scicos_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void backlash(scicos_block *block, int flag)
{
    double* rw = NULL, t  = 0.;
    double** work = (double**) block->work;

    if (flag == 4) /* the workspace is used to store previous values */
    {
        if ((*work = (double*) scicos_malloc(sizeof(double) * 4)) == NULL )
        {
            set_block_error(-16);
            return;
        }
        rw = *work;
        t = get_scicos_time();
        rw[0] = t;
        rw[1] = t;
        rw[2] = block->rpar[0];
        rw[3] = block->rpar[0];
    }
    else  if (flag == 5)
    {
        scicos_free(*work);
    }
    else  if (flag == 1)
    {
        rw = *work;
        t = get_scicos_time();
        if (t > rw[1])
        {
            rw[0] = rw[1];
            rw[2] = rw[3];
        }
        rw[1] = t;
        if (block->inptr[0][0] > rw[2] + block->rpar[1] / 2)
        {
            rw[3] = block->inptr[0][0] - block->rpar[1] / 2;
        }
        else if (block->inptr[0][0] < rw[2] - block->rpar[1] / 2)
        {
            rw[3] = block->inptr[0][0] + block->rpar[1] / 2;
        }
        else
        {
            rw[3] = rw[2];
        }
        block->outptr[0][0] = rw[3];
    }
    else if (flag == 9)
    {
        rw = *work;
        t = get_scicos_time();
        if (t > rw[1])
        {
            block->g[0] = block->inptr[0][0] - block->rpar[1] / 2 - rw[3];
            block->g[1] = block->inptr[0][0] + block->rpar[1] / 2 - rw[3];
        }
        else
        {
            block->g[0] = block->inptr[0][0] - block->rpar[1] / 2 - rw[2];
            block->g[1] = block->inptr[0][0] + block->rpar[1] / 2 - rw[2];
        }
    }
}
/*--------------------------------------------------------------------------*/
