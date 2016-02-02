/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Serge Steer
* Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
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
#include "sci_malloc.h"
#include "Scierror.h"
#include "sciprint.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "scisparse.h"
#include "Sciwarning.h"
/*--------------------------------------------------------------------------*/
/* fortran subroutines */
extern int C2F(qpgen2)(double *dmat, double *dvec, int *fddmat, int *n,
                       double *sol, double *crval, double *amat, double *bvec,
                       int *fdamat, int *q, int *meq, int *iact, int *nact,
                       int *iter, double *work, int *ierr);
extern int C2F(qpgen1sci)(double *dmat, double *dvec, int *fddmat, int *n, double *sol, double *crval,
                          int *colnnz, int *nzrindex, double *amat,
                          double *bvec, int *q, int *meq, int *iact, int *nact,
                          int *iter, double *work, int *ierr);
extern int C2F(spt)(int *m, int *n, int *nel, int *it, int *ptr,
                    double *A_R,  double *A_I, int *mnel,  int *A_icol,
                    double *At_R, double *At_I, int *At_mnel, int *At_icol);
/*--------------------------------------------------------------------------*/
int sci_qp_solve(char *fname,  void* pvApiCtx)
{
    SciErr sciErr;
    static int un = 1, deux = 2;
    // n : first dimension of Q
    // nbis : second dimension of Q (nbis is expected to be equal to n)
    static int n = 0, nbis = 0;
    static int unbis = 0;
    static int m = 0, next = 0;
    static int mbis = 0;
    static int pipo = 0;
    static int nact = 0;
    int r = 0;
    static int lw = 0,  k = 0;
    static SciSparse Sp;
    static int issparse = 0;
    double *work = NULL;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* piAddr5 = NULL;

    double* Q = NULL;
    double* C = NULL;
    double* p = NULL;
    double* b = NULL;
    int* me   = NULL;

    double* x = NULL;
    int* iact = NULL;
    int* iter = NULL;
    double* crval = NULL;
    int *ierr = NULL;


    /*   Check rhs and lhs   */
    CheckInputArgument(pvApiCtx, 5, 5) ;
    CheckOutputArgument(pvApiCtx, 1, 5) ;

    /*Warning this interface does not support arguments passed by reference */

    /* RhsVar: qp_solve(Q,p,C,b,me) */
    /*                1,2,3,4,5   */

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
    issparse =  (checkInputArgumentType(pvApiCtx, 3, 5));
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (!issparse)
    {
        // Retrieve a matrix of double at position 3.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &nbis, &m, &C);
        if (sciErr.iErr)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
            printError(&sciErr, 0);
            return 1;
        }
    }
    else
    {
        if (isVarComplex(pvApiCtx, piAddr3))
        {
            Sp.it = 1;
            sciErr = getComplexSparseMatrix(pvApiCtx, piAddr3, &(Sp.m), &(Sp.n), &(Sp.nel), &(Sp.mnel), &(Sp.icol), &(Sp.R), &(Sp.I));
        }
        else
        {
            Sp.it = 0;
            sciErr = getSparseMatrix(pvApiCtx, piAddr3, &(Sp.m), &(Sp.n), &(Sp.nel), &(Sp.mnel), &(Sp.icol), &(Sp.R));
        }

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        nbis = Sp.m;
        m = Sp.n;
    }

    if ( nbis != n ) // car C est passee en transposee dans la macro qpsolve
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d column(s) expected for matrix %s.\n"), fname, 3, n, "C");
        return 0;
    }

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


    /*   Variable 5 (me)   */
    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr5, &pipo, &unbis, &me);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckScalar
    if (pipo != 1 || unbis != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 5);
        return 1;
    }

    if ((*(me) < 0) || (*(me) > n))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: %s must be an integer in the range 0 to %d.\n"), fname, 5, "me", n);
        return 0;
    }

    /* nbOutputArgument(pvApiCtx) variables: x, iact, iter, crval */
    next = Rhs;
    sciErr = allocMatrixOfDouble(pvApiCtx, next + 1, n, un, &x);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, next + 2, m, un, &iact);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, next + 3, deux, un, &iter);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, next + 4, un, un, &crval);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, next + 5, un, un, &ierr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }


    r = Min(n, m);
    lw =  2 * n + r * (r + 5) / 2 + 2 * m + 1;
    if ((work = (double *)MALLOC(lw * sizeof(double))) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        return 1;
    }
    /* change the sign of  C and b.*/
    *ierr = 0;
    if (!issparse)
    {
        /* linear constraints matrix is stored full */
        C2F(qpgen2)((Q), (p), &n, &n,  (x), (crval), (C),
                    (b), &n, &m, (me), (iact), &nact, (iter), work,
                    (ierr));
    }
    else
    {
        /* linear constraints matrix is a sparse matrix */
        /* Change the linear constraints matrix representation:
        qpgen1sci requires column-compressed sparse matrix internal
        representation while Scilab sparse matrices are row-compressed */
        double *R = NULL, *I = NULL;
        int *ind = NULL;

        if ((R = (double *)MALLOC(Sp.nel * sizeof(double))) == NULL)
        {
            FREE(work);
            work = NULL;
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            return 1;
        }
        if ((ind = (int *)MALLOC((m + Sp.nel) * sizeof(int))) == NULL)
        {
            FREE(work);
            work = NULL;
            FREE(R);
            R = NULL;
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            return 1;
        }

        // Transpose the sparse matrix A
        C2F(spt)(&n, &m, &(Sp.nel) ,  &(Sp.it), (int *)work,
                 Sp.R,  Sp.I,  Sp.mnel,  Sp.icol, R, I, ind, ind + m);

        C2F(qpgen1sci)((Q), (p), &n, &n,  (x), (crval),
                       ind, ind + m,  R,
                       (b), &m, (me), (iact), &nact, (iter),
                       work, (ierr));
        FREE(work);
        work = NULL;
        FREE(R);
        R = NULL;
        FREE(ind);
        ind = NULL;
    }

    for (k = nact; k < m; k++)
    {
        (iact)[k] = 0;
    }
    /* LhsVar: [x, iact, iter, f] = qp_solve(...) */
    if (Lhs != 5)
    {
        if (*ierr == 0)
        {
            for (k = 0; k < Lhs; k++)
            {
                AssignOutputVariable(pvApiCtx, 1 + k) = next + 1 + k;
            }
            ReturnArguments(pvApiCtx);
        }
        else if (*ierr == 1)
        {
            Scierror(999, _("%s: The minimization problem has no solution.\n"), fname);
        }
        else if (*ierr == 2)
        {
            Scierror(999, _("%s: Q is not symmetric positive definite.\n"), fname);
        }
    }
    else
    {
        for (k = 0; k < Lhs; k++)
        {
            AssignOutputVariable(pvApiCtx, 1 + k) = next + 1 + k;
        }
        if (*ierr == 1)
        {
            if (getWarningMode())
            {
                Sciwarning(_("\n%s: Warning: The minimization problem has no solution. The results may be inaccurate.\n\n"), fname);
            }
        }
        else if (*ierr == 2)
        {
            if (getWarningMode())
            {
                Sciwarning(_("\n%s: Warning: Q is not symmetric positive definite. The results may be inaccurate.\n\n"), fname);
            }
        }
        ReturnArguments(pvApiCtx);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
