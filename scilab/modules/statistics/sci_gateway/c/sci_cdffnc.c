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
extern int C2F(cdffnc)(int *, double *, double *, double *, double *, double *, double *, int *, double *);
/*--------------------------------------------------------------------------*/
static void cdffncErr(int status, double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*  Interface for cdffnc
*  Cumulative Distribution Function
*  Non-central F distribution
*/
/*--------------------------------------------------------------------------*/
int cdffncI(char* fname, unsigned long l)
{
    int m1 = 0, n1 = 0, l1 = 0, mDfd = 0, nDfd = 0, lDfd = 0, mDfn = 0, nDfn = 0, lDfn = 0, i = 0;
    double *Dfd = NULL, *Dfn = NULL;
    Nbvars = 0;
    CheckRhs(5, 6);
    CheckLhs(1, 2);
    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1), "PQ") == 0)
    {
        static int callpos[6] = {4, 5, 0, 1, 2, 3};
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mDfn, &nDfn, &lDfn);
        Dfn = stk(lDfn);
        for (i = 0; i < mDfn * nDfn; ++i)
            if ((int) Dfn[i] - Dfn[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 3);
            }
        GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &mDfd, &nDfd, &lDfd);
        Dfd = stk(lDfd);
        for (i = 0; i < mDfd * nDfd; ++i)
            if ((int) Dfd[i] - Dfd[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 4);
            }
        CdfBase(fname, 4, 2, callpos, "PQ", _("F,Dfn,Dfd and Pnonc"), 1, C2F(cdffnc),
                cdffncErr);
    }
    else if ( strcmp(cstk(l1), "F") == 0)
    {
        static int callpos[6] = {3, 4, 5, 0, 1, 2};
        GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mDfn, &nDfn, &lDfn);
        Dfn = stk(lDfn);
        for (i = 0; i < mDfn * nDfn; ++i)
            if ((int) Dfn[i] - Dfn[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 2);
            }
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mDfd, &nDfd, &lDfd);
        Dfd = stk(lDfd);
        for (i = 0; i < mDfd * nDfd; ++i)
            if ((int) Dfd[i] - Dfd[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 3);
            }
        CdfBase(fname, 5, 1, callpos, "F", _("Dfn,Dfd,Pnonc,P and Q"), 2, C2F(cdffnc),
                cdffncErr);
    }
    else if ( strcmp(cstk(l1), "Dfn") == 0)
    {
        static int callpos[6] = {2, 3, 4, 5, 0, 1};
        GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mDfd, &nDfd, &lDfd);
        Dfd = stk(lDfd);
        for (i = 0; i < mDfd * nDfd; ++i)
            if ((int) Dfd[i] - Dfd[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 2);
            }
        CdfBase(fname, 5, 1, callpos, "Dfn", _("Dfd,Pnonc,P,Q and F"), 3, C2F(cdffnc),
                cdffncErr);
    }
    else if ( strcmp(cstk(l1), "Dfd") == 0)
    {
        static int callpos[6] = {1, 2, 3, 4, 5, 0};
        GetRhsVar(6, MATRIX_OF_DOUBLE_DATATYPE, &mDfn, &nDfn, &lDfn);
        Dfn = stk(lDfn);
        for (i = 0; i < mDfn * nDfn; ++i)
            if ((int) Dfn[i] - Dfn[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 6);
            }
        CdfBase(fname, 5, 1, callpos, "Dfd", _("Pnonc,P,Q,F and Dfn"), 4, C2F(cdffnc),
                cdffncErr);
    }
    else if ( strcmp(cstk(l1), "Pnonc") == 0)
    {
        static int callpos[6] = {0, 1, 2, 3, 4, 5};
        GetRhsVar(5, MATRIX_OF_DOUBLE_DATATYPE, &mDfn, &nDfn, &lDfn);
        Dfn = stk(lDfn);
        for (i = 0; i < mDfn * nDfn; ++i)
            if ((int) Dfn[i] - Dfn[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 5);
            }
        GetRhsVar(6, MATRIX_OF_DOUBLE_DATATYPE, &mDfd, &nDfd, &lDfd);
        Dfd = stk(lDfd);
        for (i = 0; i < mDfd * nDfd; ++i)
            if ((int) Dfd[i] - Dfd[i] != 0)
            {
                sciprint(_("%s: Warning: using non integer values for argument #%d may lead to incorrect results.\n"), fname, 6);
            }
        CdfBase(fname, 5, 1, callpos, "Pnonc", _("P,Q,F,Dfn and Dfd"), 5, C2F(cdffnc),
                cdffncErr);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s', '%s' or '%s' expected.\n"), fname, 1, "PQ", "F", "Dfn", "Dfd", "Pnonc");

    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdffncErr(int status, double bound)
{
    static char *param[7] = {"X", "P", "Q", "F", "Dfn", "Dfd", "Pnonc"};
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
