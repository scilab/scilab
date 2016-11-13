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
  | 7) Interface code for the Cholesky factorization of A       |
  |                                                             |
  |    Scilab call                                              |
  |    -----------                                              |
  |        [C_ptr] = taucs_chfact(A)                            |
  |                                                             |
  |             A : a real symetric definite pos. sparse matrix |
  |         C_ptr : a pointer to the Cholesky fact              |
  |                                                             |
  |   Var description (to complete...)                          |
  |   ---------------                                           |
  |      mA : number of rows of the matrix A                    |
  |      nA : number of columns of A                            |
  |       A : pointer to a sparse matrix struct                 |
  |                                                             |
  |      Require mA == nA (matrix must be square), mA > 0       |
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

int sci_taucs_chfact(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    int stat = 0;
    int* perm = NULL;
    int* invperm = NULL;
    taucs_ccs_matrix *PAPT;
    taucs_ccs_matrix B;
    void *C = NULL;
    taucs_handle_factors *pC;

    SciSparse A;
    int mA              = 0; // rows
    int nA              = 0; // cols
    int iNbItem         = 0;
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    double* pdblSpReal  = NULL;
    double* pdblSpImg   = NULL;
    int iComplex        = 0;
    int* piAddr1        = NULL;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /* get A the sparse matrix to factorize */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr1))
    {
        iComplex = 1;
        sciErr = getComplexSparseMatrix(pvApiCtx, piAddr1, &mA, &nA, &iNbItem, &piNbItemRow, &piColPos, &pdblSpReal, &pdblSpImg);
    }
    else
    {
        sciErr = getSparseMatrix(pvApiCtx, piAddr1, &mA, &nA, &iNbItem, &piNbItemRow, &piColPos, &pdblSpReal);
    }

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

    stat = spd_sci_sparse_to_taucs_sparse(&A, &B);
    if ( stat != A_PRIORI_OK )
    {
        if ( stat == MAT_IS_NOT_SPD )
        {
            freeTaucsSparse(B);
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be symmetric positive definite matrix."), fname, 1);
        }
        /* the message for the other problem (not enough memory in stk) is treated automaticaly */
        return 1;
    }

    /* find the permutation */
    taucs_ccs_genmmd(&B, &perm, &invperm);
    if ( !perm )
    {
        freeTaucsSparse(B);
        Scierror(999, _("%s: No more memory.\n") , fname);
        return 1;
    }

    /* apply permutation */
    PAPT = taucs_ccs_permute_symmetrically(&B, perm, invperm);
    FREE(invperm);
    freeTaucsSparse(B);

    /* factor */
    C = taucs_ccs_factor_llt_mf(PAPT);
    taucs_ccs_free(PAPT);

    if (C == NULL)
    {
        /*   Note : an error indicator is given in the main scilab window
         *          (out of memory, no positive definite matrix , etc ...)
         */
        Scierror(999, _("%s: An error occurred: %s\n"), fname, _("factorization"));
        return 1;
    }

    /* put in an handle (Chol fact + perm + size) */
    pC = (taucs_handle_factors*)MALLOC( sizeof(taucs_handle_factors) );
    pC->p = perm;
    pC->C = C;
    pC->n = A.n;

    /* add in the list of Chol Factors  */
    AddAdrToList((Adr) pC, 0, &ListCholFactors);  /* FIXME add a test here .. */

    /* create the scilab object to store the pointer onto the Chol handle */
    sciErr = createPointer(pvApiCtx, 2, (void *)pC);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* return the pointer */
    AssignOutputVariable(pvApiCtx, 1) = 2;
    ReturnArguments(pvApiCtx);
    return 0;
}


