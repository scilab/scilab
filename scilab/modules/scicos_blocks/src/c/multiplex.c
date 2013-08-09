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
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void multiplex(scicos_block *block, int flag)
{
    int i = 0, j = 0, k = 0;

    if (block->nin == 1)
    {
        k = 0;
        for (i = 0; i < block->nout; ++i)
        {
            for (j = 0; j < block->outsz[i]; ++j)
            {
                block->outptr[i][j] = block->inptr[0][k];
                ++k;
            }
        }
    }
    else
    {
        k = 0;
        for (i = 0; i < block->nin; ++i)
        {
            for (j = 0; j < block->insz[i]; ++j)
            {
                block->outptr[0][k] = block->inptr[i][j];
                ++k;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
