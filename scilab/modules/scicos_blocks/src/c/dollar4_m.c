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
#include <memory.h>
#include "scicos_block4.h"
#include "scicos.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
typedef struct
{
    int s;
} dol_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void dollar4_m(scicos_block *block, int flag)
{
    /* c     Copyright INRIA

    Scicos block simulator
    Ouputs delayed input */


    int m = 0, n = 0;
    double *y = NULL, *u = NULL, *oz = NULL;
    dol_struct** work = (dol_struct**) block->work;
    dol_struct *ptr = NULL;
    m = GetInPortRows(block, 1);
    n = GetInPortCols(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);
    oz = GetRealOzPtrs(block, 1);

    if (flag == 4)
    {
        *work = (dol_struct*) scicos_malloc(sizeof(dol_struct));
        if (*work == NULL)
        {
            set_block_error(-5);
            return;
        }
        ptr = *work;
        ptr->s = GetSizeOfOz(block, 1);
    }
    if (flag == 1 || flag == 6)
    {
        ptr = *work;
        memcpy(y, oz, m * n * (ptr->s));
    }
    if (flag == 2)
    {
        ptr = *work;
        memcpy(oz, u, m * n * ptr->s);
    }
    if (flag == 5)
    {
        ptr = *work;
        if (ptr != NULL)
        {
            scicos_free(ptr);
        }
    }

}
/*--------------------------------------------------------------------------*/
