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
#include "scicos.h"
#include "scicos_block4.h"
#include "sci_malloc.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "scicos_evalhermite.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
/*    Masoud Najafi, August 2007 */
/*    Copyright INRIA
 *    Scicos block simulator
 *    Signal builder block
 */
/*--------------------------------------------------------------------------*/
#define rpar     block->rpar
#define nPoints  block->ipar[0]
#define Order    block->ipar[1]
#define Periodic block->ipar[2]
#define T        rpar[nPoints-1]-rpar[0]
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void curve_c(scicos_block *block, int flag)
{
    double t = 0., a = 0., b = 0., c = 0., y1 = 0., y2 = 0., t1 = 0., t2 = 0.;
    int** work = (int**) block->work;
    int *ind = NULL, i = 0, inow = 0;
    double *y = NULL;
    double  d1 = 0., d2 = 0., h = 0., dh = 0., ddh = 0., dddh = 0.;

    switch (flag)
    {
        /* init */
        case 4  :  /* the workspace is used to store discrete counter value */
        {
            if ((*work = (int*) scicos_malloc(4 * sizeof(int))) == NULL)
            {
                set_block_error(-16);
                return;
            }
            ind = *work;
            ind[0] = nPoints - 1;
            ind[1] = nPoints;
            for (i = 0; i < nPoints; i++)
            {
                if (rpar[i] >= 0 )
                {
                    ind[0] = i - 1;
                    ind[1] = i;
                    break;
                }
            }
            ind[0] = -1;
            ind[1] = 0;
            ind[2] = 0; /* event index */
            ind[3] = 0; /* event counter */
            return;

            break;
        }
        /* event date computation */
        case 1  :
        {
            y = GetRealOutPortPtrs(block, 1);
            ind = *work;
            t = get_scicos_time();
            if (Periodic == 1)
            {
                if (ind[3] > 0)
                {
                    t = t - (ind[3] - 1) * T;
                }
            }

            inow = nPoints - 1;
            for (i = ind[0]; i < nPoints; i++)
            {
                if (i == -1)
                {
                    continue;
                }
                if (t < rpar[i])
                {
                    inow = i - 1;
                    if (inow < ind[1])
                    {
                        ind[1] = inow;
                    }
                    else
                    {
                        ind[0] = ind[1];
                        ind[1] = inow;
                    }
                    break;
                }
            }

            if (inow < 0)
            {
                y[0] = 0.0;
                break;
            }
            if (inow >= nPoints - 1)
            {
                y[0] = rpar[nPoints * 2 - 1];
                break;
            }

            if (Order == 0)
            {
                y[0] = rpar[nPoints + inow];
                break;
            }
            if (Order == 1)
            {
                t1 = rpar[inow];
                t2 = rpar[inow + 1];
                y1 = rpar[nPoints + inow];
                y2 = rpar[nPoints + inow + 1];
                y[0] = (y2 - y1) * (t - t1) / (t2 - t1) + y1;
                break;
            }

            if ((Order == 2) && (nPoints > 2))
            {
                t1 = rpar[inow];
                a = rpar[2 * nPoints + inow];
                b = rpar[2 * nPoints + inow + nPoints - 1];
                c = rpar[2 * nPoints + inow + 2 * nPoints - 2];
                y[0] = a * (t - t1) * (t - t1) + b * (t - t1) + c;
                break;
            }

            if ((Order >= 3))
            {
                t1 = rpar[inow];
                t2 = rpar[inow + 1];
                y1 = rpar[nPoints + inow];
                y2 = rpar[nPoints + inow + 1];
                d1 = rpar[2 * nPoints + inow];
                d2 = rpar[2 * nPoints + inow + 1];
                scicos_evalhermite(&t, &t1, &t2, &y1, &y2, &d1, &d2, &h, &dh, &ddh, &dddh, &inow);
                y[0] = h;
                break;
            }

            break;
        }
        /* event date computation */
        case 3  :
        {
            ind = *work;

            /*---------*/
            if ((Order == 1) || (Order == 0))
            {
                i = ind[2];
                if (i == nPoints - 1)
                {
                    if (Periodic == 1)
                    {
                        i = 0;
                        ind[0] = -1;
                        ind[1] = 0;
                    }
                }
                if (i < nPoints - 1)
                {
                    block->evout[0] = rpar[i + 1] - rpar[i];

                    ind[2] = i + 1;
                }
                if (ind[2] == 1)
                {
                    ind[3]++;
                }
            }
            /*-------------------*/
            if (Order >= 2)
            {
                if ( Periodic)
                {
                    block->evout[0] = T;
                }
                else
                {
                    if (ind[3] == 0)
                    {
                        block->evout[0] = T;
                    }
                }
                ind[3]++;
                ind[0] = -1;
                ind[1] = 0;

            }
            break;
        }

        /* finish */
        case 5  :
        {
            scicos_free(*work); /*free the workspace*/
            break;
        }

        default :
            break;
    }
}
/*--------------------------------------------------------------------------*/
