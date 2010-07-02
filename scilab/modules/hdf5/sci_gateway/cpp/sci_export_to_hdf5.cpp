/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_hdf5.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "scilabmode.h"
#include "splitpath.h"
#include "scicurdir.h"
}
#include "forceJHDF5load.hxx"

//#define PRINT_DEBUG
int iLevel = 0;

static bool export_data(int _iH5File, int *_piVar, char* _pstName);
static bool export_list(int _iH5File, int *_piVar, char* _pstName, int _iVarType);
static bool export_double(int _iH5File, int *_piVar, char* _pstName);
static bool export_poly(int _iH5File, int *_piVar, char* _pstName);
static bool export_boolean(int _iH5File, int *_piVar, char* _pstName);
static bool export_sparse(int _iH5File, int *_piVar, char* _pstName);
static bool export_boolean_sparse(int _iH5File, int *_piVar, char* _pstName);
static bool export_matlab_sparse(int *_piVar, char* _pstName);
static bool export_ints(int _iH5File, int *_piVar, char* _pstName);
static bool export_handles(int *_piVar, char* _pstName);
static bool export_strings(int _iH5File, int *_piVar, char* _pstName);
static bool export_u_function(int *_piVar, char* _pstName);
static bool export_c_function(int *_piVar, char* _pstName);
static bool export_lib(int *_piVar, char* _pstName);
static bool export_lufact_pointer(int *_piVar, char* _pstName);

void print_type(char* _pstType);
int extractVarNameList(int _iStart, int _iEnd, char** _pstNameList);

static char fname[]			= "export_to_hdf5";
/*--------------------------------------------------------------------------*/
int sci_export_to_hdf5(char *fname,unsigned long fname_len)
{
    int iRet            = 0;
    int iNbVar          = 0;
    int iLen            = 0;
    int** piAddrList    = NULL;
    char* pstFilename   = NULL;
    char** pstNameList	= NULL;
    bool bExport        = false;

    SciErr sciErr;

    CheckLhs(1,1);//output parameter

#ifndef _MSC_VER
    forceJHDF5load();
#endif

    /*get input data*/
    if(Rhs < 2)
    {
        Scierror(999,_("%s: Wrong number of input argument(s): At most %d expected.\n"), fname, 2);
        return 0;
    }

    pstNameList = (char**)MALLOC(sizeof(char*) * Rhs);
    iNbVar = extractVarNameList(1, Rhs, pstNameList);
    if(iNbVar == 0)
    {
        FREE(pstNameList);
        return 0;
    }

    piAddrList = (int**)MALLOC(sizeof(int*) * (iNbVar - 1));
    for(int i = 0 ; i < Rhs - 1 ; i++)
    {
        sciErr = getVarAddressFromName(pvApiCtx, pstNameList[i + 1], &piAddrList[i]);
        if(sciErr.iErr)
        {
            // i+1 = i for filename + 1 because starting arg number is 1 for human being
            Scierror(999,_("%s: Wrong value for input argument #%d: Defined variable expected.\n"), fname, i + 2);
            printError(&sciErr, 0);
            return 0;
        }
    }

    iLevel = 0;
    // open hdf5 file
    int iH5File = createHDF5File(pstNameList[0]);

    if(iH5File < 0)
    {
        if(iH5File == -2)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" is a directory"), fname, 1, pstNameList[0]);
        }
        else
        {
            Scierror(999,_("%s: Cannot open file %s.\n"), fname, pstNameList[0]);
        }

        return 0;
    }

    // export data
    for(int i = 0 ; i < Rhs - 1; i++)
    {
        bExport = export_data(iH5File, piAddrList[i], pstNameList[i + 1]);
        if(bExport == false)
        {
            break;
        }
    }

    //close hdf5 file
    closeHDF5File(iH5File);

    //create boolean return value
    int *piReturn = NULL;
    sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, 1, &piReturn);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if(bExport == true)
    {
        piReturn[0] = 1;
    }
    else
    {
        piReturn[0] = 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

static bool export_data(int _iH5File, int* _piVar, char* _pstName)
{
    bool bReturn = false;
    int iType = 0;

    SciErr sciErr = getVarType(pvApiCtx, _piVar, &iType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    switch(iType)
    {
    case sci_matrix :
        {
            bReturn = export_double(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_poly :
        {
            bReturn = export_poly(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_boolean :
        {
            bReturn = export_boolean(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_sparse :
        {
            bReturn = export_sparse(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_boolean_sparse :
        {
            bReturn = export_boolean_sparse(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_matlab_sparse :
        {
            bReturn = export_matlab_sparse(_piVar, _pstName);
            break;
        }
    case sci_ints :
        {
            bReturn = export_ints(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_handles :
        {
            bReturn = export_handles(_piVar, _pstName);
            break;
        }
    case sci_strings :
        {
            bReturn = export_strings(_iH5File, _piVar, _pstName);
            break;
        }
    case sci_u_function :
        {
            bReturn = export_u_function(_piVar, _pstName);
            break;
        }
    case sci_c_function :
        {
            bReturn = export_c_function(_piVar, _pstName);
            break;
        }
    case sci_lib :
        {
            bReturn = export_lib(_piVar, _pstName);
            break;
        }
    case sci_list :
    case sci_tlist :
    case sci_mlist :
        {
            bReturn = export_list(_iH5File, _piVar, _pstName, iType);
            break;
        }
    case sci_lufact_pointer :
        {
            bReturn = export_lufact_pointer(_piVar, _pstName);
            break;
        }
    }
    return bReturn;
}

static bool export_list(int _iH5File, int *_piVar, char* _pstName, int _iVarType)
{
    int iRet = 0;
    bool bReturn = false;
    int iItemNumber = 0;
    SciErr sciErr = getListItemNumber(pvApiCtx, _piVar, &iItemNumber);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }


    //create groupe name
    char* pstGroupName	= createGroupName(_pstName);

    char pstMsg[256];
    sprintf(pstMsg, "list (%d)", iItemNumber);
    print_type(pstMsg);

    iLevel++;
    //open list
    void *pvList = openList(_iH5File, pstGroupName, iItemNumber);
    for(int i = 0 ; i < iItemNumber ; i++)
    {
        int *piNewVar = NULL;
        getListItemAddress(pvApiCtx, _piVar, i + 1, &piNewVar);//1 indexed
        if(piNewVar == NULL)
        {
            Scierror(999,_("%s: Item not found.\n"), fname);
            return 0;
        }

        char* pstPathName		= createPathName(pstGroupName, i);
        bReturn							= export_data(_iH5File, piNewVar, pstPathName);
        iRet = addItemInList(_iH5File, pvList, i, pstPathName);

        FREE(pstPathName);
        if(bReturn == false || iRet)
            return false;
    }
    iLevel--;
    closeList(_iH5File, pvList, _pstName, iItemNumber, _iVarType);
    FREE(pstGroupName);
    //close list
    return true;
}

static bool export_double(int _iH5File, int *_piVar, char* _pstName)
{
    int iRet					= 0;
    int iComplex			= isVarComplex(pvApiCtx, _piVar);
    int iRows					= 0;
    int iCols					= 0;
    int iType = 0;
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    SciErr sciErr = getVarType(pvApiCtx, _piVar, &iType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    if(iType != sci_matrix)
    {
        return false;
    }

    if(iComplex)
    {
        sciErr = getComplexMatrixOfDouble(pvApiCtx, _piVar, &iRows, &iCols, &pdblReal, &pdblImg);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeDoubleComplexMatrix(_iH5File, _pstName, iRows, iCols, pdblReal, pdblImg);
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, _piVar, &iRows, &iCols, &pdblReal);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writeDoubleMatrix(_iH5File, _pstName, iRows, iCols, pdblReal);
    }

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "double (%d x %d)", iRows, iCols);
    print_type(pstMsg);
    return true;
}

static bool export_poly(int _iH5File, int *_piVar, char* _pstName)
{
    int iRet						= 0;
    int iRows						= 0;
    int iCols						= 0;
    int* piNbCoef				= NULL;
    double** pdblReal		= NULL;
    double** pdblImg		= NULL;
    char pstVarName[64]	= {0};
    int iVarNameLen			= 0;

    SciErr sciErr = getPolyVariableName(pvApiCtx, _piVar, pstVarName, &iVarNameLen);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    if(isVarComplex(pvApiCtx, _piVar))
    {
        sciErr = getComplexMatrixOfPoly(pvApiCtx, _piVar, &iRows, &iCols, NULL, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        piNbCoef = (int*)MALLOC(iRows * iCols * sizeof(int));
        sciErr = getComplexMatrixOfPoly(pvApiCtx, _piVar, &iRows, &iCols, piNbCoef, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        pdblReal = (double**)MALLOC(sizeof(double*) * iRows * iCols);
        pdblImg = (double**)MALLOC(sizeof(double*) * iRows * iCols);
        for(int i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
            pdblImg[i]	= (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
        }
        sciErr = getComplexMatrixOfPoly(pvApiCtx, _piVar, &iRows, &iCols, piNbCoef, pdblReal, pdblImg);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writePolyComplexMatrix(_iH5File, _pstName, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        sciErr = getMatrixOfPoly(pvApiCtx, _piVar, &iRows, &iCols, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        piNbCoef = (int*)MALLOC(iRows * iCols * sizeof(int));
        sciErr = getMatrixOfPoly(pvApiCtx, _piVar, &iRows, &iCols, piNbCoef, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        pdblReal = (double**)MALLOC(sizeof(double*) * iRows * iCols);
        for(int i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
        }
        sciErr = getMatrixOfPoly(pvApiCtx, _piVar, &iRows, &iCols, piNbCoef, pdblReal);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }

        iRet = writePolyMatrix(_iH5File, _pstName, pstVarName, iRows, iCols, piNbCoef, pdblReal);
    }

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "poly (%d x %d)", iRows, iCols);
    print_type(pstMsg);

    if(pdblReal)
    {
        for(int i = 0 ; i < iRows * iCols ; i++)
        {
            FREE(pdblReal[i]);
        }
        FREE(pdblReal);
    }

    if(pdblImg)
    {
        for(int i = 0 ; i < iRows * iCols ; i++)
        {
            FREE(pdblImg[i]);
        }
        FREE(pdblImg);
    }

    FREE(piNbCoef);
    return true;
}

static bool export_boolean(int _iH5File, int *_piVar, char* _pstName)
{
    int iRows					= 0;
    int iCols					= 0;
    int *piData				= NULL;

    //for error management
    SciErr sciErr = getMatrixOfBoolean(pvApiCtx, _piVar, &iRows, &iCols, &piData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int iRet = writeBooleanMatrix(_iH5File, _pstName, iRows, iCols, piData);
    if(iRet)
    {
        //Msg ??
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "bool (%d x %d)", iRows, iCols);
    print_type(pstMsg);
    return true;
}

static bool export_boolean_sparse(int _iH5File, int *_piVar, char* _pstName)
{
    int iRet						= 0;
    int iRows						= 0;
    int iCols						= 0;
    int iNbItem					= 0;
    int* piNbCoef				= NULL;
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

static bool export_sparse(int _iH5File, int *_piVar, char* _pstName)
{
    int iRet						= 0;
    int iRows						= 0;
    int iCols						= 0;
    int iNbItem					= 0;
    int* piNbCoef				= NULL;
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

static bool export_matlab_sparse(int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return true;
}

static bool export_ints(int _iH5File, int *_piVar, char* _pstName)
{
    int iRet					= 0;
    int iRows					= 0;
    int iCols					= 0;
    void *piData			= NULL;
    int iPrec					= 0;

    SciErr sciErr = getMatrixOfIntegerPrecision(pvApiCtx, _piVar, &iPrec);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    switch(iPrec)
    {
    case SCI_INT8 :
        sciErr = getMatrixOfInteger8(pvApiCtx, _piVar, &iRows, &iCols, (char**)&piData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }
        iRet = writeInterger8Matrix(_iH5File, _pstName, iRows, iCols, (char*)piData);
        break;
    case SCI_UINT8 :
        sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, _piVar, &iRows, &iCols, (unsigned char**)&piData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }
        iRet = writeUnsignedInterger8Matrix(_iH5File, _pstName, iRows, iCols, (unsigned char*)piData);
        break;
    case SCI_INT16 : 
        sciErr = getMatrixOfInteger16(pvApiCtx, _piVar, &iRows, &iCols, (short**)&piData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }
        iRet = writeInterger16Matrix(_iH5File, _pstName, iRows, iCols, (short*)piData);
        break;
    case SCI_UINT16 :
        sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, _piVar, &iRows, &iCols, (unsigned short**)&piData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }
        iRet = writeUnsignedInterger16Matrix(_iH5File, _pstName, iRows, iCols, (unsigned short*)piData);
        break;
    case SCI_INT32 : 
        sciErr = getMatrixOfInteger32(pvApiCtx, _piVar, &iRows, &iCols, (int**)&piData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }
        iRet = writeInterger32Matrix(_iH5File, _pstName, iRows, iCols, (int*)piData);
        break;
    case SCI_UINT32 :
        sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, _piVar, &iRows, &iCols, (unsigned int**)&piData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return false;
        }
        iRet = writeUnsignedInterger32Matrix(_iH5File, _pstName, iRows, iCols, (unsigned int*)piData);
        break;
    case SCI_INT64 : 
        //sciErr = getMatrixOfInteger64(_piVar, &iRows, &iCols, (long long**)&piData);
        //if(sciErr.iErr)
        //{
        //	printError(&sciErr, 0);
        //	return false;
        //}
        //iRet = writeInterger64Matrix(_iH5File, _pstName, iRows, iCols, (long long*)piData);
        //break;
    case SCI_UINT64 : 
        //sciErr = getMatrixOfUnsignedInteger64(_piVar, &iRows, &iCols, (unsigned long long**)&piData);
        //if(sciErr.iErr)
        //{
        //	printError(&sciErr, 0);
        //	return false;
        //}
        //iRet = writeUnsignedInterger64Matrix(_iH5File, _pstName, iRows, iCols, (unsigned long long*)piData);
        //break;
    default : 
        return 1;
        break;
    }

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "int%d (%d x %d)", 8*iPrec, iRows, iCols);
    print_type(pstMsg);
    return true;
}

static bool export_handles(int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return true;
}

static bool export_strings(int _iH5File, int *_piVar, char* _pstName)
{
    int iRet				= 0;
    int iRows				= 0;
    int iCols				= 0;
    int* piLen			= NULL;
    char** pstData	= NULL;


    SciErr sciErr = getMatrixOfString(pvApiCtx, _piVar, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    piLen = (int*)MALLOC(iRows * iCols * sizeof(int));
    sciErr = getMatrixOfString(pvApiCtx, _piVar, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    pstData = (char**)MALLOC(sizeof(char*) * iRows * iCols);
    for(int i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));// for null termination
    }
    sciErr = getMatrixOfString(pvApiCtx, _piVar, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    iRet = writeStringMatrix(_iH5File, _pstName, iRows, iCols, pstData);

    if(iRet)
    {
        return false;
    }

    char pstMsg[512];
    sprintf(pstMsg, "string (%d x %d)", iRows, iCols);
    print_type(pstMsg);

    freeArrayOfString(pstData, iRows * iCols);
    return true;
}

static bool export_u_function(int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return true;
}

static bool export_c_function(int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return true;
}

static bool export_lib(int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return true;
}

static bool export_lufact_pointer(int *_piVar, char* _pstName)
{
    print_type(_pstName);
    return true;
}

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

int extractVarNameList(int _iStart, int _iEnd, char** _pstNameList)
{
    int iCount = 0;

    for(int i = _iStart ; i <= _iEnd ; i++)
    {
        int iRows					= 0;
        int iCols					= 0;
        int iLen					= 0;
        int* piAddr				= NULL;
        int iType					= 0;

        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        //get filename
        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if(iType != sci_strings)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), "export_to_hdf5", i);
            return 0;
        }

        sciErr = getVarDimension(pvApiCtx, piAddr, &iRows, &iCols);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if(iRows != 1 || iCols != 1)
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), "export_to_hdf5", i);
            Scierror(999,_("%s: Type: %dx%d\n"), "export_to_hdf5", iRows, iCols);
            return 0;
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        _pstNameList[iCount] = (char*)MALLOC((iLen + 1) * sizeof(char));//+1 for null termination
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, &_pstNameList[iCount]);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        iCount++;
    }
    return iCount;
}

/*--------------------------------------------------------------------------*/
