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
SCICOS_BLOCKS_IMPEXP void matmul_ui16n(scicos_block *block, int flag)
{
    if ((flag == 1) | (flag == 6))
    {
        unsigned short *u1, *u2, *y;
        double k, C, D, t;
        int mu1, nu1, nu2, i, j, l, ji, jl, il;

        mu1 = GetInPortRows(block, 1);
        nu1 = GetInPortCols(block, 1);
        nu2 = GetInPortCols(block, 2);
        u1 = Getuint16InPortPtrs(block, 1);
        u2 = Getuint16InPortPtrs(block, 2);
        y = Getuint16OutPortPtrs(block, 1);

        k = pow(2, 16);
        for (l = 0; l < nu2; l++)
        {
            for (j = 0; j < mu1; j++)
            {
                D = 0;
                jl = j + l * mu1;
                for (i = 0; i < nu1; i++)
                {
                    ji = j + i * mu1;

                    il = i + l * nu1;
                    C = (double)(u1[ji]) * (double)(u2[il]);
                    D = D + C;
                }
                t = D - (int)(D / (k)) * ((k));
                y[jl] = (unsigned short)t;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
