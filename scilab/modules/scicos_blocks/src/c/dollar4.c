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
SCICOS_BLOCKS_IMPEXP void dollar4(scicos_block *block, int flag)
{
    /* c     Copyright INRIA

    Scicos block simulator
    Ouputs delayed input */

    int i = 0;
    for (i = 0; i < block->insz[0]; i++)
    {
        if (flag == 1 || flag == 6 || flag == 4)
        {
            block->outptr[0][i] = block->z[i];
        }
        else if (flag == 2)
        {
            block->z[i] = block->inptr[0][i];
        }
    }
}
/*--------------------------------------------------------------------------*/
