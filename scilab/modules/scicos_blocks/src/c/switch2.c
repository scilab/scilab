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
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void  switch2(scicos_block *block, int flag)
{
    int i = 0, j = 0, phase = 0;
    if (flag == 1)
    {
        phase = get_phase_simulation();
        if (phase == 1 || block->ng == 0)
        {
            i = 2;
            if (*block->ipar == 0)
            {
                if (*block->inptr[1] >= *block->rpar)
                {
                    i = 0;
                }
            }
            else if (*block->ipar == 1)
            {
                if (*block->inptr[1] > *block->rpar)
                {
                    i = 0;
                }
            }
            else
            {
                if (*block->inptr[1] != *block->rpar)
                {
                    i = 0;
                }
            }
        }
        else
        {
            if (block->mode[0] == 1)
            {
                i = 0;
            }
            else if (block->mode[0] == 2)
            {
                i = 2;
            }
        }
        for (j = 0; j < block->insz[0]; j++)
        {
            block->outptr[0][j] = block->inptr[i][j];
        }
    }
    else if (flag == 9)
    {
        phase = get_phase_simulation();
        block->g[0] = *block->inptr[1] - (*block->rpar);
        if (phase == 1)
        {
            i = 2;
            if (*block->ipar == 0)
            {
                if (block->g[0] >= 0.0)
                {
                    i = 0;
                }
            }
            else if (*block->ipar == 1)
            {
                if (block->g[0] > 0.0)
                {
                    i = 0;
                }
            }
            else
            {
                if (block->g[0] != 0.0)
                {
                    i = 0;
                }
            }
            if (i == 0)
            {
                block->mode[0] = 1;
            }
            else
            {
                block->mode[0] = 2;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
