/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "xscion.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
int C2F(xscion) (int *i)
{
    if (getScilabMode() == SCILAB_STD)
    {
        *i = 1;
    }
    else
    {
        *i = 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int getINXscilab(void)
{
    return getScilabMode() == SCILAB_STD;
}
/*--------------------------------------------------------------------------*/
