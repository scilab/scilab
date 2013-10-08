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

#include "types.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "gatewaystruct.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "gatewaystruct.hxx"
#include "overload.hxx"
#include "context.hxx"
#include "symbol.hxx"

extern "C"
{
#include <string.h>
#include <stdlib.h>
#include "api_scilab.h"
#include "api_internal_common.h"
#include "call_scilab.h"
#include "stackinfo.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "MALLOC.h"
}

static int api_fake_int; //only for compatibility with Scilab 5 stack functions
/*Global structure for scilab 5.x*/
extern "C"
{
    //StrCtx *pvApiCtx = NULL;
}

using namespace types;
/*--------------------------------------------------------------------------*/
/* Defined in SCI/modules/core/src/fortran/cvname.f */
extern "C"
{
    extern int C2F(cvnamel) (int *id, char *str, int *jobptr, int *str_len);
    extern  int C2F(cvname)(int *, char *, int *, unsigned long int);
    /* *jobptr==0: Get Scilab codes from C-string */
    /* *jobptr==1: Get C-string from Scilab codes */

    extern int C2F(stackp) (int *, int *);
};
/*--------------------------------------------------------------------------*/
#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz)
#define CvNameL(id,str,jobptr,str_len) C2F(cvnamel)(id,str,jobptr,str_len);

static SciErr getinternalVarAddress(void* _pvCtx, int _iVar, int** _piAddress);

/*--------------------------------------------------------------------------*/
/* Replaces Rhs */
int* getNbInputArgument(void* _pvCtx)
{
    GatewayStruct *pStr =  (GatewayStruct*)_pvCtx;

    if (pStr == NULL)
    {
        std::cout << "pStr == NULL" << std::endl;
        return 0;
    }

    if (pStr->m_pIn == NULL)
    {
        std::cout << "pStr->m_pin == NULL" << std::endl;
        return 0;
    }

    return &pStr->m_iIn;;
}

/* Replaces Lhs */
int* getNbOutputArgument(void* _pvCtx)
{
    GatewayStruct *pStr =  (GatewayStruct*)_pvCtx;

    if (pStr == NULL)
    {
        return 0;
    }

    if (pStr->m_piRetCount == NULL)
    {
        return 0;
    }

    return &pStr->m_iOut;
}

int* assignOutputVariable(void* _pvCtx, int _iVal)
{
    //do nothing but don't crash
    if (_pvCtx == NULL)
    {
        return &api_fake_int;
    }

    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;

    //do nothing but don't crash
    if (_iVal > *pStr->m_piRetCount)
    {
        return &api_fake_int;
    }

    int* pVal = &(pStr->m_pOutOrder[_iVal - 1]);
    return pVal;
}

int returnArguments(void* _pvCtx)
{
    return 0;
}

int checkInputArgument(void* _pvCtx, int _iMin, int _iMax)
{
    SciErr sciErr = sciErrInit();

    GatewayStruct *pStr = (GatewayStruct*)_pvCtx;
    int iRhs            = *getNbInputArgument(_pvCtx);

    if (_iMin <= nbInputArgument(_pvCtx) && _iMax >= nbInputArgument(_pvCtx))
    {
        return 1;
    }

    if (_iMin == _iMax)
    {
        /* No optional argument */
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), pStr->m_pstName, _iMax);
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), pStr->m_pstName, _iMin, _iMax);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
int checkInputArgumentAtLeast(void* _pvCtx, int _iMin)
{
    SciErr sciErr = sciErrInit();

    if (_iMin <= nbInputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(77, _("%s: Wrong number of input argument(s): at least %d expected.\n"), ((GatewayStruct*)_pvCtx)->m_pstName, _iMin);
    return 0;
}

/*--------------------------------------------------------------------------*/
int checkInputArgumentAtMost(void* _pvCtx, int _iMax)
{
    SciErr sciErr = sciErrInit();

    if (_iMax >= nbInputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(77, _("%s: Wrong number of input argument(s): at most %d expected.\n"), ((GatewayStruct*)_pvCtx)->m_pstName, _iMax);
    return 0;
}

/*--------------------------------------------------------------------------*/
int checkOutputArgument(void* _pvCtx, int _iMin, int _iMax)
{
    SciErr sciErr = sciErrInit();

    if (_iMin <= nbOutputArgument(_pvCtx) && _iMax >= nbOutputArgument(_pvCtx))
    {
        return 1;
    }

    if (_iMax == _iMin)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), ((GatewayStruct*)_pvCtx)->m_pstName, _iMax);
    }
    else
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), ((GatewayStruct*)_pvCtx)->m_pstName, _iMin, _iMax);
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int checkOutputArgumentAtLeast(void* _pvCtx, int _iMin)
{
    SciErr sciErr = sciErrInit();

    if (_iMin <= nbOutputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(78, _("%s: Wrong number of output argument(s): at least %d expected.\n"), ((GatewayStruct*)_pvCtx)->m_pstName, _iMin);
    return 0;
}

/*--------------------------------------------------------------------------*/
int checkOutputArgumentAtMost(void* _pvCtx, int _iMax)
{
    SciErr sciErr = sciErrInit();

    if (_iMax >= nbOutputArgument(_pvCtx))
    {
        return 1;
    }

    Scierror(78, _("%s: Wrong number of output argument(s): at most %d expected.\n"), ((GatewayStruct*)_pvCtx)->m_pstName, _iMax);
    return 0;
}

/*--------------------------------------------------------------------------*/
int callOverloadFunction(void* _pvCtx, int _iVar, char* _pstName, unsigned int _iNameLen)
{
    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    Function::ReturnValue callResult;
    typed_list tlReturnedValues;

    wchar_t* pwstName = to_wide_string(pStr->m_pstName);
    std::wstring wsFunName;

    if (_iVar == 0)
    {
        wsFunName = std::wstring(L"%_") + std::wstring(pwstName);
    }
    else
    {
        wsFunName = std::wstring(L"%") + (*pStr->m_pIn)[_iVar - 1]->getShortTypeStr() + L"_" + std::wstring(pwstName);
    }

    //protect input arguments
    for (int i = 0 ; i < pStr->m_pIn->size() ; i++)
    {
        (*pStr->m_pIn)[i]->IncreaseRef();
    }

    callResult = Overload::call(wsFunName, *(pStr->m_pIn), *(pStr->m_piRetCount), tlReturnedValues, pStr->m_pVisitor);

    //unprotect input arguments
    for (int i = 0 ; i < pStr->m_pIn->size() ; i++)
    {
        (*pStr->m_pIn)[i]->DecreaseRef();
    }

    if (callResult == Function::OK)
    {
        int i = 0;
        typed_list::iterator it;
        for (it = tlReturnedValues.begin() ; it != tlReturnedValues.end() ; ++it, ++i)
        {
            (pStr->m_pOut)[i] = *it;
            pStr->m_pOutOrder[i] = (int)pStr->m_pIn->size() + i + 1;
        }
    }

    FREE(pwstName);
    return 0;
}

/*--------------------------------------------------------------------------*/
SciErr getVarDimension(void *_pvCtx, int *_piAddress, int *_piRows, int *_piCols)
{
    SciErr sciErr = sciErrInit();

    if (_piAddress != NULL && isVarMatrixType(_pvCtx, _piAddress))
    {
        *_piRows = ((InternalType*)_piAddress)->getAs<GenericType>()->getRows();
        *_piCols = ((InternalType*)_piAddress)->getAs<GenericType>()->getCols();
    }
    else
    {
        *_piRows = 0;
        *_piCols = 0;
        if (_piAddress == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getVarDimension");
        }
        else
        {
            addErrorMessage(&sciErr, API_ERROR_NOT_MATRIX_TYPE, _("%s: matrix argument expected"), "getVarDimension");
        }
    }
    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr getNamedVarDimension(void *_pvCtx, const char *_pstName, int *_piRows, int *_piCols)
{
    SciErr sciErr = sciErrInit();
    int *piAddr = NULL;

    sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_NAMED_VARDIM, _("%s: Unable to get dimension of variable \"%s\""), "getNamedVarDimension", _pstName);
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, piAddr, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_NAMED_VARDIM, _("%s: Unable to get dimension of variable \"%s\""), "getNamedVarDimension", _pstName);
        return sciErr;
    }

    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr getVarAddressFromPosition(void *_pvCtx, int _iVar, int **_piAddress)
{
    SciErr sciErr = getinternalVarAddress(_pvCtx, _iVar, _piAddress);
    //sciprint("type : %d(%c)\n", (*_piAddress)[0], intersci_.ntypes[_iVar - 1]);
    //update variable state to "read
    //intersci_.ntypes[_iVar - 1] = '$';
    return sciErr;
}

/*--------------------------------------------------------------------------*/
static SciErr getinternalVarAddress(void *_pvCtx, int _iVar, int **_piAddress)
{
    SciErr sciErr = sciErrInit();
    int iAddr = 0;
    int iValType = 0;

    if (_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POSITION, _("%s: bad call to %s! (1rst argument).\n"), "",
                        "getVarAddressFromPosition");
        return sciErr;
    }

    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    typed_list in = *pStr->m_pIn;
    int*    piRetCount = pStr->m_piRetCount;

    /* we accept a call to getVarAddressFromPosition after a create... call */
    if (_iVar > in.size())
    {
        //manage case where _iVar > in.size(), then look in out to get recent create variable.
        addErrorMessage(&sciErr, API_ERROR_INVALID_POSITION, _("%s: bad call to %s! (1rst argument).\n"), pStr->m_pstName, "getVarAddressFromPosition");
        return sciErr;
    }

    *_piAddress = (int*)in[_iVar - 1];
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getVarNameFromPosition(void *_pvCtx, int _iVar, char *_pstName)
{
    SciErr sciErr = sciErrInit();
#if 0
    int iNameLen = 0;
    int iJob1 = 1;

    CvNameL(&vstk_.idstk[(_iVar - 1) * 6], _pstName, &iJob1, &iNameLen);
    if (iNameLen == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Unable to get name of argument #%d"), "getVarNameFromPosition", _iVar);
        return sciErr;
    }

    _pstName[iNameLen] = '\0';
#endif
    return sciErr;
}
/*--------------------------------------------------------------------------*/
int getNewVarAddressFromPosition(void *_pvCtx, int _iVar, int **_piAddress)
{
    // FIXME
    return 0;
}

/*--------------------------------------------------------------------------*/
SciErr getVarAddressFromName(void *_pvCtx, const char *_pstName, int **_piAddress)
{
    SciErr sciErr = sciErrInit();

    wchar_t* pwstName = to_wide_string(_pstName);
    symbol::Context* pCtx = symbol::Context::getInstance();
    InternalType* pVar = pCtx->get(symbol::Symbol(pwstName));
    FREE(pwstName);

    if (pVar == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Unable to get address of variable \"%s\""), "getVarAddressFromName", _pstName);
    }
    else
    {
        *_piAddress = (int*)pVar;
    }

    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr getVarType(void *_pvCtx, int *_piAddress, int *_piType)
{
    SciErr sciErr = sciErrInit();

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getVarType");
        return sciErr;
    }

    switch (((InternalType*)_piAddress)->getType())
    {
        case GenericType::RealDouble :
            *_piType = sci_matrix;
            break;
        case GenericType::RealPoly :
            *_piType = sci_poly;
            break;
        case GenericType::RealBool :
            *_piType = sci_boolean;
            break;
        case GenericType::RealSparse :
            *_piType = sci_sparse;
            break;
        case GenericType::RealSparseBool :
            *_piType = sci_boolean_sparse;
            break;
            //case GenericType::RealMatlabSparse :
            //    *_piType = sci_matlab_sparse;
            //    break;
        case GenericType::RealInt8 :
        case GenericType::RealUInt8 :
        case GenericType::RealInt16 :
        case GenericType::RealUInt16 :
        case GenericType::RealInt32 :
        case GenericType::RealUInt32 :
        case GenericType::RealInt64 :
        case GenericType::RealUInt64 :
            *_piType = sci_ints;
            break;
        case GenericType::RealHandle :
            *_piType = sci_handles;
            break;
        case GenericType::RealString :
            *_piType = sci_strings;
            break;
        case GenericType::RealMacroFile :
            *_piType = sci_u_function;
            break;
        case GenericType::RealMacro :
            *_piType = sci_c_function;
            break;
        case GenericType::RealList :
            *_piType = sci_list;
            break;
        case GenericType::RealCell :
            *_piType = sci_mlist;
            break;
        case GenericType::RealTList :
            *_piType = sci_tlist;
            break;
        case GenericType::RealMList :
            *_piType = sci_mlist;
            break;
        case GenericType::RealStruct :
            // Scilab < 6 compatibility... Struct have type 17;
            *_piType = sci_mlist;
            break;
        case GenericType::RealUserType :
            *_piType = sci_pointer;
            break;
        case GenericType::RealImplicitList :
            *_piType = sci_implicit_poly;
            break;
        case GenericType::RealFunction :
            *_piType = sci_intrinsic_function;
            break;
        default :
            *_piType = 0;
    }

    return sciErr;
}

/*--------------------------------------------------------------------------*/
// _pvCtx will not be used by getVarAddressFromName neither getVarType
// it can then be NULL.
SciErr getNamedVarType(void *_pvCtx, const char *_pstName, int *_piType)
{
    int *piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_NAMED_UNDEFINED_VAR, _("%s: Unable to get variable \"%s\""), "getNamedVarType", _pstName);
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, piAddr, _piType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_NAMED_TYPE, _("%s: Unable to get type of variable \"%s\""), "getNamedVarType", _pstName);
        return sciErr;
    }
    return sciErr;
}

/*--------------------------------------------------------------------------*/
int isVarComplex(void *_pvCtx, int *_piAddress)
{
    SciErr sciErr;
    int iType = 0;
    int iComplex = 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "isVarComplex");
        return 0;
    }

    InternalType* pIT = (InternalType*)_piAddress;
    GenericType* pGT = dynamic_cast<GenericType*>(pIT);
    if (pGT == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "isVarComplex");
        return 0;
    }

    return pGT->isComplex();
}

/*--------------------------------------------------------------------------*/
int isNamedVarComplex(void *_pvCtx, const char *_pstName)
{
    int *piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "isNamedVarComplex");
        return 0;
    }
    return isVarComplex(_pvCtx, piAddr);
}

/*--------------------------------------------------------------------------*/
void createNamedVariable(int *_piVarID)
{
    //deprecated
}

/*--------------------------------------------------------------------------*/
int updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress)
{
    //deprecated
    return 0;
}

/*--------------------------------------------------------------------------*/
int updateLstk(int _iNewpos, int _iSCIDataAddress, int _iVarSize)
{
    //deprecated
    return 0;
}

/*--------------------------------------------------------------------------*/
int isVarMatrixType(void *_pvCtx, int *_piAddress)
{
    if (_piAddress != NULL)
    {
        int iType = 0;
        getVarType(_pvCtx, _piAddress, &iType);

        switch (iType)
        {
            case sci_matrix:
            case sci_poly:
            case sci_boolean:
            case sci_sparse:
            case sci_boolean_sparse:
            case sci_matlab_sparse:
            case sci_ints:
            case sci_handles:
            case sci_strings:
                return 1;
            default:
                return 0;
        }
    }
    else
    {
        return 0;
    }
    return 1;
}

/*--------------------------------------------------------------------------*/
int isNamedVarMatrixType(void *_pvCtx, const char *_pstName)
{
    int *piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        return 0;
    }
    return isVarMatrixType(_pvCtx, piAddr);
}

/*--------------------------------------------------------------------------*/
SciErr getProcessMode(void *_pvCtx, int _iPos, int *_piAddRef, int *_piMode)
{
    int iRows1 = 0;
    int iCols1 = 0;
    int iRows2 = 0;
    int iCols2 = 0;
    int iType2 = 0;
    int iMode = 0;
    int *piAddr2 = NULL;

    SciErr sciErr = getVarDimension(_pvCtx, _piAddRef, &iRows1, &iCols1);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
        return sciErr;
    }

    //sciprint("getProcessMode ->");
    sciErr = getinternalVarAddress(_pvCtx, _iPos, &piAddr2);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get variable address"), "getProcessMode");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, piAddr2, &iType2);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument type"), "getProcessMode");
        return sciErr;
    }

    if (iType2 == sci_matrix && !isVarComplex(_pvCtx, piAddr2))
    {
        double *pdblReal2 = NULL;

        sciErr = getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblReal2);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
            return sciErr;
        }

        if (iRows2 != 1 || iCols2 != 1)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument #%d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1,
                            1);
            return sciErr;
        }

        iMode = (int)pdblReal2[0];
    }
    else if (iType2 == sci_strings)
    {
        int iLen = 0;
        char *pstMode[1] = { "" };

        sciErr = getVarDimension(_pvCtx, piAddr2, &iRows2, &iCols2);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
            return sciErr;
        }

        if (iRows2 != 1 || iCols2 != 1)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument #%d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1,
                            1);
            return sciErr;
        }

        sciErr = getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, NULL);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
            return sciErr;
        }

        pstMode[0] = (char *)MALLOC(sizeof(char) * (iLen + 1)); //+1 for null termination
        sciErr = getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, pstMode);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
            return sciErr;
        }

        iMode = (int)pstMode[0][0];
        FREE(pstMode[0]);
    }
    else
    {
        addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"),
                        "getProcessMode", _iPos);
        return sciErr;
    }

    if (iMode == ROW_LETTER || iMode == BY_ROWS)
    {
        *_piMode = BY_ROWS;
    }
    else if (iMode == COL_LETTER || iMode == BY_COLS)
    {
        *_piMode = BY_COLS;
    }
    else if (iMode == STAR_LETTER || iMode == BY_ALL)
    {
        *_piMode = BY_ALL;
    }
    else if (iMode == MTLB_LETTER || iMode == BY_MTLB)
    {
        *_piMode = 0;
        if (iRows1 > 1)
        {
            *_piMode = 1;
        }
        else if (iCols1 > 1)
        {
            *_piMode = 2;
        }
    }
    else
    {
        addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "getProcessMode",
                        _iPos, "'*', 'r', 'c', 'm', '0', '1', '2'", "-1");
        return sciErr;
    }
    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr getDimFromVar(void *_pvCtx, int *_piAddress, int *_piVal)
{
    int iType = 0;
    int iRows = 0;
    int iCols = 0;
    double *pdblReal = NULL;

    SciErr sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument type"), "getDimFromVar");
        return sciErr;
    }

    if (iType == sci_matrix)
    {
        if (isVarComplex(_pvCtx, _piAddress))
        {
            addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "getDimFromVar",
                            getRhsFromAddress(_pvCtx, _piAddress));
            return sciErr;
        }

        sciErr = getMatrixOfDouble(_pvCtx, _piAddress, &iRows, &iCols, &pdblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
            return sciErr;
        }

        *_piVal = (int)Max(pdblReal[0], 0);
    }
    else if (iType == sci_ints)
    {
        int iPrec = 0;

        sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument dimension"), "getDimFromVar");
            return sciErr;
        }

        if (iRows != 1 || iCols != 1)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong size for argument #%d: (%d,%d) expected.\n"), "getProcessMode",
                            getRhsFromAddress(_pvCtx, _piAddress), 1, 1);
            return sciErr;
        }

        sciErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument precision"), "getDimFromVar");
            return sciErr;
        }

        switch (iPrec)
        {
            case SCI_INT8:
            {
                char *pcData = NULL;

                sciErr = getMatrixOfInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pcData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = pcData[0];
            }
            break;
            case SCI_UINT8:
            {
                unsigned char *pucData = NULL;

                sciErr = getMatrixOfUnsignedInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pucData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = pucData[0];
            }
            break;
            case SCI_INT16:
            {
                short *psData = NULL;

                sciErr = getMatrixOfInteger16(_pvCtx, _piAddress, &iRows, &iCols, &psData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = psData[0];
            }
            break;
            case SCI_UINT16:
            {
                unsigned short *pusData = NULL;

                sciErr = getMatrixOfUnsignedInteger16(_pvCtx, _piAddress, &iRows, &iCols, &pusData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = pusData[0];
            }
            break;
            case SCI_INT32:
            {
                int *piData = NULL;

                sciErr = getMatrixOfInteger32(_pvCtx, _piAddress, &iRows, &iCols, &piData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = piData[0];
            }
            break;
            case SCI_UINT32:
            {
                unsigned int *puiData = NULL;

                sciErr = getMatrixOfUnsignedInteger32(_pvCtx, _piAddress, &iRows, &iCols, &puiData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = puiData[0];
            }
            break;
#ifdef __SCILAB_INT64__
            case SCI_INT64:
            {
                long long *pllData = NULL;

                sciErr = getMatrixOfInteger64(_pvCtx, _piAddress, &iRows, &iCols, &pllData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = (int)pllData[0];
            }
            break;
            case SCI_UINT64:
            {
                unsigned long long *pullData = NULL;

                sciErr = getMatrixOfUnsignedInteger64(_pvCtx, _piAddress, &iRows, &iCols, &pullData);
                if (sciErr.iErr)
                {
                    addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
                    return sciErr;
                }
                *_piVal = (int)pullData[0];
            }
            break;
#endif
        }
    }
    else
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for input argument #%d: A real scalar or an integer scalar expected.\n"),
                        "getDimFromVar", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }
    return sciErr;
}

/*--------------------------------------------------------------------------*/
SciErr getDimFromNamedVar(void *_pvCtx, const char *_pstName, int *_piVal)
{
    int *piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_DIMFROMVAR, _("%s: Unable to get dimension from variable \"%s\""), "getDimFromNamedVar",
                        _pstName);
        return sciErr;
    }

    sciErr = getDimFromVar(_pvCtx, piAddr, _piVal);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_DIMFROMVAR, _("%s: Unable to get dimension from variable \"%s\""), "getDimFromNamedVar",
                        _pstName);
        return sciErr;
    }

    return sciErr;
}

/*--------------------------------------------------------------------------*/
int getRhsFromAddress(void *_pvCtx, int *_piAddress)
{
    int i = 0;
    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    typed_list in = *pStr->m_pIn;

    for (i = 0 ; i < in.size() ; i++)
    {
        if (_piAddress == (int*)in[i])
        {
            return i + 1;
        }
    }
    return -1;
}

/*short cut functions*/

/*--------------------------------------------------------------------------*/
int isRowVector(void *_pvCtx, int *_piAddress)
{
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    SciErr sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_ROW_VECTOR, _("%s: Unable to get argument dimension"), "isRowVector");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (iRows == 1 && iCols > 1)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isNamedRowVector(void *_pvCtx, const char *_pstName)
{
    int iRows = 0;
    int iCols = 0;

    if (isNamedVarMatrixType(_pvCtx, _pstName) == 0)
    {
        return 0;
    }

    SciErr sciErr = getNamedVarDimension(_pvCtx, _pstName, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_NAMED_ROW_VECTOR, _("%s: Unable to get argument dimension"), "isNamedRowVector");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (iRows == 1 && iCols > 1)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isColumnVector(void *_pvCtx, int *_piAddress)
{
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    SciErr sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_COLUMN_VECTOR, _("%s: Unable to get argument dimension"), "isColumnVector");
        printError(&sciErr, 0);
        return 0;
    }

    if (iCols == 1 && iRows > 1)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isNamedColumnVector(void *_pvCtx, const char *_pstName)
{
    int iRows = 0;
    int iCols = 0;

    if (isNamedVarMatrixType(_pvCtx, _pstName) == 0)
    {
        return 0;
    }

    SciErr sciErr = getNamedVarDimension(_pvCtx, _pstName, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_NAMED_COLUMN_VECTOR, _("%s: Unable to get argument dimension"), "isNamedColumnVector");
        printError(&sciErr, 0);
        return 0;
    }

    if (iCols == 1 && iRows > 1)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isVector(void *_pvCtx, int *_piAddress)
{
    return isRowVector(_pvCtx, _piAddress) || isColumnVector(_pvCtx, _piAddress);
}

/*--------------------------------------------------------------------------*/
int isNamedVector(void *_pvCtx, const char *_pstName)
{
    return isNamedRowVector(_pvCtx, _pstName) || isNamedColumnVector(_pvCtx, _pstName);
}

/*--------------------------------------------------------------------------*/
int isScalar(void *_pvCtx, int *_piAddress)
{
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    SciErr sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_SCALAR, _("%s: Unable to get argument dimension"), "isScalar");
        printError(&sciErr, 0);
        return 0;
    }

    if (iCols == 1 && iRows == 1)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isNamedScalar(void *_pvCtx, const char *_pstName)
{
    int iRows = 0;
    int iCols = 0;

    if (isNamedVarMatrixType(_pvCtx, _pstName) == 0)
    {
        return 0;
    }

    SciErr sciErr = getNamedVarDimension(_pvCtx, _pstName, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_NAMED_SCALAR, _("%s: Unable to get argument dimension"), "isNamedScalar");
        printError(&sciErr, 0);
        return 0;
    }

    if (iCols == 1 && iRows == 1)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isSquareMatrix(void *_pvCtx, int *_piAddress)
{
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    SciErr sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_SQUARE, _("%s: Unable to get argument dimension"), "isSquareMatrix");
        printError(&sciErr, 0);
        return 0;
    }

    if (iRows > 1 && iCols == iRows)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isNamedSquareMatrix(void *_pvCtx, const char *_pstName)
{
    int iRows = 0;
    int iCols = 0;

    if (isNamedVarMatrixType(_pvCtx, _pstName) == 0)
    {
        return 0;
    }

    SciErr sciErr = getNamedVarDimension(_pvCtx, _pstName, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_IS_NAMED_SQUARE, _("%s: Unable to get argument dimension"), "isNamedSquareMatrix");
        printError(&sciErr, 0);
        return 0;
    }

    if (iRows > 1 && iCols == iRows)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int checkVarDimension(void *_pvCtx, int *_piAddress, int _iRows, int _iCols)
{
    int iRows = 0;
    int iCols = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    if (isVarMatrixType(_pvCtx, _piAddress) == 0)
    {
        return 0;
    }

    SciErr sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CHECK_VAR_DIMENSION, _("%s: Unable to get argument dimension"), "checkVarDimension");
        printError(&sciErr, 0);
        return 0;
    }

    if ((_iRows == iRows || _iRows == -1) && (_iCols == iCols || _iCols == -1))
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int checkNamedVarDimension(void *_pvCtx, const char *_pstName, int _iRows, int _iCols)
{
    int iRows = 0;
    int iCols = 0;

    if (isNamedVarMatrixType(_pvCtx, _pstName) == 0)
    {
        return 0;
    }

    SciErr sciErr = getNamedVarDimension(_pvCtx, _pstName, &iRows, &iCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CHECK_NAMED_VAR_DIMENSION, _("%s: Unable to get argument dimension"), "checkNamedVarDimension");
        printError(&sciErr, 0);
        return 0;
    }

    if ((_iRows == iRows || _iRows == -1) && (_iCols == iCols || _iCols == -1))
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int checkVarType(void *_pvCtx, int *_piAddress, int _iType)
{
    int iType = 0;

    if (_piAddress == NULL)
    {
        return 0;
    }

    SciErr sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (iType == _iType)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int checkNamedVarType(void *_pvCtx, const char *_pstName, int _iType)
{
    int iType = 0;

    SciErr sciErr = getNamedVarType(_pvCtx, _pstName, &iType);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (iType == _iType)
    {
        return 1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int getInputArgumentType(void* _pvCtx, int _iVar)
{
    int* piAddr = NULL;
    int iType = 0;

    SciErr sciErr = getinternalVarAddress(_pvCtx, _iVar, &piAddr);
    if (sciErr.iErr)
    {
        return 0;
    }

    sciErr = getVarType(_pvCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        return 0;
    }

    return iType;
}

/*--------------------------------------------------------------------------*/
int checkInputArgumentType(void* _pvCtx, int _iVar, int _iType)
{
    return getInputArgumentType(_pvCtx, _iVar) == _iType;
}

/*--------------------------------------------------------------------------*/
int isEmptyMatrix(void *_pvCtx, int *_piAddress)
{
    if (checkVarType(_pvCtx, _piAddress, sci_matrix))
    {
        return checkVarDimension(_pvCtx, _piAddress, 0, 0);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
int isNamedEmptyMatrix(void *_pvCtx, const char *_pstName)
{
    if (checkNamedVarType(_pvCtx, _pstName, sci_matrix))
    {
        return checkNamedVarDimension(_pvCtx, _pstName, 0, 0);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
int createEmptyMatrix(void *_pvCtx, int _iVar)
{
    double dblReal = 0;

    SciErr sciErr = createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int createNamedEmptyMatrix(void *_pvCtx, const char *_pstName)
{
    double dblOne = 0;

    SciErr sciErr = createNamedMatrixOfDouble(_pvCtx, _pstName, 0, 0, &dblOne);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createNamedEmptyMatrix");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int isNamedVarExist(void *_pvCtx, const char *_pstName)
{
    SciErr sciErr = sciErrInit();
    int *piAddr = NULL;

    sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr || piAddr == NULL)
    {
        return 0;
    }

    return 1;
}

/*--------------------------------------------------------------------------*/
int checkNamedVarFormat(void* _pvCtx, const char *_pstName)
{
#define FORBIDDEN_CHARS " */\\.,;:^@><!=+-&|()~\n\t'\""

    // check pointer
    if (_pstName == NULL)
    {
        return 0;
    }

    // check length _pstName <> 0
    if (strlen(_pstName) == 0)
    {
        return 0;
    }

    // forbidden characters
    if (strpbrk(_pstName, FORBIDDEN_CHARS) != NULL)
    {
        return 0;
    }

    // variable does not begin by a digit
    if (isdigit(_pstName[0]))
    {
        return 0;
    }

    // check that we have only ascii characters
    for (int i = 0; i < (int)strlen(_pstName); i++)
    {
        if (!isascii(_pstName[i]))
        {
            return 0;
        }
    }

    // add here some others rules

    return 1;
}
/*--------------------------------------------------------------------------*/
int deleteNamedVariable(void* _pvCtx, const char* _pstName)
{
    SciErr sciErr = sciErrInit();

    if (isNamedVarExist(_pvCtx, _pstName) == 0)
    {
        return 0;
    }

    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name."), "createNamedComplexZMatrixOfDouble");
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int increaseValRef(void* _pvCtx, int* _piAddress)
{
    if (_piAddress)
    {
        types::InternalType* pIT = (types::InternalType*)_piAddress;
        types::InternalType* pIT2 = dynamic_cast<types::InternalType*>(pIT);
        if (pIT2)
        {
            pIT->IncreaseRef();
            return 1;
        }
        else
        {
            Scierror(999, _("Invalid type pointer in '%s'\n"), "increaseValRef");
            return -1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int decreaseValRef(void* _pvCtx, int* _piAddress)
{
    if (_piAddress)
    {
        types::InternalType* pIT = (types::InternalType*)_piAddress;
        types::InternalType* pIT2 = dynamic_cast<types::InternalType*>(pIT);
        if (pIT2)
        {
            pIT->DecreaseRef();
            if (pIT->isDeletable())
            {
                delete pIT;
            }
            return 1;
        }
        else
        {
            Scierror(999, _("Invalid type pointer in '%s'\n"), "decreaseValRef");
            return -1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
SciErr sciErrInit()
{
    int i = 0 ;
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    for (; i < MESSAGE_STACK_SIZE ; i++)
    {
        sciErr.pstMsg[i] = "";
    }

    return sciErr;
}

