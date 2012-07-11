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

#include "string.hxx"
#include "bool.hxx"
#include "polynom.hxx"
#include "int.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "function.hxx"
#include "context.hxx"
#include "hdf5_gw.hxx"
#include "listundefined.hxx"

extern "C"
{
#include "sci_types.h"
#include <hdf5.h>
#include <string.h>
#include "gw_hdf5.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile.h"
}

#include "forceJHDF5load.hxx"


//#define PRINT_DEBUG
//#define TIME_DEBUG

static int iTab = 0;
static int iCloseList = 0;


void print_tree(char* _pstMsg);

static types::InternalType* import_data(int _iDatasetId);
static types::InternalType* import_double(int _iDatasetId);
static types::InternalType* import_string(int _iDatasetId);
static types::InternalType* import_boolean(int _iDatasetId);
static types::InternalType* import_integer(int _iDatasetId);
//static types::InternalType* import_sparse(int _iDatasetId);
//static types::InternalType* import_boolean_sparse(int _iDatasetId);
static types::InternalType* import_poly(int _iDatasetId);
static types::InternalType* import_list(int _iDatasetId, int _iVarType);
//static types::InternalType* import_void(int _iDatasetId);
static types::InternalType* import_undefined(int _iDatasetId);

types::Function::ReturnValue sci_import_from_hdf5(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char* pstFileName       = NULL;
    int iNbItem             = 0;
    int iFile               = 0;
    bool bImport            = false;

#ifndef _MSC_VER
    forceJHDF5load();
#endif

    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"import_from_hdf5", 1);
        return types::Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"import_from_hdf5", 1);
        return types::Function::Error;
    }

    pstFileName = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));

    //open hdf5 file
    iFile = openHDF5File(pstFileName);
    if(iFile < 0)
    {
        ScierrorW(999,_W("%ls: Cannot open file %s.\n"), L"import_to_hdf5", pstFileName);
        return types::Function::Error;
    }
    iNbItem = getVariableNames(iFile, NULL);
    if(iNbItem != 0)
    {
        char** pstVarNameList = (char**)MALLOC(sizeof(char*) * iNbItem);
        wchar_t** pwstVarNameList = (wchar_t**)MALLOC(sizeof(wchar_t*) * iNbItem);
        iNbItem = getVariableNames(iFile, pstVarNameList);
        for(int i=0; i<iNbItem; i++)
        {
            pwstVarNameList[i] = to_wide_string(pstVarNameList[i]);
        }

        //import all data
        for(int i = 0 ; i < iNbItem ; i++)
        {
            int iDataSetId = getDataSetIdFromName(iFile, pstVarNameList[i]);
            if(iDataSetId == 0)
            {
                return types::Function::Error;
            }

            types::InternalType* pIT = import_data(iDataSetId);

            if(pIT == NULL)
            {
                break;
            }

            symbol::Context::getInstance()->put(symbol::Symbol(pwstVarNameList[i]), *pIT);
            bImport = true;
        }
    }
    //close the file
    closeHDF5File(iFile);

    //create boolean return value
    types::Bool* pOut = new types::Bool(bImport);
    out.push_back(pOut);

    FREE(pstFileName);

    return types::Function::OK;
}

//static types::InternalType* import_data(void* pvApiCtx, int _iDatasetId)
static types::InternalType* import_data(int _iDatasetId)
{
    types::InternalType* pIT = NULL;
    bool bRet = false;
    //get var type
    int iVarType = getScilabTypeFromDataSet(_iDatasetId);
    switch(iVarType)
    {
    case sci_matrix :
        {
            pIT = import_double(_iDatasetId);
            break;
        }
    case sci_strings :
        {
            pIT = import_string(_iDatasetId);
            break;
        }
    case sci_list :
    case sci_tlist :
    case sci_mlist :
        {
            pIT = import_list(_iDatasetId, iVarType);
            break;
        }
    case sci_boolean :
        {
            pIT = import_boolean(_iDatasetId);
            break;
        }
    case sci_poly :
        {
            pIT = import_poly(_iDatasetId);
            break;
        }
    case sci_ints:
        {
            pIT = import_integer(_iDatasetId);
            break;
        }
        /*
    case sci_sparse :
        {
            pIT = import_sparse(pvApiCtx, _iDatasetId);
            break;
        }
    case sci_boolean_sparse :
        {
            pIT = import_boolean_sparse(pvApiCtx, _iDatasetId);
            break;
        }
        */
    case sci_void : //void item only on list variable
        {
            //pIT = import_void(_iDatasetId);
            break;
        }
    case sci_undefined : //undefined item only on list variable
        {
            pIT = import_undefined(_iDatasetId);
            break;
        }
    default :
        {
            Scierror(999,_("%s: Invalid HDF5 Scilab format.\n"), "import_from_hdf5");
#ifdef PRINT_DEBUG
            {
                char pstMsg[512];
                sprintf(pstMsg, "Unknown type : %d", iVarType);
                print_tree(pstMsg);
            }
#endif
            return NULL;
        }
    }

    return pIT;
}
/*
static types::InternalType* import_void(int _iDatasetId)
{

    if(_piAddress)
    {
        sciErr = createVoidInNamedList(pvApiCtx);
    }
    else
    {
        return NULL;
    }

    return pVoid;
}
*/
static types::InternalType* import_undefined(int _iDatasetId)
{
    return new types::ListUndefined();
}

static types::InternalType* import_double(int _iDatasetId)
{
    int iRet            = 0;
    int iComplex        = 0;
    double *pdblReal    = NULL;
    double *pdblImg     = NULL;
    int	iRows           = 0;
    int iCols           = 0;

    iRet        = getDatasetDims(_iDatasetId, &iRows, &iCols);
    iComplex    = isComplexData(_iDatasetId);
    if(iRet)
    {
        return NULL;
    }
    if(iRows * iCols != 0)
    {
        if(iComplex)
        {
            pdblReal	= (double *) MALLOC(iRows * iCols * sizeof(double));
            pdblImg		= (double *) MALLOC(iRows * iCols * sizeof(double));
            iRet        = readDoubleComplexMatrix(_iDatasetId, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            pdblReal	= (double *) MALLOC(iRows * iCols * sizeof(double));
            iRet        = readDoubleMatrix(_iDatasetId, iRows, iCols, pdblReal);
        }

        if(iRet)
        {
            return NULL;
        }
    }
    else
    {
        //bug 7224 : to close dataset
        iRet    = readEmptyMatrix(_iDatasetId);
        if(iRet)
        {
            return NULL;
        }
    }

    types::Double* pDbl = new types::Double(iRows, iCols, iComplex == 0 ? false : true);
    pDbl->set(pdblReal);
    if(pDbl->isComplex())
    {
        pDbl->setImg(pdblImg);
    }

    if(pdblReal)
    {
        FREE(pdblReal);
    }

    if(pdblImg)
    {
        FREE(pdblImg);
    }

    return pDbl;

}

static types::InternalType* import_string(int _iDatasetId)
{
    int iRet            = 0;
    int	iRows           = 0;
    int iCols           = 0;
    char **pstData      = NULL;
    wchar_t** pwstData   = NULL;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    if(iRet)
    {
        return NULL;
    }

    pstData = (char **) MALLOC(iRows * iCols * sizeof(char*));
    pwstData = (wchar_t **) MALLOC(iRows * iCols * sizeof(wchar_t*));

    iRet = readStringMatrix(_iDatasetId, iRows, iCols, pstData);
    if(iRet)
    {
        return NULL;
    }

    for(int i=0; i < iRows * iCols; i++)
    {
        pwstData[i] = to_wide_string(pstData[i]);
    }

    types::String* pStr = new types::String(iRows,iCols);
    pStr->set(pwstData);

    FREE(pstData);
    FREE(pwstData);
    return pStr;
}

static types::InternalType* import_integer( int _iDatasetId)
{
    int iRet    = 0;
    int	iRows   = 0;
    int iCols   = 0;
    int iPrec   = 0;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    if(iRet)
    {
        return NULL;
    }

    iRet = getDatasetPrecision(_iDatasetId, &iPrec);
    if(iRet)
    {
        return NULL;
    }

    switch(iPrec)
    {
    case sci_int8 :
        {
            char* pcData	= NULL;
            pcData = (char*)MALLOC(sizeof(char) * iRows * iCols);
            iRet = readInteger8Matrix(_iDatasetId, iRows, iCols, pcData);
            if(iRet)
            {
                return NULL;
            }
            types::Int8* pInt = new types::Int8(iRows,iCols);;
            pInt->set(pcData);
            return pInt;
        }
    case sci_uint8 :
        {
            unsigned char* pucData	= NULL;
            pucData = (unsigned char*)MALLOC(sizeof(unsigned char) * iRows * iCols);
            iRet = readUnsignedInteger8Matrix(_iDatasetId, iRows, iCols, pucData);
            if(iRet)
            {
                return NULL;
            }
            types::UInt8* pInt = new types::UInt8(iRows,iCols);;
            pInt->set(pucData);
            return pInt;
        }
    case sci_int16 :
        {
            short* psData	= NULL;
            psData = (short*)MALLOC(sizeof(short) * iRows * iCols);
            iRet = readInteger16Matrix(_iDatasetId, iRows, iCols, psData);
            if(iRet)
            {
                return NULL;
            }

            types::Int16* pInt = new types::Int16(iRows,iCols);
            pInt->set(psData);
            return pInt;
        }
    case sci_uint16 :
        {
            unsigned short* pusData	= NULL;
            pusData = (unsigned short*)MALLOC(sizeof(unsigned short) * iRows * iCols);
            iRet = readUnsignedInteger16Matrix(_iDatasetId, iRows, iCols, pusData);
            if(iRet)
            {
                return NULL;
            }

            types::UInt16* pInt = new types::UInt16(iRows,iCols);;
            pInt->set(pusData);
            return pInt;
        }
    case sci_int32 :
        {
            int* piData	= NULL;
            piData = (int*)MALLOC(sizeof(int) * iRows * iCols);
            iRet = readInteger32Matrix(_iDatasetId, iRows, iCols, piData);
            if(iRet)
            {
                return NULL;
            }

            types::Int32* pInt = new types::Int32(iRows,iCols);;
            pInt->set(piData);
            return pInt;
        }
    case sci_uint32 :
        {
            unsigned int* puiData	= NULL;
            puiData = (unsigned int*)MALLOC(sizeof(unsigned int) * iRows * iCols);
            iRet = readUnsignedInteger32Matrix(_iDatasetId, iRows, iCols, puiData);
            if(iRet)
            {
                return NULL;
            }

            types::UInt32* pInt = new types::UInt32(iRows,iCols);
            pInt->set(puiData);
            return pInt;
        }
    case sci_int64 :
        {
            long long* pllData	= NULL;
            pllData = (long long*)MALLOC(sizeof(long long) * iRows * iCols);
            iRet = readInteger64Matrix(_iDatasetId, iRows, iCols, pllData);
            if(iRet)
            {
                return NULL;
            }

            types::Int64* pInt = new types::Int64(iRows,iCols);
            pInt->set(pllData);
            return pInt;
        }
        break;
    case sci_uint64 :
        {
            unsigned long long* pullData	= NULL;
            pullData = (unsigned long long*)MALLOC(sizeof(unsigned long long) * iRows * iCols);
            iRet = readUnsignedInteger64Matrix(_iDatasetId, iRows, iCols, pullData);
            if(iRet)
            {
                return NULL;
            }

            types::UInt64* pInt = new types::UInt64(iRows,iCols);
            pInt->set(pullData);
            return pInt;
        }
        break;
    default :
        return NULL;
    }

    return NULL;
}

static types::InternalType* import_boolean(int _iDatasetId)
{
    int iRet        = 0;
    int* piData	    = NULL;
    int	iRows       = 0;
    int iCols       = 0;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    if(iRet)
    {
        return NULL;
    }

    if(iRows * iCols != 0)
    {
        piData  = (int *) MALLOC(iRows * iCols * sizeof(int));
        iRet    = readBooleanMatrix(_iDatasetId, iRows, iCols, piData);
        if(iRet)
        {
            return NULL;
        }
    }

    types::Bool* pBool = new types::Bool(iRows,iCols);
    pBool->set(piData);

    if(piData)
    {
        FREE(piData);
    }

    return pBool;
}

static types::InternalType* import_poly(int _iDatasetId)
{
    int iRet                = 0;
    int i                   = 0;
    int	iRows               = 0;
    int iCols               = 0;
    int iComplex            = 0;
    char pstVarName[64]	    = {0};
    wchar_t* pwstVarName    = NULL;
    double **pdblReal       = NULL;
    double **pdblImg        = NULL;
    int *piNbCoef           = NULL;

    iRet = getDatasetDims(_iDatasetId, &iRows, &iCols);
    if(iRet)
    {
        return NULL;
    }

    iComplex = isComplexData(_iDatasetId);

    if(iComplex)
    {
        piNbCoef    = (int*)MALLOC(iRows * iCols * sizeof(int));
        pdblReal    = (double**)MALLOC(iRows * iCols * sizeof(double*));
        pdblImg     = (double**)MALLOC(iRows * iCols * sizeof(double*));
        iRet        = readPolyComplexMatrix(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        piNbCoef    = (int*)MALLOC(iRows * iCols * sizeof(int));
        pdblReal    = (double**)MALLOC(iRows * iCols * sizeof(double*));
        iRet    	= readPolyMatrix(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal);
    }

    if(iRet)
    {
        return NULL;
    }

    pwstVarName = to_wide_string(pstVarName);
    types::Polynom* pPoly = new types::Polynom(pwstVarName,iRows,iCols,piNbCoef);

    for(int i = 0 ; i < iRows * iCols ; i++)
    {
        types::Double* pCoef = new types::Double(1, piNbCoef[i],  iComplex == 0 ? false : true);
        pCoef->set(pdblReal[i]);
        if(iComplex)
        {
            pCoef->setImg(pdblImg[i]);
        }
        pPoly->setCoef(i, pCoef);
    }

    for(i = 0 ; i < iRows * iCols ; i++)
    {
        FREE(pdblReal[i]);
        if(iComplex)
        {
            FREE(pdblImg[i]);
        }
    }

    FREE(pdblReal);
    if(iComplex)
    {
        FREE(pdblImg);
    }

    FREE(piNbCoef);

    if(iRet)
    {
        return NULL;
    }

    return pPoly;
}
/*
static types::InternalType* import_sparse(void* pvApiCtx, int _iDatasetId)
{
    int iRet						= 0;
    int	iRows						= 0;
    int iCols						= 0;
    int iComplex				= 0;
    double *pdblReal		= NULL;
    double *pdblImg			= NULL;
    int iNbItem					= 0;
    int* piNbItemRow		= NULL;
    int* piColPos				= NULL;
    SciErr sciErr;

    iRet								= getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if(iRet)
    {
        return false;
    }

    iComplex						= isComplexData(_iDatasetId);


    if(iComplex)
    {
        piNbItemRow				= (int*)MALLOC(iRows * sizeof(int));
        piColPos					= (int*)MALLOC(iNbItem * sizeof(int));
        pdblReal					= (double*)MALLOC(iNbItem * sizeof(double));
        pdblImg						= (double*)MALLOC(iNbItem * sizeof(double));
        iRet							= readSparseComplexMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
    }
    else
    {
        piNbItemRow				= (int*)MALLOC(iRows * sizeof(int));
        piColPos					= (int*)MALLOC(iNbItem * sizeof(int));
        pdblReal					= (double*)MALLOC(iNbItem * sizeof(double));
        iRet							= readSparseMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
    }

    if(iRet)
    {
        return false;
    }

    if(_piAddress == NULL)
    {
        if(iComplex)
        {
            sciErr = createNamedComplexSparseMatrix(pvApiCtx, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedSparseMatrix(pvApiCtx, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
        }
    }
    else //if not null this variable is in a list
    {
        if(iComplex)
        {
            sciErr = createComplexSparseMatrixInNamedList(pvApiCtx, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createSparseMatrixInNamedList(pvApiCtx, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
        }
    }

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];
    sprintf(pstMsg, "sparse_%d (%d x %d)", iRows, iCols);
    print_tree(pstMsg);
#endif

    FREE(piNbItemRow);
    FREE(piColPos);
    FREE(pdblReal);
    if(iComplex)
    {
        FREE(pdblImg);
    }


    if(iRet)
    {
        return false;
    }

    return true;
}

static types::InternalType* import_boolean_sparse(void* pvApiCtx, int _iDatasetId)
{
    int iRet						= 0;
    int	iRows						= 0;
    int iCols						= 0;
    int     types::Polynom* pPoly = NULL;iNbItem					= 0;
    int* piNbItemRow		= NULL;
    int* piColPos				= NULL;
    SciErr sciErr;

    iRet								= getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if(iRet)
    {
        return false;
    }


    piNbItemRow				= (int*)MALLOC(iRows * sizeof(int));
    piColPos					= (int*)MALLOC(iNbItem * sizeof(int));
    iRet							= readBooleanSparseMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    if(iRet)
    {
        return false;
    }

    if(_piAddress == NULL)
    {
        sciErr					= createNamedBooleanSparseMatrix(pvApiCtx, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    }
    else //if not null this variable is in a list
    {
        sciErr					= createBooleanSparseMatrixInNamedList(pvApiCtx, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    }

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];
    sprintf(pstMsg, "boolean sparse_%d (%d x %d)", iRows, iCols);
    print_tree(pstMsg);
#endif

    FREE(piNbItemRow);
    FREE(piColPos);


    if(iRet)
    {
        return false;
    }

    return true;
}
*/
static types::InternalType* import_list(int _iDatasetId, int _iVarType)
{
    int iRet                = 0;
    int i                   = 0;
    int	iItems              = 0;
    hobj_ref_t* piItemRef   = NULL;

    iRet = getListDims(_iDatasetId, &iItems);
    if(iRet)
    {
        return NULL;
    }


    if(iItems == 0)
    {//special case for empty list
    }
    else
    {
        iRet = getListItemReferences(_iDatasetId, &piItemRef);
        if(iRet)
        {
            return NULL;
        }
    }

    types::List* pList  = NULL;

    switch(_iVarType)
    {
    case sci_list :
    {
        pList = new types::List();
        break;
    }
    case sci_tlist :
    {
        pList = new types::TList();
        break;
    }
    case sci_mlist :
    {
        pList = new types::MList();
        break;
    }
    default :
        return NULL;
    }

    iTab++;
    for(i = 0 ; i < iItems ; i++)
    {
        int iItemDataset = 0;
        iRet = getListItemDataset(_iDatasetId, piItemRef, i, &iItemDataset);
        if(iRet || iItemDataset == 0)
        {
            return NULL;
        }

        types::InternalType* pItem = import_data(iItemDataset);
        if(pItem == NULL)
        {
            return NULL;
        }
        pList->append(pItem);
    }
    iTab--;

    iRet = deleteListItemReferences(_iDatasetId, piItemRef);
    if(iRet)
    {
        return NULL;
    }

    return pList;
}

void print_tree(char* _pstMsg)
{
#ifdef PRINT_DEBUG
    for(int i = 0 ; i < iTab ; i++)
    {
        printf("\t");
    }
    printf("%s\n", _pstMsg);
#endif
}

/*--------------------------------------------------------------------------*/
