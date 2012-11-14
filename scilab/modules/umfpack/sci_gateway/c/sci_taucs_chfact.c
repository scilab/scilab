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
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
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
#include "MALLOC.h"
#include "localization.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chfact(char* fname, unsigned long l)
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


