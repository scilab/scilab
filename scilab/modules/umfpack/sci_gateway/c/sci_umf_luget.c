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
|   6) Interface code for getting the LU factors              |
|                                                             |
|   Scilab call                                               |
|   -----------                                               |
|   [L,U,p,q,R] = umf_luget(LU_ptr)                           |
|                                                             |
+------------------------------------------------------------*/
#include "api_scilab.h"
#include "sciumfpack.h"
#include "gw_umfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"

extern CellAdr *ListNumeric;

int sci_umf_luget(char* fname, void* pvApiCtx)
{
    /*
    *  LU_ptr is (a pointer to) a factorization of A, we have:
    *             -1
    *          P R  A Q = L U
    *
    *      A is n_row x n_col
    *      L is n_row x n
    *      U is n     x n_col     n = min(n_row, n_col)
    */

    SciErr sciErr;
    void* Numeric = NULL;
    int lnz = 0, unz = 0, n_row = 0, n_col = 0, n = 0, nz_udiag = 0, i = 0, stat = 0, do_recip = 0, it_flag = 0;
    int *L_mnel = NULL, *L_icol = NULL, *L_ptrow = NULL, *U_mnel = NULL, *U_icol = NULL, *U_ptrow = NULL, *V_irow = NULL, *V_ptcol = NULL;
    double *L_R = NULL, *L_I = NULL, *U_R = NULL, *U_I = NULL, *V_R = NULL, *V_I = NULL, *Rs = NULL;
    int *p = NULL, *q = NULL, pl_miss = 0, error_flag = 0 ;

    int* piAddr1 = NULL;
    int iType1   = 0;

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 5);

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
    if ( IsAdrInList(Numeric, ListNumeric, &it_flag) )
    {
        if (it_flag == 0 )
        {
            umfpack_di_get_lunz(&lnz, &unz, &n_row, &n_col, &nz_udiag, Numeric);
        }
        else
        {
            umfpack_zi_get_lunz(&lnz, &unz, &n_row, &n_col, &nz_udiag, Numeric);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be a valid reference to (umf) LU factors.\n"), fname, 1);
        return 1;
    }

    if (n_row <= n_col)
    {
        n = n_row;
    }
    else
    {
        n = n_col;
    }
    L_mnel  = (int*)MALLOC(n_row * sizeof(int));
    L_icol  = (int*)MALLOC(lnz * sizeof(int));
    L_ptrow = (int*)MALLOC((n_row + 1) * sizeof(int));
    L_R     = (double*)MALLOC( lnz * sizeof(double));
    U_mnel  = (int*)MALLOC(n * sizeof(int));
    U_icol  = (int*)MALLOC(unz * sizeof(int));
    U_ptrow = (int*)MALLOC((n + 1) * sizeof(int));
    U_R     = (double*)MALLOC( unz * sizeof(double));
    V_irow  = (int*)MALLOC(unz * sizeof(int));
    V_ptcol = (int*)MALLOC((n_col + 1) * sizeof(int));
    V_R     = (double*)MALLOC( unz * sizeof(double));
    p       = (int*)MALLOC(n_row * sizeof(int));
    q       = (int*)MALLOC(n_col * sizeof(int));
    Rs      = (double*)MALLOC(n_row * sizeof(double));

    if ( it_flag == 1 )
    {
        L_I = (double*)MALLOC(lnz * sizeof(double));
        U_I = (double*)MALLOC(unz * sizeof(double));
        V_I = (double*)MALLOC(unz * sizeof(double));
    }
    else
    {
        L_I = U_I = V_I = NULL;
    }

    if (    !(L_mnel && L_icol && L_R && L_ptrow  && p &&
              U_mnel && U_icol && U_R && U_ptrow  && q &&
              V_irow && V_R && V_ptcol  && Rs)
            || (it_flag && !(L_I && U_I && V_I))   )
    {
        error_flag = 1;
        goto the_end;
    }

    if ( it_flag == 0 )
    {
        stat = umfpack_di_get_numeric(L_ptrow, L_icol, L_R, V_ptcol, V_irow, V_R,
                                      p, q, (double *)NULL, &do_recip, Rs, Numeric);
    }
    else
    {
        stat = umfpack_zi_get_numeric(L_ptrow, L_icol, L_R, L_I, V_ptcol, V_irow, V_R, V_I,
                                      p, q, (double *)NULL, (double *)NULL, &do_recip, Rs, Numeric);
    }

    if ( stat != UMFPACK_OK )
    {
        error_flag = 2;
        goto the_end;
    };

    if ( do_recip )
    {
        for ( i = 0 ; i < n_row ; i++ )
        {
            Rs[i] = 1.0 / Rs[i];
        }
    }

    if ( it_flag == 0 )
    {
        stat = umfpack_di_transpose(n, n_col, V_ptcol, V_irow, V_R, (int *) NULL,
                                    (int*) NULL, U_ptrow, U_icol, U_R);
    }
    else
    {
        stat = umfpack_zi_transpose(n, n_col, V_ptcol, V_irow, V_R, V_I, (int *) NULL,
                                    (int*) NULL, U_ptrow, U_icol, U_R, U_I, 0);
    }

    if ( stat != UMFPACK_OK )
    {
        error_flag = 2;
        goto the_end;
    };

    for ( i = 0 ; i < n_row ; i++ )
    {
        L_mnel[i] = L_ptrow[i + 1] - L_ptrow[i];
    }
    for ( i = 0 ; i < n ; i++ )
    {
        U_mnel[i] = U_ptrow[i + 1] - U_ptrow[i];
    }

    for ( i = 0 ; i < lnz ; i++ )
    {
        L_icol[i]++;
    }
    for ( i = 0 ; i < unz ; i++ )
    {
        U_icol[i]++;
    }

    for ( i = 0 ; i < n_row ; i++ )
    {
        p[i]++;
    }
    for ( i = 0 ; i < n_col ; i++ )
    {
        q[i]++;
    }

    /* output L */
    if (it_flag) // complex
    {
        sciErr = createComplexSparseMatrix(pvApiCtx, 2, n_row, n, lnz, L_mnel, L_icol, L_R, L_I);
    }
    else
    {
        sciErr = createSparseMatrix(pvApiCtx, 2, n_row, n, lnz, L_mnel, L_icol, L_R);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(L_mnel);
        FREE(U_mnel);
        return 1;
    }

    /* output U */
    if (it_flag) // complex
    {
        sciErr = createComplexSparseMatrix(pvApiCtx, 3, n, n_col, unz, U_mnel, U_icol, U_R, U_I);
    }
    else
    {
        sciErr = createSparseMatrix(pvApiCtx, 3, n, n_col, unz, U_mnel, U_icol, U_R);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(L_mnel);
        FREE(U_mnel);
        return 1;
    }

    /* output p */
    sciErr = createMatrixOfDoubleAsInteger(pvApiCtx, 4, n_row, 1, p);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(L_mnel);
        FREE(U_mnel);
        return 1;
    }

    /* output q */
    sciErr = createMatrixOfDoubleAsInteger(pvApiCtx, 5, n_col, 1, q);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(L_mnel);
        FREE(U_mnel);
        return 1;
    }

    /* output res */
    sciErr = createMatrixOfDouble(pvApiCtx, 6, n_row, 1, Rs);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(L_mnel);
        FREE(U_mnel);
        return 1;
    }

the_end:
    FREE(L_mnel);
    FREE(L_icol);
    FREE(L_R);
    FREE(L_ptrow);
    FREE(p);
    FREE(U_mnel);
    FREE(U_icol);
    FREE(U_R);
    FREE(U_ptrow);
    FREE(q);
    FREE(V_irow);
    FREE(V_R);
    FREE(V_ptcol);
    FREE(Rs);

    if ( it_flag == 1 )
    {
        FREE(L_I);
        FREE(V_I);
        FREE(U_I);
    }

    switch (error_flag)
    {
        case 0:   /* no error */
            AssignOutputVariable(pvApiCtx, 1) = 2;
            AssignOutputVariable(pvApiCtx, 2) = 3;
            AssignOutputVariable(pvApiCtx, 3) = 4;
            AssignOutputVariable(pvApiCtx, 4) = 5;
            AssignOutputVariable(pvApiCtx, 5) = 6;
            ReturnArguments(pvApiCtx);
            return 0;

        case 1:   /* enough memory (with malloc) */
            Scierror(999, _("%s: No more memory.\n"), fname);
            break;

        case 2:   /* a problem with one umfpack routine */
            Scierror(999, "%s: %s\n", fname, UmfErrorMes(stat));
            break;
    }

    return 1;
}


