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
SCICOS_BLOCKS_IMPEXP void logicalop(scicos_block *block, int flag)
{
    int i = 0, j = 0, k = 0, l = 0;
    i = block->ipar[0];
    switch (i)
    {
        case 0:
            if (block->nin == 1)
            {
                block->outptr[0][0] = 1.0;
                for (j = 0; j < block->insz[0]; j++)
                {
                    if (block->inptr[0][j] <= 0)
                    {
                        block->outptr[0][0] = 0.0;
                        break;
                    }
                }
            }
            else
            {
                for (j = 0; j < block->insz[0]; j++)
                {
                    block->outptr[0][j] = 1.0;
                    for (k = 0; k < block->nin; k++)
                    {
                        if (block->inptr[k][j] <= 0)
                        {
                            block->outptr[0][j] = 0.0;
                            break;
                        }
                    }
                }
            }
            break;

        case 1:
            if (block->nin == 1)
            {
                block->outptr[0][0] = 0.0;
                for (j = 0; j < block->insz[0]; j++)
                {
                    if (block->inptr[0][j] > 0)
                    {
                        block->outptr[0][0] = 1.0;
                        break;
                    }
                }
            }
            else
            {
                for (j = 0; j < block->insz[0]; j++)
                {
                    block->outptr[0][j] = 0.0;
                    for (k = 0; k < block->nin; k++)
                    {
                        if (block->inptr[k][j] > 0)
                        {
                            block->outptr[0][j] = 1.0;
                            break;
                        }
                    }
                }
            }
            break;

        case 2:
            if (block->nin == 1)
            {
                block->outptr[0][0] = 0.0;
                for (j = 0; j < block->insz[0]; j++)
                {
                    if (block->inptr[0][j] <= 0)
                    {
                        block->outptr[0][0] = 1.0;
                        break;
                    }
                }
            }
            else
            {
                for (j = 0; j < block->insz[0]; j++)
                {
                    block->outptr[0][j] = 0.0;
                    for (k = 0; k < block->nin; k++)
                    {
                        if (block->inptr[k][j] <= 0)
                        {
                            block->outptr[0][j] = 1.0;
                            break;
                        }
                    }
                }
            }
            break;

        case 3:
            if (block->nin == 1)
            {
                block->outptr[0][0] = 1.0;
                for (j = 0; j < block->insz[0]; j++)
                {
                    if (block->inptr[0][j] > 0)
                    {
                        block->outptr[0][0] = 0.0;
                        break;
                    }
                }
            }
            else
            {
                for (j = 0; j < block->insz[0]; j++)
                {
                    block->outptr[0][j] = 1.0;
                    for (k = 0; k < block->nin; k++)
                    {
                        if (block->inptr[k][j] > 0)
                        {
                            block->outptr[0][j] = 0.0;
                            break;
                        }
                    }
                }
            }
            break;

        case 4:
            if (block->nin == 1)
            {
                l = 0;
                for (j = 0; j < block->insz[0]; j++)
                {
                    if (block->inptr[0][j] > 0)
                    {
                        l = (l + 1) % 2;
                    }
                }
                block->outptr[0][0] = (double) l;
            }
            else
            {
                for (j = 0; j < block->insz[0]; j++)
                {
                    l = 0;
                    for (k = 0; k < block->nin; k++)
                    {
                        if (block->inptr[k][j] > 0)
                        {
                            l = (l + 1) % 2;
                        }
                    }
                    block->outptr[0][j] = (double) l;
                }
            }
            break;

        case 5:
            for (j = 0; j < block->insz[0]; j++)
            {
                if (block->inptr[0][j] > 0)
                {
                    block->outptr[0][j] = 0.0;
                }
                else
                {
                    block->outptr[0][j] = 1.0;
                }
            }
    }
}
/*--------------------------------------------------------------------------*/
