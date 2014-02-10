/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_sparse.h"
#include "localization.h"

#include "MALLOC.h"
#include "call_scilab.h"


static int getCommonAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);
static int getCommonNamedAllocatedSparseMatrix(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);


//internal sparse functions
SciErr getSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
    return getCommonSparseMatrix(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr getComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    return getCommonSparseMatrix(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr getCommonSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iPos	= 0;
    int iType	= 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (iType != sci_sparse)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix", _("sparse matrix"));
        return sciErr;
    }

    if (isVarComplex(_pvCtx, _piAddress) != _iComplex)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix");
        return sciErr;
    }


    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    *_piNbItem = _piAddress[4];

    if (_piNbItemRow == NULL)
    {
        return sciErr;
    }
    *_piNbItemRow = _piAddress + 5;//4 for header + 1 for NbItem

    if (_piColPos == NULL)
    {
        return sciErr;
    }
    *_piColPos = *_piNbItemRow + *_piRows;

    if (_pdblReal == NULL)
    {
        return sciErr;
    }

    iPos = (*_piNbItem + *_piRows) % 2 == 1 ? 0 : 1;
    *_pdblReal = (double*)(*_piColPos + *_piNbItem + iPos);
    if (_iComplex == 1 && _pdblImg != NULL)
    {
        *_pdblImg = *_pdblReal + *_piNbItem;
    }
    return sciErr;
}

SciErr allocSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
    return allocCommonSparseMatrix(_pvCtx, _iVar, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr allocComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    return allocCommonSparseMatrix(_pvCtx, _iVar, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr allocCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iNewPos     = Top - Rhs + _iVar;
    int iAddr       = *Lstk(iNewPos);
    int	iTotalSize  = 0;
    int iOffset     = 0;
    int* piAddr     = NULL;

    //return empty matrix
    if (_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        sciErr = createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        }
        return sciErr;
    }

    //header + offset
    int iMemSize = (5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2)) / 2;
    //+ items size
    iMemSize += _iNbItem * (_iComplex + 1);
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
    if (iMemSize > iFreeSpace)
    {
        addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

    sciErr = fillCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalSize);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_SPARSE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "allocComplexSparseMatrix" : "allocSparseMatrix");
        return sciErr;
    }

    iOffset	= 5;//4 for header + 1 for NbItem
    iOffset		+= _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);

    updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + iOffset));
    updateLstk(iNewPos, sadr(iadr(iAddr) + iOffset), iTotalSize);
    return sciErr;
}

SciErr fillCommonSparseMatrix(void* _pvCtx, int *_piAddress, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg, int* _piTotalSize)
{
    SciErr sciErr = sciErrInit();
    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillCommonSparseMatrix");
        return sciErr;
    }

    _piAddress[0] = sci_sparse;
    _piAddress[1] = Min(_iRows, _iRows * _iCols);
    _piAddress[2] = Min(_iCols, _iRows * _iCols);
    _piAddress[3] = _iComplex;

    _piAddress[4] = _iNbItem;

    *_piNbItemRow = _piAddress + 5;//4 for header + 1 for NbItem
    *_piColPos = *_piNbItemRow + _iRows;
    *_pdblReal = (double*)(*_piColPos + _iNbItem + !((_iRows + _iNbItem) % 2));

    if (_iComplex == 1)
    {
        *_pdblImg = *_pdblReal + _iNbItem;
    }

    *_piTotalSize = _iNbItem * (_iComplex + 1);
    return sciErr;
}

SciErr createSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal)
{
    return createCommonSparseMatrix(_pvCtx, _iVar, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr createComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonSparseMatrix(_pvCtx, _iVar, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr createCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    int iOne            = 1;
    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    if (_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        SciErr sciErr = createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        }
        return sciErr;
    }

    SciErr sciErr = allocCommonSparseMatrix(_pvCtx, _iVar, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SPARSE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createComplexSparseMatrix" : "createSparseMatrix");
        return sciErr;
    }

    memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
    memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
    C2F(dcopy)(&_iNbItem, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
    if (_iComplex)
    {
        C2F(dcopy)(&_iNbItem, const_cast<double*>(_pdblImg), &iOne, pdblImg, &iOne);
    }
    return sciErr;
}

SciErr createNamedSparseMatrix(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal)
{
    return createCommonNamedSparseMatrix(_pvCtx, _pstName, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr createNamedComplexSparseMatrix(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonNamedSparseMatrix(_pvCtx, _pstName, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr createCommonNamedSparseMatrix(void* _pvCtx, const char* _pstName, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iVarID[nsiz];
    int iSaveRhs        = Rhs;
    int iSaveTop        = Top;
    int iTotalSize      = 0;
    int iPos            = 0;

    int* piAddr         = NULL;
    int* piNbItemRow    = NULL;
    int* piColPos       = NULL;
    int iOne            = 1;
    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    //return named empty matrix
    if (_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        sciErr = createNamedMatrixOfDouble(_pvCtx, _pstName, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createNamedEmptyMatrix");
        }
        return sciErr;
    }

    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createCommonNamedSparseMatrix", _pstName);
        return sciErr;
    }

    C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
    Top = Top + Nbvars + 1;

    //header + offset
    int iMemSize = (5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2)) / 2;
    //+ items size
    iMemSize += _iNbItem * (_iComplex + 1);
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(*Lstk(Top)));
    if (iMemSize > iFreeSpace)
    {
        addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

    sciErr = fillCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg, &iTotalSize);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SPARSE, _("%s: Unable to create %s named \"%s\""), _iComplex ? "createNamedComplexSparseMatrix" : "createNamedSparseMatrix", _("sparse matrix"), _pstName);
        return sciErr;
    }

    memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
    memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
    C2F(dcopy)(&_iNbItem, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
    if (_iComplex)
    {
        C2F(dcopy)(&_iNbItem, const_cast<double*>(_pdblImg), &iOne, pdblImg, &iOne);
    }

    iPos	= 5;//4 for header + 1 for NbItem
    iPos += _iRows + _iNbItem;

    //update "variable index"
    updateLstk(Top, *Lstk(Top) + iPos, iTotalSize);

    Rhs = 0;
    //Add name in stack reference list
    createNamedVariable(iVarID);

    Top = iSaveTop;
    Rhs = iSaveRhs;

    return sciErr;

}

SciErr readNamedSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
    return readCommonNamedSparseMatrix(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr readNamedComplexSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
    return readCommonNamedSparseMatrix(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr readCommonNamedSparseMatrix(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
    int* piAddr = NULL;
    int* piNbItemRow = 0;
    int* piColPos = 0;
    int iOne = 1;

    double* pdblReal	= NULL;
    double* pdblImg		= NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_SPARSE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexSparseMatrix" : "readNamedSparseMatrix", _pstName);
        return sciErr;
    }

    if (_iComplex == 1)
    {
        sciErr = getComplexSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
    }
    else
    {
        sciErr = getSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_SPARSE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexSparseMatrix" : "readNamedSparseMatrix", _pstName);
        return sciErr;
    }

    if (_piNbItemRow == NULL)
    {
        return sciErr;
    }

    memcpy(_piNbItemRow, piNbItemRow, *_piRows * sizeof(int));

    if (_piColPos == NULL)
    {
        return sciErr;
    }

    memcpy(_piColPos, piColPos, *_piNbItem * sizeof(int));


    if (_pdblReal == NULL)
    {
        return sciErr;
    }

    C2F(dcopy)(_piNbItem, pdblReal, &iOne, _pdblReal, &iOne);

    if (_iComplex && _pdblImg)
    {
        C2F(dcopy)(_piNbItem, pdblImg, &iOne, _pdblImg, &iOne);
    }

    return sciErr;
}

/* shortcut functions */

/*--------------------------------------------------------------------------*/
int isSparseType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_sparse);
}
/*--------------------------------------------------------------------------*/
int isNamedSparseType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_sparse);
}
/*--------------------------------------------------------------------------*/
int getAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
    return getCommonAllocatedSparseMatrix(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    return getCommonAllocatedSparseMatrix(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    int* piNbItemRow = NULL;
    int* piColPos = NULL;
    int iOne = 1;
    double* pdblReal = NULL;
    double* pdblImg = NULL;

    SciErr sciErr = getCommonSparseMatrix(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedComplexSparseMatrix" : "getAllocatedSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_piNbItemRow = (int*)MALLOC(sizeof(int) **_piRows);
    memcpy(*_piNbItemRow, piNbItemRow, sizeof(int) **_piRows);

    *_piColPos = (int*)MALLOC(sizeof(int) **_piNbItem);
    memcpy(*_piColPos, piColPos, sizeof(int) **_piNbItem);

    *_pdblReal = (double*)MALLOC(sizeof(double) **_piNbItem);
    C2F(dcopy)(_piNbItem, pdblReal, &iOne, *_pdblReal, &iOne);

    if (_iComplex)
    {
        *_pdblImg = (double*)MALLOC(sizeof(double) **_piNbItem);
        C2F(dcopy)(_piNbItem, pdblImg, &iOne, *_pdblImg, &iOne);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedAllocatedSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
    return getCommonNamedAllocatedSparseMatrix(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getNamedAllocatedComplexSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    return getCommonNamedAllocatedSparseMatrix(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonNamedAllocatedSparseMatrix(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = readCommonNamedSparseMatrix(_pvCtx, _pstName, _iComplex, _piRows, _piCols, _piNbItem, NULL, NULL, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_ALLOC_SPARSE, _("%s: Unable to get argument \"%s\""), _iComplex ? "getNamedAllocatedComplexSparseMatrix" : "getNamedAllocatedSparseMatrix", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_piNbItemRow = (int*)MALLOC(sizeof(int) **_piRows);
    *_piColPos = (int*)MALLOC(sizeof(int) **_piNbItem);

    *_pdblReal = (double*)MALLOC(sizeof(double) **_piNbItem);
    if (_iComplex)
    {
        *_pdblImg = (double*)MALLOC(sizeof(double) **_piNbItem);
    }

    sciErr = readCommonNamedSparseMatrix(_pvCtx, _pstName, _iComplex, _piRows, _piCols, _piNbItem, *_piNbItemRow, *_piColPos, *_pdblReal, *_pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_ALLOC_SPARSE, _("%s: Unable to get argument \"%s\""), _iComplex ? "getNamedAllocatedComplexSparseMatrix" : "getNamedAllocatedSparseMatrix", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal)
{
    FREE(_piNbItemRows);
    FREE(_piColPos);
    FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedComplexSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
    freeAllocatedSparseMatrix(_piNbItemRows, _piColPos, _pdblReal);
    FREE(_pdblImg);
}
/*--------------------------------------------------------------------------*/
