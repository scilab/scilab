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
|   9) Interface code to free the memory                      |
|      used by the Cholesky factors (C + permutation)         |
|                                                             |
|   Scilab call                                               |
|   -----------                                               |
|   taucs_chdel(LU_ptr)  or taucs_chdel() for freed all fact. |
|                                                             |
+------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "taucs_scilab.h"
#include "gw_umfpack.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern CellAdr *ListCholFactors;
/*--------------------------------------------------------------------------*/
int sci_taucs_chdel(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    int it_flag = 0;
    taucs_handle_factors * pC = NULL;
    CellAdr * Cell = NULL;

    int* piAddr1 = NULL;
    void* pvPtr  = NULL;

    nbInputArgument(pvApiCtx) = Max(nbInputArgument(pvApiCtx), 0);

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 0)      /* destroy all */
    {
        while ( ListCholFactors )
        {
            Cell = ListCholFactors;
            ListCholFactors = ListCholFactors->next;
            pC = (taucs_handle_factors *) Cell->adr;
            taucs_supernodal_factor_free(pC->C);  /* free the super nodal struct */
            FREE(pC->p);                          /* free the permutation vector */
            FREE(pC);                             /* free the handle             */
            FREE(Cell);
        }
    }
    else
    {
        /* get the pointer to the Cholesky factors */
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
        if (RetrieveAdrFromList(pC , &ListCholFactors, &it_flag))
            /* free the memory of the objects */
        {
            taucs_supernodal_factor_free(pC->C);
            FREE(pC->p);
            FREE(pC);
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: not a valid reference to Cholesky factors.\n"), fname, 1);
            return 1;
        }
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
