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

void* create_output(int _iCoeff, int _iSize, int _iRows, int _iCols, void* _pvDataIn);
int read_integer(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    //output variable info
    int iRows8					= 0;
    int iCols8					= 0;
    int iRows16					= 0;
    int iCols16					= 0;
    int iRows32					= 0;
    int iCols32					= 0;
    int iRowsu8					= 0;
    int iColsu8					= 0;
    int iRowsu16				= 0;
    int iColsu16				= 0;
    int iRowsu32				= 0;
    int iColsu32				= 0;
    int iPrec					= 0;
    int* piAddr8				= NULL;
    int* piAddr16				= NULL;
    int* piAddr32				= NULL;
    int* piAddru8				= NULL;
    int* piAddru16				= NULL;
    int* piAddru32				= NULL;
    char* pcData				= NULL;
    short* psData				= NULL;
    int* piData					= NULL;
    unsigned char* pucData		= NULL;
    unsigned short* pusData		= NULL;
    unsigned int* puiData		= NULL;
    char* pcDataOut				= NULL;
    short* psDataOut			= NULL;
    int* piDataOut				= NULL;
    unsigned char* pucDataOut	= NULL;
    unsigned short* pusDataOut	= NULL;
    unsigned int* puiDataOut	= NULL;

    //check input/output arguments count
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 6, 6);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddru8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr16);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddru16);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr32);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddru32);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check variable precision
    sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr8, &iPrec);
    if (sciErr.iErr || iPrec != SCI_INT8)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check variable precision
    sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddru8, &iPrec);
    if (sciErr.iErr || iPrec != SCI_UINT8)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check variable precision
    sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr16, &iPrec);
    if (sciErr.iErr || iPrec != SCI_INT16)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check variable precision
    sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddru16, &iPrec);
    if (sciErr.iErr || iPrec != SCI_UINT16)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check variable precision
    sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr32, &iPrec);
    if (sciErr.iErr || iPrec != SCI_INT32)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check variable precision
    sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddru32, &iPrec);
    if (sciErr.iErr || iPrec != SCI_UINT32)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrieve dimensions and data
    sciErr = getMatrixOfInteger8(pvApiCtx, piAddr8, &iRows8, &iCols8, &pcData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrieve dimensions and data
    sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddru8, &iRowsu8, &iColsu8, &pucData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrieve dimensions and data
    sciErr = getMatrixOfInteger16(pvApiCtx, piAddr16, &iRows16, &iCols16, &psData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrieve dimensions and data
    sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddru16, &iRowsu16, &iColsu16, &pusData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrieve dimensions and data
    sciErr = getMatrixOfInteger32(pvApiCtx, piAddr32, &iRows32, &iCols32, &piData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrieve dimensions and data
    sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddru32, &iRowsu32, &iColsu32, &puiData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //alloc and fill new variable
    pcDataOut   = (char*)create_output(2, 1, iRows8, iCols8, (void*)pcData);
    pucDataOut  = (unsigned char*)create_output(4, 1, iRowsu8, iColsu8, (void*)pucData);
    psDataOut   = (short*)create_output(8, 2, iRows16, iCols16, (void*)psData);
    pusDataOut  = (unsigned short*)create_output(16, 2, iRowsu16, iColsu16, (void*)pusData);
    piDataOut   = (int*)create_output(32, 4, iRows32, iCols32, (void*)piData);
    puiDataOut  = (unsigned int*)create_output(64, 4, iRowsu32, iColsu32, (void*)puiData);

    //create new variable
    sciErr = createMatrixOfInteger8(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows8, iCols8, pcDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //create new variable
    sciErr = createMatrixOfUnsignedInteger8(pvApiCtx, nbInputArgument(pvApiCtx) + 2, iRowsu8, iColsu8, pucDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //create new variable
    sciErr = createMatrixOfInteger16(pvApiCtx, nbInputArgument(pvApiCtx) + 3, iRows16, iCols16, psDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //create new variable
    sciErr = createMatrixOfUnsignedInteger16(pvApiCtx, nbInputArgument(pvApiCtx) + 4, iRowsu16, iColsu16, pusDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //create new variable
    sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 5, iRows32, iCols32, piDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //create new variable
    sciErr = createMatrixOfUnsignedInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 6, iRowsu32, iColsu32, puiDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    FREE(pcDataOut);
    FREE(pucDataOut);
    FREE(psDataOut);
    FREE(pusDataOut);
    FREE(piDataOut);
    FREE(puiDataOut);

    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;
    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
    AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;
    return 0;
}

void* create_output(int _iCoeff, int _iSize, int _iRows, int _iCols, void* _pvDataIn)
{
    int i = 0;
    void* pvDataOut = (void*)MALLOC(_iSize * _iRows * _iCols);
    for (i = 0 ; i < _iRows * _iCols ; i++)
    {
        int iVal = 0;
        memcpy(&iVal, (char*)_pvDataIn + i * _iSize, _iSize);
        iVal *= _iCoeff;
        memcpy((char*)pvDataOut + i * _iSize, &iVal, _iSize);
    }
    return pvDataOut;
}
