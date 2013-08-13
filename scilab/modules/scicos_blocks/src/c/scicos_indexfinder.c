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
#include "scicos_indexfinder.h"
/*--------------------------------------------------------------------------*/
int scicos_indexfinder(double x, int n, double *LT)
{
    int i1 = 0, i2 = 0, i_mid = 0;

    /* if X(k-1)<= x < X(k) then i2=k */
    if (x <= LT[0]  )
    {
        return 1;
    }
    if (x >= LT[n - 1])
    {
        return n - 1;
    }
    i1 = 0;
    i2 = n - 1;

    while (i1 != i2 - 1)
    {
        i_mid = (int)((i1 + i2) / 2);
        if (x >= LT[i_mid])
        {
            i1 = i_mid;
        }
        else
        {
            i2 = i_mid;
        }
    }
    return i2;
}
/*--------------------------------------------------------------------------*/
