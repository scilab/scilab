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
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void delay4(scicos_block *block, int flag)
{
    /* Copyright INRIA

       Scicos block simulator
       Ouputs nx*dt delayed input */

    double* y = block->outptr[0];
    double* z = block->z;
    int nz = block->nz;
    double* u = block->inptr[0];

    int i;

    if (flag == 1 || flag == 4 || flag == 6)
    {
        y[0] = z[0];
    }
    else if (flag == 2)
    {
        /*  shift buffer */
        for (i = 0; i <= nz - 2; i++)
        {
            z[i] = z[i + 1];
        }
        /* add new point to the buffer */
        z[nz - 1] = u[0];
    }
}
/*--------------------------------------------------------------------------*/

