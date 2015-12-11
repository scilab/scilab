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
#include "machine.h" /* C2F */
#include "sci_malloc.h"
#include "scicos.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(zgetrf)(int *m, int *n, double *a,
                       int *lda, int *ipiv, int *info);
/*--------------------------------------------------------------------------*/
typedef struct FCOMPLEX
{
    double r, i;
} fcomplex;

typedef struct
{
    int *ipiv;
    double *wrk;
} mat_det_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_det(scicos_block *block, int flag)
{
    double *ur = NULL;
    double *yr = NULL;
    double *ui = NULL;
    double *yi = NULL;
    int nu = 0;
    int info = 0;
    int i = 0;
    fcomplex D, l;
    double A = 0.;
    mat_det_struct** work = (mat_det_struct**) block->work;
    mat_det_struct *mdet = NULL;

    nu = GetInPortRows(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
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
        if ((mdet->wrk = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
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
            mdet->wrk[2 * i] = ur[i];
            mdet->wrk[2 * i + 1] = ui[i];
        }
        C2F(zgetrf)(&nu, &nu, mdet->wrk, &nu, mdet->ipiv, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        D.r = 1;
        D.i = 0;
        for (i = 0; i < nu; i++)
        {
            if ((*(mdet->ipiv + i)) != i + 1)
            {
                D.r = -D.r;
                D.i = -D.i;
            }
            l.r = *(mdet->wrk + i * 2 * (nu + 1));
            l.i = *(mdet->wrk + 1 + i * 2 * (nu + 1));
            A = D.r;
            D.r = D.r * l.r - D.i * l.i;
            D.i = D.i * l.r + A * l.i;
        };
        *yr = D.r;
        *yi = D.i;
    }
}
/*--------------------------------------------------------------------------*/
