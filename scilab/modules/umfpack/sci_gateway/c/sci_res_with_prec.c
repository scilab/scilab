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
#include "api_scilab.h"
#include "gw_umfpack.h"
#include "sciumfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

int sci_res_with_prec(char* fname, unsigned long l)
{
    SciErr sciErr;

    int mx = 0, nx = 0, mb = 0, nb = 0, i = 0;

    SciSparse A;
    int mA              = 0; // rows
    int nA              = 0; // cols
    int iNbItem         = 0;
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    double* pdblSpReal  = NULL;
    double* pdblSpImg   = NULL;

    int iComplex = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;

    double* pdblXR = NULL;
    double* pdblXI = NULL;
    double* pdblBR = NULL;
    double* pdblBI = NULL;
    double* pdblNR = NULL;
    double* pdblNI = NULL;
    double* pdblRR = NULL;
    double* pdblRI = NULL;

    int nbInputArg = nbInputArgument(pvApiCtx);

    /* Check numbers of input/output arguments */
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 2);

    /* get A the sparse matrix */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
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
        printError(&sciErr, 0);
        return 1;
    }

    // fill struct sparse
    A.m     = mA;
    A.n     = nA;
    A.it    = iComplex;
    A.nel   = iNbItem;
    A.mnel  = piNbItemRow;
    A.icol  = piColPos;
    A.R     = pdblSpReal;
    A.I     = pdblSpImg;

    /* get x */
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr2))
    {
        iComplex = 1;
        sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr2, &mx, &nx, &pdblXR, &pdblXI);
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &mx, &nx, &pdblXR);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* get b */
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr3))
    {
        iComplex = 1;
        sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr3, &mb, &nb, &pdblBR, &pdblBI);
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &mb, &nb, &pdblBR);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* check size of inputs */
    if ( nx < 1 || nb != nx || mx != nA || mb != mA )
    {
        Scierror(999, _("%s: Wrong size for input arguments: Same sizes expected.\n"), fname);
        return 1;
    }

    /* Create the matrix as return of the function */
    if (iComplex)
    {
        sciErr = allocComplexMatrixOfDouble(pvApiCtx, 4, mb, nb, &pdblRR, &pdblRI);
    }
    else
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 4, mb, nb, &pdblRR);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* Create the matrix as return of the function */
    sciErr = allocMatrixOfDouble(pvApiCtx, 5, 1, nb, &pdblNR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* perform operations */
    if (iComplex == 0)
    {
        for ( i = 0 ; i < nb ; i++ )
        {
            residu_with_prec(&A, pdblXR + i * mx, pdblBR + i * mb, pdblRR + i * mb, pdblNR + i);
        }
    }
    else
    {
        if (pdblXI == NULL)
        {
            int iSize = mx * nx * sizeof(double);
            pdblXI = (double*)MALLOC(iSize);
            memset(pdblXI, 0x00, iSize);
        }

        if (pdblBI == NULL)
        {
            int iSize = mb * nb * sizeof(double);
            pdblBI = (double*)MALLOC(iSize);
            memset(pdblBI, 0x00, iSize);
        }

        if (pdblSpImg == NULL)
        {
            /* Create the matrix as return of the function */
            int iSize = nb * sizeof(double);
            pdblNI = (double*)MALLOC(iSize);
            memset(pdblNI, 0x00, iSize);

            for ( i = 0 ; i < nb ; i++ )
            {
                residu_with_prec(&A, pdblXR + i * mx, pdblBR + i * mb, pdblRR + i * mb, pdblNR + i);
            }

            for ( i = 0 ; i < nb ; i++ )
            {
                residu_with_prec(&A, pdblXI + i * mx, pdblBI + i * mb, pdblRI + i * mb, pdblNI + i);
            }

            for ( i = 0 ; i < nb ; i++ )
            {
                pdblNR[i] = sqrt(pdblNR[i] * pdblNR[i] + pdblNI[i] * pdblNI[i]);
            }
        }
        else
        {
            for ( i = 0 ; i < nb ; i++ )
            {
                cmplx_residu_with_prec(&A,  pdblXR + i * mx, pdblXI + i * mx,
                                       pdblBR + i * mb, pdblBI + i * mb,
                                       pdblRR + i * mb, pdblRI + i * mb,
                                       pdblNR + i);
            }
        }
    }

    if (isVarComplex(pvApiCtx, piAddr1) == 0)
    {
        FREE(pdblNI);
    }

    if (isVarComplex(pvApiCtx, piAddr2) == 0)
    {
        FREE(pdblXI);
    }

    if (isVarComplex(pvApiCtx, piAddr3) == 0)
    {
        FREE(pdblBI);
    }

    AssignOutputVariable(pvApiCtx, 1) = 4;
    AssignOutputVariable(pvApiCtx, 2) = 5;
    ReturnArguments(pvApiCtx);
    return 0;
}
