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
#include "scicos_block.h"
#include "scicos_print.h"
#include "localization.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void edgetrig(scicos_block *block, int flag)
{
    double z = block->z[0], u = block->inptr[0][0];
    if (flag == 2 || flag == 6)
    {
        block->z[0] = u;
    }
    else if (flag == 1)
    {
        if (block->ipar[0] != 0)
        {
            z = z * block->ipar[0];
            u = u * block->ipar[0];
            if (((z <= 0) & (u > 0)) || ((z < 0) & (u >= 0)))
            {
                block->outptr[0][0] = 1.;
            }
            else
            {
                block->outptr[0][0] = 0.;
            }
        }
        else    /* rising and falling edge */
        {
            if (((z <= 0) & (u > 0)) || ((z < 0) & (u >= 0)) || ((z > 0) & (u <= 0)) || ((z >= 0) & (u < 0)))
            {
                block->outptr[0][0] = 1.;
            }
            else
            {
                block->outptr[0][0] = 0.;
            }
        }
    }
    else if (flag == 4)
    {
        if (block->ng > 0)
        {
            set_block_error(-1);
            scicos_print(_("Trigger block must have discrete time input."));
            return;
        }
    }
}
/*--------------------------------------------------------------------------*/
