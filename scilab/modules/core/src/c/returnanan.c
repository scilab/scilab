/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

