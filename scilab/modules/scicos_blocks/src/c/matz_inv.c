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
extern int C2F(zgetrf)(int *m, int *n, double *a,
                       int *lda, int *ipiv, int *info);
extern int C2F(zgetri)(int *n, double *a, int *lda,
                       int *ipiv, double *work, int *lwork, int *info);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int *ipiv;
    double *wrk;
    double *LX;
} mat_inv_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void matz_inv(scicos_block *block, int flag)
{
    double *ur = NULL;
    double *yr = NULL;
    double *ui = NULL;
    double *yi = NULL;
    int nu = 0;
    int info = 0;
    int i = 0;
    mat_inv_struct** work = (mat_inv_struct**) block->work;
    mat_inv_struct *ptr = NULL;

    nu = GetInPortRows(block, 1);
    ur = GetRealInPortPtrs(block, 1);
    ui = GetImagInPortPtrs(block, 1);
    yr = GetRealOutPortPtrs(block, 1);
    yi = GetImagOutPortPtrs(block, 1);
    /*init : initialization*/
    if (flag == 4)

    {
        if ((*work = (mat_inv_struct*) scicos_malloc(sizeof(mat_inv_struct))) == NULL)
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
        if ((ptr->wrk = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
        if ((ptr->LX = (double*) scicos_malloc(sizeof(double) * (2 * nu * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->LX);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->LX) != NULL)
        {
            scicos_free(ptr->ipiv);
            scicos_free(ptr->LX);
            scicos_free(ptr->wrk);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        for (i = 0; i < (nu * nu); i++)
        {
            ptr->LX[2 * i] = ur[i];
            ptr->LX[2 * i + 1] = ui[i];
        }
        C2F(zgetrf)(&nu, &nu, ptr->LX, &nu, ptr->ipiv, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
        C2F(zgetri)(&nu, ptr->LX, &nu, ptr->ipiv, ptr->wrk, &nu, &info);
        for (i = 0; i < (nu * nu); i++)
        {
            yr[i] = ptr->LX[2 * i];
            yi[i] = ptr->LX[2 * i + 1];
        }
    }
}
/*--------------------------------------------------------------------------*/
