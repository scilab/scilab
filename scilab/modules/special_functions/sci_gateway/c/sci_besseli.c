/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
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
#include "gw_special_functions.h"
#include "Scierror.h"
#include "msgs.h"
#include "sci_malloc.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern void  C2F(dbesiv) (double *x, int* nx, double *alpha, int *na, int *kode, double *r, double *w, int *ierr);
extern void  C2F(zbesiv) (double *xr, double *xi, int* nx, double *alpha, int *na, int *kode, double *rr, double *ri, double *wr, double *wi, int *ierr);
/*--------------------------------------------------------------------------*/
int sci_besseli(char *fname, void* pvApiCtx)
{
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int mr = 0, nr = 0, itr = 0, nw = 0;
    int r1 = 0, r2 = 0, na = 0, nx = 0, kode = 0;
    int isint = 0, ispos = 0, i = 0, t = 0;
    int un = 1, nl2 = 0, ierr = 0;
    int nbInputArg = 0;

    double zero = 0.0;

    double* pdblXR = NULL;
    double* pdblXI = NULL;
    double* pdbl1  = NULL;

    int* piAddr1 = NULL;
    int* piAddr3 = NULL;
    int* piAddrX = NULL;

    double* lwr = NULL;
    double* lwi = NULL;
    double* lr  = NULL;
    double* li  = NULL;

    SciErr sciErr;

    CheckInputArgument(pvApiCtx, 2, 3);

    nbInputArg = nbInputArgument(pvApiCtx);

    kode = 1;
    if (nbInputArg == 3)
    {
        /* normalized bessel required */
        //get variable address of the input argument
        double* l1;
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (m1 * n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 3);
            return 1;
        }

        kode = (int)l1[0] + 1;
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
        /*besseli([],x) */
        AssignOutputVariable(pvApiCtx, 1) = 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    /* get x */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrX);
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
        /*besseli(alpha,[]) */
        AssignOutputVariable(pvApiCtx, 1) = 2;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    /* determine if the result is real or complex */
    if (pdblXI)
    {
        itr = 1;
    }
    else
    {
        isint = 1;
        ispos = 1;
        for (i = 0; i < m1 * n1; i++)
        {
            t = (int)pdbl1[i];
            if ((double)t != pdbl1[i])
            {
                isint = 0;
                break;
            }
        }

        if (isint == 0)
        {
            for (i = 0; i < m2 * n2; i++)
            {
                if (pdblXR[i] < 0.0)
                {
                    ispos = 0;
                    break;
                }
            }
        }

        if (ispos == 0)
        {
            itr = 1;
        }
    }

    if (itr == 1 && pdblXI == NULL)
    {
        /* transform to complex */
        double* l2r = NULL;
        double* l2i = NULL;

        int iSize = m2 * n2 * sizeof(double);
        pdblXI = (double*)MALLOC(iSize);
        memset(pdblXI, 0x00, iSize);
    }

    if (m1 * n1 == 1)
    {
        /*besseli(scalar,matrix) */
        double wr[2], wi[2];
        double* lr = NULL;
        double* li = NULL;

        nx = m2 * n2;
        na = 1;
        if (itr == 0)
        {
            allocMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2, n2, &lr);
            C2F(dbesiv) (pdblXR, &nx, pdbl1, &na, &kode, lr, wr, &ierr);
        }
        else
        {
            allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2, n2, &lr, &li);
            C2F(zbesiv) (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, lr, li, wr, wi, &ierr);
        }
    }
    else if (m2 * n2 == 1)
    {
        /* besseli(matrix,scalar) */
        nx = 1;
        na = m1 * n1;
        nw = 2 * na;

        if (itr == 0)
        {
            allocMatrixOfDouble(pvApiCtx, nbInputArg + 1, m1, n1, &lr);
            allocMatrixOfDouble(pvApiCtx, nbInputArg + 2, nx, nw, &lwr);
            C2F(dbesiv) (pdblXR, &nx, pdbl1, &na, &kode, lr, lwr, &ierr);
        }
        else
        {
            allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m1, n1, &lr, &li);
            allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 2, nx, nw, &lwr, &lwi);
            C2F(zbesiv) (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, lr, li, lwr, lwi, &ierr);
        }
    }
    else if ((m1 == 1 && n2 == 1) || (n1 == 1 && m2 == 1))
    {
        /* besseli(row,col) or besseli(col,row) */
        mr = m2 * n2;
        nr = m1 * n1;
        nx = m2 * n2;
        na = m1 * n1;
        nw = 2  * na;

        if (itr == 0)
        {
            allocMatrixOfDouble(pvApiCtx, nbInputArg + 1, mr, nr, &lr);
            allocMatrixOfDouble(pvApiCtx, nbInputArg + 2, 1, nw, &lwr);
            C2F(dbesiv) (pdblXR, &nx, pdbl1, &na, &kode, lr, lwr, &ierr);
        }
        else
        {
            allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, mr, nr, &lr, &li);
            allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 2, 1, nw, &lwr, &lwi);
            C2F(zbesiv) (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, lr, li, lwr, lwi, &ierr);
        }
    }
    else
    {
        /* element wise case */
        double wr[2], wi[2];

        if (m1 * n1 != m2 * n2)
        {
            Scierror(999, _("%s: arguments #%d and #%d have incompatible dimensions.\n"), fname, 1, 2);

            if (itr == 1 && isVarComplex(pvApiCtx, piAddrX) == 0)
            {
                FREE(pdblXI);
            }

            return 1;
        }

        nx = m2 * n2;
        na = -1;

        if (itr == 0)
        {
            allocMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2, n2, &lr);
            C2F(dbesiv) (pdblXR, &nx, pdbl1, &na, &kode, lr, wr, &ierr);
        }
        else
        {
            allocComplexMatrixOfDouble(pvApiCtx, nbInputArg + 1, m2, n2, &lr, &li);
            C2F(zbesiv) (pdblXR, pdblXI, &nx, pdbl1, &na, &kode, lr, li, wr, wi, &ierr);
        }
    }

    if (itr == 1 && isVarComplex(pvApiCtx, piAddrX) == 0)
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
        //        else if ( C2F(errgst).ieee == 1)
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
