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
#include "dynlib_scicos_blocks.h"

SCICOS_BLOCKS_IMPEXP void zcross2(int *flag, int *nevprt,
                                  double *t, double xd[],
                                  double x[], int *nx,
                                  double z[], int *nz,
                                  double tvec[], int *ntvec,
                                  double rpar[], int *nrpar,
                                  int ipar[], int *nipar,
                                  double u[], int *nu,
                                  double g[], int *ng)
{
    int i = 0, j = 0;
    int surface_matched = 0, exist_enabled_surface = 0;
    exist_enabled_surface = 0;

    if ((*flag == 3) && (*nevprt < 0))
    {
        for (i = 0; i < *ntvec; i++)
        {
            surface_matched = 1;
            exist_enabled_surface = 0;

            for (j = 0; j < *ng; j++)
            {
                if (rpar[(*ng + 1)*i + j] != 0)
                {
                    exist_enabled_surface = 1;
                    if ((rpar[(*ng + 1)*i + j]*g[j]) <= 0)
                    {
                        surface_matched = 0;
                    }
                }
            }

            if (( surface_matched == 1) && (exist_enabled_surface == 1))
            {
                tvec[i] = *t + rpar[(*ng + 1) * i + *ng];
            }
            else
            {
                tvec[i] = -1;
            }
        }
    }
    else
    {
        if (*flag == 9)
        {
            for (i = 0; i < *ng; i++)
            {
                g[i] = u[i];
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
