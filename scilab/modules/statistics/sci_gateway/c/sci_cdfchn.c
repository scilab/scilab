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
extern int C2F(cdfchn)(int *, double *, double *, double*, double *, double *, int *, double *);
/*--------------------------------------------------------------------------*/
static void cdfchnErr(int status, double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdfchn
*  Non-central Chi-Square
*/
int cdfchnI(char* fname, unsigned long l)
{
    int m1 = 0, n1 = 0, l1 = 0, mDf = 0, nDf = 0, lDf = 0, i = 0;
    double *Df = NULL;
    Nbvars = 0;
    CheckRhs(4, 5);
    CheckLhs(1, 2);
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1), "PQ") == 0)
    {
        static int callpos[5] = {3, 4, 0, 1, 2};
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 3);
            }
        CdfBase(fname, 3, 2, callpos, "PQ", _("X,Df and Pnonc"), 1, C2F(cdfchn),
                cdfchnErr);
    }
    else if ( strcmp(cstk(l1), "X") == 0)
    {
        static int callpos[5] = {2, 3, 4, 0, 1};
        GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 2);
            }
        CdfBase(fname, 4, 1, callpos, "X", _("Df,Pnonc,P and Q"), 2, C2F(cdfchn),
                cdfchnErr);
    }
    else if ( strcmp(cstk(l1), "Df") == 0)
    {
        static int callpos[5] = {1, 2, 3, 4, 0};
        CdfBase(fname, 4, 1, callpos, "Df", _("Pnonc,P,Q and X"), 3, C2F(cdfchn),
                cdfchnErr);
    }
    else if ( strcmp(cstk(l1), "Pnonc") == 0)
    {
        static int callpos[5] = {0, 1, 2, 3, 4};
        GetRhsVar(5, MATRIX_OF_DOUBLE_DATATYPE, &mDf, &nDf, &lDf);
        Df = stk(lDf);
        for (i = 0; i < mDf * nDf; ++i)
            if ((int) Df[i] - Df[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 5);
            }
        CdfBase(fname, 4, 1, callpos, "Pnonc", _("P,Q,X and Df"), 4, C2F(cdfchn),
                cdfchnErr);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), fname, 1, "PQ", "X", "Df", "Pnonc");

    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdfchnErr(int status, double bound)
{
    static char *param[7] = {"X", "P", "Q", "F", "Dfn", "Dfd"};
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
        default :
        {
            CdfDefaultError(param, status, bound);
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
