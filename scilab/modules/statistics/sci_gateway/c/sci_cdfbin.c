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

extern int C2F(cdfbin)(int *, double *, double *, double *, double *, double *, double *, int *, double *);

/**
 * Interface to dcdflib's cdfbin
 * SUBROUTINE CDFBIN ( WHICH, P, Q, S, XN, PR, OMPR, STATUS, BOUND )
 * Cumulative Distribution Function, BINomial distribution
 */
int sci_cdfbin(char* fname, void* pvApiCtx)
{
    struct cdf_item items[] =
    {
        {"PQ"    , 4, 2, 2},
        {"S"     , 5, 1, 3},
        {"Xn"    , 5, 1, 4},
        {"PrOmpr", 4, 2, 0}
    };
    struct cdf_descriptor cdf = mkcdf(cdfbin, 5, 6, 1, 2, items);
    return cdf_generic(fname, pvApiCtx, &cdf);
}
