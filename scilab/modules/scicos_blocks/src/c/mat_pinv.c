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
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dgesvd)(char *jobu, char *jobvt, int *m, int *n,
                       double *a, int *lda, double *s, double *u, int *
                       ldu, double *vt, int *ldvt, double *work, int *lwork,
                       int *info);
extern int C2F(dlaset)(char *uplo, int *m, int *n, double *
                       alpha, double *beta, double *a, int *lda);
extern int C2F(dlacpy)(char *uplo, int *m, int *n, double *
                       a, int *lda, double *b, int *ldb);
extern int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__,
                      int *nc, int *l, int *m, int *n);
/*--------------------------------------------------------------------------*/
typedef struct
{
    double *l0;
    double *LA;
    double *LC;
    double *LS;
    double *LSV;
    double *LSW;
    double *LU;
    double *LUT;
    double *LV;
    double *LVT;
    double *dwork;
} mat_pinv_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_pinv(scicos_block *block, int flag)
{
    double *u = NULL;
    double *y = NULL;
    int nu = 0, mu = 0;
    int info = 0;
    int i = 0, j = 0, ij = 0, ji = 0, ii = 0, lwork = 0;
    mat_pinv_struct** work = (mat_pinv_struct**) block->work;
    mat_pinv_struct *ptr = NULL;

    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);

    /* for lapack 3.1 (2006)*/
    lwork = Max(3 * Min(mu, nu) + Max(mu, nu), 5 * Min(mu, nu));
    lwork = Max(1, lwork);

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
        if ((ptr->LA = (double*) scicos_malloc(sizeof(double) * (mu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LC = (double*) scicos_malloc(sizeof(double) * (nu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LS = (double*) scicos_malloc(sizeof(double) * (nu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LSV = (double*) scicos_malloc(sizeof(double) * (Min(mu, nu)))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LSW = (double*) scicos_malloc(sizeof(double) * (Min(mu, nu)))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LU = (double*) scicos_malloc(sizeof(double) * (mu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LUT = (double*) scicos_malloc(sizeof(double) * (mu * mu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LU);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LV = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LUT);
            scicos_free(ptr->LU);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVT = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LV);
            scicos_free(ptr->LUT);
            scicos_free(ptr->LU);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
            scicos_free(ptr->LA);
            scicos_free(ptr->l0);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * lwork)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVT);
            scicos_free(ptr->LV);
            scicos_free(ptr->LUT);
            scicos_free(ptr->LU);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LS);
            scicos_free(ptr->LC);
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
        if ((ptr->dwork) != 0)
        {
            scicos_free(ptr->LC);
            scicos_free(ptr->l0);
            scicos_free(ptr->LA);
            scicos_free(ptr->LSW);
            scicos_free(ptr->LS);
            scicos_free(ptr->LSV);
            scicos_free(ptr->LU);
            scicos_free(ptr->LUT);
            scicos_free(ptr->LV);
            scicos_free(ptr->LVT);
            scicos_free(ptr->dwork);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        for (i = 0; i < mu * nu; i++)
        {
            y[i] = 0;
        }
        C2F(dlacpy)("F", &mu, &nu, u, &mu, ptr->LA, &mu);
        C2F(dgesvd)("A", "A", &mu, &nu, ptr->LA, &mu, ptr->LSV, ptr->LU, &mu, ptr->LVT, &nu, ptr->dwork, &lwork, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
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
        *(ptr->l0) = 0;
        C2F(dlaset)("F", &nu, &mu, ptr->l0, ptr->l0, ptr->LS, &nu);
        for (i = 0; i < Min(mu, nu); i++)
        {
            ii = i + i * nu;
            *(ptr->LS + ii) = *(ptr->LSW + i);
        }
        for (j = 0; j < mu; j++)
        {
            for (i = j; i < mu; i++)
            {
                ij = i + j * mu;
                ji = j + i * mu;
                *(ptr->LUT + ij) = *(ptr->LU + ji);
                *(ptr->LUT + ji) = *(ptr->LU + ij);
            }
        }
        for (j = 0; j < nu; j++)
        {
            for (i = j; i < nu; i++)
            {
                ij = i + j * nu;
                ji = j + i * nu;
                *(ptr->LV + ij) = *(ptr->LVT + ji);
                *(ptr->LV + ji) = *(ptr->LVT + ij);
            }
        }
        C2F(dmmul)(ptr->LV, &nu, ptr->LS, &nu, ptr->LC, &nu, &nu, &nu, &mu);
        C2F(dmmul)(ptr->LC, &nu, ptr->LUT, &mu, y, &nu, &nu, &mu, &mu);
    }
}
/*--------------------------------------------------------------------------*/
