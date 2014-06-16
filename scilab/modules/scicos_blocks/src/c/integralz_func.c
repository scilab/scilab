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
#include "scicos.h"
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void  integralz_func(scicos_block *block, int flag)
{
    int i = 0;
    double *ur = NULL, *ui = NULL;
    double *yr = NULL, *yi = NULL;
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    if (flag == 0)
    {
        if (block->ng > 0)
        {
            for (i = 0; i < (block->nx) / 2; ++i)
            {
                if (block->mode[i] == 3)
                {
                    block->xd[i] = ur[i];
                    block->xd[i + (block->nx) / 2] = ui[i];
                }
                else
                {
                    block->xd[i] = 0.0;
                    block->xd[i + (block->nx) / 2] = 0.0;
                }
            }
        }
        else
        {
            for (i = 0; i < (block->nx) / 2; ++i)
            {
                block->xd[i] = ur[i];
                block->xd[i + (block->nx) / 2] = ui[i];
            }
        }
    }
    else if (flag == 1 || flag == 6)
    {
        for (i = 0; i < (block->nx) / 2; ++i)
        {
            yr[i] = block->x[i];
            yi[i] = block->x[i + (block->nx) / 2];
        }
    }
    else if (flag == 2 && block->nevprt == 1)
    {
        for (i = 0; i < (block->nx) / 2; ++i)
        {
            block->x[i] = ur[i];
            block->x[i + (block->nx) / 2] = ui[i];
        }
    }
    else if (flag == 9)
    {
        for (i = 0; i < (block->nx) / 2; ++i)
        {
            if (block->mode[i] == 3)
            {
                block->g[i] = (block->x[i] - (block->rpar[i])) * (block->x[i] - (block->rpar[(block->nx) / 2 + i]));
                block->g[i + (block->nx) / 2] = (block->x[i + (block->nx) / 2] - (block->rpar[i + (block->nx)])) * (block->x[i + (block->nx) / 2] - (block->rpar[3 * (block->nx) / 2 + i]));
            }
            else
            {
                block->g[i] = ur[i];
                block->g[i + (block->nx) / 2] = ui[i];
            }
            if (get_phase_simulation() == 1)
            {
                if (ur[i] >= 0 && block->x[i] >= block->rpar[i] && ui[i >= 0] && block->x[i + (block->nx) / 2] >= block->rpar[i + (block->nx)])
                {
                    block->mode[i] = 1;
                }
                else if (ur[i] <= 0 && block->x[i] <= block->rpar[(block->nx) / 2 + i] && ui[i] <= 0 && block->x[i + (block->nx) / 2] <= block->rpar[3 * (block->nx) / 2 + i])
                {
                    block->mode[i] = 2;
                }
                else
                {
                    block->mode[i] = 3;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
