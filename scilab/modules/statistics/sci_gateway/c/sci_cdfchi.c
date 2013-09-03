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
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cdfchi)(int *, double *, double *, double *, double *, int *, double *);
/*--------------------------------------------------------------------------*/
static void cdfchiErr(int status, double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdfchi
*
*      SUBROUTINE CDFCHI( WHICH, P, Q, X, DF, STATUS, BOUND )
*              Cumulative Distribution Function
*              CHI-Square distribution
*/
/*--------------------------------------------------------------------------*/
int cdfchiI(char* fname, unsigned long l)
{
    int m1 = 0, n1 = 0, l1 = 0, mDf = 0, nDf = 0, lDf = 0, i = 0;
    double *Df = NULL;
    Nbvars = 0;
    CheckRhs(3, 4);
    CheckLhs(1, 2);
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1), "PQ") == 0)
    {
        static int callpos[4] = {2, 3, 0, 1};
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 3);
            }
        CdfBase(fname, 2, 2, callpos, "PQ", _("X and Df"), 1, C2F(cdfchi),
                cdfchiErr);
    }
    else if ( strcmp(cstk(l1), "X") == 0)
    {
        static int callpos[4] = {1, 2, 3, 0};
        GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 2);
            }
        CdfBase(fname, 3, 1, callpos, "X", _("Df, P and Q"), 2, C2F(cdfchi),
                cdfchiErr);
    }
    else if ( strcmp(cstk(l1), "Df") == 0)
    {
        static int callpos[4] = {0, 1, 2, 3};
        CdfBase(fname, 3, 1, callpos, "Df", _("P,Q and X"), 3, C2F(cdfchi),
                cdfchiErr);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 1, "PQ", "X", "Df");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdfchiErr(int status, double bound)
{
    static char *param[7] = {"X", "P", "Q", "X", "Df"};
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
        case 10 :
        {
            Scierror(999, _("cdfchi: Error in cumgam\n"));
        }
        break ;
        default :
        {
            CdfDefaultError(param, status, bound);
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
