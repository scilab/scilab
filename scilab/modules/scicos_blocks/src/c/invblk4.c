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
#include <stdio.h>
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void invblk4(scicos_block *block, int flag)
{
    /* Copyright INRIA

    Scicos block simulator
    Outputs the inverse of the input */

    int i = 0;
    double ww = 0.;
    if (flag == 6)
    {
        for (i = 0; i < block->insz[0]; i++)
        {
            ww = block->inptr[0][i];
            if (ww != 0.0)
            {
                block->outptr[0][i] = 1.0 / ww;
            }
        }
    }
    if (flag == 1)
    {
        for (i = 0; i < block->insz[0]; i++)
        {
            ww = block->inptr[0][i];
            if (ww != 0.0)
            {
                block->outptr[0][i] = 1.0 / ww;
            }
            else
            {
                if (block->rpar[0] == 0.0)
                {
                    set_block_error(-2);
                    return;
                }
                else
                {
                    // ignore the divide by zero, divide by a value stored in rpar
                    block->outptr[0][i] = 1.0 / block->rpar[0];
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
