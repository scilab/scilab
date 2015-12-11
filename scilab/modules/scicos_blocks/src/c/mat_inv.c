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

#include "machine.h"            /* C2F */
#include "sci_malloc.h"
#include "localization.h"

#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dgetrf) (int *m, int *n, double *a, int *
                        lda, int *ipiv, int *info);
extern int C2F(dgetri) (int *n, double *a, int *lda, int
                        *ipiv, double *work, int *lwork, int *info);

/*--------------------------------------------------------------------------*/
typedef struct
{
    int *ipiv;
    double *dwork;
} mat_inv_struct;

/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_inv(scicos_block * block, int flag)
{
    double *u = NULL;
    double *y = NULL;
    int nu = 0;
    int info = 0;
    int i = 0;
    mat_inv_struct** work = (mat_inv_struct**) block->work;
    mat_inv_struct *ptr = NULL;

    nu = GetInPortRows(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);

    /*init : initialization */
    if (flag == 4)
    {
        if ((*work = (mat_inv_struct *) scicos_malloc(sizeof(mat_inv_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->ipiv = (int *)scicos_malloc(sizeof(int) * nu)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double *)scicos_malloc(sizeof(double) * nu)) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->ipiv);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if ((ptr->dwork) != NULL)
        {
            scicos_free(ptr->ipiv);
            scicos_free(ptr->dwork);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        for (i = 0; i < (nu * nu); i++)
        {
            y[i] = u[i];
        }
        C2F(dgetrf) (&nu, &nu, &y[0], &nu, ptr->ipiv, &info);
        if (info != 0)
        {
            if (flag != 6)
            {
                Coserror(_("The LU factorization has been completed, but the factor U is exactly singular : U(%d,%d) is exactly zero."), info, info);
                return;
            }
        }
        C2F(dgetri) (&nu, y, &nu, ptr->ipiv, ptr->dwork, &nu, &info);

    }
}

/*--------------------------------------------------------------------------*/
