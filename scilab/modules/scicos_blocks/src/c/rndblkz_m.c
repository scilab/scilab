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
#include "machine.h"
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern double C2F(urand)(int* iy);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void rndblkz_m(scicos_block *block, int flag)
{
    double *y_r = NULL;
    double *y_i = NULL;
    double *rpar = NULL;
    double *z = NULL;
    int *ipar = NULL;
    int ny = 0, my = 0, i = 0, iy_r = 0, iy_i = 0;
    double sr = 0., si = 0., tl = 0.;

    my = GetOutPortRows(block, 1);
    ny = GetOutPortCols(block, 1);
    ipar = GetIparPtrs(block);
    rpar = GetRparPtrs(block);
    y_r = GetRealOutPortPtrs(block, 1);
    y_i = GetImagOutPortPtrs(block, 1);
    z = GetDstate(block);

    if (flag == 2 || flag == 4)
    {
        iy_r = (int)z[0];
        iy_i = (int)z[1];
        if (ipar[0] == 0)
        {
            for (i = 0; i < my * ny; i++)
            {
                *(z + i + 2) = C2F(urand)(&iy_r);
                *(z + i + 2 + my * ny) = C2F(urand)(&iy_i);
            }
        }
        else
        {
            for (i = 0; i < my * ny; i++)
            {
                do
                {
                    sr = 2.0 * C2F(urand)(&iy_r) - 1.0;
                    si = 2.0 * C2F(urand)(&iy_r) - 1.0;
                    tl = sr * sr + si * si;
                }
                while (tl > 1.0);
                z[i + 2] = sr * (sqrt(-2.0 * log(tl) / tl));
                do
                {
                    sr = 2.0 * C2F(urand)(&iy_i) - 1.0;
                    si = 2.0 * C2F(urand)(&iy_i) - 1.0;
                    tl = sr * sr + si * si;
                }
                while (tl > 1.0);
                z[i + 2 + my * ny] = sr * (sqrt(-2.0 * log(tl) / tl));
            }
        }
        *(z) = iy_r;
        *(z + 1) = iy_i;
    }

    if (flag == 1 || flag == 6)
    {
        for (i = 0; i < my * ny; i++)
        {
            *(y_r + i) = *(rpar + i) + (*(rpar + i + 2 * my * ny)) * (*(z + i + 2));
            *(y_i + i) = *(rpar + i + my * ny) + (*(rpar + i + 3 * my * ny)) * (*(z + i + 2 + my * ny));
        }
    }
}
/*--------------------------------------------------------------------------*/
