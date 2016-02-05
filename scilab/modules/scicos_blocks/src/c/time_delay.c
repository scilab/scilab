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
#include "sci_malloc.h"
#include "scicos_print.h"
#include "scicos_block.h"
#include "localization.h"
#include "dynlib_scicos_blocks.h"
#include "scicos_free.h"
#include "scicos_malloc.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void time_delay(scicos_block *block, int flag)
{
    /*  rpar[0]=delay, rpar[1]=init value, ipar[0]=buffer length */
    double** work = (double**) block->work;
    double *pw = NULL, del = 0., t = 0., td = 0., eps = 0.;
    int* iw = NULL;
    int i = 0, j = 0, k = 0;

    if (flag == 4)
    {
        /* the workspace is used to store previous values */
        if ((*work = (double*) scicos_malloc(sizeof(int) + sizeof(double) *
                                             block->ipar[0] * (1 + block->insz[0]))) == NULL )
        {
            set_block_error(-16);
            return;
        }
        eps = 1.0e-9; /* shift times to left to avoid replacing 0 */
        pw = *work;
        pw[0] = -block->rpar[0] * (block->ipar[0] - 1) - eps;
        for (j = 1; j < block->insz[0] + 1; j++)
        {
            pw[block->ipar[0]*j] = block->rpar[1];
        }

        for (i = 1; i < block->ipar[0]; i++)
        {
            pw[i] = pw[i - 1] + block->rpar[0] - eps;
            for (j = 1; j < block->insz[0] + 1; j++)
            {
                pw[i + block->ipar[0]*j] = block->rpar[1];
            }
        }

        iw = (int *)(pw + block->ipar[0] * (1 + block->insz[0]));
        *iw = 0;
        for (k = 0; k < block->insz[0]; k++)
        {
            block->outptr[0][k] = block->rpar[1];
        }
    }
    else  if (flag == 5)
    {
        scicos_free(*work);

    }
    else if (flag == 0 || flag == 2)
    {
        if (flag == 2)
        {
            do_cold_restart();
        }
        pw = *work;
        iw = (int *)(pw + block->ipar[0] * (1 + block->insz[0]));
        t = get_scicos_time();
        td = t - block->rpar[0];
        if (td < pw[*iw])
        {
            scicos_print(_("delayed time=%f but last stored time=%f \n"), td, pw[*iw]);
            scicos_print(_("Consider increasing the length of buffer in delay block \n"));
        }

        if (t > pw[(block->ipar[0] + *iw - 1) % block->ipar[0]])
        {
            for (j = 1; j < block->insz[0] + 1; j++)
            {
                pw[*iw + block->ipar[0]*j] = block->inptr[0][j - 1];
            }
            pw[*iw] = t;
            /*scicos_print(_("**time is %f. I put %f, in %d \n"), t,block->inptr[0][0],*iw);*/
            *iw = (*iw + 1) % block->ipar[0];

        }
        else
        {
            for (j = 1; j < block->insz[0] + 1; j++)
            {
                pw[(block->ipar[0] + *iw - 1) % block->ipar[0] + block->ipar[0]*j] = block->inptr[0][j - 1];
            }
            pw[(block->ipar[0] + *iw - 1) % block->ipar[0]] = t;
            /*scicos_print("**time is %f. I put %f, in %d \n", t,block->inptr[0][0],*iw);*/

        }

    }
    else if (flag == 1)
    {
        pw = *work;
        iw = (int *) (pw + block->ipar[0] * (1 + block->insz[0]));
        t = get_scicos_time();
        td = t - block->rpar[0];

        i = 0;
        j = block->ipar[0] - 1;

        while (j - i > 1)
        {
            k = (i + j) / 2;
            if (td < pw[(k + *iw) % block->ipar[0]])
            {
                j = k;
            }
            else if (td > pw[(k + *iw) % block->ipar[0]])
            {
                i = k;
            }
            else
            {
                i = k;
                j = k;
                break;
            }
        }
        i = (i + *iw) % block->ipar[0];
        j = (j + *iw) % block->ipar[0];
        del = pw[j] - pw[i];
        /*    scicos_print(_("time is %f. interpolating %d and %d, i.e. %f, %f\n"), t,i,j,pw[i],pw[j]);
        scicos_print(_("values are  %f   %f.\n"),pw[i+block->ipar[0]],pw[j+block->ipar[0]]);*/
        if (del != 0.0)
        {
            for (k = 1; k < block->insz[0] + 1; k++)
            {
                block->outptr[0][k - 1] = ((pw[j] - td) * pw[i + block->ipar[0] * k] +
                                           (td - pw[i]) * pw[j + block->ipar[0] * k]) / del;
            }
        }
        else
        {
            for (k = 1; k < block->insz[0] + 1; k++)
            {
                block->outptr[0][k - 1] = pw[i + block->ipar[0] * k];
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
