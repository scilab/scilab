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
#include <memory.h>
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void samphold4_m(scicos_block *block, int flag)
{
    /* c     Copyright INRIA

    Scicos block simulator
    returns sample and hold  of the input */

    if (flag == 1)
    {
        int m = GetInPortRows(block, 1);
        int n = GetInPortCols(block, 1);
        void *u = GetInPortPtrs(block, 1);
        void *y = GetOutPortPtrs(block, 1);
        int sz = GetSizeOfOut(block, 1);
        memcpy(y, u, m * n * sz);
    }
}
/*--------------------------------------------------------------------------*/
