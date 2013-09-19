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
#define matmul2(y1,u1,u2,mu,nu)  {for (i=0;i<mu*nu;i++) y1[i]=u1[i]*u2[i];}
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matmul2_m(scicos_block *block, int flag)
{
    if (flag == 1)
    {
        int i = 0;
        int ut = GetInType(block, 1);
        int mu = GetOutPortRows(block, 1);
        int nu = GetOutPortCols(block, 1);
        switch (ut)
        {
            case SCSREAL_N :
            {

                SCSREAL_COP *u1 = GetRealInPortPtrs(block, 1);
                SCSREAL_COP *u2 = GetRealInPortPtrs(block, 2);
                SCSREAL_COP *y1 = GetRealOutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSINT32_N :
            {
                SCSINT32_COP *u1 = Getint32InPortPtrs(block, 1);
                SCSINT32_COP *u2 = Getint32InPortPtrs(block, 2);
                SCSINT32_COP *y1 = Getint32OutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSINT16_N :
            {

                SCSINT16_COP *u1 = Getint16InPortPtrs(block, 1);
                SCSINT16_COP *u2 = Getint16InPortPtrs(block, 2);
                SCSINT16_COP *y1 = Getint16OutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSINT8_N :
            {

                SCSINT8_COP *u1 = Getint8InPortPtrs(block, 1);
                SCSINT8_COP *u2 = Getint8InPortPtrs(block, 2);
                SCSINT8_COP *y1 = Getint8OutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSUINT32_N :
            {

                SCSUINT32_COP *u1 = Getuint32InPortPtrs(block, 1);
                SCSUINT32_COP *u2 = Getuint32InPortPtrs(block, 2);
                SCSUINT32_COP *y1 = Getuint32OutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSUINT16_N :
            {

                SCSUINT16_COP *u1 = Getuint16InPortPtrs(block, 1);
                SCSUINT16_COP *u2 = Getuint16InPortPtrs(block, 2);
                SCSUINT16_COP *y1 = Getuint16OutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSUINT8_N :
            {

                SCSUINT8_COP *u1 = Getuint8InPortPtrs(block, 1);
                SCSUINT8_COP *u2 = Getuint8InPortPtrs(block, 2);
                SCSUINT8_COP *y1 = Getuint8OutPortPtrs(block, 1);
                matmul2(y1, u1, u2, mu, nu);
                break;
            }

            case SCSCOMPLEX_N :
            {
                SCSREAL_COP *u1r = GetRealInPortPtrs(block, 1);
                SCSREAL_COP *u2r = GetRealInPortPtrs(block, 2);
                SCSREAL_COP *y1r = GetRealOutPortPtrs(block, 1);
                SCSREAL_COP *u1i = GetImagInPortPtrs(block, 1);
                SCSREAL_COP *u2i = GetImagInPortPtrs(block, 2);
                SCSREAL_COP *y1i = GetImagOutPortPtrs(block, 1);
                for (i = 0; i < mu * nu; i++)
                {
                    y1r[i] = (u1r[i] * u2r[i]) - (u1i[i] * u2i[i]);
                    y1i[i] = (u1r[i] * u2i[i]) + (u1i[i] * u2r[i]);
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
