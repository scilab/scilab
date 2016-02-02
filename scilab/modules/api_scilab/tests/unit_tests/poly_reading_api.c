/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"

int read_poly(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i, j;
    //variable info
    int iRows			= 0;
    int iCols			= 0;
    int iVarLen			= 0;
    int* piAddr			= NULL;
    int* piNbCoef		= NULL;
    double** pdblReal	= NULL;
    double** pdblImg	= NULL;
    char* pstVarname	= NULL;

    //check input and output arguments
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddr) == FALSE)
    {
        //Error
        return 0;
    }

    //get variable name length
    sciErr = getPolyVariableName(pvApiCtx, piAddr, NULL, &iVarLen);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //alloc buff to receive variable name
    pstVarname = (char*)MALLOC(sizeof(char) * (iVarLen + 1));//1 for null termination

    //get variable name
    sciErr = getPolyVariableName(pvApiCtx, piAddr, pstVarname, &iVarLen);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //First call: retrieve dimmension
    sciErr = getComplexMatrixOfPoly(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //alloc array of coefficient
    piNbCoef = (int*)MALLOC(sizeof(int) * iRows * iCols);

    //Second call: retrieve coefficient
    sciErr = getComplexMatrixOfPoly(pvApiCtx, piAddr, &iRows, &iCols, piNbCoef, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //alloc arrays of data
    pdblReal    = (double**)MALLOC(sizeof(double*) * iRows * iCols);
    pdblImg     = (double**)MALLOC(sizeof(double*) * iRows * iCols);

    for (i = 0 ; i < iRows * iCols ; i++)
    {
        pdblReal[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);
        pdblImg[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);
    }

    //Third call: retrieve data
    sciErr = getComplexMatrixOfPoly(pvApiCtx, piAddr, &iRows, &iCols, piNbCoef, pdblReal, pdblImg);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Do something with Data
    //Invert polynomials in the matrix and invert coefficients
    for (i = 0 ; i < (iRows * iCols) / 2 ; i++)
    {
        int iPos1			= iRows * iCols - 1 - i;
        double* pdblSave	= NULL;
        int iNbCoefSave		= 0;
        //switch array of coefficient
        pdblSave			= pdblReal[i];
        pdblReal[i]			= pdblReal[iPos1];
        pdblReal[iPos1]		= pdblSave;
        pdblSave			= pdblImg[i];
        pdblImg[i]			= pdblImg[iPos1];
        pdblImg[iPos1]		= pdblSave;
        //switch number of coefficient
        iNbCoefSave			= piNbCoef[i];
        piNbCoef[i]			= piNbCoef[iPos1];
        piNbCoef[iPos1]		= iNbCoefSave;
    }

    //switch coefficient
    for (i = 0 ; i < iRows * iCols ; i++)
    {
        for (j = 0 ; j < piNbCoef[i] / 2 ; j++)
        {
            int iPos2			= piNbCoef[i] - 1 - j;
            double dblVal		= pdblReal[i][j];
            pdblReal[i][j]		= pdblReal[i][iPos2];
            pdblReal[i][iPos2]	= dblVal;
            dblVal				= pdblImg[i][j];
            pdblImg[i][j]		= pdblImg[i][iPos2];
            pdblImg[i][iPos2]	= dblVal;
        }
    }

    sciErr = createComplexMatrixOfPoly(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstVarname, iRows, iCols, piNbCoef, pdblReal, pdblImg);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //free OS memory
    FREE(pstVarname);
    FREE(piNbCoef);
    for (i = 0 ; i < iRows * iCols ; i++)
    {
        FREE(pdblReal[i]);
        FREE(pdblImg[i]);
    }
    FREE(pdblReal);
    FREE(pdblImg);
    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
