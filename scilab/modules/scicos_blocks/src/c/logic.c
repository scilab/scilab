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
SCICOS_BLOCKS_IMPEXP void logic(scicos_block *block, int flag)
{
    char *u = NULL, *y = NULL;
    char inp, num;
    int i  = 0, nin = 0, nout = 0, mo = 0;
    char *opar = NULL;
    nin = GetNin(block);
    nout = GetNout(block);
    opar = Getint8OparPtrs(block, 1);
    mo = GetOparSize(block, 1, 1);
    if (flag == 1)
    {
        num = 0;
        for (i = 0; i < nin; i++)
        {
            u = Getint8InPortPtrs(block, i + 1);
            inp = *u;
            if (inp > 0)
            {
                inp = 1;
            }
            else
            {
                inp = 0;
            }
            inp = inp << i;
            num = num + inp;
        }
        for (i = 0; i < nout; i++)
        {
            y = Getint8OutPortPtrs(block, i + 1);
            *y = *(opar + num + i * mo);
        }
    }
    if (flag == 6)
    {
        u = Getint8InPortPtrs(block, 1);
        for (i = 0; i < nout; i++)
        {
            y = Getint8OutPortPtrs(block, i + 1);
            *y = *u;
        }
    }
}
/*--------------------------------------------------------------------------*/
