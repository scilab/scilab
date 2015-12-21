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
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void cumsumz_r(scicos_block *block, int flag)
{
    double *ur = NULL;
    double *ui = NULL;
    double *yr = NULL;
    double *yi = NULL;
    int nu = 0, mu = 0, i = 0, j = 0, ij = 0;

    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    for (j = 0; j < nu; j++)
    {
        i = 0;
        ij = i + j * mu;
        yr[ij] = ur[ij];
        yi[ij] = ui[ij];
        for (i = 1; i < mu; i++)
        {
            ij = i + j * mu;
            yr[ij] = ur[ij] + yr[ij - 1];
            yi[ij] = ui[ij] + yi[ij - 1];
        }
    }
}
/*--------------------------------------------------------------------------*/
