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
#include <stdlib.h>
#include "core_math.h"
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void minmax(scicos_block *block, int flag)
{
    /*ipar[0]=1 -> min,  ipar[0]=2 -> max */

    int i = 0;
    double maxmin = 0.;
    int phase = get_phase_simulation();

    if (flag == 1)
    {
        if (block->nin == 1)
        {
            if ((block->ng == 0) || (phase == 1))
            {
                maxmin = block->inptr[0][0];
                for (i = 1; i < block->insz[0]; ++i)
                {
                    if (block->ipar[0] == 1)
                    {
                        if (block->inptr[0][i] < maxmin)
                        {
                            maxmin = block->inptr[0][i];
                        }
                    }
                    else
                    {
                        if (block->inptr[0][i] > maxmin)
                        {
                            maxmin = block->inptr[0][i];
                        }
                    }
                }
            }
            else
            {
                maxmin = block->inptr[0][block->mode[0] - 1];
            }
            block->outptr[0][0] = maxmin;

        }
        else if (block->nin == 2)
        {
            for (i = 0; i < block->insz[0]; ++i)
            {
                if ((block->ng == 0) || (phase == 1))
                {
                    if (block->ipar[0] == 1)
                    {
                        block->outptr[0][i] = Min(block->inptr[0][i], block->inptr[1][i]);
                    }
                    else
                    {
                        block->outptr[0][i] = Max(block->inptr[0][i], block->inptr[1][i]);
                    }
                }
                else
                {
                    block->outptr[0][i] = block->inptr[block->mode[0] - 1][i];
                }
            }
        }
    }
    else if (flag == 9)
    {
        if (block->nin == 1)
        {

            if (phase == 2)
            {
                for (i = 0; i < block->insz[0]; ++i)
                {
                    if (i != block->mode[0] - 1)
                    {
                        block->g[i] = block->inptr[0][i] - block->inptr[0][block->mode[0] - 1];
                    }
                    else
                    {
                        block->g[i] = 1.0;
                    }
                }
            }
            else if (phase == 1)
            {
                maxmin = block->inptr[0][0];
                block->mode[0] = 1;
                for (i = 1; i < block->insz[0]; ++i)
                {
                    if (block->ipar[0] == 1)
                    {
                        if (block->inptr[0][i] < maxmin)
                        {
                            maxmin = block->inptr[0][i];
                            block->mode[0] = i + 1;
                        }
                    }
                    else
                    {
                        if (block->inptr[0][i] > maxmin)
                        {
                            maxmin = block->inptr[0][i];
                            block->mode[0] = i + 1;
                        }
                    }
                }
            }
        }
        else if (block->nin == 2)
        {
            for (i = 0; i < block->insz[0]; ++i)
            {
                block->g[i] = block->inptr[0][i] - block->inptr[1][i];
                if (phase == 1)
                {
                    if (block->ipar[0] == 1)
                    {
                        if (block->g[i] > 0)
                        {
                            block->mode[i] = 2;
                        }
                        else
                        {
                            block->mode[i] = 1;
                        }
                    }
                    else
                    {
                        if (block->g[i] < 0)
                        {
                            block->mode[i] = 2;
                        }
                        else
                        {
                            block->mode[i] = 1;
                        }
                    }
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
