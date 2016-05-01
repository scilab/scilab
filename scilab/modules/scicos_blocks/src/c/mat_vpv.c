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
extern int C2F(dlaset)(char *uplo, int *m, int *n, double *
                       alpha, double *beta, double *a, int *lda);
extern int C2F(dsyev)(char *jobz, char *uplo, int *n, double *a,
                      int *lda, double *w, double *work, int *lwork,
                      int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    double *LA;
    double *L0;
    double *LVR;
    double *LW;
    double *LWR;
    double *LWI;
    double *dwork;
    double *dwork1;
} mat_vps_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_vpv(scicos_block *block, int flag)
{
    double *u = NULL;
    double *y1 = NULL;
    double *y2 = NULL;
    double *y3 = NULL;
    double *y4 = NULL;
    int nu = 0;
    int info = 0;
    int i = 0, j = 0, lwork = 0, lwork1 = 0, ii = 0, ij = 0, ij1 = 0, ji = 0;
    int symmetric = 0;
    mat_vps_struct** work = (mat_vps_struct**) block->work;
    mat_vps_struct *ptr = NULL;

    nu = GetInPortRows(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y1 = GetRealOutPortPtrs(block, 1);
    y2 = GetImagOutPortPtrs(block, 1);
    y3 = GetRealOutPortPtrs(block, 2);
    y4 = GetImagOutPortPtrs(block, 2);
    lwork1 = 4 * nu;
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
        if ((ptr->L0 = (double*) scicos_malloc(sizeof(double))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVR = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->L0);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LW = (double*) scicos_malloc(sizeof(double) * (nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVR);
            scicos_free(ptr->L0);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LWR = (double*) scicos_malloc(sizeof(double) * (nu * 1))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LW);
            scicos_free(ptr->LVR);
            scicos_free(ptr->L0);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LWI = (double*) scicos_malloc(sizeof(double) * (nu * 1))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LWR);
            scicos_free(ptr->LW);
            scicos_free(ptr->LVR);
            scicos_free(ptr->L0);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * lwork)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LWI);
            scicos_free(ptr->LWR);
            scicos_free(ptr->LW);
            scicos_free(ptr->LVR);
            scicos_free(ptr->L0);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork1 = (double*) scicos_malloc(sizeof(double) * lwork1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LWI);
            scicos_free(ptr->LWR);
            scicos_free(ptr->LW);
            scicos_free(ptr->LVR);
            scicos_free(ptr->L0);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if (ptr->dwork1 != NULL)
        {
            scicos_free(ptr->LA);
            scicos_free(ptr->L0);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LW);
            scicos_free(ptr->LWI);
            scicos_free(ptr->LWR);
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
            C2F(dsyev)("V", "U", &nu, ptr->LA, &nu, ptr->LW, ptr->dwork, &lwork, &info);
            if (info != 0)
            {
                if (flag != 6)
                {
                    set_block_error(-7);
                    return;
                }
            }
            for (i = 0; i < nu; i++)
            {
                ii = i + i * nu;
                *(y1 + ii) = *(ptr->LW + i);
            }
            C2F(dlacpy)("F", &nu, &nu, ptr->LA, &nu, y3, &nu);
        }
        else
        {
            C2F(dgeev)("N", "V", &nu, ptr->LA, &nu, ptr->LWR, ptr->LWI, ptr->dwork1, &nu, ptr->LVR, &nu, ptr->dwork1, &lwork1, &info);
            if (info != 0)
            {
                if (flag != 6)
                {
                    set_block_error(-7);
                    return;
                }
            }

            *(ptr->L0) = 0;
            C2F(dlaset)("F", &nu, &nu, ptr->L0, ptr->L0, y1, &nu);
            C2F(dlaset)("F", &nu, &nu, ptr->L0, ptr->L0, y2, &nu);
            for (i = 0; i < nu; i++)
            {
                ii = i + i * nu;
                *(y1 + ii) = *(ptr->LWR + i);
                *(y2 + ii) = *(ptr->LWI + i);
            }
            for (j = 0; j < nu; j++)
            {
                if (*(ptr->LWI + j) == 0)
                    for (i = 0; i < nu; i++)
                    {
                        ij = i + (j) * nu;
                        *(y3 + ij) = *(ptr->LVR + ij);
                        *(y4 + ij) = 0;
                    }
                else
                {
                    for (i = 0; i < nu; i++)
                    {
                        ij = i + (j) * nu;
                        ij1 = i + (j + 1) * nu;
                        *(y3 + ij) = *(ptr->LVR + ij);
                        *(y4 + ij) = *(ptr->LVR + ij1);
                        *(y3 + ij1) = *(ptr->LVR + ij);
                        *(y4 + ij1) = -(*(ptr->LVR + ij1));
                    }
                    j++;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
