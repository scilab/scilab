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
SCICOS_BLOCKS_IMPEXP void matzmul2_m(scicos_block *block, int flag)
{
    int i = 0;

    int mu = GetInPortRows(block, 1);
    int nu = GetInPortCols(block, 1);

    double *u1r = GetRealInPortPtrs(block, 1);
    double *u1i = GetImagInPortPtrs(block, 1);
    double *u2r = GetRealInPortPtrs(block, 2);
    double *u2i = GetImagInPortPtrs(block, 2);
    double *yr = GetRealOutPortPtrs(block, 1);
    double *yi = GetImagOutPortPtrs(block, 1);

    for (i = 0; i < mu * nu; i++)
    {
        yr[i] = u1r[i] * u2r[i] - u1i[i] * u2i[i];
        yi[i] = u1r[i] * u2i[i] + u1i[i] * u2r[i];
    }
}
/*--------------------------------------------------------------------------*/
