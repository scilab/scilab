/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "gw_special_functions.h"
#include "Scierror.h"
#include "msgs.h"
#include "zbeshv.h"
#include "sci_malloc.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_besselh(char *fname, void* pvApiCtx)
{
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int rhs1 = 0, nw = 0;
    int na = 0, nx = 0, kode = 0, nbInputArg = 0;
    int un = 1, ierr = 0;
    int K = 0;

    double* pdbl1   = NULL;
    double* pdbl2   = NULL;
    double* pdblXR  = NULL;
    double* pdblXI  = NULL;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr4 = NULL;
    int* piAddrX = NULL;

    double *lr  = NULL;
    double *li  = NULL;
    double *lwr = NULL;
    double *lwi = NULL;

    SciErr sciErr;

    CheckInputArgument(pvApiCtx, 2, 4);

    nbInputArg = nbInputArgument(pvApiCtx);
    rhs1 = nbInputArg;

    kode = 1;
    if (nbInputArg == 4)
    {
        /* normalized bessel required */
        //get variable address of the input argument
        double* l1 = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (m1 * n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 4);
            return 1;
        }

        kode = (int)l1[0] + 1;
        rhs1--;
    }

    K = 1;
    if (nbInputArgument(pvApiCtx) > 2)
    {
        /* normalized bessel required */
        //get variable address of the input argument
        double* l1 = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (m1 * n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
            return 1;
        }

        K = (int)l1[0];
    }

    /* get alpha */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &pdbl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (m1 * n1 == 0)
    {
        /*besselh([],x) */
        AssignOutputVariable(pvApiCtx, 1) = 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    /* get x */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, rhs1, &piAddrX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddrX, &m2, &n2, &pdblXR, &pdblXI);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (m2 * n2 == 0)
    {
        /*besselh(alpha,[]) */
        AssignOutputVariable(pvApiCtx, 1) = rhs1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    // if X is real, create imaginary part
    if (pdblXI == NULL)
    {
        int iSize = m2 * n2 * sizeof(double);
        pdblXI = (double*)MALLOC(iSize);
        memset(pdblXI, 0x00, iSize);
    }

    if (m1 * n1 == 1)
    {
        /*bessely(scalar,matrix) */
        double wr[3], wi[3];
        double *yr = NULL;
        double *yi = NULL;

        nx = m2 * n2;
        na = 1;

        allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2, n2, &yr, &yi);

        zbeshv(pdblXR, pdblXI, &nx, pdbl1, &na, &kode, &K, yr, yi, wr, wi, &ierr);
    }
    else if (m2 * n2 == 1)
    {
        /* bessely(matrix,scalar) */
        nx = 1;
        na = m1 * n1;
        nw = 3 * na;

        allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m1, n1, &lr, &li);
        allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 2, nx, nw, &lwr, &lwi);

        zbeshv (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, &K, lr, li, lwr, lwi, &ierr);
    }
    else if ((m1 == 1 && n2 == 1) || (n1 == 1 && m2 == 1))
    {
        /* bessely(row,col) or bessely(col,row) */
        nx = m2 * n2;
        na = m1 * n1;
        nw = 3 * na;

        allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2 * n2, m1 * n1, &lr, &li);
        allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 2, 1, nw, &lwr, &lwi);

        zbeshv (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, &K, lr, li, lwr, lwi, &ierr);
    }
    else
    {
        /* element wise case */
        double wr[2], wi[2];
        if (m1 * n1 != m2 * n2)
        {
            Scierror(999, _("%s: arguments #%d and #%d have incompatible dimensions.\n"), fname, 1, 2);

            if (isVarComplex(pvApiCtx, piAddrX) == 0)
            {
                FREE(pdblXI);
            }

            return 1;
        }

        nx = m2 * n2;
        na = -1;

        allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2, n2, &lr, &li);

        zbeshv (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, &K, lr, li, wr, wi, &ierr);
    }

    if (isVarComplex(pvApiCtx, piAddrX) == 0)
    {
        FREE(pdblXI);
    }

    if (ierr == 2)
    {
        // FIX ME
        //        if ( C2F(errgst).ieee == 0)
        //        {
        //            ierr = 69;
        //            SciError(ierr);
        //        }
        //        else if (C2F(errgst).ieee == 1)
        //        {
        //            ierr = 63;
        //            C2F(msgs)(&ierr, &un);
        //        }
    }
    else if (ierr == 3)
    {
        /* inaccurate result */
        ierr = 4;
        C2F(msgs)(&ierr, &un);
    }
    else if (ierr == 4 || ierr == 5)
    {
        // FIX ME
        //        if ( C2F(errgst).ieee == 0)
        //        {
        //            ierr = 69;
        //            SciError(ierr);
        //        }
        //        else if ( C2F(errgst).ieee == 1)
        //        {
        //            ierr = 107;
        //            C2F(msgs)(&ierr, &un);
        //        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArg + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
