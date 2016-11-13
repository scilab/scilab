/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#include "freeArrayOfString.h"
}

#include "listvar_in_hdf5_v1.hxx"
#include <vector>

typedef struct __VAR_INFO__
{
    char pstInfo[128];
    char varName[128];
    int iType;
    int iSize;
    int iDims;
    int piDims[2];

    __VAR_INFO__() : iType(0), iSize(0), iDims(0)
    {
        memset(pstInfo, 0, sizeof(pstInfo) / sizeof(pstInfo[0]));
        memset(varName, 0, sizeof(varName) / sizeof(varName[0]));
    }
} VarInfo;

static bool read_data(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_double(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_string(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_boolean(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_integer(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_boolean_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_poly(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_list(int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_void(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);
static bool read_undefined(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo);

static void generateInfo(VarInfo* _pInfo, const char* _pstType);

int sci_hdf5_listvar_v2(char *fname, int* pvApiCtx)
{
    SciErr sciErr;
    int *piAddr     = NULL;
    char* pstFile   = NULL;
    int iFile       = 0;
    int iNbItem     = 0;
    VarInfo* pInfo  = NULL;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 4);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFile))
    {
        if (pstFile)
        {
            FREE(pstFile);
        }

        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    char* pstFileName = expandPathVariable(pstFile);
    iFile = openHDF5File(pstFileName, 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname, pstFile);
        FREE(pstFileName);
        FREE(pstFile);
        return 1;
    }

    FREE(pstFileName);
    FREE(pstFile);

    //manage version information
    int iVersion = getSODFormatAttribute(iFile);
    if (iVersion != SOD_FILE_VERSION)
    {
        if (iVersion > SOD_FILE_VERSION)
        {
            //can't read file with version newer that me !
            closeHDF5File(iFile);
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname, SOD_FILE_VERSION, iVersion);
            return 1;
        }
        else
        {
            //call older import functions and exit or ... EXIT !
            if (iVersion == 1 || iVersion == -1)
            {
                //sciprint("old sci_listvar_in_hdf5_v1\n");
                return sci_listvar_in_hdf5_v1(fname, pvApiCtx);
            }
        }
    }

    iNbItem = getVariableNames(iFile, NULL);
    if (iNbItem != 0)
    {
        char** pstVarNameList = (char**)MALLOC(sizeof(char*) * iNbItem);
        pInfo = (VarInfo*)MALLOC(iNbItem * sizeof(VarInfo));
        int b;

        if (Lhs == 1)
        {
            sciprint("Name                     Type           Size            Bytes\n");
            sciprint("---------------------------------------------------------------\n");
        }

        iNbItem = getVariableNames(iFile, pstVarNameList);
        for (int i = 0; i < iNbItem; i++)
        {
            int iDataSetId = getDataSetIdFromName(iFile, pstVarNameList[i]);
            if (iDataSetId == 0)
            {
                break;
            }

            strncpy(pInfo[i].varName, pstVarNameList[i], sizeof(pInfo[i].varName) - 1);
            pInfo[i].iSize = 0;
            b = read_data(iDataSetId, 0, NULL, &pInfo[i]) == false;
            if (b)
            {
                break;
            }

            if (Lhs == 1)
            {
                sciprint("%s\n", pInfo[i].pstInfo);
            }
        }

        freeArrayOfString(pstVarNameList, iNbItem);
    }
    else
    {
        //no variable returms [] for each Lhs
        for (int i = 0 ; i < Lhs ; i++)
        {
            createEmptyMatrix(pvApiCtx, nbIn + i + 1);
            AssignOutputVariable(pvApiCtx, i + 1) = nbIn + i + 1;
        }

        ReturnArguments(pvApiCtx);
        return 0;
    }

    closeHDF5File(iFile);

    //1st Lhs
    char** pstVarName = (char**)MALLOC(sizeof(char*) * iNbItem);
    for (int i = 0 ; i < iNbItem ; i++)
    {
        pstVarName[i] = pInfo[i].varName;
    }

    sciErr = createMatrixOfString(pvApiCtx, nbIn + 1, iNbItem, 1, pstVarName);
    FREE(pstVarName);
    if (sciErr.iErr)
    {
        FREE(pInfo);
        printError(&sciErr, 0);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;

    if (Lhs > 1)
    {
        //2nd Lhs
        double* pdblType;
        sciErr = allocMatrixOfDouble(pvApiCtx, nbIn + 2, iNbItem, 1, &pdblType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            FREE(pInfo);
            return 1;
        }

        for (int i = 0 ; i < iNbItem ; i++)
        {
            pdblType[i] = pInfo[i].iType;
        }

        AssignOutputVariable(pvApiCtx, 2) = nbIn + 2;

        if (Lhs > 2)
        {
            //3rd Lhs
            int* pList = NULL;
            sciErr = createList(pvApiCtx, nbIn + 3, iNbItem, &pList);
            for (int i = 0 ; i < iNbItem ; i++)
            {
                double* pdblDims = NULL;
                allocMatrixOfDoubleInList(pvApiCtx, nbIn + 3, pList, i + 1, 1, pInfo[i].iDims, &pdblDims);
                for (int j = 0 ; j < pInfo[i].iDims ; j++)
                {
                    pdblDims[j] = pInfo[i].piDims[j];
                }
            }

            AssignOutputVariable(pvApiCtx, 3) = nbIn + 3;
        }

        if (Lhs > 3)
        {
            //4th Lhs
            double* pdblSize = NULL;
            sciErr = allocMatrixOfDouble(pvApiCtx, nbIn + 4, iNbItem, 1, &pdblSize);
            for (int i = 0 ; i < iNbItem ; i++)
            {
                pdblSize[i] = pInfo[i].iSize;
            }

            AssignOutputVariable(pvApiCtx, 4) = nbIn + 4;
        }

    }

    FREE(pInfo);
    ReturnArguments(pvApiCtx);
    return 0;
}

static bool read_data(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    bool bRet = false;

    _pInfo->iType = getScilabTypeFromDataSet(_iDatasetId);
    switch (_pInfo->iType)
    {
        case sci_matrix:
        {
            bRet = read_double(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_strings:
        {
            bRet = read_string(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_list:
        case sci_tlist:
        case sci_mlist:
        {
            bRet = read_list(_iDatasetId, _pInfo->iType, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_boolean:
        {
            bRet = read_boolean(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_poly:
        {
            bRet = read_poly(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_ints:
        {
            bRet = read_integer(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_sparse:
        {
            bRet = read_sparse(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_boolean_sparse:
        {
            bRet = read_boolean_sparse(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_void:             //void item only on list variable
        {
            bRet = read_void(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_undefined:        //undefined item only on list variable
        {
            bRet = read_undefined(_iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        default:
        {
            Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), "listvar_in_hdf5");
            break;
        }
    }

    return bRet;
}

static bool read_double(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iSize = 0;
    int iComplex = 0;

    iSize = getDatasetInfo(_iDatasetId, &iComplex, &_pInfo->iDims, _pInfo->piDims);
    _pInfo->iSize = (2 + (iSize * (iComplex + 1))) * 8;

    generateInfo(_pInfo, "constant");
    closeDataSet(_iDatasetId);
    return true;
}

static bool read_string(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iSize = 0;
    int iComplex = 0;
    char** pstData = NULL;

    iSize = getDatasetInfo(_iDatasetId, &iComplex, &_pInfo->iDims, _pInfo->piDims);
    if (iSize < 0)
    {
        return false;
    }

    pstData = (char **)MALLOC(iSize * sizeof(char *));
    iRet = readStringMatrix(_iDatasetId, pstData);


    for (int i = 0 ; i < _pInfo->piDims[0] * _pInfo->piDims[1] ; i++)
    {
        _pInfo->iSize += (int)strlen(pstData[i]) * 4;
    }


    freeStringMatrix(_iDatasetId, pstData);
    FREE(pstData);
    //always full double size
    _pInfo->iSize += (8 - (_pInfo->iSize % 8));
    //header + offset
    _pInfo->iSize += 16 + (1 + iSize) * 4;

    generateInfo(_pInfo, "string");
    return true;
}

static bool read_boolean(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iSize = 0;
    int iComplex = 0;

    iSize = getDatasetInfo(_iDatasetId, &iComplex, &_pInfo->iDims, _pInfo->piDims);
    _pInfo->iSize = (3 + iSize) * 4;

    generateInfo(_pInfo, "boolean");
    closeDataSet(_iDatasetId);
    return true;
}

static bool read_integer(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iPrec = 0;
    int iSize = 0;
    int iComplex = 0;

    iSize = getDatasetInfo(_iDatasetId, &iComplex, &_pInfo->iDims, _pInfo->piDims);
    getDatasetPrecision(_iDatasetId, &iPrec);

    _pInfo->iSize = 16 + iSize * (iPrec % 10);

    generateInfo(_pInfo, "integer");
    closeDataSet(_iDatasetId);
    return true;
}

static bool read_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int iComplex = 0;

    iRet = getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 20 + iRows * 4 + iNbItem * 4 + (iNbItem * (iComplex + 1) * 8);

    generateInfo(_pInfo, "sparse");
    closeDataSet(_iDatasetId);
    return true;
}

static bool read_boolean_sparse(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int iComplex = 0;

    iRet = getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 20 + iRows * 4 + iNbItem * 4;

    generateInfo(_pInfo, "boolean sparse");
    closeDataSet(_iDatasetId);
    return true;
}

static bool read_poly(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iComplex = 0;
    char pstVarName[64] = { 0 };
    double **pdblReal = NULL;
    double **pdblImg = NULL;
    int *piNbCoef = NULL;
    int iSize = 0;

    iSize = getDatasetInfo(_iDatasetId, &iComplex, &_pInfo->iDims, _pInfo->piDims);
    if (iSize < 0)
    {
        return false;
    }
    _pInfo->iSize = 8 * 4 + (iSize + 1) * 4;

    if (iComplex)
    {
        piNbCoef = (int *)MALLOC(iSize * sizeof(int));
        pdblReal = (double **)MALLOC(iSize * sizeof(double *));
        pdblImg = (double **)MALLOC(iSize * sizeof(double *));
        iRet = readPolyComplexMatrix(_iDatasetId, pstVarName, 2, _pInfo->piDims, piNbCoef, pdblReal, pdblImg);
    }
    else
    {
        piNbCoef = (int *)MALLOC(iSize * sizeof(int));
        pdblReal = (double **)MALLOC(iSize * sizeof(double *));
        iRet = readPolyMatrix(_iDatasetId, pstVarName, 2, _pInfo->piDims, piNbCoef, pdblReal);
    }

    for (int i = 0 ; i < iSize ; i++)
    {
        _pInfo->iSize += piNbCoef[i] * 8 * (iComplex + 1);
        FREE(pdblReal[i]);
        if (iComplex)
        {
            FREE(pdblImg[i]);
        }
    }

    FREE(piNbCoef);
    FREE(pdblReal);
    if (iComplex)
    {
        FREE(pdblImg);
    }

    generateInfo(_pInfo, "polynomial");
    return true;
}

static bool read_list(int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    int iRet = 0;
    int iItems = 0;
    int *piListAddr = NULL;
    hobj_ref_t *piItemRef = NULL;

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
    //_pInfo = (VarInfo*)MALLOC(sizeof(VarInfo));
    _pInfo->iDims = 1;
    _pInfo->piDims[0] = iItems;
    _pInfo->iSize = (2 + iItems + 1) * 4;

    for (int i = 0; i < iItems; i++)
    {
        int iItemDataset = 0;

        iRet = getListItemDataset(_iDatasetId, piItemRef, i, &iItemDataset);
        if (iRet || iItemDataset == 0)
        {
            return false;
        }
        VarInfo info;
        bool bRet = read_data(iItemDataset, i + 1, piListAddr, &info);
        if (bRet == false)
        {
            return false;
        }

        _pInfo->iSize += info.iSize;
    }

    if (_iVarType == sci_list)
    {
        generateInfo(_pInfo, "list");
    }
    else if (_iVarType == sci_tlist)
    {
        generateInfo(_pInfo, "tlist");
    }
    else if (_iVarType == sci_mlist)
    {
        generateInfo(_pInfo, "mlist");
    }

    iRet = deleteListItemReferences(_iDatasetId, piItemRef);
    if (iRet)
    {
        return false;
    }


    return true;
}

static bool read_void(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    _pInfo->iSize = 1;
    closeDataSet(_iDatasetId);
    return true;
}

static bool read_undefined(int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo* _pInfo)
{
    _pInfo->iSize = 1;
    closeDataSet(_iDatasetId);
    return true;
}

static void generateInfo(VarInfo* _pInfo, const char* _pstType)
{
    char pstSize[17];

    if (_pInfo->iDims == 2)
    {
        sprintf(pstSize, "%d by %d", _pInfo->piDims[0], _pInfo->piDims[1]);
    }
    else if (_pInfo->iDims == 1)
    {
        sprintf(pstSize, "%d", _pInfo->piDims[0]);
    }
    else
    {
        pstSize[0] = '\0';
    }

    sprintf(_pInfo->pstInfo, "%-*s%-*s%-*s%-*d", 25, _pInfo->varName, 15, _pstType, 16, pstSize, 10, _pInfo->iSize);
}
