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
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void selector_m(scicos_block *block, int flag)
{
    void *u = NULL;
    void *y = NULL;
    double *z = NULL;
    int nu = 0, mu = 0, ic = 0, nev = 0, nin = 0, so = 0;

    z = GetDstate(block);
    nin = GetNin(block);
    ic = (int)z[0];

    if (flag < 3)
    {
        ic = 0;
        nev = GetNevIn(block);
        while (nev >= 1)
        {
            ic = ic + 1;
            nev = nev / 2;
        }
    }
    if (nin > 1)
    {
        mu = GetInPortRows(block, ic);
        nu = GetInPortCols(block, ic);
        u = GetInPortPtrs(block, ic);
        so = GetSizeOfOut(block, 1);
        y = GetOutPortPtrs(block, 1);
        memcpy(y, u, mu * nu * so);
    }
    else
    {
        mu = GetInPortRows(block, 1);
        nu = GetInPortCols(block, 1);
        u = GetInPortPtrs(block, 1);
        y = GetOutPortPtrs(block, ic);
        so = GetSizeOfIn(block, 1);
        memcpy(y, u, mu * nu * so);
    }
}
/*--------------------------------------------------------------------------*/
