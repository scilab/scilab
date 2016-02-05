/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#include <hdf5.h>
#include "context.hxx"
#include "list.hxx"

extern "C"
{
#include <string.h>
#include "gw_hdf5.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile_v1.h"
#include "h5_attributeConstants.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
}

//#define PRINT_DEBUG
//#define TIME_DEBUG

static int iTab = 0;
static int iCloseList = 0;

void print_tree_v1(char *_pstMsg);

static bool import_variable_v1(int* pvCtx, int _iFile, char* _pstVarName);
static bool import_data_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_double_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_string_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_boolean_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_integer_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_boolean_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_poly_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_list_v1(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_hypermat_v1(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_void_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_undefined_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);

static const std::string fname("load");

int sci_hdf5_load_v1(char *fn, int* pvApiCtx)
{
    SciErr sciErr;

    int* piAddr = NULL;
    char* pstFilename = NULL;
    char* pstExpandedFilename = NULL;
    bool bImport = true;

    const int nbIn = Rhs;
    int iSelectedVar = Rhs - 1;

    CheckInputArgumentAtLeast(pvApiCtx, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    iCloseList = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFilename))
    {
        if (pstFilename)
        {
            FREE(pstFilename);
        }

        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname.data(), 2);
        return 1;
    }

    //open hdf5 file
    pstExpandedFilename = expandPathVariable(pstFilename);
    int iFile = openHDF5File(pstExpandedFilename, 0);
    if (iFile < 0)
    {
        FREE(pstExpandedFilename);
        Scierror(999, _("%s: Unable to open file: %s\n"), fname.data(), pstFilename);
        FREE(pstFilename);
        return 1;
    }

    FREE(pstExpandedFilename);
    FREE(pstFilename);

    std::vector<wchar_t*> varList;
    if (iSelectedVar)
    {
        //selected variable
        char* pstVarName = NULL;
        for (int i = 0 ; i < iSelectedVar ; i++)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, i + 2, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddr, &pstVarName))
            {
                if (pstVarName)
                {
                    FREE(pstVarName);
                }

                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname.data(), i + 1);
                return 1;
            }

            if (import_variable_v1(pvApiCtx, iFile, pstVarName) == false)
            {
                FREE(pstVarName);
                bImport = false;
                break;
            }

            varList.push_back(to_wide_string(pstVarName));
            FREE(pstVarName);
            pstVarName = NULL;
        }
    }
    else
    {
        //all variables
        int iNbItem = 0;
        iNbItem = getVariableNames_v1(iFile, NULL);
        if (iNbItem != 0)
        {
            char **pstVarNameList = (char **)MALLOC(sizeof(char *) * iNbItem);

            iNbItem = getVariableNames_v1(iFile, pstVarNameList);

            //import all data
            for (int i = 0; i < iNbItem; i++)
            {
                if (import_variable_v1(pvApiCtx, iFile, pstVarNameList[i]) == false)
                {
                    bImport = false;
                    break;
                }

                varList.push_back(to_wide_string(pstVarNameList[i]));
            }

            freeArrayOfString(pstVarNameList, iNbItem);
        }
    }
    //close the file
    closeHDF5File(iFile);

    if (bImport == true && varList.size() != 0)
    {
        createMatrixOfWideString(pvApiCtx, nbIn + 1, 1, static_cast<int>(varList.size()), varList.data());
    }
    else
    {
        createEmptyMatrix(pvApiCtx, nbIn + 1);
    }

    for (auto & i : varList)
    {
        FREE(i);
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    //  printf("End gateway !!!\n");
    return 0;
}

static bool import_variable_v1(int* pvCtx, int _iFile, char* _pstVarName)
{
    int iDataSetId = getDataSetIdFromName_v1(_iFile, _pstVarName);
    if (iDataSetId == 0)
    {
        return false;
    }

    return import_data_v1(pvCtx, iDataSetId, 0, NULL, _pstVarName);
}

static bool import_data_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    bool bRet = false;

    //get var type
    int iVarType = getScilabTypeFromDataSet_v1(_iDatasetId);

    switch (iVarType)
    {
        case sci_matrix:
        {
            bRet = import_double_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_strings:
        {
            bRet = import_string_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_list:
        case sci_tlist:
        case sci_mlist:
        {
            bRet = import_hypermat_v1(pvCtx, _iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            if (bRet == false)
            {
                bRet = import_list_v1(pvCtx, _iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            }
            break;
        }
        case sci_boolean:
        {
            bRet = import_boolean_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_poly:
        {
            bRet = import_poly_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_ints:
        {
            bRet = import_integer_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_sparse:
        {
            bRet = import_sparse_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_boolean_sparse:
        {
            bRet = import_boolean_sparse_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_void:             //void item only on list variable
        {
            bRet = import_void_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_undefined:        //undefined item only on list variable
        {
            bRet = import_undefined_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        default:
        {
            Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), fname.data());
#ifdef PRINT_DEBUG
            {
                char pstMsg[512];

                sprintf(pstMsg, "Unknown type : %d", iVarType);
                print_tree(pstMsg);
            }
#endif
        }
    }

    return bRet;
}

static bool import_void_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    SciErr sciErr;

    if (_piAddress)
    {
        sciErr = createVoidInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos);
    }
    else
    {
        return false;
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }
    return true;
}

static bool import_undefined_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    SciErr sciErr;

    if (_piAddress)
    {
        sciErr = createUndefinedInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos);
    }
    else
    {
        return false;
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }
    return true;
}

static bool import_double_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    double *pdblReal = NULL;
    double *pdblImg = NULL;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;
    SciErr sciErr;

#ifdef TIME_DEBUG
    LARGE_INTEGER iStart, iEnd, iFreq;

    QueryPerformanceFrequency(&iFreq);
    QueryPerformanceCounter(&iStart);
#endif

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    iComplex = isComplexData_v1(_iDatasetId);
    if (iRet)
    {
        return false;
    }

    if (iRows * iCols != 0)
    {
        if (iComplex)
        {
            pdblReal = (double *)MALLOC(iRows * iCols * sizeof(double));
            pdblImg = (double *)MALLOC(iRows * iCols * sizeof(double));
            iRet = readDoubleComplexMatrix_v1(_iDatasetId, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            pdblReal = (double *)MALLOC(iRows * iCols * sizeof(double));
            iRet = readDoubleMatrix_v1(_iDatasetId, iRows, iCols, pdblReal);
        }

        if (iRet)
        {
            FREE(pdblReal);
            if (iComplex)
            {
                FREE(pdblImg);
            }

            return false;
        }
    }
    else
    {
        /*bug 7224 : to close dataset */
        iRet = readEmptyMatrix_v1(_iDatasetId);
        if (iRet)
        {
            return false;
        }

        pdblReal = (double*)MALLOC(sizeof(double) * 1);
        pdblReal[0] = 0;
        iComplex = 0;
    }

    if (_piAddress == NULL)
    {
        if (iComplex)
        {
            sciErr = createNamedComplexMatrixOfDouble(pvCtx, _pstVarname, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfDouble(pvCtx, _pstVarname, iRows, iCols, pdblReal);
        }
    }
    else                        //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr = createComplexMatrixOfDoubleInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createMatrixOfDoubleInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pdblReal);
        }
    }

    FREE(pdblReal);
    if (iComplex)
    {
        FREE(pdblImg);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "double_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iEnd);
    double dblTime = ((iEnd.QuadPart - iStart.QuadPart) * 1000.0) / iFreq.QuadPart;

    printf("Total Double : %0.3f ms\n\n", dblTime);
#endif

    return true;
}

static bool import_string_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int i = 0;
    int iRows = 0;
    int iCols = 0;
    char **pstData = NULL;
    SciErr sciErr;

#ifdef TIME_DEBUG
    LARGE_INTEGER iStart1, iEnd1, iStart2, iEnd2, iStart3, iEnd3, iFreq;

    QueryPerformanceFrequency(&iFreq);
    QueryPerformanceCounter(&iStart1);
#endif
    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    if (iRet)
    {
        return false;
    }

    pstData = (char **)MALLOC(iRows * iCols * sizeof(char *));
    memset(pstData, 0x00, iRows * iCols * sizeof(char *));

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iStart1);
#endif

    iRet = readStringMatrix_v1(_iDatasetId, iRows, iCols, pstData);
    if (iRet)
    {
        freeArrayOfString(pstData, iRows * iCols);
        return false;
    }

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iEnd1);
#endif
#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iStart2);
#endif

    if (_piAddress == NULL)
    {
        sciErr = createNamedMatrixOfString(pvCtx, _pstVarname, iRows, iCols, pstData);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createMatrixOfStringInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pstData);
    }

    freeArrayOfString(pstData, iRows * iCols);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iEnd2);
    QueryPerformanceCounter(&iStart3);
#endif
#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "string_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&iEnd3);

    //double dblTime    =((iEnd1.QuadPart - iStart1.QuadPart) * 1000.0) / iFreq.QuadPart;
    //printf("HDF5 : %0.3f ms\n", dblTime);
    //dblTime   =((iEnd2.QuadPart - iStart2.QuadPart) * 1000.0) / iFreq.QuadPart;
    //printf("Stack : %0.3f ms\n", dblTime);
    //dblTime   =((iEnd3.QuadPart - iStart3.QuadPart) * 1000.0) / iFreq.QuadPart;
    //printf("Clear : %0.3f ms\n", dblTime);
    double dblTime = ((iEnd3.QuadPart - iStart1.QuadPart) * 1000.0) / iFreq.QuadPart;

    printf("Total String: %0.3f ms\n\n", dblTime);
#endif
    return true;
}

static bool import_integer_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iPrec = 0;
    SciErr sciErr;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    if (iRet)
    {
        return false;
    }

    iRet = getDatasetPrecision_v1(_iDatasetId, &iPrec);
    if (iRet)
    {
        return false;
    }

    switch (iPrec)
    {
        case SCI_INT8:
        {
            char *pcData = NULL;

            pcData = (char *)MALLOC(sizeof(char) * iRows * iCols);
            iRet = readInteger8Matrix_v1(_iDatasetId, iRows, iCols, pcData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger8(pvCtx, _pstVarname, iRows, iCols, pcData);
            }
            else
            {
                sciErr = createMatrixOfInteger8InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pcData);
            }

            FREE(pcData);
        }
        break;
        case SCI_UINT8:
        {
            unsigned char *pucData = NULL;

            pucData = (unsigned char *)MALLOC(sizeof(unsigned char) * iRows * iCols);
            iRet = readUnsignedInteger8Matrix_v1(_iDatasetId, iRows, iCols, pucData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger8(pvCtx, _pstVarname, iRows, iCols, pucData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger8InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pucData);
            }

            FREE(pucData);
        }
        break;
        case SCI_INT16:
        {
            short *psData = NULL;

            psData = (short *)MALLOC(sizeof(short) * iRows * iCols);
            iRet = readInteger16Matrix_v1(_iDatasetId, iRows, iCols, psData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger16(pvCtx, _pstVarname, iRows, iCols, psData);
            }
            else
            {
                sciErr = createMatrixOfInteger16InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, psData);
            }

            FREE(psData);
        }
        break;
        case SCI_UINT16:
        {
            unsigned short *pusData = NULL;

            pusData = (unsigned short *)MALLOC(sizeof(unsigned short) * iRows * iCols);
            iRet = readUnsignedInteger16Matrix_v1(_iDatasetId, iRows, iCols, pusData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger16(pvCtx, _pstVarname, iRows, iCols, pusData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger16InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pusData);
            }

            FREE(pusData);
        }
        break;
        case SCI_INT32:
        {
            int *piData = NULL;

            piData = (int *)MALLOC(sizeof(int) * iRows * iCols);
            iRet = readInteger32Matrix_v1(_iDatasetId, iRows, iCols, piData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger32(pvCtx, _pstVarname, iRows, iCols, piData);
            }
            else
            {
                sciErr = createMatrixOfInteger32InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, piData);
            }

            FREE(piData);
        }
        break;
        case SCI_UINT32:
        {
            unsigned int *puiData = NULL;

            puiData = (unsigned int *)MALLOC(sizeof(unsigned int) * iRows * iCols);
            iRet = readUnsignedInteger32Matrix_v1(_iDatasetId, iRows, iCols, puiData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger32(pvCtx, _pstVarname, iRows, iCols, puiData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger32InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, puiData);
            }

            FREE(puiData);
        }
        break;
        case SCI_INT64:
        {
#ifdef __SCILAB_INT64__
            long long *pllData = NULL;

            pllData = (long long *)MALLOC(sizeof(long long) * iRows * iCols);
            iRet = readInteger64Matrix_v1(_iDatasetId, iRows, iCols, pllData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger64(pvCtx, _pstVarname, iRows, iCols, pllData);
            }
            else
            {
                sciErr = createMatrixOfInteger64InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pllData);
            }

            FREE(pllData);
#else
            return false;
#endif
        }
        break;
        case SCI_UINT64:
        {
#ifdef __SCILAB_INT64__
            unsigned long long *pullData = NULL;

            pullData = (unsigned long long *)MALLOC(sizeof(unsigned long long) * iRows * iCols);
            iRet = readUnsignedInteger64Matrix_v1(_iDatasetId, iRows, iCols, pullData);
            if (iRet)
            {
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger64(pvCtx, _pstVarname, iRows, iCols, pullData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger64InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pullData);
            }

            FREE(pullData);
#else
            return false;
#endif
        }
        break;
        default:
            return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "integer_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    return true;
}

static bool import_boolean_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int *piData = NULL;
    int iRows = 0;
    int iCols = 0;
    SciErr sciErr;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    if (iRet)
    {
        return false;
    }

    if (iRows * iCols == 0)
    {
        return false;
    }
    piData = (int *)MALLOC(iRows * iCols * sizeof(int));
    iRet = readBooleanMatrix_v1(_iDatasetId, iRows, iCols, piData);
    if (iRet)
    {
        FREE(piData);
        return false;
    }

    if (_piAddress == NULL)
    {
        sciErr = createNamedMatrixOfBoolean(pvCtx, _pstVarname, iRows, iCols, piData);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createMatrixOfBooleanInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, piData);
    }

    FREE(piData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "boolean_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif
    return true;
}

static bool import_poly_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;
    char pstVarName[64] = { 0 };
    double **pdblReal = NULL;
    double **pdblImg = NULL;
    int *piNbCoef = NULL;
    SciErr sciErr;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData_v1(_iDatasetId);

    if (iComplex)
    {
        piNbCoef = (int *)MALLOC(iRows * iCols * sizeof(int));
        pdblReal = (double **)MALLOC(iRows * iCols * sizeof(double *));
        pdblImg = (double **)MALLOC(iRows * iCols * sizeof(double *));
        iRet = readPolyComplexMatrix_v1(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        piNbCoef = (int *)MALLOC(iRows * iCols * sizeof(int));
        pdblReal = (double **)MALLOC(iRows * iCols * sizeof(double *));
        iRet = readPolyMatrix_v1(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal);
    }

    if (iRet)
    {
        FREE(piNbCoef);
        for (int i = 0; i < iRows * iCols; i++)
        {
            FREE(pdblReal[i]);
        }

        FREE(pdblReal);

        if (iComplex)
        {
            for (int i = 0; i < iRows * iCols; i++)
            {
                FREE(pdblImg[i]);
            }

            FREE(pdblImg);
        }

        return false;
    }

    if (_piAddress == NULL)
    {
        if (iComplex)
        {
            sciErr = createNamedComplexMatrixOfPoly(pvCtx, _pstVarname, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfPoly(pvCtx, _pstVarname, pstVarName, iRows, iCols, piNbCoef, pdblReal);
        }
    }
    else                        //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr =
                createComplexMatrixOfPolyInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, iRows, iCols, piNbCoef, pdblReal,
                        pdblImg);
        }
        else
        {
            sciErr = createMatrixOfPolyInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, iRows, iCols, piNbCoef, pdblReal);
        }
    }

    FREE(piNbCoef);
    for (int i = 0; i < iRows * iCols; i++)
    {
        FREE(pdblReal[i]);
    }

    FREE(pdblReal);

    if (iComplex)
    {
        for (int i = 0; i < iRows * iCols; i++)
        {
            FREE(pdblImg[i]);
        }

        FREE(pdblImg);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "poly_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif

    return true;
}

static bool import_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;
    double *pdblReal = NULL;
    double *pdblImg = NULL;
    int iNbItem = 0;
    int *piNbItemRow = NULL;
    int *piColPos = NULL;
    SciErr sciErr;

    iRet = getSparseDimension_v1(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData_v1(_iDatasetId);

    if (iComplex)
    {
        piNbItemRow = (int *)MALLOC(iRows * sizeof(int));
        piColPos = (int *)MALLOC(iNbItem * sizeof(int));
        pdblReal = (double *)MALLOC(iNbItem * sizeof(double));
        pdblImg = (double *)MALLOC(iNbItem * sizeof(double));
        iRet = readSparseComplexMatrix_v1(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
    }
    else
    {
        piNbItemRow = (int *)MALLOC(iRows * sizeof(int));
        piColPos = (int *)MALLOC(iNbItem * sizeof(int));
        pdblReal = (double *)MALLOC(iNbItem * sizeof(double));
        iRet = readSparseMatrix_v1(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
    }

    if (iRet)
    {
        FREE(piNbItemRow);
        FREE(piColPos);
        FREE(pdblReal);
        if (iComplex)
        {
            FREE(pdblImg);
        }

        return false;
    }

    if (_piAddress == NULL)
    {
        if (iComplex)
        {
            sciErr = createNamedComplexSparseMatrix(pvCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedSparseMatrix(pvCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
        }
    }
    else                        //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr =
                createComplexSparseMatrixInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos,
                        pdblReal, pdblImg);
        }
        else
        {
            sciErr =
                createSparseMatrixInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
        }
    }

    FREE(piNbItemRow);
    FREE(piColPos);
    FREE(pdblReal);
    if (iComplex)
    {
        FREE(pdblImg);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "sparse_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif
    return true;
}

static bool import_boolean_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int *piNbItemRow = NULL;
    int *piColPos = NULL;
    SciErr sciErr;

    iRet = getSparseDimension_v1(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    piNbItemRow = (int *)MALLOC(iRows * sizeof(int));
    piColPos = (int *)MALLOC(iNbItem * sizeof(int));
    iRet = readBooleanSparseMatrix_v1(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    if (iRet)
    {
        FREE(piNbItemRow);
        FREE(piColPos);
        return false;
    }

    if (_piAddress == NULL)
    {
        sciErr = createNamedBooleanSparseMatrix(pvCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createBooleanSparseMatrixInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    }

    FREE(piNbItemRow);
    FREE(piColPos);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "boolean sparse_%d (%d x %d)", _iItemPos, iRows, iCols);
    print_tree(pstMsg);
#endif
    return true;
}

static bool import_list_v1(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int i = 0;
    int iItems = 0;
    int *piListAddr = NULL;
    hobj_ref_t *piItemRef = NULL;
    SciErr sciErr;

    iRet = getListDims_v1(_iDatasetId, &iItems);
    if (iRet)
    {
        return false;
    }

    if (iItems == 0)
    {
        //special case for empty list
    }
    else
    {
        iRet = getListItemReferences_v1(_iDatasetId, &piItemRef);
        if (iRet)
        {
            return false;
        }
    }

#ifdef PRINT_DEBUG
    char pstMsg[512];

    sprintf(pstMsg, "list_%d (%d)", _iItemPos, iItems);
    print_tree(pstMsg);
#endif

    if (_piAddress == 0)
    {
        switch (_iVarType)
        {
            case sci_list:
                sciErr = createNamedList(pvCtx, _pstVarname, iItems, &piListAddr);
                break;
            case sci_tlist:
                sciErr = createNamedTList(pvCtx, _pstVarname, iItems, &piListAddr);
                break;
            case sci_mlist:
                sciErr = createNamedMList(pvCtx, _pstVarname, iItems, &piListAddr);
                break;
            default:
                return false;
        }
    }
    else                        //if not null this variable is in a list
    {
        switch (_iVarType)
        {
            case sci_list:
                sciErr = createListInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iItems, &piListAddr);
                break;
            case sci_tlist:
                sciErr = createTListInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iItems, &piListAddr);
                break;
            case sci_mlist:
                sciErr = createMListInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iItems, &piListAddr);
                break;
            default:
                return false;
        }
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    iTab++;
    for (i = 0; i < iItems; i++)
    {
        int iItemDataset = 0;

        iRet = getListItemDataset_v1(_iDatasetId, piItemRef, i, &iItemDataset);
        if (iRet || iItemDataset == 0)
        {
            return false;
        }

        bool bRet = import_data_v1(pvCtx, iItemDataset, i + 1, piListAddr, _pstVarname);

        if (bRet == false)
        {
            return false;
        }
    }
    iTab--;

    iRet = deleteListItemReferences_v1(_iDatasetId, piItemRef);
    if (iRet)
    {
        return false;
    }

#ifdef TIME_DEBUG
    printf("Close List %d\n\n", iCloseList++);
#endif

#ifdef PRINT_DEBUG
    char pstMsg1[512];

    sprintf(pstMsg1, "ListEnd_%d", _iItemPos);
    print_tree(pstMsg1);
#endif
    return true;
}

void print_tree(char *_pstMsg)
{
#ifdef PRINT_DEBUG
    for (int i = 0; i < iTab; i++)
    {
        printf("\t");
    }
    printf("%s\n", _pstMsg);
#endif
}
static bool import_hypermat_v1(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iItems = 0;
    hobj_ref_t *piItemRef = NULL;

    // an hypermatrix is stored in an mlist
    if (_iVarType != sci_mlist)
    {
        return false;
    }

    iRet = getListDims_v1(_iDatasetId, &iItems);
    if (iRet)
    {
        return false;
    }

    if (iItems != 3)
    {
        // hypermatrix have 3 elements
        return false;
    }

    iRet = getListItemReferences_v1(_iDatasetId, &piItemRef);
    if (iRet)
    {
        return false;
    }

    // get first item
    int iItemDataset = 0;
    iRet = getListItemDataset_v1(_iDatasetId, piItemRef, 0, &iItemDataset);
    if (iRet || iItemDataset == 0)
    {
        return false;
    }

    // get first item type
    int iItemType = getScilabTypeFromDataSet_v1(iItemDataset);
    if (iItemType != sci_strings)
    {
        return false;
    }

    // get size of first item
    iRet = getDatasetDims_v1(iItemDataset, &iRows, &iCols);
    if (iRet < 0)
    {
        return false;
    }

    if (iRows * iCols != 3)
    {
        return false;
    }

    // get data of first item for check the type of mlist
    char** pstData = new char*[iRows * iCols];
    iRet = readStringMatrix_v1(iItemDataset, iRows, iCols, pstData);
    if (iRet || strcmp(pstData[0], "hm") != 0)
    {
        FREE(piItemRef);
        for (int i = 0; i < iRows * iCols; i++)
        {
            FREE(pstData[i]);
        }
        delete[] pstData;
        return false;
    }

    for (int i = 0; i < iRows * iCols; i++)
    {
        FREE(pstData[i]);
    }
    delete[] pstData;
    pstData = NULL;

    // get second item, the Size of hypermatrix
    iRet = getListItemDataset_v1(_iDatasetId, piItemRef, 1, &iItemDataset);
    if (iRet)
    {
        return false;
    }

    iRet = getDatasetDims_v1(iItemDataset, &iRows, &iCols);
    if (iRet < 0)
    {
        return false;
    }

    if (iRows != 1)
    {
        return false;
    }

    int* piDimsArray = new int[iCols];
    iRet = readInteger32Matrix_v1(iItemDataset, iRows, iCols, piDimsArray);
    if (iRet)
    {
        delete[] piDimsArray;
        return false;
    }

    // get third item, the Data of hypermatrix
    // import data like a "type" (Double, Int, ...) instead of mlist
    iRet = getListItemDataset_v1(_iDatasetId, piItemRef, 2, &iItemDataset);
    bool bRet = import_data_v1(pvCtx, iItemDataset, _iItemPos, _piAddress, _pstVarname);
    if (bRet == false)
    {
        delete[] piDimsArray;
        return false;
    }

    // get imported hypermatrix from List or Context
    types::GenericType* pGT = NULL;
    types::InternalType* pIT = NULL;
    if (_piAddress)
    {
        types::List* pL = (types::List*)_piAddress;
        pIT = pL->get(_iItemPos - 1);
    }
    else
    {
        wchar_t* pwcsName = to_wide_string(_pstVarname);
        pIT = symbol::Context::getInstance()->getCurrentLevel(symbol::Symbol(pwcsName));
        FREE(pwcsName);
    }

    // reshape data with size of hypermatrix
    pGT = pIT->getAs<types::GenericType>();
    pGT->reshape(piDimsArray, iCols);

    delete[] piDimsArray;


    iRet = deleteListItemReferences_v1(_iDatasetId, piItemRef);
    if (iRet)
    {
        return false;
    }

    return true;
}

/*--------------------------------------------------------------------------*/
