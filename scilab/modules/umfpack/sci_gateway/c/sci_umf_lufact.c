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
| 2) Interface code for the  LU factorization of A            |
|                                                             |
|    Scilab call                                              |
|    -----------                                              |
|        [LU_ptr] = umf_lufact(A)                             |
|                                                             |
|             A : a square real sparse matrix                 |
|        LU_ptr : a pointer to the LU fact                    |
|                                                             |
|   Var description (to complete...)                          |
|   ---------------                                           |
|      mA : number of rows of the matrix A                    |
|      nA : number of columns of A                            |
|       A : pointer to a sparse matrix struct                 |
|                                                             |
|      Require mA, nA  > 0                                    |
|                                                             |
+------------------------------------------------------------*/
#include "api_scilab.h"
#include "gw_umfpack.h"
#include "sciumfpack.h"
#include "Scierror.h"
#include "sciprint.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "Sciwarning.h"

CellAdr *ListNumeric = NULL;
CellAdr *ListCholFactors = NULL;

/* RAJOUTER un controle sur la taille du pivot */

int sci_umf_lufact(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    int stat = 0;
    SciSparse AA;
    CcsSparse A;

    int mA              = 0; // rows
    int nA              = 0; // cols
    int iNbItem         = 0;
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    double* pdblSpReal  = NULL;
    double* pdblSpImg   = NULL;

    /* umfpack stuff */
    double* Control = NULL;
    double* Info    = NULL;
    void* Symbolic  = NULL;
    void* Numeric   = NULL;

    int* piAddr1 = NULL;
    int iComplex = 0;
    int iType1   = 0;

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

    /* check if the first argument is a sparse matrix */
    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr || iType1 != sci_sparse)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), fname, 1);
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
        FREE(piNbItemRow);
        FREE(piColPos);
        FREE(pdblSpReal);
        if (pdblSpImg)
        {
            FREE(pdblSpImg);
        }
        printError(&sciErr, 0);
        return 1;
    }

    // fill struct sparse
    AA.m     = mA;
    AA.n     = nA;
    AA.it    = iComplex;
    AA.nel   = iNbItem;
    AA.mnel  = piNbItemRow;
    AA.icol  = piColPos;
    AA.R     = pdblSpReal;
    AA.I     = pdblSpImg;

    if (nA <= 0 || mA <= 0)
    {
        FREE(piNbItemRow);
        FREE(piColPos);
        FREE(pdblSpReal);
        if (pdblSpImg)
        {
            FREE(pdblSpImg);
        }
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
        return 1;
    }

    SciSparseToCcsSparse(&AA, &A);

    FREE(piNbItemRow);
    FREE(piColPos);
    FREE(pdblSpReal);
    if (pdblSpImg)
    {
        FREE(pdblSpImg);
    }

    /* symbolic factorization */
    if (A.it == 1)
    {
        stat = umfpack_zi_symbolic(nA, mA, A.p, A.irow, A.R, A.I, &Symbolic, Control, Info);
    }
    else
    {
        stat = umfpack_di_symbolic(nA, mA, A.p, A.irow, A.R, &Symbolic, Control, Info);
    }

    if (stat != UMFPACK_OK)
    {
        freeCcsSparse(A);
        Scierror(999, _("%s: An error occurred: %s: %s\n"), fname, _("symbolic factorization"), UmfErrorMes(stat));
        return 1;
    }

    /* numeric factorization */
    if (A.it == 1)
    {
        stat = umfpack_zi_numeric(A.p, A.irow, A.R, A.I, Symbolic, &Numeric, Control, Info);
    }
    else
    {
        stat = umfpack_di_numeric(A.p, A.irow, A.R, Symbolic, &Numeric, Control, Info);
    }

    if (A.it == 1)
    {
        umfpack_zi_free_symbolic(&Symbolic);
    }
    else
    {
        umfpack_di_free_symbolic(&Symbolic);
    }

    if ( stat != UMFPACK_OK  &&  stat != UMFPACK_WARNING_singular_matrix )
    {
        freeCcsSparse(A);
        Scierror(999, _("%s: An error occurred: %s: %s\n"), fname, _("symbolic factorization"), UmfErrorMes(stat));
        return 1;
    }

    if ( stat == UMFPACK_WARNING_singular_matrix  &&  mA == nA )
    {
        if (getWarningMode())
        {
            Sciwarning("\n%s:%s\n", _("Warning"), _("The (square) matrix appears to be singular."));
        }
    }

    /*  add the pointer in the list ListNumeric  */
    if (! AddAdrToList(Numeric, A.it, &ListNumeric))
    {
        /* AddAdrToList return 0 if malloc have failed : as it is just
        for storing 2 pointers this is unlikely to occurs but ... */
        if (A.it == 1)
        {
            umfpack_zi_free_numeric(&Numeric);
        }
        else
        {
            umfpack_di_free_numeric(&Numeric);
        }

        freeCcsSparse(A);
        Scierror(999, _("%s: An error occurred: %s\n"), fname, _("no place to store the LU pointer in ListNumeric."));
        return 1;
    }

    freeCcsSparse(A);

    /* create the scilab object to store the pointer onto the LU factors */
    sciErr = createPointer(pvApiCtx, 2, Numeric);
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
