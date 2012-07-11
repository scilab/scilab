/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <stdio.h>

#include "string.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "polynom.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "function.hxx"
#include "context.hxx"
#include "hdf5_gw.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "h5_fileManagement.h"
#include "deleteafile.h"
#include "h5_writeDataToFile.h"
#include "sci_types.h"
}

#include "forceJHDF5load.hxx"

//#define PRINT_DEBUG
int iLevel = 0;

static bool export_data(int _iH5File, types::InternalType* pIT, wchar_t* _pwstName);
static bool export_list(int _iH5File, types::InternalType* pIT, wchar_t* _pwstName);
static bool export_double(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
static bool export_poly(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
static bool export_boolean(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
static bool export_sparse(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
//static bool export_boolean_sparse(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
//static bool export_matlab_sparse(void* pvApiCtx, int *_piVar, char* _pstName);
static bool export_ints(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
//static bool export_handles(void* pvApiCtx, int *_piVar, char* _pstName);
static bool export_strings(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);
//static bool export_u_function(void* pvApiCtx, int *_piVar, char* _pstName);
//static bool export_c_function(void* pvApiCtx, int *_piVar, char* _pstName);
//static bool export_lib(void* pvApiCtx, int *_piVar, char* _pstName);
//static bool export_lufact_pointer(void* pvApiCtx, int *_piVar, char* _pstName);

static bool export_void(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName);


void print_type(char* _pstType);
int extractVarNameList(types::typed_list &in, wchar_t** _pstNameList);

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_export_to_hdf5(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iNbVar              = 0;
    wchar_t** pwstNameList	= NULL;
    char* pstFileName       = NULL;
    bool bExport            = false;

#ifndef _MSC_VER
    forceJHDF5load();
#endif

    if(in.size() < 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): At least %d expected.\n"), L"export_to_hdf5", 2);
        return types::Function::Error;
    }

    pwstNameList = (wchar_t**)MALLOC(sizeof(wchar_t*) * in.size());
    iNbVar = extractVarNameList(in, pwstNameList);
    if(iNbVar != 0)
    {
        //convert UTF16 to UTF8
        pstFileName = wide_string_to_UTF8(pwstNameList[0]);

        // open hdf5 file
        int iH5File = createHDF5File(pstFileName);
        if(iH5File < 0)
        {
            if(iH5File == -2)
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: \"%s\" is a directory"), L"export_to_hdf5", 1, pstFileName);
            }
            else
            {
                ScierrorW(999,_W("%ls: Cannot open file %s.\n"), L"export_to_hdf5", pstFileName);
            }

            FREE(pstFileName);
            FREE(pwstNameList);
            return types::Function::Error;
        }

        for(int i = 0 ; i < in.size() - 1; i++)
        {
            types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(pwstNameList[i + 1]));

            bExport = export_data(iH5File, pIT, pwstNameList[i + 1]);
            if(bExport == false)
            {
                break;
            }
        }

        //close hdf5 file
        closeHDF5File(iH5File);
        if(bExport == false)
        {//remove file
            deleteafileW(pwstNameList[0]);
        }
    }
    //create boolean return value
    types::Bool* pOut = new types::Bool(bExport);
    out.push_back(pOut);

    //free allocated memory
    FREE(pstFileName);
    FREE(pwstNameList);

    return types::Function::OK;
}

static bool export_data(int _iH5File, types::InternalType* pIT, wchar_t* _pwstName)
{
    bool bReturn = false;

    switch(pIT->getType())
    {
    case types::InternalType::RealDouble :
        {
            bReturn = export_double(_iH5File, pIT, _pwstName);
            break;
        }
   case types::InternalType::RealPoly :
        {
            bReturn = export_poly(_iH5File, pIT, _pwstName);
            break;
        }
    case types::InternalType::RealBool :
        {
            bReturn = export_boolean(_iH5File, pIT, _pwstName);
            break;
        }
        /*
    case sci_sparse :
        {
            bReturn = export_sparse(_iH5File, pIT, _pwstName);
            break;
        }
    case sci_boolean_sparse :
        {
            bReturn = export_boolean_sparse(_iH5File, pIT, _pwstName);
            break;
        }
    case sci_matlab_sparse :
        {
            bReturn = export_matlab_sparse(pvApiCtx, _piVar, _pwstName);
            break;
        }
     */
    case types::InternalType::RealInt8 :
    case types::InternalType::RealUInt8 :
    case types::InternalType::RealInt16 :
    case types::InternalType::RealUInt16 :
    case types::InternalType::RealInt32 :
    case types::InternalType::RealUInt32 :
    case types::InternalType::RealInt64 :
    case types::InternalType::RealUInt64 :
        {
            bReturn = export_ints(_iH5File, pIT, _pwstName);
            break;
        }
        /*
    case sci_handles :
        {
            bReturn = export_handles(pvApiCtx, _piVar, _pwstName);
            break;
        }
        */
    case types::InternalType::RealString :
        {
            bReturn = export_strings(_iH5File, pIT, _pwstName);
            break;
        }
        /*
    case types::Function :
        {
            bReturn = export_u_function(pvApiCtx, _piVar, _pwstName);
            break;
        }
    case sci_c_function :
        {
            bReturn = export_c_function(pvApiCtx, _piVar, _pwstName);
            break;
        }
    case sci_lib :
        {
            bReturn = export_lib(pvApiCtx, _piVar, _pwstName);
            break;
        }
        */
    case types::InternalType::RealList :
    case types::InternalType::RealTList :
    case types::InternalType::RealMList :
        {
            bReturn = export_list(_iH5File, pIT, _pwstName);
            break;
        }
        /*
    case sci_lufact_pointer :
        {
            bReturn = export_lufact_pointer(pvApiCtx, _piVar, _pwstName);
            break;
        }
        */
    case 0 : //void case to "null" items in list
        {
            bReturn = export_void(_iH5File, pIT, _pwstName);
            break;
        }
    }
    return bReturn;
}


static bool export_void(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet = writeVoid(_iH5File, wide_string_to_UTF8(_pwstName));
    if(iRet)
    {
        return false;
    }

    char pstMsg[] = "void";
    print_type(pstMsg);
    return true;
}

static bool export_undefined(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet = writeUndefined(_iH5File, wide_string_to_UTF8(_pwstName));
    if(iRet)
    {
        return false;
    }

    char pstMsg[] = "void";
    print_type(pstMsg);
    return true;
}

static bool export_list(int _iH5File, types::InternalType* pIT, wchar_t* _pwstName)
{
    int iRet        = 0;
    char* pstName   = wide_string_to_UTF8(_pwstName);
    bool bReturn    = false;

    types::List* pList = pIT->getAs<types::List>();
    int iItemNumber = pList->getSize();
    //create groupe name
    char* pstGroupName	= createGroupName(pstName);

    iLevel++;
    //open list
    void *pvList = openList(_iH5File, pstGroupName, iItemNumber);
    for(int i = 0 ; i < iItemNumber ; i++)
    {
        int *piNewVar = NULL;
       // getListItemAddress(_piVar, i + 1, &piNewVar);//1 indexed
        types::InternalType* pNewIT =  pList->get(i);
        char* pstPathName   = createPathName(pstGroupName, i);
        wchar_t* pwstPathName = to_wide_string(pstPathName);
        if(pNewIT->isListUndefined())
        {//undefined item
            bReturn = export_undefined(_iH5File, pNewIT, pwstPathName);
        }
        else
        {
            bReturn = export_data(_iH5File, pNewIT, pwstPathName);
        }

        iRet = addItemInList(_iH5File, pvList, i, pstPathName);
        FREE(pstPathName);
        FREE(pwstPathName);

        if(bReturn == false || iRet)
            return false;
    }
    iLevel--;

    int type = 0;
    switch(pIT->getType())
    {
    case types::InternalType::RealList :
        type = sci_list;
        break;
    case types::InternalType::RealTList :
        type = sci_tlist;
        break;
    case types::InternalType::RealMList :
        type = sci_mlist;
        break;
    default : return false;
    }

    closeList(_iH5File, pvList, pstName, iItemNumber, type);
    FREE(pstGroupName);
    //close list
    return true;
}

static bool export_double(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet            = 0;
    types::Double* pDbl = pIT->getAs<types::Double>();
    char* pstName       = wide_string_to_UTF8(_pwstName);

    if(pDbl->isComplex())
        iRet = writeDoubleComplexMatrix(_iH5File, pstName, pDbl->getRows(), pDbl->getCols(), pDbl->getReal(), pDbl->getImg());
    else
        iRet = writeDoubleMatrix(_iH5File, pstName, pDbl->getRows(), pDbl->getCols(), pDbl->getReal());

    FREE(pstName);

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "double (%d x %d)", pDbl->getRows(), pDbl->getCols());
    print_type(pstMsg);
    return true;
}

static bool export_poly(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet                = 0;
    types::Polynom* pPoly   = pIT->getAs<types::Polynom>();
    char* pstName           = wide_string_to_UTF8(_pwstName);
    char* varName           = wide_string_to_UTF8(pPoly->getVariableName().c_str());
    double** real           = NULL;
    double** img            = NULL;
    int* nbrCoef            = NULL;
    int size                = pPoly->getRows()*pPoly->getCols();

    real = (double**)MALLOC(size*sizeof(double*));
    nbrCoef = (int*)MALLOC(size*sizeof(int));

    for(int i=0; i < size; i++)
    {
        nbrCoef[i] = pPoly->get(i)->getRank();
        real[i] = pPoly->get(i)->getCoefReal();
    }

    if(pPoly->isComplex())
    {
        img = (double**)MALLOC(size*sizeof(double*));
        for(int i=0; i <size; i++)
        {
            img[i] = pPoly->get(i)->getCoefImg();
        }

        iRet = writePolyComplexMatrix(_iH5File, pstName, varName , pPoly->getRows(), pPoly->getCols(), nbrCoef, real, img);
        FREE(img);
    }
    else
    {
        iRet = writePolyMatrix(_iH5File, pstName, varName, pPoly->getRows(), pPoly->getCols(), nbrCoef, real);
    }

    FREE(nbrCoef);
    FREE(real);
    FREE(pstName);
    FREE(varName);

    if(iRet)
    {
        return false;
    }
/*
    char pstMsg[512];
    sprintf(pstMsg, "poly (%d x %d)", iRows, iCols);
    print_type(pstMsg);
*/
    return true;
}

static bool export_boolean(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet            = 0;
    types::Bool* pBool  = pIT->getAs<types::Bool>();
    char* pstName       = wide_string_to_UTF8(_pwstName);

    iRet = writeBooleanMatrix(_iH5File, pstName, pBool->getRows(), pBool->getCols(), pBool->get());

    FREE(pstName);

    if(iRet)
    {
        //Msg ??
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "bool (%d x %d)", pBool->getRows(), pBool->getCols());
    print_type(pstMsg);
    return true;
}
/*
static bool export_boolean_sparse(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet						= 0;
    int iRows						= 0;InternalType::Real
    int iCols						= 0;
    int iNbItem					= 0;
    int* piNbItemRow		= NULL;
    int* piColPos				= NULL;

    SciErr sciErr = getBooleanSparseMatrix(pvApiCtx, _piVar, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    iRet = writeBooleanSparseMatrix(_iH5File, _pstName, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    if(iRet)
    {
        //Msg ??
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "boolean sparse (%d x %d)", iRows, iCols);
    print_type(pstMsg);
    return true;
}
/*
static bool export_sparse(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet						= 0;
    int iRows						= 0;
    int iCols						= 0;
    int iNbItem					= 0;
    int* piNbItemRow		= NULL;
    int* piColPos				= NULL;
    double* pdblReal		= NULL;
    double* pdblImg		= NULL;
    SciErr sciErr;

    if(isVarComplex(pvApiCtx, _piVar))
    {
        sciErr = getComplexSparseMatrix(pvApiCtx, _piVar, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeSparseComplexMatrix(_iH5File, _pstName, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
    }
    else
    {
        sciErr = getSparseMatrix(pvApiCtx, _piVar, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeSparseMatrix(_iH5File, _pstName, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
    }

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "sparse (%d x %d)", iRows, iCols);
    print_type(pstMsg);
    return true;
}
/*
static bool export_matlab_sparse(void* pvApiCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}
*/
static bool export_ints(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{
    int iRet            = 0;
    char* pstName       = wide_string_to_UTF8(_pwstName);

    switch(pIT->getType())
    {
        case types::InternalType::RealInt8 :
        {
            types::Int8* pInt    = pIT->getAs<types::Int8>();
            iRet = writeInteger8Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealUInt8 :
        {
            types::UInt8* pInt    = pIT->getAs<types::UInt8>();
            iRet = writeUnsignedInteger8Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealInt16 :
        {
            types::Int16* pInt    = pIT->getAs<types::Int16>();
            iRet = writeInteger16Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealUInt16 :
        {
            types::UInt16* pInt    = pIT->getAs<types::UInt16>();
            iRet = writeUnsignedInteger16Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealInt32 :
        {
            types::Int32* pInt    = pIT->getAs<types::Int32>();
            iRet = writeInteger32Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealUInt32 :
        {
            types::UInt32* pInt    = pIT->getAs<types::UInt32>();
            iRet = writeUnsignedInteger32Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealInt64 :
        {
            types::Int64* pInt    = pIT->getAs<types::Int64>();
            iRet = writeInteger64Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        case types::InternalType::RealUInt64 :
        {
            types::UInt64* pInt    = pIT->getAs<types::UInt64>();
            iRet = writeUnsignedInteger64Matrix(_iH5File, pstName, pInt->getRows(), pInt->getCols(), pInt->get());
            break;
        }
        default :
            iRet = true;
    }

    if(iRet)
    {
        return false;
    }
/*
    char pstMsg[512];
    sprintf(pstMsg, "int%d (%d x %d)", 8*iPrec, iRows, iCols);
    print_type(pstMsg);
*/
    return true;
}
/*
static bool export_handles(void* pvApiCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}
*/
static bool export_strings(int _iH5File, types::InternalType* pIT,wchar_t* _pwstName)
{

    int iRet            = 0;
    types::String* pStr = pIT->getAs<types::String>();
    char* pstName       = wide_string_to_UTF8(_pwstName);
    char** pstData      = NULL;

    pstData = (char**)malloc(pStr->getRows()*pStr->getCols()*sizeof(char*));

    for(int i=0; i < pStr->getRows()*pStr->getCols(); i++)
        pstData[i] = wide_string_to_UTF8(pStr->get(i));

    iRet = writeStringMatrix(_iH5File, pstName, pStr->getRows(), pStr->getCols(),pstData);

    FREE(pstName);
    FREE(pstData);

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "string (%d x %d)", pStr->getRows(), pStr->getCols());
    print_type(pstMsg);

    return true;
}
/*
static bool export_u_function(void* pvApiCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}
/*
static bool export_c_function(void* pvApiCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}
/*
static bool export_lib(void* pvApiCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}
/*
static bool export_lufact_pointer(void* pvApiCtx, int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return false;
}
*/
void print_type(char* _pstType)
{
#ifdef PRINT_DEBUG
    for(int i = 0 ; i < iLevel ; i++)
    {
        sciprint("\t");
    }
    sciprint("%s\n", _pstType);
#endif
}


int extractVarNameList(types::typed_list &in, wchar_t** _pstNameList)
{
    int iCount = 0;

    for(int i = 0 ; i < in.size() ; i++)
    {
        if(in[i]->isString() == false || in[i]->getAs<types::String>()->isScalar() == false)
        {
            ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"export_to_hdf5", i + 1);
            return 0;
        }

        _pstNameList[i] = in[i]->getAs<types::String>()->get(0);
        iCount++;
    }
    return iCount;
}
/*--------------------------------------------------------------------------*/
