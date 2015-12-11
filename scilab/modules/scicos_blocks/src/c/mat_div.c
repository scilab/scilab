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
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
#include "numericconstants_interface.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dgetrf)(int *m, int *n, double *a, int *
                       lda, int *ipiv, int *info);
extern double C2F(dlange)(char *norm, int *m, int *n, double *a, int
                          *lda, double *work);
extern int C2F(dlacpy)(char *uplo, int *m, int *n, double *
                       a, int *lda, double *b, int *ldb);
extern int C2F(dgecon)(char *norm, int *n, double *a, int *
                       lda, double *anorm, double *rcond, double *work, int *
                       iwork, int *info);
extern int C2F(dgetrs)(char *trans, int *n, int *nrhs,
                       double *a, int *lda, int *ipiv, double *b, int *
                       ldb, int *info);
extern int C2F(dgelsy1)(int *m, int *n, int *nrhs, double *a, int *
                        lda, double *b, int *ldb, int *jpvt, double *rcond, int *rank, double *work, int *lwork, int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int *ipiv;
    int *rank;
    int *jpvt;
    int *iwork;
    double *dwork;
    double *LAF;
    double *LBT;
    double *LAT;
} mat_div_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_div(scicos_block *block, int flag)
{
    double *u1 = NULL;
    double *u2 = NULL;
    double *y = NULL;
    int mu1 = 0, mu2 = 0;
    int nu = 0;
    int info = 0;
    int i = 0, j = 0, l = 0, lw = 0, lu = 0, ij = 0, ji = 0;
    mat_div_struct** work = (mat_div_struct**) block->work;
    mat_div_struct *ptr = NULL;
    double rcond = 0., ANORM = 0., EPS = 0.;

    mu2 = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    mu1 = GetInPortRows(block, 2);
    u2 = GetRealInPortPtrs(block, 1);
    u1 = GetRealInPortPtrs(block, 2);
    y = GetRealOutPortPtrs(block, 1);
    l = Max(mu1, nu);
    lu = Max(4 * nu, Min(mu1, nu) + 3 * mu1 + 1);
    lw = Max(lu, 2 * Min(mu1, nu) + mu2);
    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_div_struct*) scicos_malloc(sizeof(mat_div_struct))) == NULL)
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
        if ((ptr->iwork = (int*) scicos_malloc(sizeof(int) * nu)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * lw)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LAF = (double*) scicos_malloc(sizeof(double) * (nu * mu1))) == NULL)
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
        if ((ptr->LBT = (double*) scicos_malloc(sizeof(double) * (l * mu2))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LAF);
            scicos_free(ptr->dwork);
            scicos_free(ptr->iwork);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->rank);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LAT = (double*) scicos_malloc(sizeof(double) * (nu * mu1))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LBT);
            scicos_free(ptr->LAF);
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
        if ((ptr->LAT) != NULL)
        {
            scicos_free(ptr->ipiv);
            scicos_free(ptr->rank);
            scicos_free(ptr->jpvt);
            scicos_free(ptr->iwork);
            scicos_free(ptr->LAF);
            scicos_free(ptr->LBT);
            scicos_free(ptr->LAT);
            scicos_free(ptr->dwork);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        EPS = nc_eps();
        ANORM = C2F(dlange)("l", &mu1, &nu, u1, &mu1, ptr->dwork);
        for (j = 0; j < mu1; j++)
        {
            for (i = 0; i < nu; i++)
            {
                ij = i + j * nu;
                ji = j + i * mu1;
                *(ptr->LAT + ij) = *(u1 + ji);
            }
        }
        for (j = 0; j < mu2; j++)
        {
            for (i = 0; i < nu; i++)
            {
                ij = i + j * l;
                ji = j + i * mu2;
                *(ptr->LBT + ij) = *(u2 + ji);
            }
        }
        if (mu1 == nu)
        {
            C2F(dlacpy)("F", &nu, &nu, ptr->LAT, &nu, ptr->LAF, &nu);
            C2F(dgetrf)(&nu, &nu, ptr->LAF, &nu, ptr->ipiv, &info);
            rcond = 0;
            if (info == 0)
            {
                C2F(dgecon)("1", &nu, ptr->LAF, &nu, &ANORM, &rcond, ptr->dwork, ptr->iwork, &info);
                if (rcond > pow(EPS, 0.5))
                {
                    C2F(dgetrs)("N", &nu, &mu2, ptr->LAF, &nu, ptr->ipiv, ptr->LBT, &nu, &info);
                    for (j = 0; j < nu; j++)
                    {
                        for (i = 0; i < mu2; i++)
                        {
                            ij = i + j * mu2;
                            ji = j + i * nu;
                            *(y + ij) = *(ptr->LBT + ji);
                        }
                    }
                    return;
                }
            }
        }
        rcond = pow(EPS, 0.5);
        for (i = 0; i < mu1; i++)
        {
            *(ptr->jpvt + i) = 0;
        }
        C2F(dgelsy1)(&nu, &mu1, &mu2, ptr->LAT, &nu, ptr->LBT, &l, ptr->jpvt, &rcond, ptr->rank, ptr->dwork, &lw, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        for (j = 0; j < mu1; j++)
        {
            for (i = 0; i < mu2; i++)
            {
                ij = i + j * mu2;
                ji = j + i * l;
                *(y + ij) = *(ptr->LBT + ji);
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
