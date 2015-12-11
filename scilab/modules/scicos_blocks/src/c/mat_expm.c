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
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dexpm1)(int* ia, int* n, double* a, double* ea, int* iea, double* w, int* iw, int* ierr);
/*--------------------------------------------------------------------------*/
typedef struct
{
    int *iwork;
    double *dwork;
} mat_exp_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_expm(scicos_block *block, int flag)
{
    double *u = NULL;
    double *y = NULL;
    int nu = 0;
    int ierr = 0;
    mat_exp_struct** work = (mat_exp_struct**) block->work;
    mat_exp_struct *ptr = NULL;

    nu = GetInPortCols(block, 1);
    u = GetRealInPortPtrs(block, 1);
    y = GetRealOutPortPtrs(block, 1);

    /*init : initialization*/
    if (flag == 4)
    {
        if ((*work = (mat_exp_struct*) scicos_malloc(sizeof(mat_exp_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }
        ptr = *work;
        if ((ptr->iwork = (int*) scicos_malloc(sizeof(int) * (2 * nu))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            return;
        }
        if ((ptr->dwork = (double*) scicos_malloc(sizeof(double) * (nu * (2 * nu + 2 * nu + 5)))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr->iwork);
            scicos_free(ptr);
            return;
        }
    }

    /* Terminaison */
    else if (flag == 5)
    {
        ptr = *work;
        if (ptr->dwork != NULL)
        {
            scicos_free(ptr->iwork);
            scicos_free(ptr->dwork);
            scicos_free(ptr);
            return;
        }
    }

    else
    {
        ptr = *work;
        C2F(dexpm1)(&nu, &nu, u, y, &nu, ptr->dwork, ptr->iwork, &ierr);
        if (ierr != 0)
        {
            if (flag != 6)
            {
                set_block_error(-7);
                return;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
