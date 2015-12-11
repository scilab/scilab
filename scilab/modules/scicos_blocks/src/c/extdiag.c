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
#include "core_math.h"
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void extdiag(scicos_block *block, int flag)
{
    double *u1 = NULL;
    double *y = NULL;

    int mu = 0, nu = 0, i = 0, ii = 0;

    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    u1 = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);
    for (i = 0; i < mu * nu; i++)
    {
        *(y + i) = 0;
    }
    for (i = 0; i < Min(mu, nu); i++)
    {
        ii = i + i * mu;
        *(y + ii) = *(u1 + ii);
    }
}
/*--------------------------------------------------------------------------*/
