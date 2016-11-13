/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) ????-2008 - Université de Nancy - Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
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

#include <string.h>
#include "api_scilab.h"
#include "sci_malloc.h"
#include "gw_special_functions.h"
#include "localization.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern void C2F(dxlegf)(double *dnu1, int *nudiff, int *mu1, int *mu2,
                        double *x, int *id, double *pqa, int *ipqa, int *ierror);
/*--------------------------------------------------------------------------*/
static double return_an_inf(void);
static int verify_cstr(double x[], int nb_elt, int *xmin, int *xmax);
/*--------------------------------------------------------------------------*/
int sci_legendre(char *fname, void* pvApiCtx)
{
    /*
    *   Interface onto the (Slatec) dxleg.f code.
    *   Scilab syntax:
    *
    *   p = legendre(n, m, x [, norm_flag] )
    *
    *      x is a vector with mnx elements (it is better to
    *        have a row vector but this is not forced)
    *
    *      n : a non negative int scalar (or a vector of such
    *          int regularly speced with an increment of 1)
    *      m : same constraints than for n
    *
    *      n and m may not be both vectors
    *
    *      norm_flag : optional. When it is present and equal to "norm"
    *                  it is a normalized version which is computed
    *    AUTHOR
    *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    */
    int it = 0, lc = 0, mM = 0, nM = 0, m1 = 0, m2 = 0, mN = 0, nN = 0;
    int n1 = 0, n2 = 0, mx = 0, nx = 0, mnx = 0, ms = 0, ns = 0;
    int M_is_scalar = 0, N_is_scalar = 0, normalized = 0, MNp1 = 0, *ipqa = NULL;
    double xx = 0., dnu1 = 0.;
    int id = 0, ierror = 0, i = 0, j = 0, nudiff = 0;

    SciErr sciErr;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;

    int nbInputArg = nbInputArgument(pvApiCtx);

    double* pdblN   = NULL;
    double* pdblM   = NULL;
    double* pdblX   = NULL;
    double* pdblPQA = NULL;
    int* piPQA      = NULL;

    int iType1 = 0;
    int iType2 = 0;
    int iType3 = 0;

    CheckInputArgument(pvApiCtx, 3, 4);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /* get N */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType1 != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Integer or vector of integers expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &mN, &nN, &pdblN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (verify_cstr(pdblN, mN * nN, &n1, &n2) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Integer >= %d expected.\n"), fname, 1, 0);
        return 1;
    }

    if ( mN == 1 && nN == 1)
    {
        N_is_scalar = 1;
    }

    /* get M */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType2 != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Integer or vector of integers expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &mM, &nM, &pdblM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (verify_cstr(pdblM, mM * nM, &m1, &m2) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Integer >= %d expected.\n"), fname, 2, 0);
        return 1;
    }

    if ( mM == 1 && nM == 1)
    {
        M_is_scalar = 1;
    }

    if ( ! M_is_scalar  &&  ! N_is_scalar )
    {
        Scierror(999, _("%s: Only one of arg1 and arg2 may be a vector.\n"), fname);
        return 1;
    }

    /* get X */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr3, &iType3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType3 != sci_matrix || isVarComplex(pvApiCtx, piAddr3))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 3);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &mx, &nx, &pdblX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    mnx = mx * nx;

    for ( i = 0 ; i < mnx ; i++ )
    {
        if ((fabs(pdblX[i]) <= 1.0) == 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Matrix with elements in [%d,%d] expected.\n"), fname, 3, -1, 1);
            return 1;
        }
    }

    if ( nbInputArg == 4 )
    {
        //get variable address
        int iRet = 0;
        char* lschar = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a single string at position 4
        iRet = getAllocatedSingleString(pvApiCtx, piAddr4, &lschar);
        if (iRet)
        {
            return iRet;
        }

        if ( strcmp(lschar, "norm") == 0)
        {
            normalized = 1;
        }
        else
        {
            normalized = 0;
        }
        freeAllocatedSingleString(lschar);
    }
    else
    {
        normalized = 0;
    }

    MNp1 = Max (n2 - n1, m2 - m1) + 1;

    allocMatrixOfDouble(pvApiCtx, nbInputArg + 1, MNp1, mnx, &pdblPQA);
    piPQA = (int*)MALLOC(MNp1 * mnx * sizeof(int));

    if ( normalized )
    {
        id = 4;
    }
    else
    {
        id = 3;
    }

    nudiff = n2 - n1;
    dnu1 = (double) n1;

    for ( i = 0 ; i < mnx ; i++ )
    {
        xx = fabs(pdblX[i]);
        /* manage the case where xx = 1 with n scalar*/
        if (N_is_scalar && xx == 1)
        {
            if (m1 == 0)
            {
                pdblPQA[i * MNp1] = 1;
            }
            else
            {
                pdblPQA[i * MNp1] = 0;
            }

            if (normalized)
            {
                pdblPQA[i * MNp1] = pdblPQA[i * MNp1]  * sqrt(dnu1 + 0.5);
            }

            piPQA[i * MNp1] = 0;

            for (j = 1; j < MNp1; j++)
            {
                pdblPQA[i * MNp1 + j] = 0;
                piPQA[i * MNp1 + j] = 0;
            }
        }
        else
        {
            /* dxleg computes only for x in [0,1] */
            C2F(dxlegf) (&dnu1, &nudiff, &m1, &m2, &xx, &id, pdblPQA + i * MNp1, piPQA + i * MNp1, &ierror);
            if ( ierror != 0 )
            {
                if ( ierror == 207 ) /* @TODO what is 207 ? */
                {
                    Scierror(999, _("%s: overflow or underflow of an extended range number\n"), fname);
                }
                else
                {
                    Scierror(999, _("%s: error number %d\n"), fname, ierror);
                }
                return 0;
            }
        }
    }

    /*  dxlegf returns the result under a form (pqa,ipqa) (to
    *  compute internaly with an extended exponent range)
    *  When the "exponent" part (ipqa) is 0 then the number is exactly
    *  given by pqa else it leads to an overflow or an underflow.
    */
    for ( i = 0 ; i < mnx * MNp1 ; i++ )
    {
        if ( piPQA[i] < 0 )
        {
            pdblPQA[i] = 0.0;
        }

        if ( piPQA[i] > 0 )
        {
            pdblPQA[i] = pdblPQA[i] * return_an_inf(); /* pqa[i] * Inf  to have the sign */
        }
    }

    FREE(piPQA);

    /* complete the result by odd/even symmetry for negative x */
    for ( i = 0 ; i < mnx ; i++ )
    {
        if ( pdblX[i] < 0.0 )
        {
            if ( (n1 + m1) % 2 == 1 )
            {
                for ( j = 0 ; j < MNp1 ; j += 2 )
                {
                    pdblPQA[i * MNp1 + j] = -pdblPQA[i * MNp1 + j];
                }
            }
            else
            {
                for ( j = 1 ; j < MNp1 ; j += 2 )
                {
                    pdblPQA[i * MNp1 + j] = -pdblPQA[i * MNp1 + j];
                }
            }
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArg + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
static double return_an_inf()
{
    /*    AUTHOR
    *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    */
    static int first = 1;
    static double inf = 1.0;

    if ( first )
    {
        inf = inf / (inf - (double) first);
        first = 0;
    }
    return (inf);
}
/*--------------------------------------------------------------------------*/
static int verify_cstr(double x[], int nb_elt, int *xmin, int *xmax)
{
    /*    1/ verify that the array x is formed by non negative integers
    *       regularly spaced with increment equal to 1 (if yes return 1
    *       if not return 0)
    *    2/ computes the min and the max
    *
    *    AUTHOR
    *       Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    */
    int i = 0;
    if ( ! (floor(x[0]) == x[0]  &&  x[0] >= 0 ))
    {
        return 0;
    }
    for ( i = 1 ; i < nb_elt ; i++ )
    {
        if ( x[i] != x[i - 1] + 1.0 )
        {
            return 0;
        }
    }

    *xmin = (int) x[0];
    *xmax = (int) x[nb_elt - 1];
    return 1;
}
/*--------------------------------------------------------------------------*/
