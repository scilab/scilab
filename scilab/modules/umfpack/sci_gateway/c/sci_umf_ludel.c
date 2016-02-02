/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 *  Copyright (C) 2012 - DIGITEO - Allan CORNET
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
|   4) Interface code to free the memory                      |
|      used by the LU factors                                 |
|                                                             |
|   Scilab call                                               |
|   -----------                                               |
|   umf_ludel(LU_ptr)  or umf_ludel() for freed all fact.     |
|                                                             |
+------------------------------------------------------------*/
#include "api_scilab.h"
#include "sci_malloc.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "Scierror.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern CellAdr *ListNumeric;
/*--------------------------------------------------------------------------*/
int sci_umf_ludel(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    int it_flag     = 0;
    void * Numeric  = NULL;
    int* piAddr1    = NULL;
    CellAdr *Cell   = NULL;
    int iType1      = 0;

    nbInputArgument(pvApiCtx) = Max(nbInputArgument(pvApiCtx), 0);

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 0)      /* destroy all */
    {
        while ( ListNumeric )
        {
            Cell = ListNumeric;
            ListNumeric = ListNumeric->next;
            if (Cell->it == 0)
            {
                umfpack_di_free_numeric(&(Cell->adr));
            }
            else
            {
                umfpack_zi_free_numeric(&(Cell->adr));
            }
            FREE(Cell);
        }
    }
    else
    {
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
        if (RetrieveAdrFromList(Numeric, &ListNumeric, &it_flag))
        {
            /* free the memory of the numeric object */
            if ( it_flag == 0 )
            {
                umfpack_di_free_numeric(&Numeric);
            }
            else
            {
                umfpack_zi_free_numeric(&Numeric);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be a valid reference to (umf) LU factors.\n"), fname, 1);
            return 1;
        }
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
