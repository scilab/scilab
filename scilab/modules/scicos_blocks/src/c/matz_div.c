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
#include <math.h>
#include "machine.h" /* C2F */
#include "sci_malloc.h"
#include "scicos.h"
#include "core_math.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
#include "numericconstants_interface.h"
/*--------------------------------------------------------------------------*/
extern int C2F(zlacpy)(char *uplo, int *m, int *n,
                       double *a, int *lda, double *b, int *ldb);
extern int C2F(zgetrf)(int *m, int *n, double *a,
                       int *lda, int *ipiv, int *info);
extern int C2F(mtran)(double* a, int* na, double* b, int* nb, int* m, int* n);
extern double C2F(zlange)(char *norm, int *m, int *n, double *a,
                          int *lda, double *work);
extern int C2F(zgecon)(char *norm, int *n, double *a,
                       int *lda, double *anorm, double *rcond, double *
                       work, double *rwork, int *info);
extern int C2F(zgetrs)(char *trans, int *n, int *nrhs,
                       double *a, int *lda, int *ipiv, double *b,
                       int *ldb, int *info);
extern int C2F(zgelsy1)(int* M, int* N, int* NRHS, double* A, int* LDA, double* B, int* LDB, int* JPVT, double* RCOND, int* RANK, double* WORK, int* LWORK, double* RWORK, int* INFO);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int *ipiv;
    int *rank;
    int *jpvt;
    double *iwork;
    double *dwork;
    double *IN1F;
    double *IN1;
    double *urT1, *uiT1;
    double *IN2X;
    double *IN2;
    double *urT2, *uiT2;
    double *yrT, *yiT;
} mat_bksl_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_div(scicos_block *block, int flag)
{
    double *u1r = NULL, *u1i = NULL;
    double *u2r = NULL, *u2i = NULL;
    double *yr = NULL, *yi = NULL;
    int mu1 = 0;
    int nu = 0;
    int mu2 = 0;
    int info = 0;
    int i = 0, l = 0, lw = 0, lu = 0;
    mat_bksl_struct** work = (mat_bksl_struct**) block->work;
    mat_bksl_struct *ptr = NULL;
    double rcond = 0., ANORM = 0., EPS = 0.;

    mu1 = GetInPortRows(block, 2);
    nu = GetInPortCols(block, 1);
    mu2 = GetInPortRows(block, 1);
    u1r = GetRealInPortPtrs(block, 2);
    u1i = GetImagInPortPtrs(block, 2);
    u2r = GetRealInPortPtrs(block, 1);
    u2i = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    l = Max(mu1, nu);
    lw = Max(2 * Min(mu1, nu), mu1 + 1);
    lu = Max(lw, Min(mu1, nu) + mu2);
    lw = Max(2 * nu, Min(mu1, nu) + lu);

    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_bksl_struct*) scicos_malloc(sizeof(mat_bksl_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->ipiv = (int*) scicos_malloc(sizeof(int) * nu)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->rank = (int*) scicos_malloc(sizeof(int))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->jpvt = (int*) scicos_malloc(sizeof(int) * mu1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->iwork = (double*) scicos_malloc(sizeof(double) * 2 * mu1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * 2 * lw)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->IN1F = (double*) scicos_malloc(sizeof(double) * (2 * mu1 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->IN1 = (double*) scicos_malloc(sizeof(double) * (2 * mu1 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->urT1 = (double*) scicos_malloc(sizeof(double) * (mu1 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->uiT1 = (double*) scicos_malloc(sizeof(double) * (mu1 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->IN2X = (double*) scicos_malloc(sizeof(double) * (2 * l * mu2))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->IN2 = (double*) scicos_malloc(sizeof(double) * (2 * mu2 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->IN2X);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->urT2 = (double*) scicos_malloc(sizeof(double) * (mu2 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->IN2);
            scicos_free(ptr->IN2X);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->uiT2 = (double*) scicos_malloc(sizeof(double) * (mu2 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->urT2);
            scicos_free(ptr->IN2);
            scicos_free(ptr->IN2X);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->yiT = (double*) scicos_malloc(sizeof(double) * (mu2 * l))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->uiT2);
            scicos_free(ptr->urT2);
            scicos_free(ptr->IN2);
            scicos_free(ptr->IN2X);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->yrT = (double*) scicos_malloc(sizeof(double) * (mu2 * l))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->yiT);
            scicos_free(ptr->uiT2);
            scicos_free(ptr->urT2);
            scicos_free(ptr->IN2);
            scicos_free(ptr->IN2X);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->IN1);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->yrT) != NULL)
        {
            scicos_free(ptr->ipiv);
            scicos_free(ptr->rank);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->iwork);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->IN1);
            scicos_free(ptr->urT1);
            scicos_free(ptr->uiT1);
            scicos_free(ptr->urT2);
            scicos_free(ptr->uiT2);
            scicos_free(ptr->yrT);
            scicos_free(ptr->yiT);
            scicos_free(ptr->IN2X);
            scicos_free(ptr->IN2);
            scicos_free(ptr->dwork);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        C2F(mtran)(u1r, &mu1, ptr->urT1, &nu, &mu1, &nu);
        C2F(mtran)(u1i, &mu1, ptr->uiT1, &nu, &mu1, &nu);
        C2F(mtran)(u2r, &mu2, ptr->urT2, &nu, &mu2, &nu);
        C2F(mtran)(u2i, &mu2, ptr->uiT2, &nu, &mu2, &nu);
        for (i = 0; i < (mu1 * nu); i++)
        {
            ptr->IN1[2 * i] = ptr->urT1[i];
            ptr->IN1[2 * i + 1] = -ptr->uiT1[i];
        }

        for (i = 0; i < (mu2 * nu); i++)
        {
            ptr->IN2[2 * i] = ptr->urT2[i];
            ptr->IN2[2 * i + 1] = -ptr->uiT2[i];
        }
        EPS = nc_eps();
        ANORM = C2F(zlange)("1", &nu, &mu1, ptr->IN1, &nu, ptr->dwork);
        if (mu1 == nu)
        {
            C2F(zlacpy)("F", &nu, &nu, ptr->IN1, &nu, ptr->IN1F, &nu);
            C2F(zgetrf)(&nu, &nu, ptr->IN1F, &nu, ptr->ipiv, &info);
            rcond = 0;
            if (info == 0)
            {
                C2F(zgecon)("1", &nu, ptr->IN1F, &nu, &ANORM, &rcond, ptr->dwork, ptr->iwork, &info);
                if (rcond > pow(EPS, 0.5))
                {
                    C2F(zgetrs)("N", &nu, &mu2, ptr->IN1F, &nu, ptr->ipiv, ptr->IN2, &nu, &info);
                    for (i = 0; i < (mu2 * nu); i++)
                    {
                        *(ptr->yrT + i) = *(ptr->IN2 + 2 * i);
                        *(ptr->yiT + i) = -(*(ptr->IN2 + (2 * i) + 1));
                    }
                    C2F(mtran)(ptr->yrT, &mu1, yr, &mu2, &mu1, &mu2);
                    C2F(mtran)(ptr->yiT, &mu1, yi, &mu2, &mu1, &mu2);
                    return;
                }
            }
        }
        rcond = pow(EPS, 0.5);
        for (i = 0; i < mu1; i++)
        {
            *(ptr->jpvt + i) = 0;
        }
        C2F(zlacpy)("F", &nu, &mu2, ptr->IN2, &nu, ptr->IN2X, &l);
        C2F(zgelsy1)(&nu, &mu1, &mu2, ptr->IN1, &nu, ptr->IN2X, &l, ptr->jpvt, &rcond, ptr->rank, ptr->dwork, &lw, ptr->iwork, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        for (i = 0; i < (l * mu2); i++)
        {
            *(ptr->yrT + i) = *(ptr->IN2X + 2 * i);
            *(ptr->yiT + i) = -(*(ptr->IN2X + (2 * i) + 1));
        }
        C2F(mtran)(ptr->yrT, &l, yr, &mu2, &mu1, &mu2);
        C2F(mtran)(ptr->yiT, &l, yi, &mu2, &mu1, &mu2);
    }
}
/*--------------------------------------------------------------------------*/
