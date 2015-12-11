/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ???? - INRIA
 * Copyright (C) 2003 - Bruno PINCON
 * Copyright (C) 2007 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*----------------------------------------------------------------------*/
/* @author INRIA (initial fortran version)                              */
/* @author Bruno Pincon (2003) bug fix for the nan problem              */
/* @author Allan Cornet (2007) rewrite to C                             */
/*----------------------------------------------------------------------*/
#include "idmax.h"
#include "isanan.h"
/*--------------------------------------------------------------------------*/
int C2F(idmax)(int *n, double *x, int *incx)
{
    int x_dim1 = 0, x_offset = 0, ret_val = 0, i1;

    /* Local variables */
    int i = 0, j = 0;
    double xmax;

    x_dim1 = *incx;
    x_offset = 1 + x_dim1;
    x -= x_offset;

    ret_val = 1;
    /* initialize the max with the first component being not a nan */
    j = 1;
    while (C2F(isanan)(&x[j * x_dim1 + 1]) == 1)
    {
        ++j;
        if (j > *n)
        {
            return ret_val;
        }
    }
    xmax = x[j * x_dim1 + 1];
    ret_val = j;

    /* the usual loop */
    i1 = *n;
    for (i = j + 1; i <= i1; ++i)
    {
        if (x[i * x_dim1 + 1] > xmax)
        {
            /* the previous test must return false if x[i*x_dim1+1] is a nan
             * so should not branch here in this case (for compiler
             * managing ieee754 ... Intel Compiler doesn't)
             */
            xmax = x[i * x_dim1 + 1];
            ret_val = i;
        }
    }
    return ret_val;
}
/*--------------------------------------------------------------------------*/

