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
#include <stdio.h>
#include "machine.h"
#include "sci_malloc.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dgetrf)(int *m, int *n, double *a, int *
                       lda, int *ipiv, int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int *ipiv;
    double *wrk;
} mat_det_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_det(scicos_block *block, int flag)
{
    double *u = NULL;
    double *y = NULL;
    int nu = 0;
    int info = 0;
    int i = 0;
    double D = 0., l = 0.;
    mat_det_struct** work = (mat_det_struct**) block->work;
    mat_det_struct *mdet = NULL;

    nu = GetInPortRows(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);

    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_det_struct*) scicos_malloc(sizeof(mat_det_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        mdet = *work;
        if ((mdet->ipiv = (int*) scicos_malloc(sizeof(int) * nu)) == NULL)
        {
            set_block_error(-16);
            scicos_free(mdet);
            return;
        }
        if ((mdet->wrk = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(mdet->ipiv);
            scicos_free(mdet);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        mdet = *work;
        if (mdet->wrk != NULL)
        {
            scicos_free(mdet->ipiv);
            scicos_free(mdet->wrk);
            scicos_free(mdet);
            return;
        }
    }

    else
    {
        mdet = *work;
        for (i = 0; i < (nu * nu); i++)
        {
            mdet->wrk[i] = u[i];
        }
        C2F(dgetrf)(&nu, &nu, mdet->wrk, &nu, mdet->ipiv, &info);
        if (info < 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        D = 1;
        for (i = 0; i < nu; i++)
        {
            if ((*(mdet->ipiv + i)) != i + 1)
            {
                D = -D;
            }
            l = *(mdet->wrk + i * (nu + 1));
            D = D * l;
        };
        *y = D;
    }
}
/*--------------------------------------------------------------------------*/
