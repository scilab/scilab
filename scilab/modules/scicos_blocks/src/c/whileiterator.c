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
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void whileiterator(scicos_block *block, int flag)
{
    void *y = NULL, *z = NULL;
    int so = 0;

    z = GetOzPtrs(block, 1);
    so = GetSizeOfOut(block, 1);

    if (flag == 1)
    {
        y = GetOutPortPtrs(block, 1);
        memcpy(y, z, so);
    }
    else if (flag == 2)
    {
        if (so == 1)
        {
            char *inp;
            inp = (char *)z;
            *inp = *inp + 1;
        }
        else if (so == 2)
        {
            short *inp;
            inp = (short *)z;
            *inp = *inp + 1;
        }
        else if (so == 4)
        {
            long *inp;
            inp = (long *)z;
            *inp = *inp + 1;
        }
        else if (so == 8)
        {
            double *inp;
            inp = (double *)z;
            *inp = *inp + 1;
        }
    }
}
/*--------------------------------------------------------------------------*/




