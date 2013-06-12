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
SCICOS_BLOCKS_IMPEXP void submatz(scicos_block *block, int flag)
{
    int i = 0, j = 0;

    int mu = GetInPortRows(block, 1);
    int *r = GetIparPtrs(block);
    double *ur = GetRealInPortPtrs(block, 1);
    double *ui = GetImagInPortPtrs(block, 1);
    double *yr = GetRealOutPortPtrs(block, 1);
    double *yi = GetImagOutPortPtrs(block, 1);
    int k = 0;

    for (j = r[2] - 1; j < r[3]; j++)
    {
        for (i = r[0] - 1; i < r[1]; i++)
        {
            int ij = i + j * mu;
            *(yr + k) = *(ur + ij);
            *(yi + k) = *(ui + ij);
            k++;
        }
    }
}
/*--------------------------------------------------------------------------*/
