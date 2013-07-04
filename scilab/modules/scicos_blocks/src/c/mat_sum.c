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
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_sum(scicos_block *block, int flag)
{
    int i = 0;
    int mu = GetInPortRows(block, 1);
    int nu = GetInPortCols(block, 1);
    double *u = GetRealInPortPtrs(block, 1);
    double *y = GetRealOutPortPtrs(block, 1);

    y[0] = 0.0;
    for (i = 0; i < nu * mu; i++)
    {
        y[0] = u[i] + y[0];
    }
}
/*--------------------------------------------------------------------------*/
