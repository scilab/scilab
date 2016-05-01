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
#include <memory.h>
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_print.h"
#include "localization.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void convert(scicos_block *block, int flag)
{
    int m = 0, n = 0, i = 0;
    int *ipar = NULL;
    double v = 0., w = 0., k = 0.;

    m = GetInPortRows(block, 1);
    n = GetInPortCols(block, 1);
    ipar = GetIparPtrs(block);

    if ((flag == 1) | (flag == 6))
    {
        switch (*ipar)
        {
            case 1:
            {
                void *u = NULL, *y = NULL;
                int so;
                so = GetSizeOfOut(block, 1);
                u = GetInPortPtrs(block, 1);
                y = GetOutPortPtrs(block, 1);
                memcpy(y, u, m * n * so);
                break;
            }
            case 2:
            {
                SCSREAL_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (fabs(w) > k / 2 - 1)
                    {
                        if (w >= 0)
                        {
                            w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                        else
                        {
                            w = -(-(k / 2) + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                    }
                    y[i] = (SCSINT32_COP)w;
                }
                break;
            }
            case 3:
            {
                SCSREAL_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (fabs(w) > k / 2 - 1)
                    {
                        if (w >= 0)
                        {
                            w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                        else
                        {
                            w = -(-(k / 2) + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                    }
                    y[i] = (SCSINT16_COP)w;
                }
                break;
            }
            case 4:
            {
                SCSREAL_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (fabs(w) > k / 2 - 1)
                    {
                        if (w >= 0)
                        {
                            w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                        else
                        {
                            w = -(-(k / 2) + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                    }
                    y[i] = (SCSINT8_COP)w;
                }
                break;
            }
            case 5:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT32_COP)w;
                }
                break;
            }
            case 6:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT16_COP)w;
                }
                break;
            }
            case 7:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT8_COP)w;
                }
                break;
            }
            case 8:
            {
                SCSINT32_COP *u = NULL;
                SCSREAL_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = GetRealOutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSREAL_COP) u[i];
                }
                break;
            }
            case 9:
            {
                SCSINT32_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (fabs(w) > k / 2 - 1)
                    {
                        if (w >= 0)
                        {
                            w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                        else
                        {
                            w = -(-(k / 2) + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                    }
                    y[i] = (SCSINT16_COP)w;
                }
                break;
            }
            case 10:
            {
                SCSINT32_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (fabs(w) > k / 2 - 1)
                    {
                        if (w >= 0)
                        {
                            w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                        else
                        {
                            w = -(-(k / 2) + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                    }
                    y[i] = (SCSINT8_COP)w;
                }
                break;
            }
            case 11:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT16_COP)w;
                }
                break;
            }
            case 12:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT8_COP)w;
                }
                break;
            }
            case 13:
            {
                SCSINT16_COP *u = NULL;
                double *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = GetRealOutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (double) u[i];
                }
                break;
            }
            case 14:
            {
                SCSINT16_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSINT32_COP)u[i];
                }
                break;
            }
            case 15:
            {
                SCSINT16_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (fabs(w) > k / 2 - 1)
                    {
                        if (w >= 0)
                        {
                            w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                        else
                        {
                            w = -(-(k / 2) + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                        }
                    }
                    y[i] = (SCSINT8_COP)w;
                }
                break;
            }
            case 16:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSUINT32_COP)u[i];
                }
                break;
            }
            case 17:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT8_COP)w;
                }
                break;
            }
            case 18:
            {
                SCSINT8_COP *u = NULL;
                SCSREAL_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = GetRealOutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSREAL_COP) u[i];
                }
                break;
            }
            case 19:
            {
                SCSINT8_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSINT32_COP)u[i];
                }
                break;
            }
            case 20:
            {
                SCSINT8_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSINT16_COP)u[i];
                }
                break;
            }
            case 21:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSUINT32_COP)u[i];
                }
                break;
            }
            case 22:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSUINT16_COP)u[i];
                }
                break;
            }
            case 23:
            {
                SCSUINT32_COP *u = NULL;
                SCSREAL_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = GetRealOutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSREAL_COP) u[i];
                }
                break;
            }
            case 24:
            {
                SCSUINT32_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if ((w) > k / 2 - 1)
                    {
                        w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                    }
                    y[i] = (SCSINT16_COP)w;
                }
                break;
            }
            case 25:
            {
                SCSUINT32_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if ((w) > k / 2 - 1)
                    {
                        w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                    }
                    y[i] = (SCSINT8_COP)w;
                }
                break;
            }
            case 26:
            {
                SCSUINT32_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT16_COP)w;
                }
                break;
            }
            case 27:
            {
                SCSUINT32_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT8_COP)w;
                }
                break;
            }
            case 28:
            {
                SCSUINT16_COP *u = NULL;
                SCSREAL_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = GetRealOutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSREAL_COP) u[i];
                }
                break;
            }
            case 29:
            {
                SCSUINT16_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSINT32_COP)u[i];
                }
                break;
            }
            case 30:
            {
                SCSUINT16_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    if (w > k / 2 - 1)
                    {
                        w = (-k / 2 + fabs(w - (double)((int)(w / (k / 2))) * (k / 2)));
                    }
                    y[i] = (SCSINT8_COP)w;
                }
                break;
            }
            case 31:
            {
                SCSUINT16_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSUINT32_COP)u[i];
                }
                break;
            }
            case 32:
            {
                SCSUINT16_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    v = (double)u[i];
                    w = v - (double)((int)(v / k)) * k;
                    y[i] = (SCSUINT8_COP)w;
                }
                break;
            }
            case 33:
            {
                SCSUINT8_COP *u = NULL;
                SCSREAL_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = GetRealOutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSREAL_COP) u[i];
                }
                break;
            }
            case 34:
            {
                SCSUINT8_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSINT32_COP)u[i];
                }
                break;
            }
            case 35:
            {
                SCSUINT8_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSINT16_COP)u[i];
                }
                break;
            }
            case 36:
            {
                SCSUINT8_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSUINT32_COP)u[i];
                }
                break;
            }
            case 37:
            {
                SCSUINT8_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    y[i] = (SCSUINT16_COP)u[i];
                }
                break;
            }
            case 38:
            {
                SCSREAL_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > k / 2 - 1)
                    {
                        y[i] = (SCSINT32_COP)(k / 2 - 1);
                    }
                    else if (u[i] < -(k / 2))
                    {
                        y[i] = -(SCSINT32_COP)(k / 2);
                    }
                    else
                    {
                        y[i] = (SCSINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 39:
            {
                SCSREAL_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > k / 2 - 1)
                    {
                        y[i] = (SCSINT16_COP)(k / 2 - 1);
                    }
                    else if (u[i] < -(k / 2))
                    {
                        y[i] = -(SCSINT16_COP)(k / 2);
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 40:
            {
                SCSREAL_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > k / 2 - 1)
                    {
                        y[i] = (SCSINT8_COP)(k / 2 - 1);
                    }
                    else if (u[i] < -(k / 2))
                    {
                        y[i] = -(SCSINT8_COP)(k / 2);
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 41:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] >= k)
                    {
                        y[i] = (SCSUINT32_COP)(k - 1);
                    }
                    else if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 42:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] >= k)
                    {
                        y[i] = (SCSUINT16_COP)(k - 1);
                    }
                    else if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 43:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] >= k)
                    {
                        y[i] = (SCSUINT8_COP)(k - 1);
                    }
                    else if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 44:
            {
                SCSINT32_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT16_COP)(k / 2 - 1);
                    }
                    else if (u[i] < -(SCSINT32_COP)(k / 2))
                    {
                        y[i] = -(SCSINT16_COP)(k / 2);
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 45:
            {
                SCSINT32_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT8_COP)(k / 2 - 1);
                    }
                    else if (u[i] < -(SCSINT32_COP)(k / 2))
                    {
                        y[i] = -(SCSINT8_COP)(k / 2);
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 46:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 47:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] >= (SCSINT32_COP)(k))
                    {
                        y[i] = (SCSUINT16_COP)(k - 1);
                    }
                    else if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 48:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] >= (SCSINT32_COP)k)
                    {
                        y[i] = (SCSUINT8_COP)(k - 1);
                    }
                    else if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 49:
            {
                SCSINT16_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSINT16_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT8_COP)(k / 2 - 1);
                    }
                    else if (u[i] < -(SCSINT16_COP)(k / 2))
                    {
                        y[i] = -(SCSINT8_COP)(k / 2);
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 50:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)u[i];
                    }
                }
                break;
            }
            case 51:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 52:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] >= (SCSINT16_COP)k)
                    {
                        y[i] = (SCSUINT8_COP)(k - 1);
                    }
                    else if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 53:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)u[i];
                    }
                }
                break;
            }
            case 54:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 55:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        y[i] = 0;
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 56:
            {
                SCSINT32_COP *y = NULL;
                SCSUINT32_COP *u = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT32_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 57:
            {
                SCSUINT32_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT16_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 58:
            {
                SCSUINT32_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT8_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 59:
            {
                SCSUINT32_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSUINT16_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 60:
            {
                SCSUINT32_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT32_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSUINT8_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 61:
            {
                SCSUINT16_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT16_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT16_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 62:
            {
                SCSUINT16_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT16_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT8_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 63:
            {
                SCSUINT16_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT16_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSUINT8_COP)k / 2 - 1;
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 64:
            {
                SCSUINT8_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (SCSUINT8_COP)(k / 2 - 1))
                    {
                        y[i] = (SCSINT8_COP)(k / 2 - 1);
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 65:
            {
                SCSREAL_COP *u = NULL;
                SCSINT32_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] > k / 2 - 1) | (u[i] < -(k / 2)))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 66:
            {
                SCSREAL_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] > k / 2 - 1) | (u[i] < -(k / 2)))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 67:
            {
                SCSREAL_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] > k / 2 - 1) | (u[i] < -(k / 2)))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 68:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] >= k) | (u[i] < 0))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 69:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] >= k) | (u[i] < 0))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 70:
            {
                SCSREAL_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = GetRealInPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] >= k) | (u[i] < 0))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 71:
            {
                SCSINT32_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] > k / 2 - 1) | (u[i] < -(k / 2)))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 72:
            {
                SCSINT32_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] > k / 2 - 1) | (u[i] < -(k / 2)))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 73:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 74:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] >= k) | (u[i] < 0))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 75:
            {
                SCSINT32_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint32InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] >= k) | (u[i] < 0))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 76:
            {
                SCSINT16_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if ((u[i] > k / 2 - 1) | (u[i] < -(k / 2)))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 77:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)u[i];
                    }
                }
                break;
            }
            case 78:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 79:
            {
                SCSINT16_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint16InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (((u[i] >= k) | (u[i] < 0)) & (flag == 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 80:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT32_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT32_COP)u[i];
                    }
                }
                break;
            }
            case 81:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 82:
            {
                SCSINT8_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getint8InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] < 0)
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 83:
            {
                SCSINT32_COP *y = NULL;
                SCSUINT32_COP *u = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint32OutPortPtrs(block, 1);
                k = pow(2, 32);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT32_COP)(u[i]);
                    }
                }
                break;
            }
            case 84:
            {
                SCSUINT32_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 85:
            {
                SCSUINT32_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 86:
            {
                SCSUINT32_COP *u = NULL;
                SCSUINT16_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getuint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 87:
            {
                SCSUINT32_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getuint32InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 88:
            {
                SCSUINT16_COP *u = NULL;
                SCSINT16_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getint16OutPortPtrs(block, 1);
                k = pow(2, 16);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT16_COP)(u[i]);
                    }
                }
                break;
            }
            case 89:
            {
                SCSUINT16_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 90:
            {
                SCSUINT16_COP *u = NULL;
                SCSUINT8_COP *y = NULL;
                u = Getuint16InPortPtrs(block, 1);
                y = Getuint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSUINT8_COP)(u[i]);
                    }
                }
                break;
            }
            case 91:
            {
                SCSUINT8_COP *u = NULL;
                SCSINT8_COP *y = NULL;
                u = Getuint8InPortPtrs(block, 1);
                y = Getint8OutPortPtrs(block, 1);
                k = pow(2, 8);
                for (i = 0; i < m * n; i++)
                {
                    if (u[i] > (k / 2 - 1))
                    {
                        if (flag == 1)
                        {
                            scicos_print(_("overflow error"));
                            set_block_error(-4);
                            return;
                        }
                    }
                    else
                    {
                        y[i] = (SCSINT8_COP)(u[i]);
                    }
                }
                break;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
