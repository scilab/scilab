/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/* Workaround for bug 8175 with f2c Visual studio solution ONLY */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
/*--------------------------------------------------------------------------*/
extern double C2F(slamch)();
/*--------------------------------------------------------------------------*/
double C2F(radix)(float *n)
{
    return C2F(slamch)("b", strlen("b"));
}
/*--------------------------------------------------------------------------*/
double C2F(tiny)(float *n)
{
    return C2F(slamch)("u", strlen("u"));
}
/*--------------------------------------------------------------------------*/
double C2F(huge)(float *n)
{
    return C2F(slamch)("o", strlen("o"));
}
/*--------------------------------------------------------------------------*/
