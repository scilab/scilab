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
    double *IN2X;
    double *IN2;
} mat_bksl_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_bksl(scicos_block *block, int flag)
{
    double *u1r = NULL, *u1i = NULL;
    double *u2r = NULL, *u2i = NULL;
    double *yr = NULL, *yi = NULL;
    int mu = 0, vu = 0, wu = 0;
    int nu1 = 0;
    int nu2 = 0;
    int info = 0;
    int i = 0, j = 0, l = 0, lw = 0, lu = 0, ij = 0, k = 0;
    mat_bksl_struct** work = (mat_bksl_struct**) block->work;
    mat_bksl_struct *ptr = NULL;
    double rcond = 0., ANORM = 0., EPS = 0.;

    vu = GetOutPortRows(block, 1);
    wu = GetOutPortCols(block, 1);
    mu = GetInPortRows(block, 1);
    nu1 = GetInPortCols(block, 1);
    nu2 = GetInPortCols(block, 2);
    u1r = GetRealInPortPtrs(block, 1);
    u1i = GetImagInPortPtrs(block, 1);
    u2r = GetRealInPortPtrs(block, 2);
    u2i = GetImagInPortPtrs(block, 2);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    l = Max(mu, nu1);
    lw = Max(2 * Min(mu, nu1), nu1 + 1);
    lu = Max(lw, Min(mu, nu1) + nu2);
    lw = Max(2 * nu1, Min(mu, nu1) + lu);

    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_bksl_struct*) scicos_malloc(sizeof(mat_bksl_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->ipiv = (int*) scicos_malloc(sizeof(int) * nu1)) == NULL)
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
        if ((ptr->jpvt = (int*) scicos_malloc(sizeof(int) * nu1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->iwork = (double*) scicos_malloc(sizeof(double) * 2 * nu1)) == NULL)
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
        if ((ptr->IN1F = (double*) scicos_malloc(sizeof(double) * (2 * mu * nu1))) == NULL)
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
        if ((ptr->IN1 = (double*) scicos_malloc(sizeof(double) * (2 * mu * nu1))) == NULL)
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
        if ((ptr->IN2X = (double*) scicos_malloc(sizeof(double) * (2 * l * nu2))) == NULL)
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
        if ((ptr->IN2 = (double*) scicos_malloc(sizeof(double) * (2 * mu * nu2))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->IN2);
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
        if ((ptr->IN2) != NULL)
        {
            scicos_free(ptr->ipiv);
            scicos_free(ptr->rank);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->iwork);
            scicos_free(ptr->IN1F);
            scicos_free(ptr->IN1);
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
        for (i = 0; i < (mu * nu1); i++)
        {
            ptr->IN1[2 * i] = u1r[i];
            ptr->IN1[2 * i + 1] = u1i[i];
        }
        for (i = 0; i < (mu * nu2); i++)
        {
            ptr->IN2[2 * i] = u2r[i];
            ptr->IN2[2 * i + 1] = u2i[i];
        }
        EPS = nc_eps();
        ANORM = C2F(zlange)("1", &mu, &nu1, ptr->IN1, &mu, ptr->dwork);
        if (mu == nu1)
        {
            C2F(zlacpy)("F", &mu, &nu1, ptr->IN1, &mu, ptr->IN1F, &mu);
            C2F(zgetrf)(&nu1, &nu1, ptr->IN1F, &nu1, ptr->ipiv, &info);
            rcond = 0;
            if (info == 0)
            {
                C2F(zgecon)("1", &nu1, ptr->IN1F, &nu1, &ANORM, &rcond, ptr->dwork, ptr->iwork, &info);
                if (rcond > pow(EPS, 0.5))
                {
                    C2F(zgetrs)("N", &nu1, &nu2, ptr->IN1F, &nu1, ptr->ipiv, ptr->IN2, &nu1, &info);
                    for (i = 0; i < (mu * nu2); i++)
                    {
                        *(yr + i) = *(ptr->IN2 + 2 * i);
                        *(yi + i) = *(ptr->IN2 + (2 * i) + 1);
                    }
                    return;
                }
            }
        }
        rcond = pow(EPS, 0.5);
        for (i = 0; i < nu1; i++)
        {
            *(ptr->jpvt + i) = 0;
        }
        C2F(zlacpy)("F", &mu, &nu2, ptr->IN2, &mu, ptr->IN2X, &l);
        C2F(zgelsy1)(&mu, &nu1, &nu2, ptr->IN1, &mu, ptr->IN2X, &l, ptr->jpvt, &rcond, ptr->rank, ptr->dwork, &lw, ptr->iwork, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        k = 0;
        for (j = 0; j < wu; j++)
        {
            for (i = 0; i < vu; i++)
            {
                ij = i + j * l;
                *(yr + k) = *(ptr->IN2X + 2 * ij);
                *(yi + k) = *(ptr->IN2X + (2 * ij) + 1);
                k++;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
