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
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void  switch2_m(scicos_block *block, int flag)
{
    int i = 0, phase = 0, ipar = 0, mu = 0, nu = 0, so = 0;
    int *iparptrs = NULL;
    double *rpar = NULL;
    double *u2 = NULL;
    void *y = NULL, *u = NULL;

    iparptrs = GetIparPtrs(block);
    ipar = *iparptrs;
    rpar = GetRparPtrs(block);
    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    u2 = GetRealInPortPtrs(block, 2);
    y = GetOutPortPtrs(block, 1);
    if (flag == 1)
    {
        phase = get_phase_simulation();
        if (phase == 1 || block->ng == 0)
        {
            i = 3;
            if (ipar == 0)
            {
                if (*u2 >= *rpar)
                {
                    i = 1;
                }
            }
            else if (ipar == 1)
            {
                if (*u2 > *rpar)
                {
                    i = 1;
                }
            }
            else
            {
                if (*u2 != *rpar)
                {
                    i = 1;
                }
            }
        }
        else
        {
            if (block->mode[0] == 1)
            {
                i = 1;
            }
            else if (block->mode[0] == 2)
            {
                i = 3;
            }
        }
        u = GetInPortPtrs(block, i);
        so = GetSizeOfOut(block, 1);
        memcpy(y, u, mu * nu * so);
    }
    else if (flag == 9)
    {
        phase = get_phase_simulation();
        block->g[0] = *u2 - *rpar;
        if (phase == 1)
        {
            i = 3;
            if (ipar == 0)
            {
                if (block->g[0] >= 0.0)
                {
                    i = 1;
                }
            }
            else if (ipar == 1)
            {
                if (block->g[0] > 0.0)
                {
                    i = 1;
                }
            }
            else
            {
                if (block->g[0] != 0.0)
                {
                    i = 1;
                }
            }
            if (i == 1)
            {
                block->mode[0] = 1;
            }
            else
            {
                block->mode[0] = 2;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
