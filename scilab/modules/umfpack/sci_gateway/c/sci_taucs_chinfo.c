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
  |   10) Interface code for getting some information           |
  |       about the Cholesky factor                             |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [OK, n, cnz] = taucs_chinfo(C_ptr)                        |
  |                                                             |
  |    OK  : a boolean true if C_ptr is a valid pointer to an   |
  |          taucs Choleski numeric handle                      |
  |    n   : size of the matrix (n,n)                           |
  |    cnz : number of non zero elements in C                   |
  |                                                             |
  +------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"
#include "Scierror.h"

extern CellAdr *ListCholFactors;

int sci_taucs_chinfo(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    taucs_handle_factors * pC;

    int OK      = 0;
    int cnz     = 0;
    int n       = 0;
    int it_flag = 0;
    int iErr    = 0;

    void* pvPtr  = NULL;
    int* piAddr1 = NULL;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 3);

    /* get the pointer to the Choleski handle factor */
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
    if ( IsAdrInList( (Adr)pC, ListCholFactors, &it_flag) )
    {
        n   = pC->n;
        cnz = taucs_get_nnz_from_supernodal_factor(pC->C);
        OK  = 1;
    }
    else
    {
        OK  = 0;
        cnz = 0;
        n   = 0;
    }

    iErr = createScalarBoolean(pvApiCtx, 2, OK);
    if (iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iErr = createScalarDouble(pvApiCtx, 3, (double) n);
    if (iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iErr = createScalarDouble(pvApiCtx, 4, (double) cnz);
    if (iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 2;
    AssignOutputVariable(pvApiCtx, 2) = 3;
    AssignOutputVariable(pvApiCtx, 3) = 4;
    ReturnArguments(pvApiCtx);
    return 0;
}
