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

int sci_taucs_chinfo(char* fname, unsigned long l)
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
