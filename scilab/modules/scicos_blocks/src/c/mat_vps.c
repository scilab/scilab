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
#include "../../../linear_algebra/includes/issymmetric.h"
#include "sci_malloc.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dlacpy)(char *uplo, int *m, int *n, double *
                       a, int *lda, double *b, int *ldb);
extern int C2F(dgeev)(char *jobvl, char *jobvr, int *n, double *
                      a, int *lda, double *wr, double *wi, double *vl,
                      int *ldvl, double *vr, int *ldvr, double *work,
                      int *lwork, int *info);
extern int C2F(zlaset)(char *uplo, int *m, int *n,
                       double *alpha, double *beta, double *a, int *
                       lda);
extern int C2F(dsyev)(char *jobz, char *uplo, int *n, double *a,
                      int *lda, double *w, double *work, int *lwork,
                      int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    double *LA;
    double *LVR;
    double *dwork;
    double *dwork1;
} mat_vps_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_vps(scicos_block *block, int flag)
{
    double *u = NULL;
    double *y1 = NULL;
    double *y2 = NULL;
    int nu = 0;
    int info = 0;
    int i = 0, lwork = 0, lwork1 = 0, j = 0, ij = 0, ji = 0;
    int symmetric = 0;
    mat_vps_struct** work = (mat_vps_struct**) block->work;
    mat_vps_struct *ptr = NULL;

    nu = GetInPortRows(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y1 = GetRealOutPortPtrs(block, 1);
    y2 = GetImagOutPortPtrs(block, 1);
    lwork1 = 3 * nu;
    lwork = 3 * nu - 1;
    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_vps_struct*) scicos_malloc(sizeof(mat_vps_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->LA = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVR = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * lwork)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork1 = (double*) scicos_malloc(sizeof(double) * lwork1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->dwork1) != NULL)
        {
            scicos_free(ptr->LA);
            scicos_free(ptr->LVR);
            scicos_free(ptr->dwork);
            scicos_free(ptr->dwork1);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        C2F(dlacpy)("F", &nu, &nu, u, &nu, ptr->LA, &nu);
        symmetric = 1;
        for (j = 0; j < nu; j++)
        {
            for (i = j; i < nu; i++)
            {
                ij = i + j * nu;
                ji = j + i * nu;
                if (i != j)
                {
                    if (*(ptr->LA + ij) == *(ptr->LA + ji))
                    {
                        symmetric *= 1;
                    }
                    else
                    {
                        symmetric *= 0;
                        break;
                    }
                }
            }
        }
        if (symmetric == 1)
        {
            C2F(dsyev)("N", "U", &nu, ptr->LA, &nu, y1, ptr->dwork, &lwork, &info);
            if (info != 0)
            {
                if (flag != 6)
                {
                    set_block_error(-7);
                    return;
                }
            }
        }
        else
        {
            C2F(dgeev)("N", "N", &nu, ptr->LA, &nu, y1, y2, ptr->dwork1, &nu, ptr->LVR, &nu, ptr->dwork1, &lwork1, &info);
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
}
/*--------------------------------------------------------------------------*/
