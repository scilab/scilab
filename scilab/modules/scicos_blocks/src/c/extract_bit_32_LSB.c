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
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void extract_bit_32_LSB(scicos_block *block, int flag)
{
    int i = 0;
    SCSINT32_COP *y, *u, ref, n;
    int *ipar;
    y = Getint32OutPortPtrs(block, 1);
    u = Getint32InPortPtrs(block, 1);
    ipar = GetIparPtrs(block);
    ref = 0;
    for (i = 0; i < *ipar; i++)
    {
        n = (SCSINT32_COP)pow(2, i);
        ref = ref + n;
    }
    *y = (*u) & (ref);
}
/*--------------------------------------------------------------------------*/
