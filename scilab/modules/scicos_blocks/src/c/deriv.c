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
#include "dynlib_scicos_blocks.h"
#include "scicos_free.h"
#include "scicos_malloc.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void deriv(scicos_block *block, int flag)
{
    double** work = (double**) block->work;
    double* rw = NULL;
    double t = 0., dt = 0.;
    int i = 0;
    if (flag == 4) /* the workspace is used to store previous values */
    {
        if ((*work = (double*) scicos_malloc(sizeof(double) * 2 * (1 + block->insz[0]))) == NULL )
        {
            set_block_error(-16);
            return;
        }
        rw = *work;
        t = get_scicos_time();
        rw[0] = t;
        rw[1] = t;
        for (i = 0; i < block->insz[0]; ++i)
        {
            rw[2 + 2 * i] = 0;
            rw[3 + 2 * i] = 0;
        }
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
            for (i = 0; i < block->insz[0]; ++i)
            {
                rw[2 + 2 * i] = rw[3 + 2 * i];
            }
        }
        rw[1] = t;
        for (i = 0; i < block->insz[0]; ++i)
        {
            rw[3 + 2 * i] = block->inptr[0][i];
        }
        dt = rw[1] - rw[0];

        if (dt != 0.0)
        {
            for (i = 0; i < block->insz[0]; ++i)
            {
                block->outptr[0][i] = (rw[3 + 2 * i] - rw[2 + 2 * i]) / dt;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
