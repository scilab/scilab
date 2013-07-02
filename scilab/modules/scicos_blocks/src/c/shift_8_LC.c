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
SCICOS_BLOCKS_IMPEXP void shift_8_LC(scicos_block *block, int flag)
{
    int i = 0, j = 0;
    int mu = GetInPortRows(block, 1);
    int nu = GetInPortCols(block, 1);
    char *u = Getint8InPortPtrs(block, 1);
    char *y = Getint8OutPortPtrs(block, 1);
    int *ipar = GetIparPtrs(block);
    unsigned char k = (unsigned char)pow(2, 8 - 1);

    for (i = 0; i < mu * nu; i++)
    {
        char v = u[i];
        for (j = 0; j < ipar[0]; j++)
        {
            y[i] = v & k;
            if (y[i] == 0)
            {
                y[i] = v << 1;
            }
            else
            {
                y[i] = v << 1;
                y[i] = (y[i]) | (1);
            }
            v = y[i];
        }
    }
}
/*--------------------------------------------------------------------------*/
