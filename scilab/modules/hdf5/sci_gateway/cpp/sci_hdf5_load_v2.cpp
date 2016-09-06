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
#include "struct.hxx"
#include "cell.hxx"

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
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"
#include "expandPathVariable.h"
#include "stdlib.h"
#include "freeArrayOfString.h"
}

#include "hdf5_load_v1.hxx"

static bool import_variable(int* pvCtx, int _iFile, char* _pstVarName);
static bool import_data(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_double(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_string(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_boolean(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_integer(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_sparse(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_boolean_sparse(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_poly(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_list(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_hypermat(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_struct(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_cell(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_void(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_undefined(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);

static const std::string fname("load");

int sci_hdf5_load_v2(char *fn, int* pvApiCtx)
{
    SciErr sciErr;

    int* piAddr = NULL;
    char* pstFilename = NULL;
    char* pstExpandedFilename = NULL;
    bool bImport = true;
    const int nbIn = nbInputArgument(pvApiCtx);
    int iSelectedVar = nbIn - 1;

    CheckInputArgumentAtLeast(pvApiCtx , 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

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
            freeAllocatedSingleString(pstFilename);
        }

        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname.data(), 2);
        return 1;
    }

    //open hdf5 file
    pstExpandedFilename = expandPathVariable(pstFilename);
    int iFile = openHDF5File(pstExpandedFilename, 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname.data(), pstFilename);
        FREE(pstExpandedFilename);
        FREE(pstFilename);
        return 1;
    }

    FREE(pstExpandedFilename);
    FREE(pstFilename);

    //manage version information
    int iVersion = getSODFormatAttribute(iFile);
    if (iVersion != SOD_FILE_VERSION)
    {
        if (iVersion > SOD_FILE_VERSION)
        {
            //can't read file with version newer that me !
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, iVersion);
            return 1;
        }
        else
        {
            //call older import functions and exit or ... EXIT !
            if (iVersion == 1 || iVersion == -1)
            {
                return sci_hdf5_load_v1(fn, pvApiCtx);
            }
        }
    }

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
                    freeAllocatedSingleString(pstVarName);
                }

                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname.data(), i + 1);
                return 1;
            }

            if (import_variable(pvApiCtx, iFile, pstVarName) == false)
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
        iNbItem = getVariableNames(iFile, NULL);
        if (iNbItem != 0)
        {
            char **pstVarNameList = (char **)MALLOC(sizeof(char *) * iNbItem);

            iNbItem = getVariableNames(iFile, pstVarNameList);

            //import all data
            for (int i = 0; i < iNbItem; i++)
            {
                if (import_variable(pvApiCtx, iFile, pstVarNameList[i]) == false)
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

static bool import_variable(int* pvCtx, int _iFile, char* _pstVarName)
{
    int iDataSetId = getDataSetIdFromName(_iFile, _pstVarName);
    if (iDataSetId <= 0)
    {
        return false;
    }

    return import_data(pvCtx, iDataSetId, 0, NULL, _pstVarName);
}

static bool import_data(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    bool bRet = false;

    //get var type
    int iVarType = getScilabTypeFromDataSet(_iDatasetId);

    switch (iVarType)
    {
        case sci_matrix:
        {
            bRet = import_double(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_strings:
        {
            bRet = import_string(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_list:
        case sci_tlist:
        case sci_mlist:
        {
            bRet = import_hypermat(pvCtx, _iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            if (bRet == false)
            {
                bRet = import_struct(pvCtx, _iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            }
            if (bRet == false)
            {
                bRet = import_cell(pvCtx, _iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            }
            if (bRet == false)
            {
                bRet = import_list(pvCtx, _iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            }
            break;
        }
        case sci_boolean:
        {
            bRet = import_boolean(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_poly:
        {
            bRet = import_poly(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_ints:
        {
            bRet = import_integer(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_sparse:
        {
            bRet = import_sparse(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_boolean_sparse:
        {
            bRet = import_boolean_sparse(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_void:             //void item only on list variable
        {
            bRet = import_void(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_undefined:        //undefined item only on list variable
        {
            bRet = import_undefined(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        default:
        {
            Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), fname.data());
        }
    }
    return bRet;
}

static bool import_void(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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

    //close void dataset
    closeDataSet(_iDatasetId);
    return true;
}

static bool import_undefined(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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

    //close undefined dataset
    closeDataSet(_iDatasetId);
    return true;
}

static bool import_double(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    SciErr sciErr;
    int iRet = 0;
    double *pdblReal = NULL;
    double *pdblImg = NULL;
    int iDims = 0;
    int* piDims = NULL;
    int iComplex = 0;
    int iSize = 0;

    iRet = getDatasetInfo(_iDatasetId, &iComplex, &iDims, NULL);
    if (iRet < 0)
    {
        return false;
    }

    if (iDims)
    {
        if (iDims > 2)
        {
            //hypermatrix
            return false;
        }

        piDims = (int*)MALLOC(sizeof(int) * iDims);
        iSize = getDatasetInfo(_iDatasetId, &iComplex, &iDims, piDims);
        if (iSize < 0)
        {
            FREE(piDims);
            return false;
        }

        if (iSize > 0)
        {
            pdblReal = (double *)MALLOC(iSize * sizeof(double));

            if (iComplex)
            {
                pdblImg = (double *)MALLOC(iSize * sizeof(double));
                iRet = readDoubleComplexMatrix(_iDatasetId, pdblReal, pdblImg);
            }
            else
            {
                iRet = readDoubleMatrix(_iDatasetId, pdblReal);
            }
            if (iRet < 0)
            {
                FREE(piDims);
                FREE(pdblReal);
                if (iComplex)
                {
                    FREE(pdblImg);
                }
                return false;
            }

            //to be sure ti have 2 dims
            if (iDims == 1)
            {
                FREE(piDims);
                piDims = (int*)MALLOC(sizeof(int) * 2);
                piDims[0] = 1;
                piDims[1] = iSize;
            }
        }
    }

    if (iDims == 0 || iSize == 0) //empty matrix
    {
        if (piDims)
        {
            FREE(piDims);
        }

        /*bug 7224 : to close dataset */
        iRet = readEmptyMatrix(_iDatasetId);
        if (iRet)
        {
            return false;
        }

        // Hack to sure that piDims will not be null at line 372.
        iDims = 2;
        piDims = (int*)MALLOC(sizeof(int) * iDims);
        memset(piDims, 0, sizeof(int) * iDims);
        pdblReal = (double*)MALLOC(sizeof(double) * 1);
        pdblReal[0] = 0;
        iComplex = 0;
    }

    if (_piAddress == NULL)
    {
        if (iComplex)
        {
            sciErr = createNamedComplexMatrixOfDouble(pvCtx, _pstVarname, piDims[0], piDims[1], pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfDouble(pvCtx, _pstVarname, piDims[0], piDims[1], pdblReal);
        }
    }
    else //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr = createComplexMatrixOfDoubleInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pdblReal, pdblImg);
        }
        else
        {
            sciErr = createMatrixOfDoubleInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pdblReal);
        }
    }

    FREE(piDims);
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

    return true;
}

static bool import_string(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    SciErr sciErr;
    int iRet = 0;
    char **pstData = NULL;
    int iDims = 0;
    int* piDims = NULL;
    int iComplex = 0;
    int iSize = 0;

    iRet = getDatasetInfo(_iDatasetId, &iComplex, &iDims, NULL);
    if (iRet < 0)
    {
        return false;
    }

    piDims = (int*)MALLOC(sizeof(int) * iDims);
    iSize = getDatasetInfo(_iDatasetId, &iComplex, &iDims, piDims);
    if (iSize < 0)
    {
        FREE(piDims);
        return false;
    }

    pstData = (char **)MALLOC(iSize * sizeof(char *));

    iRet = readStringMatrix(_iDatasetId, pstData);
    if (iRet)
    {
        FREE(piDims);
        return false;
    }

    if (_piAddress == NULL)
    {
        sciErr = createNamedMatrixOfString(pvCtx, _pstVarname, piDims[0], piDims[1], pstData);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createMatrixOfStringInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pstData);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(piDims);
        freeStringMatrix(_iDatasetId, pstData);
        FREE(pstData);
        return false;
    }

    FREE(piDims);
    freeStringMatrix(_iDatasetId, pstData);
    FREE(pstData);

    return true;
}

static bool import_integer(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iDims = 0;
    int* piDims = NULL;
    int iComplex = 0;
    int iSize = 0;
    int iPrec = 0;
    SciErr sciErr;

    iRet = getDatasetInfo(_iDatasetId, &iComplex, &iDims, NULL);
    if (iRet < 0)
    {
        return false;
    }

    piDims = (int*)MALLOC(sizeof(int) * iDims);
    iSize = getDatasetInfo(_iDatasetId, &iComplex, &iDims, piDims);
    if (iSize < 0)
    {
        FREE(piDims);
        return false;
    }

    iRet = getDatasetPrecision(_iDatasetId, &iPrec);
    if (iRet)
    {
        FREE(piDims);
        return false;
    }

    switch (iPrec)
    {
        case SCI_INT8:
        {
            char *pcData = NULL;

            pcData = (char *)MALLOC(sizeof(char) * iSize);
            iRet = readInteger8Matrix(_iDatasetId, pcData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger8(pvCtx, _pstVarname, piDims[0], piDims[1], pcData);
            }
            else
            {
                sciErr = createMatrixOfInteger8InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pcData);
            }

            FREE(pcData);
        }
        break;
        case SCI_UINT8:
        {
            unsigned char *pucData = NULL;

            pucData = (unsigned char *)MALLOC(sizeof(unsigned char) * iSize);
            iRet = readUnsignedInteger8Matrix(_iDatasetId, pucData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger8(pvCtx, _pstVarname, piDims[0], piDims[1], pucData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger8InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pucData);
            }

            FREE(pucData);
        }
        break;
        case SCI_INT16:
        {
            short *psData = NULL;

            psData = (short *)MALLOC(sizeof(short) * iSize);
            iRet = readInteger16Matrix(_iDatasetId, psData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger16(pvCtx, _pstVarname, piDims[0], piDims[1], psData);
            }
            else
            {
                sciErr = createMatrixOfInteger16InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], psData);
            }

            FREE(psData);
        }
        break;
        case SCI_UINT16:
        {
            unsigned short *pusData = NULL;

            pusData = (unsigned short *)MALLOC(sizeof(unsigned short) * iSize);
            iRet = readUnsignedInteger16Matrix(_iDatasetId, pusData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger16(pvCtx, _pstVarname, piDims[0], piDims[1], pusData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger16InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pusData);
            }

            FREE(pusData);
        }
        break;
        case SCI_INT32:
        {
            int *piData = NULL;

            piData = (int *)MALLOC(sizeof(int) * iSize);
            iRet = readInteger32Matrix(_iDatasetId, piData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger32(pvCtx, _pstVarname, piDims[0], piDims[1], piData);
            }
            else
            {
                sciErr = createMatrixOfInteger32InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], piData);
            }

            FREE(piData);
        }
        break;
        case SCI_UINT32:
        {
            unsigned int *puiData = NULL;

            puiData = (unsigned int *)MALLOC(sizeof(unsigned int) * iSize);
            iRet = readUnsignedInteger32Matrix(_iDatasetId, puiData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger32(pvCtx, _pstVarname, piDims[0], piDims[1], puiData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger32InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], puiData);
            }

            FREE(puiData);
        }
        break;
        case SCI_INT64:
        {
#ifdef __SCILAB_INT64__
            long long *pllData = NULL;

            pllData = (long long *)MALLOC(sizeof(long long) * iSize);
            iRet = readInteger64Matrix(_iDatasetId, pllData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfInteger64(pvCtx, _pstVarname, piDims[0], piDims[1], pllData);
            }
            else
            {
                sciErr = createMatrixOfInteger64InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pllData);
            }

            FREE(pllData);
#else
            FREE(piDims);
            return false;
#endif
        }
        break;
        case SCI_UINT64:
        {
#ifdef __SCILAB_INT64__
            unsigned long long *pullData = NULL;

            pullData = (unsigned long long *)MALLOC(sizeof(unsigned long long) * iSize);
            iRet = readUnsignedInteger64Matrix(_iDatasetId, pullData);
            if (iRet)
            {
                FREE(piDims);
                return false;
            }

            if (_piAddress == NULL)
            {
                sciErr = createNamedMatrixOfUnsignedInteger64(pvCtx, _pstVarname, piDims[0], piDims[1], pullData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger64InNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pullData);
            }

            FREE(pullData);
#else
            FREE(piDims);
            return false;
#endif
        }
        break;
        default:
            FREE(piDims);
            return false;
    }

    FREE(piDims);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    return true;
}

static bool import_boolean(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int *piData = NULL;
    int iDims = 0;
    int* piDims = NULL;
    int iComplex = 0;
    int iSize = 0;
    SciErr sciErr;

    iRet = getDatasetInfo(_iDatasetId, &iComplex, &iDims, NULL);
    if (iRet < 0)
    {
        return false;
    }

    piDims = (int*)MALLOC(sizeof(int) * iDims);
    iSize = getDatasetInfo(_iDatasetId, &iComplex, &iDims, piDims);
    if (iSize < 0)
    {
        FREE(piDims);
        return false;
    }

    if (iSize == 0)
    {
        FREE(piDims);
        return false;
    }

    piData = (int *)MALLOC(iSize * sizeof(int));
    iRet = readBooleanMatrix(_iDatasetId, piData);
    if (iRet)
    {
        FREE(piData);
        FREE(piDims);
        return false;
    }

    if (_piAddress == NULL)
    {
        sciErr = createNamedMatrixOfBoolean(pvCtx, _pstVarname, piDims[0], piDims[1], piData);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createMatrixOfBooleanInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], piData);
    }

    FREE(piDims);
    if (piData)
    {
        FREE(piData);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return false;
    }

    return true;
}

static bool import_poly(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iComplex = 0;
    char pstVarName[64] = { 0 };
    double **pdblReal = NULL;
    double **pdblImg = NULL;
    int *piNbCoef = NULL;
    int iDims = 0;
    int* piDims = NULL;
    int iSize = 0;
    SciErr sciErr;

    iRet = getDatasetInfo(_iDatasetId, &iComplex, &iDims, NULL);
    if (iRet < 0)
    {
        return false;
    }

    piDims = (int*)MALLOC(sizeof(int) * iDims);
    iSize = getDatasetInfo(_iDatasetId, &iComplex, &iDims, piDims);
    if (iSize < 0)
    {
        FREE(piDims);
        return false;
    }

    if (iComplex)
    {
        piNbCoef = (int *)MALLOC(iSize * sizeof(int));
        pdblReal = (double **)MALLOC(iSize * sizeof(double *));
        pdblImg = (double **)MALLOC(iSize * sizeof(double *));
        iRet = readPolyComplexMatrix(_iDatasetId, pstVarName, iDims, piDims, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        piNbCoef = (int *)MALLOC(iSize * sizeof(int));
        pdblReal = (double **)MALLOC(iSize * sizeof(double *));
        iRet = readPolyMatrix(_iDatasetId, pstVarName, iDims, piDims, piNbCoef, pdblReal);
    }

    if (iRet)
    {
        FREE(piDims);
        FREE(piNbCoef);
        for (int i = 0; i < iSize; i++)
        {
            FREE(pdblReal[i]);
        }
        FREE(pdblReal);

        if (iComplex)
        {
            for (int i = 0; i < iSize; i++)
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
            sciErr = createNamedComplexMatrixOfPoly(pvCtx, _pstVarname, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfPoly(pvCtx, _pstVarname, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal);
        }
    }
    else                        //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr =
                createComplexMatrixOfPolyInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal,
                        pdblImg);
        }
        else
        {
            sciErr = createMatrixOfPolyInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal);
        }
    }

    FREE(piDims);
    FREE(piNbCoef);
    for (int i = 0; i < iSize; i++)
    {
        FREE(pdblReal[i]);
    }

    FREE(pdblReal);

    if (iComplex)
    {
        for (int i = 0; i < iSize; i++)
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

    return true;
}

static bool import_sparse(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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

    iRet = getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData(_iDatasetId);

    if (iComplex)
    {
        piNbItemRow = (int *)MALLOC(iRows * sizeof(int));
        piColPos = (int *)MALLOC(iNbItem * sizeof(int));
        pdblReal = (double *)MALLOC(iNbItem * sizeof(double));
        pdblImg = (double *)MALLOC(iNbItem * sizeof(double));
        iRet = readSparseComplexMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
    }
    else
    {
        piNbItemRow = (int *)MALLOC(iRows * sizeof(int));
        piColPos = (int *)MALLOC(iNbItem * sizeof(int));
        pdblReal = (double *)MALLOC(iNbItem * sizeof(double));
        iRet = readSparseMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
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
    else //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr = createComplexSparseMatrixInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createSparseMatrixInNamedList(pvCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
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

    return true;
}

static bool import_boolean_sparse(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int *piNbItemRow = NULL;
    int *piColPos = NULL;
    SciErr sciErr;

    iRet = getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    piNbItemRow = (int *)MALLOC(iRows * sizeof(int));
    piColPos = (int *)MALLOC(iNbItem * sizeof(int));

    iRet = readBooleanSparseMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    if (iRet)
    {
        FREE(piNbItemRow);
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

    return true;
}

static bool import_list(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int i = 0;
    int iItems = 0;
    int *piListAddr = NULL;
    hobj_ref_t *piItemRef = NULL;
    SciErr sciErr;

    iRet = getListDims(_iDatasetId, &iItems);
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
        iRet = getListItemReferences(_iDatasetId, &piItemRef);
        if (iRet)
        {
            return false;
        }
    }

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

    for (i = 0; i < iItems; i++)
    {
        int iItemDataset = 0;

        iRet = getListItemDataset(_iDatasetId, piItemRef, i, &iItemDataset);
        if (iRet || iItemDataset == 0)
        {
            return false;
        }

        bool bRet = import_data(pvCtx, iItemDataset, i + 1, piListAddr, _pstVarname);

        if (bRet == false)
        {
            return false;
        }
    }

    iRet = deleteListItemReferences(_iDatasetId, piItemRef);
    if (iRet)
    {
        return false;
    }

    return true;
}

static bool import_hypermat(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iComplex = 0;
    int iDims = 0;
    int iItems = 0;
    hobj_ref_t *piItemRef = NULL;

    // an hypermatrix is stored in an mlist
    if (_iVarType != sci_mlist)
    {
        return false;
    }

    iRet = getListDims(_iDatasetId, &iItems);
    if (iRet)
    {
        return false;
    }

    if (iItems != 3)
    {
        // hypermatrix have 3 elements
        return false;
    }

    iRet = getListItemReferences(_iDatasetId, &piItemRef);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get first item
    int iItemDataset = 0;
    iRet = getListItemDataset(_iDatasetId, piItemRef, 0, &iItemDataset);
    if (iRet || iItemDataset == 0)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get first item type
    int iItemType = getScilabTypeFromDataSet(iItemDataset);
    if (iItemType != sci_strings)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get size of first item
    iRet = getDatasetInfo(iItemDataset, &iComplex, &iDims, NULL);
    if (iRet < 0 || iDims != 2)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    int* piDims = new int[2];
    int iSize = getDatasetInfo(iItemDataset, &iComplex, &iDims, piDims);
    if (iSize != 3)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        return false;
    }

    delete[] piDims;
    piDims = NULL;

    // get data of first item for check the type of mlist
    char** pstData = new char*[iSize];
    iRet = readStringMatrix(iItemDataset, pstData);
    if (iRet || strcmp(pstData[0], "hm") != 0)
    {
        // if not the good type, do not h5close (deleteListItemReferences)
        FREE(piItemRef);
        freeStringMatrix(iItemDataset, pstData);
        delete[] pstData;
        return false;
    }

    freeStringMatrix(iItemDataset, pstData);
    delete[] pstData;
    pstData = NULL;

    // get second item, the Size of hypermatrix
    iRet = getListItemDataset(_iDatasetId, piItemRef, 1, &iItemDataset);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    iRet = getDatasetInfo(iItemDataset, &iComplex, &iDims, NULL);
    if (iRet < 0 || iDims != 2)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    piDims = new int[2];
    iSize = getDatasetInfo(iItemDataset, &iComplex, &iDims, piDims);
    if (piDims[0] != 1)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        return false;
    }

    int* piDimsArray = new int[piDims[1]];
    iRet = readInteger32Matrix(iItemDataset, piDimsArray);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        delete[] piDimsArray;
        return false;
    }

    // get third item, the Data of hypermatrix
    // import data like a "type" (Double, Int, ...) instead of mlist
    iRet = getListItemDataset(_iDatasetId, piItemRef, 2, &iItemDataset);
    bool bRet = import_data(pvCtx, iItemDataset, _iItemPos, _piAddress, _pstVarname);
    if (bRet == false)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
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
    pGT->reshape(piDimsArray, piDims[1]);

    delete[] piDims;
    delete[] piDimsArray;


    iRet = deleteListItemReferences(_iDatasetId, piItemRef);
    if (iRet)
    {
        return false;
    }

    return true;
}
/*--------------------------------------------------------------------------*/

static bool import_struct(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iComplex = 0;
    int iDims = 0;
    int iItems = 0;
    hobj_ref_t *piItemRef = NULL;

    // an struct is stored in an mlist
    if (_iVarType != sci_mlist)
    {
        return false;
    }

    iRet = getListDims(_iDatasetId, &iItems);
    if (iRet)
    {
        return false;
    }

    if (iItems < 2)
    {
        // struct have 2 elements minimal
        return false;
    }

    iRet = getListItemReferences(_iDatasetId, &piItemRef);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get first item
    int iItemDataset = 0;
    iRet = getListItemDataset(_iDatasetId, piItemRef, 0, &iItemDataset);
    if (iRet || iItemDataset == 0)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get first item type
    int iItemType = getScilabTypeFromDataSet(iItemDataset);
    if (iItemType != sci_strings)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get size of first item
    iRet = getDatasetInfo(iItemDataset, &iComplex, &iDims, NULL);
    if (iRet < 0 || iDims != 2)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    int* piDims = new int[2];
    int iSize = getDatasetInfo(iItemDataset, &iComplex, &iDims, piDims);
    if (iSize != iItems)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        return false;
    }

    delete[] piDims;
    piDims = NULL;

    // get data of first item for check the type of mlist
    char** pstData = new char*[iSize];
    char** pstDataSave = new char*[iSize - 2];
    iRet = readStringMatrix(iItemDataset, pstData);
    if (iRet || strcmp(pstData[0], "st") != 0)
    {
        // if not the good type, do not h5close (deleteListItemReferences)
        FREE(piItemRef);
        freeStringMatrix(iItemDataset, pstData);
        delete[] pstData;
        delete[] pstDataSave;
        return false;
    }

    for (int i = 2; i < iSize; ++i)
    {
        pstDataSave[-2 + i] = new char[strlen(pstData[i]) + 1];
        strcpy(pstDataSave[-2 + i], pstData[i]);
    }

    freeStringMatrix(iItemDataset, pstData);
    delete[] pstData;
    pstData = NULL;

    // get second item, the Size of struct
    iRet = getListItemDataset(_iDatasetId, piItemRef, 1, &iItemDataset);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        for (int i = 0; i < (-2 + iItems); ++i)
        {
            delete pstDataSave[i];
        }
        delete[] pstDataSave;
        pstDataSave = NULL;
        return false;
    }

    iRet = getDatasetInfo(iItemDataset, &iComplex, &iDims, NULL);
    if (iRet < 0 || iDims != 2)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        for (int i = 0; i < (-2 + iItems); ++i)
        {
            delete pstDataSave[i];
        }
        delete[] pstDataSave;
        pstDataSave = NULL;
        return false;
    }

    piDims = new int[2];
    iSize = getDatasetInfo(iItemDataset, &iComplex, &iDims, piDims);
    if (piDims[0] != 1)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        for (int i = 0; i < (-2 + iItems); ++i)
        {
            delete pstDataSave[i];
        }
        delete[] pstDataSave;
        pstDataSave = NULL;
        delete[] piDims;
        return false;
    }

    int* piDimsArray = new int[piDims[1]];
    iRet = readInteger32Matrix(iItemDataset, piDimsArray);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        for (int i = 0; i < (-2 + iItems); ++i)
        {
            delete pstDataSave[i];
        }
        delete[] pstDataSave;
        pstDataSave = NULL;
        delete[] piDims;
        delete[] piDimsArray;
        return false;
    }

    types::Struct* pStruct = new types::Struct(piDims[1], piDimsArray);
    delete[] piDims;
    delete[] piDimsArray;

    wchar_t* pwstName = NULL;
    for (int i = 0; i < (-2 + iItems); ++i)
    {
        pwstName = to_wide_string(pstDataSave[i]);
        pStruct->addField(pwstName);
        delete pstDataSave[i];
        FREE(pwstName);
    }

    delete[] pstDataSave;
    pstDataSave = NULL;

    types::SingleStruct** ppSStruct =  pStruct->get();
    types::String* pStr = pStruct->getFieldNames();

    types::List* pList = new types::List();
    // fill the list to avoid api_scilab error about the list size.
    pList->set(0, types::Double::Empty());

    if (pStruct->getSize() == 1)
    {
        types::InternalType* pIT = NULL;
        for (int i = 0; i < pStr->getSize(); ++i)
        {
            int iItemDataset = 0;
            iRet = getListItemDataset(_iDatasetId, piItemRef, i + 2, &iItemDataset);
            if (iRet || iItemDataset == 0)
            {
                deleteListItemReferences(_iDatasetId, piItemRef);
                delete pList;
                delete pStruct;
                return false;
            }

            wchar_t* pwcsName = pStr->get(i);
            char* pcName = wide_string_to_UTF8(pwcsName);

            bool bRet = import_data(pvCtx, iItemDataset, 1, (int*)pList, pcName);
            if (bRet == false)
            {
                deleteListItemReferences(_iDatasetId, piItemRef);
                delete pList;
                delete pStruct;
                return false;
            }

            pIT = pList->get(0);
            ppSStruct[0]->set(pwcsName, pIT);
            FREE(pcName);
        }
    }
    else if (pStruct->getSize() > 1)
    {
        for (int i = 0; i < pStr->getSize(); ++i)
        {
            int iItemDataset = 0;
            iRet = getListItemDataset(_iDatasetId, piItemRef, i + 2, &iItemDataset);
            if (iRet || iItemDataset == 0)
            {
                deleteListItemReferences(_iDatasetId, piItemRef);
                delete pList;
                delete pStruct;
                return false;
            }

            wchar_t* pwcsName = pStr->get(i);
            char* pcName = wide_string_to_UTF8(pwcsName);

            bool bRet = import_data(pvCtx, iItemDataset, 1, (int*)pList, pcName);
            if (bRet == false)
            {
                deleteListItemReferences(_iDatasetId, piItemRef);
                delete pList;
                delete pStruct;
                return false;
            }

            types::List* pListData = pList->get(0)->getAs<types::List>();
            for (int iWriteData = 0; iWriteData < pStruct->getSize(); ++iWriteData)
            {
                ppSStruct[iWriteData]->set(pwcsName, pListData->get(iWriteData));
            }

            FREE(pcName);
        }

    }

    delete pList;
    if (_piAddress == NULL)
    {
        pwstName = to_wide_string(_pstVarname);
        symbol::Context::getInstance()->put(symbol::Symbol(pwstName), pStruct);
        FREE(pwstName);
    }
    else
    {
        types::List* pParentList = (types::List*)_piAddress;
        pParentList->set(_iItemPos - 1, pStruct);
    }

    iRet = deleteListItemReferences(_iDatasetId, piItemRef);
    if (iRet)
    {
        return false;
    }

    return true;
}
/*--------------------------------------------------------------------------*/


static bool import_cell(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    int iRet = 0;
    int iComplex = 0;
    int iDims = 0;
    int iItems = 0;
    hobj_ref_t *piItemRef = NULL;

    // an hypermatrix is stored in an mlist
    if (_iVarType != sci_mlist)
    {
        return false;
    }

    iRet = getListDims(_iDatasetId, &iItems);
    if (iRet)
    {
        return false;
    }

    if (iItems != 3)
    {
        // cell have 3 elements
        return false;
    }

    iRet = getListItemReferences(_iDatasetId, &piItemRef);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get first item
    int iItemDataset = 0;
    iRet = getListItemDataset(_iDatasetId, piItemRef, 0, &iItemDataset);
    if (iRet || iItemDataset == 0)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get first item type
    int iItemType = getScilabTypeFromDataSet(iItemDataset);
    if (iItemType != sci_strings)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    // get size of first item
    iRet = getDatasetInfo(iItemDataset, &iComplex, &iDims, NULL);
    if (iRet < 0 || iDims != 2)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    int* piDims = new int[2];
    int iSize = getDatasetInfo(iItemDataset, &iComplex, &iDims, piDims);
    if (iSize != 3)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        return false;
    }

    delete[] piDims;
    piDims = NULL;

    // get data of first item for check the type of mlist
    char** pstData = new char*[iSize];
    iRet = readStringMatrix(iItemDataset, pstData);
    if (iRet || strcmp(pstData[0], "ce") != 0)
    {
        // if not the good type, do not h5close (deleteListItemReferences)
        FREE(piItemRef);
        freeStringMatrix(iItemDataset, pstData);
        delete[] pstData;
        return false;
    }

    freeStringMatrix(iItemDataset, pstData);
    delete[] pstData;
    pstData = NULL;

    // get second item, the Size of cell
    iRet = getListItemDataset(_iDatasetId, piItemRef, 1, &iItemDataset);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    iRet = getDatasetInfo(iItemDataset, &iComplex, &iDims, NULL);
    if (iRet < 0 || iDims != 2)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        return false;
    }

    piDims = new int[2];
    iSize = getDatasetInfo(iItemDataset, &iComplex, &iDims, piDims);
    if (piDims[0] != 1)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        return false;
    }

    int* piDimsArray = new int[piDims[1]];
    iRet = readInteger32Matrix(iItemDataset, piDimsArray);
    if (iRet)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete[] piDims;
        delete[] piDimsArray;
        return false;
    }

    types::Cell* pCell = new types::Cell(piDims[1], piDimsArray);
    delete[] piDims;
    delete[] piDimsArray;
    types::List* pList = new types::List();
    pList->set(0, types::Double::Empty());

    iRet = getListItemDataset(_iDatasetId, piItemRef, 2, &iItemDataset);
    if (iRet || iItemDataset == 0)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete pList;
        delete pCell;
        return false;
    }

    bool bRet = import_data(pvCtx, iItemDataset, 1, (int*)pList, NULL);
    if (bRet == false)
    {
        deleteListItemReferences(_iDatasetId, piItemRef);
        delete pList;
        delete pCell;
        return false;
    }

    types::List* pListData = pList->get(0)->getAs<types::List>();
    for (int iWriteData = 0; iWriteData < pCell->getSize(); ++iWriteData)
    {
        pCell->set(iWriteData, pListData->get(iWriteData));
    }

    delete pList;

    if (_piAddress == NULL)
    {
        wchar_t* pwstName = to_wide_string(_pstVarname);
        symbol::Context::getInstance()->put(symbol::Symbol(pwstName), pCell);
        FREE(pwstName);
    }
    else
    {
        types::List* pParentList = (types::List*)_piAddress;
        pParentList->set(_iItemPos - 1, pCell);
    }

    iRet = deleteListItemReferences(_iDatasetId, piItemRef);

    if (iRet)
    {
        return false;
    }

    return true;
}
/*--------------------------------------------------------------------------*/

