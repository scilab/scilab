/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_variable.h"

            
void* create_output(int _iCoeff, int _iSize, int _iRows, int _iCols, void* _pvDataIn);

int read_integer(char *fname,unsigned long fname_len)
{
    int iRet                    = 0;

    //output variable info
    int iRows8                  = 0;
    int iCols8                  = 0;
    int iRows16                 = 0;
    int iCols16                 = 0;
    int iRows32                 = 0;
    int iCols32                 = 0;
    int iRowsu8                 = 0;
    int iColsu8                 = 0;
    int iRowsu16                = 0;
    int iColsu16                = 0;
    int iRowsu32                = 0;
    int iColsu32                = 0;
    int iPrec                   = 0;

    int* piAddr8                = NULL;
    int* piAddr16               = NULL;
    int* piAddr32               = NULL;
    int* piAddru8               = NULL;
    int* piAddru16              = NULL;
    int* piAddru32              = NULL;

    char* pcData                = NULL;
    short* psData               = NULL;
    int* piData                 = NULL;
    unsigned char* pucData      = NULL;
    unsigned short* pusData     = NULL;
    unsigned int* puiData       = NULL;

    char* pcDataOut             = NULL;
    short* psDataOut            = NULL;
    int* piDataOut              = NULL;
    unsigned char* pucDataOut   = NULL;
    unsigned short* pusDataOut  = NULL;
    unsigned int* puiDataOut    = NULL;

    //check input/ouput arguments count
    CheckRhs(6,6);
    CheckLhs(6,6);

    //get varialbe address
    getVarAddressFromPosition(1, &piAddr8);
    getVarAddressFromPosition(2, &piAddru8);
    getVarAddressFromPosition(3, &piAddr16);
    getVarAddressFromPosition(4, &piAddru16);
    getVarAddressFromPosition(5, &piAddr32);
    getVarAddressFromPosition(6, &piAddru32);

    //check variable precision
    iRet = getMatrixOfIntegerPrecision(piAddr8, &iPrec);
    if(iPrec != SCI_INT8)
    {
        return 1;
    }

    //check variable precision
    iRet = getMatrixOfIntegerPrecision(piAddru8, &iPrec);
    if(iPrec != SCI_UINT8)
    {
        return 1;
    }

    //check variable precision
    iRet = getMatrixOfIntegerPrecision(piAddr16, &iPrec);
    if(iPrec != SCI_INT16)
    {
        return 1;
    }

    //check variable precision
    iRet = getMatrixOfIntegerPrecision(piAddru16, &iPrec);
    if(iPrec != SCI_UINT16)
    {
        return 1;
    }

    //check variable precision
    iRet = getMatrixOfIntegerPrecision(piAddr32, &iPrec);
    if(iPrec != SCI_INT32)
    {
        return 1;
    }

    //check variable precision
    iRet = getMatrixOfIntegerPrecision(piAddru32, &iPrec);
    if(iPrec != SCI_UINT32)
    {
        return 1;
    }

    //retrieve dimensions and data
    iRet = getMatrixOfInteger8(piAddr8, &iRows8, &iCols8, &pcData);
    if(iRet)
    {
        return 1;
    }

    //retrieve dimensions and data
    iRet = getMatrixOfUnsignedInteger8(piAddru8, &iRowsu8, &iColsu8, &pucData);
    if(iRet)
    {
        return 1;
    }

    //retrieve dimensions and data
    iRet = getMatrixOfInteger16(piAddr16, &iRows16, &iCols16, &psData);
    if(iRet)
    {
        return 1;
    }

    //retrieve dimensions and data
    iRet = getMatrixOfUnsignedInteger16(piAddru16, &iRowsu16, &iColsu16, &pusData);
    if(iRet)
    {
        return 1;
    }

    //retrieve dimensions and data
    iRet = getMatrixOfInteger32(piAddr32, &iRows32, &iCols32, &piData);
    if(iRet)
    {
        return 1;
    }

    //retrieve dimensions and data
    iRet = getMatrixOfUnsignedInteger32(piAddru32, &iRowsu32, &iColsu32, &puiData);
    if(iRet)
    {
        return 1;
    }

    //alloc and fill new variable
    pcDataOut   = (char*)create_output(2, 1, iRows8, iCols8, (void*)pcData);
    pucDataOut  = (unsigned char*)create_output(4, 1, iRowsu8, iColsu8, (void*)pucData);
    psDataOut   = (short*)create_output(8, 2, iRows16, iCols16, (void*)psData);
    pusDataOut  = (unsigned short*)create_output(16, 2, iRowsu16, iColsu16, (void*)pusData);
    piDataOut   = (int*)create_output(32, 4, iRows32, iCols32, (void*)piData);
    puiDataOut  = (unsigned int*)create_output(64, 4, iRowsu32, iColsu32, (void*)puiData);

    //create new variable
    iRet = createMatrixOfInteger8(Rhs + 1, iRows8, iCols8, pcDataOut);
    if(iRet)
    {
        return 1;
    }

    //create new variable
    iRet = createMatrixOfUnsignedInteger8(Rhs + 2, iRowsu8, iColsu8, pucDataOut);
    if(iRet)
    {
        return 1;
    }

    //create new variable
    iRet = createMatrixOfInteger16(Rhs + 3, iRows16, iCols16, psDataOut);
    if(iRet)
    {
        return 1;
    }

    //create new variable
    iRet = createMatrixOfUnsignedInteger16(Rhs + 4, iRowsu16, iColsu16, pusDataOut);
    if(iRet)
    {
        return 1;
    }

    //create new variable
    iRet = createMatrixOfInteger32(Rhs + 5, iRows32, iCols32, piDataOut);
    if(iRet)
    {
        return 1;
    }

    //create new variable
    iRet = createMatrixOfUnsignedInteger32(Rhs + 6, iRowsu32, iColsu32, puiDataOut);
    if(iRet)
    {
        return 1;
    }

    //assign allocated variables to Lhs position
    LhsVar(1) = Rhs + 1;
    LhsVar(2) = Rhs + 2;
    LhsVar(3) = Rhs + 3;
    LhsVar(4) = Rhs + 4;
    LhsVar(5) = Rhs + 5;
    LhsVar(6) = Rhs + 6;
    PutLhsVar();
    return 0;
}

void* create_output(int _iCoeff, int _iSize, int _iRows, int _iCols, void* _pvDataIn)
{
    int i = 0;
    void* pvDataOut = (void*)malloc(_iSize * _iRows * _iCols);
    for(i = 0 ; i < _iRows * _iCols ; i++)
    {
        int iVal = 0;
        memcpy(&iVal, (char*)_pvDataIn + i * _iSize, _iSize);
        iVal *= _iCoeff;
        memcpy((char*)pvDataOut + i * _iSize, &iVal, _iSize);
    }
    return pvDataOut;
}
            
        
