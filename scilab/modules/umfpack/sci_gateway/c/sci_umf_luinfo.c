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

int sci_umf_luinfo(char* fname, unsigned long l)
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
