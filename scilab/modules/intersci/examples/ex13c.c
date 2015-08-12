/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


#include "../../routines/machine.h"

/*******************************************
 * simple example
 *******************************************/

int F2C(ext13c)( double *ar, int *m, int *n)
{
    int k;
    for (k = 0; k < *n * (*m); ++k)
    {
        ar[k] = 2 * ar[k];
    }
    return(0);
}

