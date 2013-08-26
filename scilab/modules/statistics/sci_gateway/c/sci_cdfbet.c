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
extern int C2F(cdfbet)(int *, double *, double *, double *, double *, double *, double *, int *, double *);
/*--------------------------------------------------------------------------*/
static void cdfbetErr(int status, double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdfbet
*
*      SUBROUTINE CDFBET( WHICH, P, Q, X, Y, A, B, STATUS, BOUND )
*               Cumulative Distribution Function
*                         BETa Distribution
*/
/*--------------------------------------------------------------------------*/
int cdfbetI(char* fname, unsigned long l)
{
    int minrhs = 5, maxrhs = 6, minlhs = 1, maxlhs = 2, m1 = 0, n1 = 0, l1 = 0;
    Nbvars = 0;
    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1), "PQ") == 0)
    {
        static int callpos[6] = {4, 5, 0, 1, 2, 3};
        CdfBase(fname, 4, 2, callpos, "PQ", _("X,Y,A and B"), 1, C2F(cdfbet),
                cdfbetErr);
    }
    else if ( strcmp(cstk(l1), "XY") == 0)
    {
        static int callpos[6] = {2, 3, 4, 5, 0, 1};
        CdfBase(fname, 4, 2, callpos, "XY", _("A,B,P and Q"), 2, C2F(cdfbet),
                cdfbetErr);
    }
    else if ( strcmp(cstk(l1), "A") == 0)
    {
        static int callpos[6] = {1, 2, 3, 4, 5, 0};
        CdfBase(fname, 5, 1, callpos, "A", _("B,P,Q,X and Y"), 3, C2F(cdfbet),
                cdfbetErr);
    }
    else if ( strcmp(cstk(l1), "B") == 0)
    {
        static int callpos[6] = {0, 1, 2, 3, 4, 5};
        CdfBase(fname, 5, 1, callpos, "B", _("P,Q,X,Y and A"), 4, C2F(cdfbet),
                cdfbetErr);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), fname, 1, "PQ", "XY", "A", "B");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdfbetErr(int status, double bound)
{
    static char *param[] = {"-PQXYAB"};
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
        break ;
        case 4 :
        {
            Scierror(999, " X + Y .ne. 1 \n");
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
