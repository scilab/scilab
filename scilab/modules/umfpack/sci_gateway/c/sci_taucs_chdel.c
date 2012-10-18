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
#include "MALLOC.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern CellAdr *ListCholFactors;
/*--------------------------------------------------------------------------*/
int sci_taucs_chdel(char* fname, unsigned long l)
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
