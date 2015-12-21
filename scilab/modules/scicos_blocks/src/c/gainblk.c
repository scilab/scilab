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
#include "machine.h" /* C2F */
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
#include "dmmul.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void gainblk(scicos_block *block, int flag)
{
    int i = 0;

    double *u = NULL;
    double *y = NULL;
    int nu = 0, ny = 0, my = 0;
    double *rpar = NULL;
    int nrpar = 0;

    nu = GetInPortRows(block, 1);
    ny = GetOutPortRows(block, 1);
    my = GetOutPortCols(block, 1);

    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);

    nrpar = GetNrpar(block);

    rpar = GetRparPtrs(block);

    if (nrpar == 1)
    {
        for (i = 0; i < nu * my; ++i)
        {
            y[i] = rpar[0] * u[i];
        }
    }
    else
    {
        dmmul(rpar, &ny, u, &nu, y, &ny, &ny, &nu, &my);
    }
}
/*--------------------------------------------------------------------------*/
