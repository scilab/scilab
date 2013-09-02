/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
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
#include <string.h>
#include "gw_statistics.h"
#include "stack-c.h"
#include "CdfBase.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cdfnbn)(int *, double *, double *, double *, double *, double *, double *, int *, double *);
/*--------------------------------------------------------------------------*/
static void cdfnbnErr(int status, double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdfnbn
* SUBROUTINE cdfnbn(which,p,q,s,xn,pr,ompr,status,bound)
*               Cumulative Distribution Function
*               Negative BiNomial distribution
*/
/*--------------------------------------------------------------------------*/
int cdfnbnI(char* fname, unsigned long l)
{
    int m1 = 0, n1 = 0, l1 = 0;
    Nbvars = 0;
    CheckRhs(5, 6);
    CheckLhs(1, 2);
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1), "PQ") == 0)
    {
        static int callpos[6] = {4, 5, 0, 1, 2, 3};
        CdfBase(fname, 4, 2, callpos, "PQ", _("S,XN,PR and OMPR"), 1, C2F(cdfnbn),
                cdfnbnErr);
    }
    else if ( strcmp(cstk(l1), "S") == 0)
    {
        static int callpos[6] = {3, 4, 5, 0, 1, 2};
        CdfBase(fname, 5, 1, callpos, "S", _("XN,PR,OMPR,P and Q"), 2, C2F(cdfnbn),
                cdfnbnErr);
    }
    else if ( strcmp(cstk(l1), "Xn") == 0)
    {
        static int callpos[6] = {2, 3, 4, 5, 0, 1};
        CdfBase(fname, 5, 1, callpos, "Xn", _("PR,OMPR,P,Q and S"), 3, C2F(cdfnbn),
                cdfnbnErr);
        return 0;
    }
    else if ( strcmp(cstk(l1), "PrOmpr") == 0)
    {
        static int callpos[6] = {0, 1, 2, 3, 4, 5};
        CdfBase(fname, 4, 2, callpos, "PrOmpr", _("P,Q,S and Xn"), 4, C2F(cdfnbn),
                cdfnbnErr);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), fname, 1, "PQ", "S", "Xn", "PrOmpr");

    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdfnbnErr(int status, double bound)
{
    static char *param[1] = {"-PQXYAB"};

    switch ( status )
    {
        case 1 :
        {
            cdfLowestSearchError(bound);
        }
        break;
        case 2 :
        {
            cdfGreatestSearchError(bound);
        }
        break;
        case 3 :
        {
            Scierror(999, " P + Q .ne. 1 \n");
        }
        break;
        case 4 :
        {
            Scierror(999, " Pr + Ompr .ne. 1 \n");
        }
        break;
        default :
        {
            CdfDefaultError(param, status, bound);
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
