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
SCICOS_BLOCKS_IMPEXP void product(scicos_block *block, int flag)
{
    int j = 0, k = 0;

    if (flag == 1)
    {
        if (block->nin == 1)
        {
            block->outptr[0][0] = 1.0;
            for (j = 0; j < block->insz[0]; j++)
            {
                block->outptr[0][0] = block->outptr[0][0] * block->inptr[0][j];
            }
        }
        else
        {
            for (j = 0; j < block->insz[0]; j++)
            {
                block->outptr[0][j] = 1.0;
                for (k = 0; k < block->nin; k++)
                {
                    if (block->ipar[k] > 0)
                    {
                        block->outptr[0][j] = block->outptr[0][j] * block->inptr[k][j];
                    }
                    else
                    {
                        if (block->inptr[k][j] == 0)
                        {
                            if (block->rpar[0] == 0.0)
                            {
                                set_block_error(-2);
                                return;
                            }
                            else
                            {
                                // ignore the divide by zero, divide by a value stored in rpar
                                block->outptr[0][j] = block->outptr[0][j] / block->rpar[0];
                            }
                        }
                        else
                        {
                            block->outptr[0][j] = block->outptr[0][j] / block->inptr[k][j];
                        }
                    }
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
