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
SCICOS_BLOCKS_IMPEXP void submat(scicos_block *block, int flag)
{
    int mu = GetInPortRows(block, 1);
    int *r = GetIparPtrs(block);
    double *u = GetRealInPortPtrs(block, 1);
    double *y = GetRealOutPortPtrs(block, 1);
    int k = 0;
    int j = 0;
    for (j = r[2] - 1; j < r[3]; j++)
    {
        int i = 0;
        for (i = r[0] - 1; i < r[1]; i++)
        {
            int ij = i + j * mu;
            *(y + k) = *(u + ij);
            k++;
        }
    }
}
/*--------------------------------------------------------------------------*/
