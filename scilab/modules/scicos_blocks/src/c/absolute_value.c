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
#include "dynlib_scicos_blocks.h"
#include "scicos_block.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void  absolute_value(scicos_block *block, int flag)
{
    int i = 0, j = 0;
    if (flag == 1)
    {
        if ( block->ng > 0)
        {
            for (i = 0; i < block->insz[0]; ++i)
            {
                if (get_phase_simulation() == 1)
                {
                    if (block->inptr[0][i] < 0)
                    {
                        j = 2;
                    }
                    else
                    {
                        j = 1;
                    }
                }
                else
                {
                    j = block->mode[i];
                }
                if (j == 1)
                {
                    block->outptr[0][i] = block->inptr[0][i];
                }
                else
                {
                    block->outptr[0][i] = -block->inptr[0][i];
                }
            }
        }
        else
        {
            for (i = 0; i < block->insz[0]; ++i)
            {
                if (block->inptr[0][i] < 0)
                {
                    block->outptr[0][i] = -block->inptr[0][i];
                }
                else
                {
                    block->outptr[0][i] = block->inptr[0][i];
                }
            }
        }
    }
    else if (flag == 9)
    {
        for (i = 0; i < block->insz[0]; ++i)
        {
            block->g[i] = block->inptr[0][i];
            if (get_phase_simulation() == 1)
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
/*--------------------------------------------------------------------------*/
