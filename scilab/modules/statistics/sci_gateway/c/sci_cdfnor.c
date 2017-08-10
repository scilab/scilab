/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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

#include "gw_statistics.h"
#include "CdfBase.h"

extern int C2F(cdfnor)(int *, double *, double *, double*, double *, double *, int *, double *);

/**
 * Interface to dcdflib's cdfnor
 * SUBROUTINE CDFNOR ( WHICH, P, Q, X, MEAN, SD, STATUS, BOUND )
 * Cumulative Distribution Function, NORmal distribution
 */
int sci_cdfnor(char* fname, void* pvApiCtx)
{
    struct cdf_item items[] =
    {
        {"PQ"  , 3, 2, 2},
        {"X"   , 4, 1, 3},
        {"Mean", 4, 1, 4},
        {"Std" , 4, 1, 0}
    };
    struct cdf_descriptor cdf = mkcdf(cdfnor, 4, 5, 1, 2, items);
    return cdf_generic(fname, pvApiCtx, &cdf);
}
