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
  |   5) Interface code for getting some information           |
  |      about the LU factors                                   |
  |                                                             |
  |   Scilab call                                               |
  |   -----------                                               |
  |   [OK, n, lnz, unz, it] = umf_luinfo(LU_ptr)                |
  |                                                             |
  |    OK  : a boolean true if LU_ptr is a valid pointer to an  |
  |          umfpack LU numeric handle                          |
  |    n   : size of the matrix (n,n)                           |
  |    lnz : number of non zero elements in L                   |
  |    unz : number of non zero elements in U                   |
  |    it  : 0 the factors are real, 1 the factors are complex  |
  |                                                             |
  +------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"
#include "Scierror.h"

extern CellAdr *ListNumeric;

int sci_umf_luinfo(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    void * Numeric  = NULL;
    int OK          = 0;
    int lnz         = 0;
    int unz         = 0;
    int nrow        = 0;
    int ncol        = 0;
    int nz_udiag    = 0;
    int it_flag     = 0;
    int* piAddr1    = NULL;
    int iErr        = 0;
    int iType1      = 0;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 7);

    /* get the pointer to the LU factors */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* Check if the first argument is a pointer */
    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr || iType1 != sci_pointer)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A pointer expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getPointer(pvApiCtx, piAddr1, &Numeric);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* Check if the pointer is a valid ref to ... */
    if (IsAdrInList(Numeric, ListNumeric, &it_flag))
    {
        if ( it_flag == 0 )
        {
            umfpack_di_get_lunz(&lnz, &unz, &nrow, &ncol, &nz_udiag, Numeric);
        }
        else
        {
            umfpack_zi_get_lunz(&lnz, &unz, &nrow, &ncol, &nz_udiag, Numeric);
        }

        OK = 1;

        iErr = createScalarDouble(pvApiCtx, 2, (double) nrow);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        iErr = createScalarDouble(pvApiCtx, 3, (double) ncol);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        iErr = createScalarDouble(pvApiCtx, 4, (double) lnz);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        iErr = createScalarDouble(pvApiCtx, 5, (double) unz);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        iErr = createScalarDouble(pvApiCtx, 6, (double) nz_udiag);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        iErr = createScalarDouble(pvApiCtx, 7, (double) it_flag);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }
    else
    {
        int i = 0;
        OK = 0;

        for (i = 0; i < 6; i++)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, i + 2, 0, 0, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }
        }
    }

    iErr = createScalarBoolean(pvApiCtx, 8, OK);
    if (iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 8;
    AssignOutputVariable(pvApiCtx, 2) = 2;
    AssignOutputVariable(pvApiCtx, 3) = 3;
    AssignOutputVariable(pvApiCtx, 4) = 4;
    AssignOutputVariable(pvApiCtx, 5) = 5;
    AssignOutputVariable(pvApiCtx, 6) = 6;
    AssignOutputVariable(pvApiCtx, 7) = 7;
    ReturnArguments(pvApiCtx);

    return 0;
}
