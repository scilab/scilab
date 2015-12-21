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
SCICOS_BLOCKS_IMPEXP void cumsumz_m(scicos_block *block, int flag)
{
    double *ur = NULL;
    double *ui = NULL;
    double *yr = NULL;
    double *yi = NULL;
    int nu = 0, mu = 0, j = 0;
    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    yr[0] = ur[0];
    yi[0] = ui[0];
    for (j = 1; j < mu * nu; j++)
    {
        yr[j] = ur[j] + yr[j - 1];
        yi[j] = ui[j] + yi[j - 1];
    }
}
/*--------------------------------------------------------------------------*/
