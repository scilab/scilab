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
  |   11) Interface code for getting the Choleky factorization  |
  |       p and C                                               |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [C,p] = taucs_chget(C_ptr)                                |
  |                                                             |
  |    C  : the upper triangle                                  |
  |    p  : the permutation                                     |
  |                                                             |
  +------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "localization.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chget(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    int nnz = 0, i = 0, pl_miss = 0, it_flag = 0;
    int* lp = NULL;
    taucs_handle_factors * pC;
    taucs_ccs_matrix * C;
    int* piAddr1 = NULL;
    void* pvPtr = NULL;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 3);

    /* get the pointer to the Choleski factorization handle */
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

    /* Check if the pointer is a valid ref to ... */
    if (! IsAdrInList( (Adr)pC, ListCholFactors, &it_flag) )
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be a valid reference to a Cholesky factorization"), fname, 1);
        return 1;
    }

    C = taucs_supernodal_factor_to_ccs(pC->C);
    if (! C)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }

    /* set up S fields */
    nnz = 0;
    for (i = 0 ; i < C->m ; i++)
    {
        C->colptr[i] = C->colptr[i + 1] - C->colptr[i];
        nnz += C->colptr[i];
    }

    for ( i = 0 ; i < nnz ; i++ )
    {
        C->rowind[i]++;
    }

    sciErr = createSparseMatrix(pvApiCtx, 2, C->m, C->n, nnz, C->colptr, C->rowind, C->values);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        taucs_ccs_free(C);
        return 1;
    }

    /* now p */
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 3, C->m, 1, &lp);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        taucs_ccs_free(C);
        return 1;
    }

    for (i = 0 ; i < C->m ; i++)
    {
        lp[i] = pC->p[i] + 1;
    }

    taucs_ccs_free(C);

    AssignOutputVariable(pvApiCtx, 1) = 2;
    AssignOutputVariable(pvApiCtx, 2) = 3;
    ReturnArguments(pvApiCtx);
    return 0;
}
