/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#include <string.h>
#include "gw_optimization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "numericconstants_interface.h"
/*--------------------------------------------------------------------------*/
/* fortran subroutines */
extern int C2F(ql0001)(int *m, int *me, int *mmax, int *n, int *nmax, int *mnn,
                       double *c, double *d, double *a, double *b, double *xl,
                       double *xu, double *x, double *u, int *iout, int *ifail,
                       int *iprint, double *war, int *lwar, int *iwar, int *liwar,
                       double *eps1);
/*--------------------------------------------------------------------------*/
int sci_qld(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    static int un = 1, zero = 0;
    static int n = 0, nbis = 0;
    static int unbis = 0;
    static int mmax = 0, m = 0, mnn = 0;
    static int mbis = 0;
    static int pipo = 0;
    static int ifail = 0;
    int next = 0;

    static int lwar = 0, iout = 0, k = 0, l = 0;
    static double eps1 = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* piAddr5 = NULL;
    int* piAddr6 = NULL;
    int* piAddr7 = NULL;

    double* Q  = NULL;
    double* p  = NULL;
    double* C  = NULL;
    double* b  = NULL;
    double* lb = NULL;
    double* ub = NULL;
    int* me    = NULL;

    double* x       = NULL;
    double* lambda  = NULL;
    int* inform     = NULL;
    double* war     = NULL;
    int* iwar       = NULL;

    /*   Check rhs and lhs   */
    CheckInputArgument(pvApiCtx, 7, 8) ;
    CheckOutputArgument(pvApiCtx, 1, 3) ;

    /* RhsVar: qld(Q,p,C,b,lb,ub,me,eps) */
    /*             1,2,3,4,5 ,6 ,7, 8  */
    eps1 = nc_eps();
    next = nbInputArgument(pvApiCtx) + 1;
    /*   Variable 1 (Q)   */
    //get variable address

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &n, &nbis, &Q);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckSquare
    if (n != nbis)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, 1);
        return 1;
    }


    /*   Variable 2 (p)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &nbis, &unbis, &p);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckLength
    if (nbis * unbis != n)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 2, nbis * unbis);
        return 1;
    }


    /*   Variable 3 (C)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &m, &nbis, &C);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        printError(&sciErr, 0);
        return 1;
    }

    if (( nbis != n ) && (m > 0))
    {
        Scierror(205, _("%s: Wrong size for input argument #%d: number of columns %d expected.\n"), fname, 3, n);
        return 0;
    }
    mmax = m;
    mnn = m + n + n;

    /*   Variable 4 (b)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &mbis, &unbis, &b);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckLength
    if (mbis * unbis != m)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 4, mbis * unbis);
        return 1;
    }


    /*   Variable 5 (lb)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &nbis, &unbis, &lb);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        printError(&sciErr, 0);
        return 1;
    }

    if (nbis * unbis == 0)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, next, n, un, &lb);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        for (k = 0; k < n; k++)
        {
            (lb)[k] = -nc_double_max();
        }
        next = next + 1;
    }
    else if (nbis * unbis != n) //CheckLength
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 5, nbis * unbis);
        return 1;
    }

    /*   Variable 6 (ub)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 6.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr6, &nbis, &unbis, &ub);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 6);
        printError(&sciErr, 0);
        return 1;
    }

    if (nbis * unbis == 0)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, next, n, un, &ub);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        for (k = 0; k < n; k++)
        {
            (ub)[k] = nc_double_max();
        }
        next = next + 1;
    }
    else if (nbis * unbis != n)//CheckLength
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 6, nbis * unbis);
        return 1;
    }


    /*   Variable 7 (me)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 7.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr7, &pipo, &unbis, &me);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 7);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckScalar
    if (pipo != 1 || unbis != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 7);
        return 1;
    }

    if ((*(me) < 0) || (*(me) > n))
    {
        // FIX ME
        //        Err = 7;
        SciError(116);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) == 8)
    {
        /*   Variable 8 (eps1)   */
        //get variable address
        int* piAddr8 = NULL;
        double* leps = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddr8);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 8.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr8, &pipo, &unbis, &leps);
        if (sciErr.iErr)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 8);
            printError(&sciErr, 0);
            return 1;
        }

        //CheckScalar
        if (pipo != 1 || unbis != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 8);
            return 1;
        }

        eps1 = Max(eps1, *leps);
    }

    /* Internal variables: x, lambda, inform, C_mmax, b_mmax */
    sciErr = allocMatrixOfDouble(pvApiCtx, next, n, un, &x);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, next + 1, mnn, un, &lambda);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, next + 2, un, un, &inform);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }


    lwar = 3 * n * n / 2 + 10 * n + 2 * mmax + 2;
    sciErr = allocMatrixOfDouble(pvApiCtx, next + 3, lwar, un, &war);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, next + 4, n, un, &iwar);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    (iwar)[0] = 1; /*Cholesky factorization required*/

    /* Change the sign of C*/
    for (k = 0; k < n; k++)
    {
        for (l = 0; l < m; l++)
        {
            (C)[k * m + l] = -(C)[k * m + l];
        }
    }

    iout = 0;
    ifail = 0;
    C2F(ql0001)(&m, (me), &mmax, &n, &n, &mnn, (Q), (p), (C),
                (b), (lb), (ub), (x), (lambda), &iout,
                &ifail, &zero, (war), &lwar, (iwar), &n, &eps1);


    /* LhsVar: [x, lambda, inform] = qld(...) */
    if (ifail == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = next;
        AssignOutputVariable(pvApiCtx, 2) = next + 1;
        if (nbOutputArgument(pvApiCtx) == 3)
        {
            AssignOutputVariable(pvApiCtx, 3) = next + 2;
            *(inform) = ifail;
        }
        ReturnArguments(pvApiCtx);
    }
    else if (ifail == 1)
    {
        Scierror(24, _("%s: Too many iterations (more than %d).\n"), fname, 40 * (n + m));
    }
    else if (ifail == 2)
    {
        Scierror(24, _("%s: Accuracy insufficient to satisfy convergence criterion.\n"), fname);
    }
    else if (ifail == 5)
    {
        Scierror(999, _("%s: Length of working array is too short.\n"), fname);
    }
    else if (ifail > 10)
    {
        Scierror(999, _("%s: The constraints are inconsistent.\n"), fname);
    }
    else
    {
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
