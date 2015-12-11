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
extern int C2F(wmmul)(double* Ar, double* Ai, int* na, double* Br, double* Bi, int* nb, double* Cr, double* Ci, int* nc, int* l, int* m, int* n);
extern int C2F(dlaset)(char *uplo, int *m, int *n, double *
                       alpha, double *beta, double *a, int *lda);
/*--------------------------------------------------------------------------*/
typedef struct
{
    double *l0;
    double *LA;
    double *LU;
    double *LUr, *LUi;
    double *LCr, *LCi;
    double *LSV;
    double *LSr, *LSi;
    double *LSW;
    double *LVT;
    double *LVr, *LVi;
    double *dwork;
    double *rwork;
} mat_pinv_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_pinv(scicos_block *block, int flag)
{
    double *ur = NULL, *ui = NULL;
    double *yr = NULL;
    double *yi = NULL;
    int nu = 0, mu = 0;
    int info = 0;
    int i = 0, j = 0, ij = 0, ji = 0, ii = 0, lwork = 0, rw = 0;

    mat_pinv_struct** work = (mat_pinv_struct**) block->work;
    mat_pinv_struct *ptr = NULL;

    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    lwork = Max(3 * Min(mu, nu) + Max(mu, nu), 5 * Min(mu, nu) - 4);
    rw = 5 * Min(mu, nu);
    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_pinv_struct*) scicos_malloc(sizeof(mat_pinv_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->l0 = (double*) scicos_malloc(sizeof(double))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LA = (double*) scicos_malloc(sizeof(double) * (2 * mu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LU = (double*) scicos_malloc(sizeof(double) * (2 * mu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LCr = (double*) scicos_malloc(sizeof(double) * (nu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LCi = (double*) scicos_malloc(sizeof(double) * (nu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LUr = (double*) scicos_malloc(sizeof(double) * (mu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LUi = (double*) scicos_malloc(sizeof(double) * (mu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LSV = (double*) scicos_malloc(sizeof(double) * (Min(mu, nu)))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LSW = (double*) scicos_malloc(sizeof(double) * (Min(mu, nu)))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LSr = (double*) scicos_malloc(sizeof(double) * (mu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LSi = (double*) scicos_malloc(sizeof(double) * (mu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVT = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LSi);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVi = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LSi);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVr = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVi);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LSi);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * 2 * lwork)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVr);
            scicos_free(ptr->LVi);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LSi);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->rwork = (double*) scicos_malloc(sizeof(double) * 2 * rw)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LVr);
            scicos_free(ptr->LVi);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LSi);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LU);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
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
            scicos_free(ptr->l0);
            scicos_free(ptr->LA);
            scicos_free(ptr->LU);
            scicos_free(ptr->LUr);
            scicos_free(ptr->LUi);
            scicos_free(ptr->LCr);
            scicos_free(ptr->LCi);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSr);
            scicos_free(ptr->LSi);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LVr);
            scicos_free(ptr->LVi);
            scicos_free(ptr->dwork);
            scicos_free(ptr->rwork);
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
        C2F(zgesvd)("A", "A", &mu, &nu, ptr->LA, &mu, ptr->LSV, ptr->LU, &mu, ptr->LVT, &nu, ptr->dwork, &lwork, ptr->rwork, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }

        *(ptr->l0) = 0;
        C2F(dlaset)("F", &nu, &mu, ptr->l0, ptr->l0, ptr->LSi, &nu);
        C2F(dlaset)("F", &nu, &mu, ptr->l0, ptr->l0, ptr->LSr, &nu);
        for (i = 0; i < Min(mu, nu); i++)
        {
            if (*(ptr->LSV + i) != 0)
            {
                *(ptr->LSW + i) = 1 / (*(ptr->LSV + i));
            }
            else
            {
                *(ptr->LSW + i) = 0;
            }
        }
        for (i = 0; i < Min(mu, nu); i++)
        {
            ii = i + i * nu;
            *(ptr->LSr + ii) = *(ptr->LSW + i);
        }
        for (j = 0; j < nu; j++)
        {
            for (i = j; i < nu; i++)
            {
                ij = i + j * nu;
                ji = j + i * nu;
                *(ptr->LVr + ij) = *(ptr->LVT + 2 * ji);
                *(ptr->LVr + ji) = *(ptr->LVT + 2 * ij);
                *(ptr->LVi + ij) = -(*(ptr->LVT + 2 * ji + 1));
                *(ptr->LVi + ji) = -(*(ptr->LVT + 2 * ij + 1));
            }
        }
        for (j = 0; j < mu; j++)
        {
            for (i = j; i < mu; i++)
            {
                ij = i + j * mu;
                ji = j + i * mu;
                *(ptr->LUr + ij) = *(ptr->LU + 2 * ji);
                *(ptr->LUr + ji) = *(ptr->LU + 2 * ij);
                *(ptr->LUi + ij) = -(*(ptr->LU + 2 * ji + 1));
                *(ptr->LUi + ji) = -(*(ptr->LU + 2 * ij + 1));
            }
        }
        C2F(wmmul)(ptr->LVr, ptr->LVi, &nu, ptr->LSr, ptr->LSi, &nu, ptr->LCr, ptr->LCi, &nu, &nu, &nu, &mu);
        C2F(wmmul)(ptr->LCr, ptr->LCi, &nu, ptr->LUr, ptr->LUi, &mu, yr, yi, &nu, &nu, &mu, &mu);
    }
}
/*--------------------------------------------------------------------------*/
