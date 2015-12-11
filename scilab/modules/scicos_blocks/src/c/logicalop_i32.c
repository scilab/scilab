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
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void logicalop_i32(scicos_block *block, int flag)
{
    int i = 0, m = 0, n = 0, nin = 0, k = 0, j = 0, l = 0;
    int *ipar = NULL;
    SCSINT32_COP *y = NULL, *u = NULL;
    m = GetInPortRows(block, 1);
    n = GetInPortCols(block, 1);
    y = Getint32OutPortPtrs(block, 1);
    ipar = GetIparPtrs(block);
    nin = GetNin(block);
    k = ipar[0];
    switch (k)
    {
        case 0:
            if (nin == 1)
            {
                u = Getint32InPortPtrs(block, 1);
                if (ipar[1] == 0)
                {
                    *y = 1;
                    for (i = 0; i < m * n; i++)
                    {
                        if (u[i] <= 0)
                        {
                            *y = 0;
                            break;
                        }
                    }
                }
                else
                {
                    *y = u[0];
                    for (i = 1; i < m * n; i++)
                    {
                        *y = (*y) & (u[i]);
                    }
                }
            }
            else
            {
                if (ipar[1] == 0)
                {
                    for (i = 0; i < m * n; i++)
                    {
                        y[i] = 1;
                        for (j = 1; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            if (u[i] <= 0)
                            {
                                y[i] = 0;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < m * n; i++)
                    {
                        u = Getint32InPortPtrs(block, 1);
                        y[i] = u[i];
                        for (j = 2; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            y[i] = (y[i]) & (u[i]);
                        }
                    }
                }
            }
            break;
        case 1:
            if (nin == 1)
            {
                u = Getint32InPortPtrs(block, 1);
                *y = 0;
                if (ipar[1] == 0)
                {
                    for (i = 0; i < m * n; i++)
                    {
                        if (u[i] > 0)
                        {
                            *y = 1;
                            break;
                        }
                    }
                }
                else
                {
                    for (i = 0; i < m * n; i++)
                    {
                        *y = (*y) | (u[i]);
                    }
                }
            }
            else
            {
                if (ipar[1] == 0)
                {
                    for (i = 0; i < m * n; i++)
                    {
                        y[i] = 0;
                        for (j = 1; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            if (u[i] > 0)
                            {
                                y[i] = 1;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < m * n; i++)
                    {
                        y[i] = 0;
                        for (j = 1; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            y[i] = (y[i]) | (u[i]);
                        }
                    }
                }
            }
            break;
        case 2:
            if (nin == 1)
            {
                u = Getint32InPortPtrs(block, 1);
                if (ipar[1] == 0)
                {
                    *y = 0;
                    for (i = 0; i < m * n; i++)
                    {
                        if (u[i] <= 0)
                        {
                            *y = 1;
                            break;
                        }
                    }
                }
                else
                {
                    *y = u[0];
                    for (i = 1; i < m * n; i++)
                    {
                        *y = ((*y) & (u[i]));
                    }
                    *y = ~*y;
                }
            }
            else
            {
                if (ipar[1] == 0)
                {
                    for (i = 0; i < m * n; i++)
                    {
                        y[i] = 0;
                        for (j = 1; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            if (u[i] <= 0)
                            {
                                y[i] = 1;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < m * n; i++)
                    {
                        u = Getint32InPortPtrs(block, 1);
                        y[i] = u[i];
                        for (j = 2; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            y[i] = ((y[i]) & (u[i]));
                        }
                        y[i] = ~y[i];
                    }
                }
            }
            break;
        case 3:
            if (nin == 1)
            {
                u = Getint32InPortPtrs(block, 1);
                if (ipar[1] == 0)
                {
                    *y = 1;
                    for (i = 0; i < m * n; i++)
                    {
                        if (u[i] > 0)
                        {
                            *y = 0;
                            break;
                        }
                    }
                }
                else
                {
                    *y = 0;
                    for (i = 0; i < m * n; i++)
                    {
                        *y = ((*y) | (u[i]));
                    }
                    *y = ~*y;
                }
            }
            else
            {
                if (ipar[1] == 0)
                {
                    for (i = 0; i < m * n; i++)
                    {
                        y[i] = 1;
                        for (j = 1; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            if (u[i] > 0)
                            {
                                y[i] = 0;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < m * n; i++)
                    {
                        u = Getint32InPortPtrs(block, 1);
                        y[i] = u[i];
                        for (j = 2; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            y[i] = ((y[i]) | (u[i]));
                        }
                        y[i] = ~y[i];
                    }
                }
            }
            break;
        case 4:
            if (nin == 1)
            {
                u = Getint32InPortPtrs(block, 1);
                if (ipar[1] == 0)
                {
                    l = 0;
                    for (i = 0; i < m * n; i++)
                    {
                        if (u[i] > 0)
                        {
                            l = (l + 1) % 2;
                        }
                    }
                    *y = (SCSINT32_COP)l;
                }
                else
                {
                    *y = u[0];
                    for (i = 1; i < m * n; i++)
                    {
                        *y = ((*y) ^ (u[i]));
                    }
                }
            }
            else
            {
                if (ipar[1] == 0)
                {
                    for (i = 0; i < m * n; i++)
                    {
                        l = 0;
                        for (j = 1; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            if (u[i] > 0)
                            {
                                l = (l + 1) % 2;
                            }
                        }
                        y[i] = (SCSINT32_COP)l;
                    }
                }
                else
                {
                    for (i = 0; i < m * n; i++)
                    {
                        u = Getint32InPortPtrs(block, 1);
                        y[i] = u[i];
                        for (j = 2; j < nin + 1; j++)
                        {
                            u = Getint32InPortPtrs(block, j);
                            y[i] = ((y[i]) ^ (u[i]));
                        }
                    }
                }
            }
            break;
        case 5:
            u = Getint32InPortPtrs(block, 1);
            if (ipar[1] == 0)
            {
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = 1;
                    }
                }
            }
            else
            {
                for (i = 0; i < m * n; i++)
                {
                    y[i] = ~(u[i]);
                }
            }
            break;
    }
}
/*--------------------------------------------------------------------------*/
