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
SCICOS_BLOCKS_IMPEXP void extract(scicos_block *block, int flag)
{
    double *u = NULL;
    double *y = NULL;
    int *r = NULL;
    int mu = 0, nr = 0, i = 0, j = 0, ij = 0, k = 0, nc = 0, nl = 0;

    mu = GetInPortRows(block, 1);
    nr = GetNipar(block);
    r = GetIparPtrs(block);
    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);
    nc = r[nr - 1];
    nl = r[nr - 2];
    k = 0;
    for (j = 0; j < nc; j++)
    {
        for (i = 0; i < nl; i++)
        {
            ij = r[i] - 1 + (r[nl + j] - 1) * mu;
            *(y + k) = *(u + ij);
            k++;
        }
    }
}
/*--------------------------------------------------------------------------*/
