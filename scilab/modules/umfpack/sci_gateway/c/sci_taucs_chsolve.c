/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * PURPOSE: Scilab interfaces routines onto the UMFPACK sparse solver
 * (Tim Davis) and onto the TAUCS snmf choleski solver (Sivan Teledo)
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


/*------------------------------------------------------------+
  |   8) Interface code for solving Ax=b  when the Cholesky     |
  |      factors of A are known                                 |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [x] = taucs_chsolve(C_ptr,b [,A])                         |
  |                                                             |
  |      mb : number of rows of the vector b                    |
  |      nb : number of columns of the vector b (>= 1)          |
  |      lb : index of the first element of b in the stack      |
  |       b : pointer to the vector b (gotten with b = stk(lb)) |
  |       A : (optional) if provided a refiment step if taken   |
  |                                                             |
  +------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chsolve(char* fname, void* pvApiCtx)
{
    SciErr sciErr;

    int mb = 0, nb = 0;
    int i = 0, j = 0, n = 0, it_flag = 0, Refinement = 0;
    double norm_res = 0., norm_res_bis = 0.;
    long double *wk = NULL;
    int A_is_upper_triangular = 0;
    taucs_handle_factors * pC = NULL;

    SciSparse A;
    int mA              = 0; // rows
    int nA              = 0; // cols
    int iNbItem         = 0;
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    double* pdblSpReal  = NULL;
    double* pdblSpImg   = NULL;
    int iComplex        = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;

    void* pvPtr     = NULL;
    double* pdblB   = NULL;
    double* pdblX   = NULL;
    double* pdblV   = NULL;
    double* pdblRes = NULL;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /* First get arg #1 : the pointer to the Cholesky factors */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getPointer(pvApiCtx, piAddr1, &pvPtr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    pC = (taucs_handle_factors *)pvPtr;

    /* Check if this pointer is a valid ref to a Cholesky factor object */
    if ( ! IsAdrInList( (Adr)pC, ListCholFactors, &it_flag) )
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: not a valid reference to Cholesky factors"), fname, 1);
        return 1;
    }

    /*  the number of rows/lines of the matrix  */
    n = pC->n;
    /* Get now arg #2 : the vector b */
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &mb, &nb, &pdblB);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* test if the right hand side is compatible */
    if (mb != n || nb < 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
        return 1;
    }

    if (Rhs == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (isVarComplex(pvApiCtx, piAddr3))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: not compatible with the Cholesky factorization.\n"), fname, 3);
            return 1;
        }

        sciErr = getSparseMatrix(pvApiCtx, piAddr3, &mA, &nA, &iNbItem, &piNbItemRow, &piColPos, &pdblSpReal);

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // fill struct sparse
        A.m     = mA;
        A.n     = nA;
        A.it    = iComplex;
        A.nel   = iNbItem;
        A.mnel  = piNbItemRow;
        A.icol  = piColPos;
        A.R     = pdblSpReal;
        A.I     = pdblSpImg;

        if (mA != nA || mA != n)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: not compatible with the Cholesky factorization.\n"), fname, 3);
            return 1;
        }

        Refinement = 1;
        A_is_upper_triangular = is_sparse_upper_triangular(&A);
    }
    else
    {
        Refinement = 0;
    }

    /* allocate memory for the solution x */
    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, mb, nb, &pdblX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (Refinement)
    {
        pdblRes = (double*)MALLOC(mb * sizeof(double));
        if ( A_is_upper_triangular )
        {
            if ( (wk = (long double*)MALLOC( n * sizeof(long double))) == NULL )
            {
                if (pdblRes)
                {
                    FREE(pdblRes);
                }
                Scierror(999, _("%s: not enough memory.\n"), fname);
                return 1;
            }
        }
    }
    
    /* allocate memory for a temporary vector v */
    pdblV = (double*)MALLOC(mb * sizeof(double));

    for (j = 0; j < nb ; j++)
    {
        taucs_vec_permute(n, &pdblB[j * mb], &pdblX[j * mb], pC->p);
        taucs_supernodal_solve_llt(pC->C, pdblV, &pdblX[j * mb]); /* FIXME : add a test here */
        taucs_vec_ipermute(n, pdblV, &pdblX[j * mb], pC->p);
        if (Refinement)
        {
            /* do one iterative refinement */
            residu_with_prec_for_chol(&A, &pdblX[j * mb], &pdblV[j * mb], pdblRes, &norm_res, A_is_upper_triangular, wk);
            /*  FIXME: do a test if the norm_res has an anormal value and send a warning
             *         (the user has certainly not give the good matrix A
             */
            taucs_vec_permute(n, pdblRes, pdblV, pC->p);
            taucs_supernodal_solve_llt(pC->C, pdblRes, pdblV);  /* FIXME : add a test here */
            taucs_vec_ipermute(n, pdblRes, pdblV, pC->p);
            for ( i = 0 ; i < n ; i++ )
            {
                pdblV[i] = pdblX[j * mb + i] - pdblV[i]; /* v is the refined solution */
            }

            residu_with_prec_for_chol(&A, pdblV, &pdblB[j * mb], pdblRes, &norm_res_bis, A_is_upper_triangular, wk);
            /* accept it if the 2 norm of the residual is improved */
            if ( norm_res_bis < norm_res )
            {
                for ( i = 0 ; i < n ; i++ )
                {
                    pdblX[j * mb + i] = pdblV[i];
                }
            }
        }
    }

    FREE(wk);
    FREE(pdblV);
    FREE(pdblRes);

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
