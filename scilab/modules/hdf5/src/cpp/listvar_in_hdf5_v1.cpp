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
#include "h5_readDataFromFile_v1.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
}

#include <vector>

typedef struct __VAR_INFO_V1__
{
    char pstInfo[128];
    char varName[128];
    int iType;
    int iSize;
    int iDims;
    int piDims[2];

    __VAR_INFO_V1__() : iType(0), iSize(0), iDims(0) {}
} VarInfo_v1;

static bool read_data_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_double_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_string_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_boolean_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_integer_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_boolean_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_poly_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_list_v1(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_void_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);
static bool read_undefined_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo);

static void generateInfo_v1(VarInfo_v1* _pInfo, const char* _pstType);

int sci_listvar_in_hdf5_v1(char *fname, int* pvCtx)
{
    SciErr sciErr;
    int *piAddr     = NULL;
    char* pstFile   = NULL;
    int iFile       = 0;
    int iNbItem     = 0;
    VarInfo_v1* pInfo  = NULL;

    CheckInputArgument(pvCtx, 1, 1);
    CheckOutputArgument(pvCtx, 1, 4);

    sciErr = getVarAddressFromPosition(pvCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvCtx, piAddr, &pstFile))
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
        FREE(pstFileName);
        Scierror(999, _("%s: Unable to open file: %s\n"), fname, pstFile);
        FREE(pstFile);
        return 1;
    }
    FREE(pstFileName);
    FREE(pstFile);

    iNbItem = getVariableNames_v1(iFile, NULL);
    if (iNbItem != 0)
    {
        char** pstVarNameList = (char**)MALLOC(sizeof(char*) * iNbItem);
        bool b;
        pInfo = (VarInfo_v1*)MALLOC(iNbItem * sizeof(VarInfo_v1));

        if (nbOutputArgument(pvCtx) == 1)
        {
            sciprint("Name                     Type           Size            Bytes\n");
            sciprint("---------------------------------------------------------------\n");
        }

        iNbItem = getVariableNames_v1(iFile, pstVarNameList);
        for (int i = 0; i < iNbItem; i++)
        {
            int iDataSetId = getDataSetIdFromName_v1(iFile, pstVarNameList[i]);
            if (iDataSetId == 0)
            {
                break;
            }

            strncpy(pInfo[i].varName, pstVarNameList[i], sizeof(pInfo[i].varName));
            b = read_data_v1(pvCtx, iDataSetId, 0, NULL, &pInfo[i]) == false;
            closeDataSet_v1(iDataSetId);

            if (b)
            {
                break;
            }

            if (nbOutputArgument(pvCtx) == 1)
            {
                sciprint("%s\n", pInfo[i].pstInfo);
            }
        }

        freeArrayOfString(pstVarNameList, iNbItem);
    }
    else
    {
        //no variable returms [] for each Lhs
        for (int i = 0 ; i < nbOutputArgument(pvCtx) ; i++)
        {
            createEmptyMatrix(pvCtx, nbInputArgument(pvCtx) + i + 1);
            AssignOutputVariable(pvCtx, i + 1) = nbInputArgument(pvCtx) + i + 1;
        }

        ReturnArguments(pvCtx);
        return 0;
    }

    closeHDF5File(iFile);

    //1st Lhs
    char** pstVarName = (char**)MALLOC(sizeof(char*) * iNbItem);
    for (int i = 0 ; i < iNbItem ; i++)
    {
        pstVarName[i] = pInfo[i].varName;
    }

    sciErr = createMatrixOfString(pvCtx, nbInputArgument(pvCtx) + 1, iNbItem, 1, pstVarName);
    FREE(pstVarName);
    if (sciErr.iErr)
    {
        FREE(pInfo);
        printError(&sciErr, 0);
        return 1;
    }

    AssignOutputVariable(pvCtx, 1) = nbInputArgument(pvCtx) + 1;

    if (nbOutputArgument(pvCtx) > 1)
    {
        //2nd Lhs
        double* pdblType;
        sciErr = allocMatrixOfDouble(pvCtx, nbInputArgument(pvCtx) + 2, iNbItem, 1, &pdblType);
        if (sciErr.iErr)
        {
            FREE(pInfo);
            printError(&sciErr, 0);
            return 1;
        }

        for (int i = 0 ; i < iNbItem ; i++)
        {
            pdblType[i] = pInfo[i].iType;
        }

        AssignOutputVariable(pvCtx, 2) = nbInputArgument(pvCtx) + 2;

        if (nbOutputArgument(pvCtx) > 2)
        {
            //3rd Lhs
            int* pList = NULL;
            sciErr = createList(pvCtx, nbInputArgument(pvCtx) + 3, iNbItem, &pList);
            for (int i = 0 ; i < iNbItem ; i++)
            {
                double* pdblDims = NULL;
                allocMatrixOfDoubleInList(pvCtx, nbInputArgument(pvCtx) + 3, pList, i + 1, 1, pInfo[i].iDims, &pdblDims);
                for (int j = 0 ; j < pInfo[i].iDims ; j++)
                {
                    pdblDims[j] = pInfo[i].piDims[j];
                }
            }

            AssignOutputVariable(pvCtx, 3) = nbInputArgument(pvCtx) + 3;
        }

        if (nbOutputArgument(pvCtx) > 3)
        {
            //4th Lhs
            double* pdblSize;
            sciErr = allocMatrixOfDouble(pvCtx, nbInputArgument(pvCtx) + 4, iNbItem, 1, &pdblSize);
            for (int i = 0 ; i < iNbItem ; i++)
            {
                pdblSize[i] = pInfo[i].iSize;
            }

            AssignOutputVariable(pvCtx, 4) = nbInputArgument(pvCtx) + 4;
        }

    }

    FREE(pInfo);
    ReturnArguments(pvCtx);
    return 0;
}

static bool read_data_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    bool bRet = false;

    _pInfo->iType = getScilabTypeFromDataSet_v1(_iDatasetId);
    switch (_pInfo->iType)
    {
        case sci_matrix:
        {
            bRet = read_double_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_strings:
        {
            bRet = read_string_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_list:
        case sci_tlist:
        case sci_mlist:
        {
            bRet = read_list_v1(pvCtx, _iDatasetId, _pInfo->iType, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_boolean:
        {
            bRet = read_boolean_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_poly:
        {
            bRet = read_poly_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_ints:
        {
            bRet = read_integer_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_sparse:
        {
            bRet = read_sparse_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_boolean_sparse:
        {
            bRet = read_boolean_sparse_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_void:             //void item only on list variable
        {
            bRet = read_void_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
            break;
        }
        case sci_undefined:        //undefined item only on list variable
        {
            bRet = read_undefined_v1(pvCtx, _iDatasetId, _iItemPos, _piAddress, _pInfo);
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

static bool read_double_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    iComplex = isComplexData_v1(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = (2 + (iRows * iCols * (iComplex + 1))) * 8;

    generateInfo_v1(_pInfo, "constant");
    return true;
}

static bool read_string_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    char** pstData = NULL;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;

    pstData = (char **)MALLOC(iRows * iCols * sizeof(char *));
    memset(pstData, 0x00, iRows * iCols * sizeof(char *));
    iRet = readStringMatrix_v1(_iDatasetId, iRows, iCols, pstData);

    for (int i = 0 ; i < iRows * iCols ; i++)
    {
        _pInfo->iSize += (int)strlen(pstData[i]) * 4;
        FREE(pstData[i]);
    }

    FREE(pstData);
    //always full double size
    _pInfo->iSize += (8 - (_pInfo->iSize % 8));
    //header + offset
    _pInfo->iSize += 16 + (1 + iRows * iCols) * 4;

    generateInfo_v1(_pInfo, "string");
    return true;
}

static bool read_boolean_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = (3 + iRows * iCols) * 4;

    generateInfo_v1(_pInfo, "boolean");
    return true;
}

static bool read_integer_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iPrec = 0;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    iRet = getDatasetPrecision_v1(_iDatasetId, &iPrec);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 16 + iRows * iCols * (iPrec % 10);

    generateInfo_v1(_pInfo, "integer");
    return true;
}

static bool read_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int iComplex = 0;

    iRet = getSparseDimension_v1(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData_v1(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 20 + iRows * 4 + iNbItem * 4 + (iNbItem * (iComplex + 1) * 8);

    generateInfo_v1(_pInfo, "sparse");
    return true;
}

static bool read_boolean_sparse_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iNbItem = 0;
    int iComplex = 0;

    iRet = getSparseDimension_v1(_iDatasetId, &iRows, &iCols, &iNbItem);
    if (iRet)
    {
        return false;
    }

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 20 + iRows * 4 + iNbItem * 4;

    generateInfo_v1(_pInfo, "boolean sparse");
    return true;
}

static bool read_poly_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iComplex = 0;
    char pstVarName[64] = { 0 };
    double **pdblReal = NULL;
    double **pdblImg = NULL;
    int *piNbCoef = NULL;

    iRet = getDatasetDims_v1(_iDatasetId, &iRows, &iCols);
    if (iRet)
    {
        return false;
    }

    iComplex = isComplexData_v1(_iDatasetId);

    _pInfo->iDims = 2;
    _pInfo->piDims[0] = iRows;
    _pInfo->piDims[1] = iCols;
    _pInfo->iSize = 8 * 4 + (iRows * iCols + 1) * 4;

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

    for (int i = 0 ; i < iRows * iCols ; i++)
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

    generateInfo_v1(_pInfo, "polynomial");
    return true;
}

static bool read_list_v1(int* pvCtx, int _iDatasetId, int _iVarType, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    int iRet = 0;
    int iItems = 0;
    int *piListAddr = NULL;
    hobj_ref_t *piItemRef = NULL;

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
    //_pInfo = (VarInfo_v1*)MALLOC(sizeof(VarInfo));
    _pInfo->iDims = 1;
    _pInfo->piDims[0] = iItems;
    _pInfo->iSize = (2 + iItems + 1) * 4;

    for (int i = 0; i < iItems; i++)
    {
        int iItemDataset = 0;

        iRet = getListItemDataset_v1(_iDatasetId, piItemRef, i, &iItemDataset);
        if (iRet || iItemDataset == 0)
        {
            return false;
        }
        VarInfo_v1 info;
        bool bRet = read_data_v1(pvCtx, iItemDataset, i + 1, piListAddr, &info);
        if (bRet == false)
        {
            return false;
        }

        _pInfo->iSize += info.iSize;
    }

    if (_iVarType == sci_list)
    {
        generateInfo_v1(_pInfo, "list");
    }
    else if (_iVarType == sci_tlist)
    {
        generateInfo_v1(_pInfo, "tlist");
    }
    else if (_iVarType == sci_mlist)
    {
        generateInfo_v1(_pInfo, "mlist");
    }

    return true;
}

static bool read_void_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    _pInfo->iSize = 1;
    return true;
}

static bool read_undefined_v1(int* pvCtx, int _iDatasetId, int _iItemPos, int *_piAddress, VarInfo_v1* _pInfo)
{
    _pInfo->iSize = 1;
    return true;
}

static void generateInfo_v1(VarInfo_v1* _pInfo, const char* _pstType)
{
    char pstSize[17];

    if (_pInfo->iDims == 2)
    {
        sprintf(pstSize, "%d by %d", _pInfo->piDims[0], _pInfo->piDims[1]);
    }
    else
    {
        sprintf(pstSize, "%d", _pInfo->piDims[0]);
    }
    sprintf(_pInfo->pstInfo, "%-*s%-*s%-*s%-*d", 25, _pInfo->varName, 15, _pstType, 16, pstSize, 10, _pInfo->iSize);
}

