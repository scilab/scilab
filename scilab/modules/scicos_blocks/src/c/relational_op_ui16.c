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
#include "scicos.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void relational_op_ui16(scicos_block *block, int flag)
{
    int k = 0, i = 0;

    int m = GetInPortRows(block, 1);
    int n = GetInPortCols(block, 1);
    unsigned short *u1 = Getuint16InPortPtrs(block, 1);
    unsigned short *u2 = Getuint16InPortPtrs(block, 2);
    unsigned short *y = Getuint16OutPortPtrs(block, 1);
    int *ipar = GetIparPtrs(block);

    if (flag == 1)
    {
        if ((block->ng != 0) & (get_phase_simulation() == 2))
        {
            for (i = 0; i < m * n; i++)
            {
                *(y + i) = (unsigned short)block->mode[i] - 1;
            }
        }
        else
        {
            for (i = 0; i < m * n; i++)
            {
                y[i] = 0;
            }
            k = ipar[0];
            switch (k)
            {
                case 0:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] == u2[i])
                        {
                            y[i] = 1;
                        }
                    }
                    break;
                case 1:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] != u2[i])
                        {
                            y[i] = 1;
                        }
                    }
                    break;
                case 2:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] < u2[i])
                        {
                            y[i] = 1;
                        }
                    }
                    break;
                case 3:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] <= u2[i])
                        {
                            y[i] = 1;
                        }
                    }
                    break;
                case 4:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] > u2[i])
                        {
                            y[i] = 1;
                        }
                    }
                    break;
                case 5:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] >= u2[i])
                        {
                            y[i] = 1;
                        }
                    }
                    break;
            }
        }
    }
    else if (flag == 9)
    {
        for (i = 0; i < m * n; i++)
        {
            block->g[i] = *(u1 + i) - *(u2 + i);
        }
        if (get_phase_simulation() == 1)
        {
            for (i = 0; i < m * n; i++)
            {
                block->mode[i] = (int)1;
            }
            k = ipar[0];
            switch (k)
            {
                case 0:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] == u2[i])
                        {
                            block->mode[i] = (int)2;
                        }
                    }
                    break;
                case 1:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] != u2[i])
                        {
                            block->mode[i] = (int)2;
                        }
                    }
                    break;
                case 2:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] < u2[i])
                        {
                            block->mode[i] = (int)2;
                        }
                    }
                    break;
                case 3:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] <= u2[i])
                        {
                            block->mode[i] = (int)2;
                        }
                    }
                    break;
                case 4:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] > u2[i])
                        {
                            block->mode[i] = (int)2;
                        }
                    }
                    break;
                case 5:
                    for (i = 0; i < m * n; i++)
                    {
                        if (u1[i] >= u2[i])
                        {
                            block->mode[i] = (int)2;
                        }
                    }
                    break;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
