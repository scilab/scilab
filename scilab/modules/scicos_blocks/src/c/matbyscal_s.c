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
SCICOS_BLOCKS_IMPEXP void matbyscal_s(scicos_block *block, int flag)
{
    if (flag == 1)
    {
        int i = 0;
        double v = 0.;
        int ut = GetInType(block, 1);
        int mu = GetOutPortRows(block, 1);
        int nu = GetOutPortCols(block, 1);
        double *rpar = GetRparPtrs(block);
        switch (ut)
        {
            case SCSINT32_N :
            {

                SCSINT32_COP *u1 = Getint32InPortPtrs(block, 1);
                SCSINT32_COP *u2 = Getint32InPortPtrs(block, 2);
                SCSINT32_COP *y1 = Getint32OutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    v = (double)u1[i] * (double)u2[0];
                    if (v < rpar[0])
                    {
                        v = rpar[0];
                    }
                    else if (v > rpar[1])
                    {
                        v = rpar[1];
                    }
                    y1[i] = (SCSINT32_COP)v;
                }
                break;
            }

            case SCSINT16_N :
            {

                short *u1 = Getint16InPortPtrs(block, 1);
                short *u2 = Getint16InPortPtrs(block, 2);
                short *y1 = Getint16OutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    v = (double)u1[i] * (double)u2[0];
                    if (v < rpar[0])
                    {
                        v = rpar[0];
                    }
                    else if (v > rpar[1])
                    {
                        v = rpar[1];
                    }
                    y1[i] = (short)v;
                }
                break;
            }

            case SCSINT8_N :
            {

                char *u1 = Getint8InPortPtrs(block, 1);
                char *u2 = Getint8InPortPtrs(block, 2);
                char *y1 = Getint8OutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    v = (double)u1[i] * (double)u2[0];
                    if (v < rpar[0])
                    {
                        v = rpar[0];
                    }
                    else if (v > rpar[1])
                    {
                        v = rpar[1];
                    }
                    y1[i] = (char)v;
                }
                break;
            }

            case SCSUINT32_N :
            {

                SCSUINT32_COP *u1 = Getuint32InPortPtrs(block, 1);
                SCSUINT32_COP *u2 = Getuint32InPortPtrs(block, 2);
                SCSUINT32_COP *y1 = Getuint32OutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    v = (double)u1[i] * (double)u2[0];
                    if (v < rpar[0])
                    {
                        v = rpar[0];
                    }
                    else if (v > rpar[1])
                    {
                        v = rpar[1];
                    }
                    y1[i] = (SCSUINT32_COP)v;
                }
                break;
            }

            case SCSUINT16_N :
            {

                unsigned short *u1 = Getuint16InPortPtrs(block, 1);
                unsigned short *u2 = Getuint16InPortPtrs(block, 2);
                unsigned short *y1 = Getuint16OutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    v = (double)u1[i] * (double)u2[0];
                    if (v < rpar[0])
                    {
                        v = rpar[0];
                    }
                    else if (v > rpar[1])
                    {
                        v = rpar[1];
                    }
                    y1[i] = (unsigned short)v;
                }
                break;
            }

            case SCSUINT8_N :
            {

                unsigned char *u1 = Getuint8InPortPtrs(block, 1);
                unsigned char *u2 = Getuint8InPortPtrs(block, 2);
                unsigned char *y1 = Getuint8OutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    v = (double)u1[i] * (double)u2[0];
                    if (v < rpar[0])
                    {
                        v = rpar[0];
                    }
                    else if (v > rpar[1])
                    {
                        v = rpar[1];
                    }
                    y1[i] = (unsigned char)v;
                }
                break;
            }

            default :
            {
                set_block_error(-4);
                return;
            }
        }
    }

}
/*--------------------------------------------------------------------------*/
