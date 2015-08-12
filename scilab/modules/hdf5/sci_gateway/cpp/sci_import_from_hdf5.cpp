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
*/

#include <hdf5.h>
extern "C"
{
#include <string.h>
#include "gw_hdf5.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"
#include "intmacr2tree.h"
#include "expandPathVariable.h"
#include "stdlib.h"
#include "freeArrayOfString.h"
}

#include "import_from_hdf5_v1.hxx"

static bool import_variable(int _iFile, char* _pstVarName);
static bool import_data(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_double(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_string(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_boolean(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_integer(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_boolean_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_poly(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_list(int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_void(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);
static bool import_undefined(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname);

int sci_import_from_hdf5(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddr = NULL;
    char* pstFilename = NULL;
    char* pstExpandedFilename = NULL;
    bool bImport = true;
    const int nbIn = nbInputArgument(pvApiCtx);
    int iSelectedVar = nbIn - 1;

    CheckInputArgumentAtLeast(pvApiCtx, 1);
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

        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
        return 1;
    }

    //open hdf5 file
    pstExpandedFilename = expandPathVariable(pstFilename);
    int iFile = openHDF5File(pstExpandedFilename, 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname, pstFilename);
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
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname, SOD_FILE_VERSION, iVersion);
            return 1;
        }
        else
        {
            //call older import functions and exit or ... EXIT !
            if (iVersion == 1 || iVersion == -1)
            {
                //sciprint("old sci_import_from_hdf5_v1\n");
                return sci_import_from_hdf5_v1(fname, fname_len);
            }
        }
    }

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

                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, i + 1);
                return 1;
            }

            if (import_variable(iFile, pstVarName) == false)
            {
                FREE(pstVarName);
                bImport = false;
                break;
            }

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
                if (import_variable(iFile, pstVarNameList[i]) == false)
                {
                    bImport = false;
                    break;
                }
            }

            freeArrayOfString(pstVarNameList, iNbItem);
        }
    }
    //close the file
    closeHDF5File(iFile);

    int *piReturn = NULL;

    sciErr = allocMatrixOfBoolean(pvApiCtx, nbIn + 1, 1, 1, &piReturn);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (bImport == true)
    {
        piReturn[0] = 1;
    }
    else
    {
        piReturn[0] = 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    //  printf("End gateway !!!\n");
    return 0;
}

static bool import_variable(int _iFile, char* _pstVarName)
{
    int iDataSetId = getDataSetIdFromName(_iFile, _pstVarName);
    if (iDataSetId <= 0)
    {
        return false;
    }

    return import_data(iDataSetId, 0, NULL, _pstVarName);
}

static bool import_data(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    bool bRet = false;

    //get var type
    int iVarType = getScilabTypeFromDataSet(_iDatasetId);

    switch (iVarType)
    {
        case sci_matrix:
        {
            bRet = import_double(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_strings:
        {
            bRet = import_string(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_list:
        case sci_tlist:
        case sci_mlist:
        {
            bRet = import_list(_iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_boolean:
        {
            bRet = import_boolean(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_poly:
        {
            bRet = import_poly(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_ints:
        {
            bRet = import_integer(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_sparse:
        {
            bRet = import_sparse(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_boolean_sparse:
        {
            bRet = import_boolean_sparse(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_void:             //void item only on list variable
        {
            bRet = import_void(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        case sci_undefined:        //undefined item only on list variable
        {
            bRet = import_undefined(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
            break;
        }
        default:
        {
            Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), "import_from_hdf5");
        }
    }
    return bRet;
}

static bool import_void(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    SciErr sciErr;

    if (_piAddress)
    {
        sciErr = createVoidInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos);
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

static bool import_undefined(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
{
    SciErr sciErr;

    if (_piAddress)
    {
        sciErr = createUndefinedInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos);
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

static bool import_double(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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
            FREE(piDims);
            return false;
        }

        piDims = (int*)MALLOC(sizeof(int) * iDims);
        iSize = getDatasetInfo(_iDatasetId, &iComplex, &iDims, piDims);

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
        /*bug 7224 : to close dataset */
        iRet = readEmptyMatrix(_iDatasetId);
        if (iRet)
        {
            if (piDims)
            {
                FREE(piDims);
            }

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
            sciErr = createNamedComplexMatrixOfDouble(pvApiCtx, _pstVarname, piDims[0], piDims[1], pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfDouble(pvApiCtx, _pstVarname, piDims[0], piDims[1], pdblReal);
        }
    }
    else //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr = createComplexMatrixOfDoubleInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pdblReal, pdblImg);
        }
        else
        {
            sciErr = createMatrixOfDoubleInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pdblReal);
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

static bool import_string(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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

    pstData = (char **)MALLOC(iSize * sizeof(char *));

    iRet = readStringMatrix(_iDatasetId, pstData);
    if (iRet)
    {
        FREE(piDims);
        return false;
    }

    if (_piAddress == NULL)
    {
        sciErr = createNamedMatrixOfString(pvApiCtx, _pstVarname, piDims[0], piDims[1], pstData);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createMatrixOfStringInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pstData);
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

static bool import_integer(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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
                sciErr = createNamedMatrixOfInteger8(pvApiCtx, _pstVarname, piDims[0], piDims[1], pcData);
            }
            else
            {
                sciErr = createMatrixOfInteger8InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pcData);
            }
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
                sciErr = createNamedMatrixOfUnsignedInteger8(pvApiCtx, _pstVarname, piDims[0], piDims[1], pucData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger8InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pucData);
            }
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
                sciErr = createNamedMatrixOfInteger16(pvApiCtx, _pstVarname, piDims[0], piDims[1], psData);
            }
            else
            {
                sciErr = createMatrixOfInteger16InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], psData);
            }
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
                sciErr = createNamedMatrixOfUnsignedInteger16(pvApiCtx, _pstVarname, piDims[0], piDims[1], pusData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger16InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pusData);
            }
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
                sciErr = createNamedMatrixOfInteger32(pvApiCtx, _pstVarname, piDims[0], piDims[1], piData);
            }
            else
            {
                sciErr = createMatrixOfInteger32InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], piData);
            }
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
                sciErr = createNamedMatrixOfUnsignedInteger32(pvApiCtx, _pstVarname, piDims[0], piDims[1], puiData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger32InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], puiData);
            }
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
                sciErr = createNamedMatrixOfInteger64(_pstVarname, piDims[0], piDims[1], pllData);
            }
            else
            {
                sciErr = createMatrixOfInteger64InNamedList(_pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pllData);
            }
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
                sciErr = createNamedMatrixOfUnsignedInteger64(_pstVarname, piDims[0], piDims[1], pullData);
            }
            else
            {
                sciErr = createMatrixOfUnsignedInteger64InNamedList(_pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], pullData);
            }
#else
            FREE(piDims);
            return false;
#endif
        }
        break;
        default:
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

static bool import_boolean(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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
        sciErr = createNamedMatrixOfBoolean(pvApiCtx, _pstVarname, piDims[0], piDims[1], piData);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createMatrixOfBooleanInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, piDims[0], piDims[1], piData);
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

static bool import_poly(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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
            sciErr = createNamedComplexMatrixOfPoly(pvApiCtx, _pstVarname, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedMatrixOfPoly(pvApiCtx, _pstVarname, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal);
        }
    }
    else                        //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr =
                createComplexMatrixOfPolyInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal,
                        pdblImg);
        }
        else
        {
            sciErr = createMatrixOfPolyInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, piDims[0], piDims[1], piNbCoef, pdblReal);
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

static bool import_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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
            sciErr = createNamedComplexSparseMatrix(pvApiCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createNamedSparseMatrix(pvApiCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
        }
    }
    else //if not null this variable is in a list
    {
        if (iComplex)
        {
            sciErr = createComplexSparseMatrixInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            sciErr = createSparseMatrixInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
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

static bool import_boolean_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, char *_pstVarname)
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
        sciErr = createNamedBooleanSparseMatrix(pvApiCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos);
    }
    else                        //if not null this variable is in a list
    {
        sciErr = createBooleanSparseMatrixInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos);
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

static bool import_list(int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, char *_pstVarname)
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
                sciErr = createNamedList(pvApiCtx, _pstVarname, iItems, &piListAddr);
                break;
            case sci_tlist:
                sciErr = createNamedTList(pvApiCtx, _pstVarname, iItems, &piListAddr);
                break;
            case sci_mlist:
                sciErr = createNamedMList(pvApiCtx, _pstVarname, iItems, &piListAddr);
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
                sciErr = createListInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iItems, &piListAddr);
                break;
            case sci_tlist:
                sciErr = createTListInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iItems, &piListAddr);
                break;
            case sci_mlist:
                sciErr = createMListInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iItems, &piListAddr);
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

        bool bRet = import_data(iItemDataset, i + 1, piListAddr, _pstVarname);

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

/*--------------------------------------------------------------------------*/
