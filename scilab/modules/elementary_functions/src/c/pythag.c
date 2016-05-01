/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
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
#include "elem_common.h"
#include "dynlib_elementary_functions.h"

/*
*     PURPOSE
*        computes sqrt(a^2 + b^2) with accuracy and
*        without spurious underflow / overflow problems
*
*     MOTIVATION
*        This work was motivated by the fact that the original Scilab
*        PYTHAG, which implements Moler and Morrison's algorithm is slow.
*        Some tests showed that the Kahan's algorithm, is superior in
*        precision and moreover faster than the original PYTHAG.  The speed
*        gain partly comes from not calling DLAMCH.
*
*     REFERENCE
*        This is a Fortran-77 translation of an algorithm by William Kahan,
*        which appears in his article "Branch cuts for complex elementary
*        functions, or much ado about nothing's sign bit",
*        Editors: Iserles, A. and Powell, M. J. D.
*        in "States of the Art in Numerical Analysis"
*        Oxford, Clarendon Press, 1987
*        ISBN 0-19-853614-3
**     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>,
*        Thanks to Lydia van Dijk <lvandijk@hammersmith-consulting.com>
*/
ELEMENTARY_FUNCTIONS_IMPEXP double dpythags(double _dblVal1, double _dblVal2)
{
    double dblSqrt2		= 1.41421356237309504;
    double dblSqrt2p1	= 2.41421356237309504;
    double dblEsp		= 1.25371671790502177E-16;
    double dblRMax		= getOverflowThreshold();

    double dblAbs1 = 0;
    double dblAbs2 = 0;
    double dblTemp = 0;

    if (ISNAN(_dblVal1) == 1)
    {
        return _dblVal2;
    }

    if (ISNAN(_dblVal2) == 1)
    {
        return _dblVal1;
    }

    dblAbs1 = dabss(_dblVal1);
    dblAbs2 = dabss(_dblVal2);

    //Order x and y such that 0 <= y <= x
    if (dblAbs1 < dblAbs2)
    {
        dblTemp = dblAbs1;
        dblAbs1 = dblAbs2;
        dblAbs2 = dblTemp;
    }

    //Test for overflowing x
    if ( dblAbs1 >= dblRMax)
    {
        return dblAbs1;
    }

    //Handle generic case
    dblTemp = dblAbs1 - dblAbs2;
    if (dblTemp != dblAbs1)
    {
        double dblS = 0;
        if (dblTemp > dblAbs2)
        {
            dblS = dblAbs1 / dblAbs2;
            dblS += dsqrts(1 + dblS * dblS);
        }
        else
        {
            dblS	= dblTemp / dblAbs2;
            dblTemp = (2 + dblS) * dblS;
            dblS	= ((dblEsp + dblTemp / (dblSqrt2 + dsqrts(2 + dblTemp))) + dblS) + dblSqrt2p1;
        }
        return dblAbs1 + dblAbs2 / dblS;
    }
    else
    {
        return dblAbs1;
    }
}
