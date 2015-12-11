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
extern int C2F(zgeev)(char *jobvl, char *jobvr, int *n,
                      double *a, int *lda, double *w, double *vl,
                      int *ldvl, double *vr, int *ldvr, double *work,
                      int *lwork, double *rwork, int *info);
extern int C2F(zheev)(char *jobz, char *uplo, int *n, double
                      *a, int *lda, double *w, double *work, int *lwork,
                      double *rwork, int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    double *LA;
    double *LX;
    double *LVR;
    double *dwork;
    double *rwork;
    double *dwork1;
    double *rwork1;
} mat_vps_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_vps(scicos_block *block, int flag)
{
    double *ur = NULL, *ui = NULL;
    double *yr = NULL, *yi = NULL;
    int nu = 0;
    int info = 0;
    int i = 0, lwork = 0, lwork1 = 0, j = 0, ij = 0, ji = 0, rw = 0;
    int hermitien = 0;
    mat_vps_struct** work = (mat_vps_struct**) block->work;
    mat_vps_struct *ptr = NULL;

    nu = GetInPortRows(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    lwork1 = 2 * nu;
    lwork = 2 * nu - 1;
    rw = 3 * nu - 2;
    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_vps_struct*) scicos_malloc(sizeof(mat_vps_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->LA = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LX = (double*) scicos_malloc(sizeof(double) * (2 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LVR = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LX);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * 2 * lwork)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LX);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->rwork = (double*) scicos_malloc(sizeof(double) * 2 * rw)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LX);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork1 = (double*) scicos_malloc(sizeof(double) * 2 * lwork1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->rwork);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LX);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
        if ((ptr->rwork1 = (double*) scicos_malloc(sizeof(double) * 2 * lwork1)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork1);
            scicos_free(ptr->rwork);
            scicos_free(ptr->dwork);
            scicos_free(ptr->LVR);
            scicos_free(ptr->LX);
            scicos_free(ptr->LA);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->rwork1) != NULL)
        {
            scicos_free(ptr->LA);
            scicos_free(ptr->LX);
            scicos_free(ptr->LVR);
            scicos_free(ptr->rwork);
            scicos_free(ptr->rwork1);
            scicos_free(ptr->dwork);
            scicos_free(ptr->dwork1);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        for (i = 0; i < (nu * nu); i++)
        {
            ptr->LA[2 * i] = ur[i];
            ptr->LA[2 * i + 1] = ui[i];
        }
        hermitien = 1;
        for (j = 0; j < nu; j++)
        {
            for (i = j; i < nu; i++)
            {
                ij = i + j * nu;
                ji = j + i * nu;
                if (i != j)
                {
                    if ((*(ptr->LA + 2 * ij) == *(ptr->LA + 2 * ji)) && (*(ptr->LA + 2 * ij + 1) == -(*(ptr->LA + 2 * ji + 1))))
                    {
                        hermitien *= 1;
                    }
                    else
                    {
                        hermitien *= 0;
                        break;
                    }
                }
            }
        }
        if (hermitien == 1)
        {
            C2F(zheev)("N", "U", &nu, ptr->LA, &nu, yr, ptr->dwork, &lwork, ptr->rwork, &info);
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
            C2F(zgeev)("N", "N", &nu, ptr->LA, &nu, ptr->LX, ptr->dwork1, &nu, ptr->LVR, &nu, ptr->dwork1, &lwork1, ptr->rwork1, &info);
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
                *(yr + i) = *(ptr->LX + 2 * i);
                *(yi + i) = *(ptr->LX + 2 * i + 1);
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
