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
#include "core_math.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(zgesvd)(char *jobu, char *jobvt, int *m, int *n,
                       double *a, int *lda, double *s, double *u,
                       int *ldu, double *vt, int *ldvt, double *work,
                       int *lwork, double *rwork, int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    double *LA;
    double *LX;
    double *LU;
    double *LVT;
    double *dwork;
    double *rwork;
} mat_sing_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_sing(scicos_block *block, int flag)
{
    double *ur = NULL;
    double *ui = NULL;
    double *yr = NULL;
    int nu = 0, mu = 0;
    int info = 0;
    int i = 0, rw = 0, lwork = 0;
    mat_sing_struct** work = (mat_sing_struct**) block->work;
    mat_sing_struct *ptr = NULL;

    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    lwork = Max(3 * Min(mu, nu) + Max(mu, nu), 5 * Min(mu, nu) - 4);
    rw = 5 * Min(mu, nu);

    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_sing_struct*) scicos_malloc(sizeof(mat_sing_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->LA = (double*) scicos_malloc(sizeof(double) * (2 * mu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LU = (double*) scicos_malloc(sizeof(double) * (2 * mu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVT = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LX = (double*) scicos_malloc(sizeof(double) * (2 * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * 2 * lwork)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LX);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->rwork = (double*) scicos_malloc(sizeof(double) * 2 * rw)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LX);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->rwork) != NULL)
        {
            scicos_free(ptr->LA);
            scicos_free(ptr->LU);
            scicos_free(ptr->LX);
            scicos_free(ptr->LVT);
            scicos_free(ptr->rwork);
            scicos_free(ptr->dwork);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        for (i = 0; i < (mu * nu); i++)
        {
            ptr->LA[2 * i] = ur[i];
            ptr->LA[2 * i + 1] = ui[i];
        }
        C2F(zgesvd)("A", "A", &mu, &nu, ptr->LA, &mu, yr, ptr->LU, &mu, ptr->LVT, &nu, ptr->dwork, &lwork, ptr->rwork, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
