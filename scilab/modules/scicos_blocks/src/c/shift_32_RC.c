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
#include <math.h>
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void shift_32_RC(scicos_block *block, int flag)
{
    int i = 0, j = 0;

    int mu = GetInPortRows(block, 1);
    int nu = GetInPortCols(block, 1);
    SCSINT32_COP *u = Getint32InPortPtrs(block, 1);
    SCSINT32_COP *y = Getint32OutPortPtrs(block, 1);
    int *ipar = GetIparPtrs(block);
    unsigned long k = (unsigned long)pow(2, 32 - 1);

    for (i = 0; i < mu * nu; i++)
    {
        SCSINT32_COP v = u[i];
        for (j = 0; j < -ipar[0]; j++)
        {
            y[i] = v & 1;
            if (y[i] == 0)
            {
                y[i] = v >> 1;
                y[i] = y[i] & (k - 1);
            }
            else
            {
                y[i] = v >> 1;
                y[i] = (y[i]) | (k);
            }
            v = y[i];
        }
    }
}
/*--------------------------------------------------------------------------*/
