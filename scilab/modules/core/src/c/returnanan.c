/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "returnanan.h"
#include <stdio.h>

/*--------------------------------------------------------------------------*/
double C2F(returnanan)(void)
{
    static int first = 1;
    static double nan = 1.0;

    if ( first )
    {
        nan = (nan - (double) first) / (nan - (double) first);
        first = 0;
    }
    return (nan);
}
/*--------------------------------------------------------------------------*/
// MB, 26/10/2009
// Because of a problem of management of the NANs, when used in a Fortran
// source code with implicit none.
// If the client fortran source code does not declare the type of the
// external, it is assumed as REAL instead of DOUBLE PRECISION.
// This leads to bugs.
// It is simpler and safer to use this as a subroutine, where the
// result variable must still be declared as a DOUBLE PRECISION.
// See Bug #4678 :
// http://bugzilla.scilab.org/show_bug.cgi?id=4378
void C2F(returnananfortran)(double * nan)
{
    *nan = C2F(returnanan)();
}
/*--------------------------------------------------------------------------*/

