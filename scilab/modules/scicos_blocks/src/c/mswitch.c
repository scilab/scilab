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
#include <string.h>
#include <math.h>
#include "core_math.h"
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void  mswitch(scicos_block *block, int flag)
{
    if ((flag == 1) || (flag == 6))
    {
        int i = 0, j = 0, nin = 0, so = 0, my = 0, ny = 0;
        int mu = 0, nu = 0;
        int *ipar = NULL;
        double *u1 = NULL;
        void *uj = NULL;
        void *y = NULL;

        y = GetOutPortPtrs(block, 1);
        so = GetSizeOfOut(block, 1);
        my = GetOutPortRows(block, 1);
        ny = GetOutPortCols(block, 1);
        u1 = GetRealInPortPtrs(block, 1);
        ipar = GetIparPtrs(block);
        nin = GetNin(block);
        i = *(ipar + 1);
        if (i == 0)
        {
            if (*u1 > 0)
            {
                j = (int)floor(*u1);
            }
            else
            {
                j = (int)ceil(*u1);
            }
        }
        else if (i == 1)
        {
            if (*u1 > 0)
            {
                j = (int)floor(*u1 + .5);
            }
            else
            {
                j = (int)ceil(*u1 - .5);
            }
        }
        else if (i == 2)
        {
            j = (int)ceil(*u1);
        }
        else if (i == 3)
        {
            j = (int)floor(*u1);
        }
        j = j + 1 - *ipar;
        j = Max(j, 1);
        if (nin == 2)
        {
            mu = GetInPortRows(block, 2);
            nu = GetInPortCols(block, 2);
            uj = GetInPortPtrs(block, 2);
            j = Min(j, mu * nu);
            memcpy(y, (char *)uj + (j - 1)*my * ny * so, my * ny * so);
        }
        else
        {
            j = Min(j, nin - 1);
            uj = GetInPortPtrs(block, j + 1);
            memcpy(y, uj, my * ny * so);
        }
    }
}
/*--------------------------------------------------------------------------*/
