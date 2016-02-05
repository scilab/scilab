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
#include <stdio.h>
#include "scicos_print.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "localization.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void summation_i16e(scicos_block *block, int flag)
{
    if ((flag == 1) | (flag == 6))
    {
        int j = 0, k = 0;

        short *y = Getint16OutPortPtrs(block, 1);
        int nu = GetInPortRows(block, 1);
        int mu = GetInPortCols(block, 1);
        int *ipar = GetIparPtrs(block);
        int nin = GetNin(block);

        double l = pow(2, 16) / 2;

        if (nin == 1)
        {
            double v = 0.;
            short *u = Getint16InPortPtrs(block, 1);
            for (j = 0; j < nu * mu; j++)
            {
                v = v + (double)u[j];
            }
            if ((v >= l) | (v < -l))
            {
                scicos_print(_("overflow error"));
                set_block_error(-4);
                return;
            }
            else
            {
                y[0] = (short)v;
            }
        }
        else
        {
            for (j = 0; j < nu * mu; j++)
            {
                double v = 0.;
                for (k = 0; k < nin; k++)
                {
                    short *u = Getint16InPortPtrs(block, k + 1);
                    if (ipar[k] > 0)
                    {
                        v = v + (double)u[j];
                    }
                    else
                    {
                        v = v - (double)u[j];
                    }
                }
                if ((v >= l) | (v < -l))
                {
                    scicos_print(_("overflow error"));
                    set_block_error(-4);
                    return;
                }
                else
                {
                    y[j] = (short)v;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
