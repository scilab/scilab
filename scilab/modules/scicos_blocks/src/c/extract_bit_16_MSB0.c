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
SCICOS_BLOCKS_IMPEXP void extract_bit_16_MSB0(scicos_block *block, int flag)
{
    int i = 0, maxim = 0;
    short *y = NULL, *u = NULL, ref = 0, n = 0;
    int *ipar = NULL;
    y = Getint16OutPortPtrs(block, 1);
    u = Getint16InPortPtrs(block, 1);
    ipar = GetIparPtrs(block);
    maxim = 16;
    ref = 0;
    for (i = 0; i < *ipar; i++)
    {
        n = (short)pow(2, maxim - 1 - i);
        ref = ref + n;
    }
    *y = (*u) & (ref);
}
/*--------------------------------------------------------------------------*/
