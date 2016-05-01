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
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
/*     February 2008 */
/* Scicos block simulator */
/*--------------------------------------------------------------------------*/
#define NX      block->nx
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void diffblk_c(scicos_block *block, int flag)
{
    double *y = NULL, *u = NULL;
    int i = 0;
    int* property = GetXpropPtrs(block);
    switch (flag)
    {
        case 4  :
        {
            for (i = 0; i < NX; i++)
            {
                property[i] = -1;    /* xproperties*/
            }
            break;
        }

        case 6  :
        {
            u = GetRealInPortPtrs(block, 1);
            //	for (i=0;i<NX;i++)  block->x[i]=u[i];
            break;
        }

        case 7  :
        {
            for (i = 0; i < NX; i++)
            {
                property[i] = -1;    /* xproperties*/
            }
            break;
        }

        case 0  :
        {
            u = GetRealInPortPtrs(block, 1);
            for (i = 0; i < NX; i++)
            {
                block->res[i] = u[i] - block->x[i];
            }
            break;
        }

        case 1  :
        {
            y = GetRealOutPortPtrs(block, 1);
            for (i = 0; i < NX; i++)
            {
                y[i] = block->xd[i];
            }
            break;
        }

        default :
            break;
    }
}
/*--------------------------------------------------------------------------*/
