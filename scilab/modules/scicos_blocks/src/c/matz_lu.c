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
extern int C2F(zgetrf)(int *m, int *n, double *a,
                       int *lda, int *ipiv, int *info);
extern int C2F(dlaswp)(int *n, double *a, int *lda, int
                       *k1, int *k2, int *ipiv, int *incx);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int *ipiv;
    double *dwork;
    double *IL;
    double *IU;
} mat_lu_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_lu(scicos_block *block, int flag)
{
    double *ur = NULL;
    double *ui = NULL;
    double *y1r = NULL;
    double *y1i = NULL;
    double *y2r = NULL;
    double *y2i = NULL;
    int mu = 0;
    int nu = 0;
    int info = 0;
    int i = 0, j = 0, l = 0, ij = 0, ik = 0, ij1 = 0;
    mat_lu_struct** work = (mat_lu_struct**) block->work;
    mat_lu_struct *ptr = NULL;

    mu = GetInPortRows(block, 1);
    nu = GetInPortCols(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    y1r = GetRealOutPortPtrs(block, 1);
    y1i = GetImagOutPortPtrs(block, 1);
    y2r = GetRealOutPortPtrs(block, 2);
    y2i = GetImagOutPortPtrs(block, 2);
    l = Min(mu, nu);
    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_lu_struct*) scicos_malloc(sizeof(mat_lu_struct))) == NULL)
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
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * (2 * mu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->IL = (double*) scicos_malloc(sizeof(double) * (mu * l))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->dwork);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->IU = (double*) scicos_malloc(sizeof(double) * (l * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->IL);
            scicos_free(ptr->dwork);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->IU) != NULL)
        {
            scicos_free(ptr->ipiv);
            scicos_free(ptr->dwork);
            scicos_free(ptr->IL);
            scicos_free(ptr->IU);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        for (i = 0; i < (mu * nu); i++)
        {
            ptr->dwork[2 * i] = ur[i];
            ptr->dwork[2 * i + 1] = ui[i];
        }
        C2F(zgetrf)(&mu, &nu, ptr->dwork, &mu, ptr->ipiv, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        for (j = 0; j < l; j++)
        {
            for (i = 0; i < mu; i++)
            {
                ij = i + j * mu;
                ij1 = 2 * ij;
                if (i == j)
                {
                    *(y2r + ij) = 1;
                    *(y2i + ij) = 0;
                }
                else if (i > j)
                {
                    *(y2r + ij) = *(ptr->dwork + ij1);
                    *(y2i + ij) = *(ptr->dwork + ij1 + 1);
                }
                else
                {
                    *(y2r + ij) = 0;
                    *(y2i + ij) = 0;
                }
            }
        }
        for (j = 0; j < nu; j++)
        {
            for (i = 0; i < l; i++)
            {
                ij = i + j * l;
                ik = 2 * (i + j * mu);
                if (i <= j)
                {
                    *(y1r + ij) = *(ptr->dwork + ik);
                    *(y1i + ij) = *(ptr->dwork + ik + 1);
                }
                else
                {
                    *(y1r + ij) = 0;
                    *(y1i + ij) = 0;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
