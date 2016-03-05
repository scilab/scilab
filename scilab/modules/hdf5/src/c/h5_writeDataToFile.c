/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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

#define H5_USE_16_API

#include <hdf5.h>
#include <sci_malloc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sci_types.h"
#include "version.h"
#include "core_math.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"
#include "doublecomplex.h"

static hid_t enableCompression(int _iLevel, int _iRank, const hsize_t * _piDims)
{
    /*hid_t iRet = 0;
    int iLevel = _iLevel;*/

    return H5P_DEFAULT;
    /*
      if(iLevel < 0)
        {
            iLevel = 0;
        }

      if(iLevel > 9)
        {
            iLevel = 9;
        }

        if(iLevel)
        {
            iRet = H5Pcreate(H5P_DATASET_CREATE);
            if(iRet < 0)
            {
                iRet = 0;
            }
            else
            {
                if(H5Pset_layout(iRet,H5D_COMPACT)<0)
                {
                    H5Pclose(iRet);
                    iRet = 0;
                }
                else
                {
                    if(H5Pset_chunk(iRet,_iRank, _piDims)<0)
                    {
                        H5Pclose(iRet);
                        iRet = 0;
                    }
                    else
                    {
                        if(H5Pset_deflate(iRet,iLevel)<0)
                        {
                            H5Pclose(iRet);
                            iRet = 0;
                        }
                    }
                }
            }
        }
        else
        {
            iRet = H5Pcopy(H5P_DEFAULT);
        }
        return iRet;
    */
}

static hsize_t* convertDims(int* _piRank, int* _piDims, int* _piSize)
{
    int iSize = 1;
    int i = 0;
    hsize_t* piDims = (hsize_t*)malloc(sizeof(hsize_t) **_piRank);
    for (i = 0 ; i < *_piRank ; i++)
    {
        //reverse dimensions to improve rendering in external tools
        piDims[i] = _piDims[*_piRank - 1 - i];
        iSize *= (int)piDims[i];
    }
    /*
     * Fix bug under Linux due to this HDF5 error:
     * HDF5-DIAG: Error detected in HDF5 (1.8.4-patch1) thread 140525686855488:
     *   #000: ../../../src/H5S.c line 1335 in H5Screate_simple(): zero sized dimension for non-unlimited dimension
     *     major: Invalid arguments to routine
     *     minor: Bad value
     */
    if (iSize == 0)
    {
        *_piRank = 0;
    }

    *_piSize = iSize;
    return piDims;
}

static herr_t addIntAttribute(int _iDatasetId, const char *_pstName, const int _iVal)
{
    hsize_t attributeDims[1] = { 1 };
    hid_t attributeTypeId, attributeSpace;
    herr_t status;

    //Create attribute dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    attributeSpace = H5Screate_simple(1, attributeDims, NULL);

    //Create the attribute and write it.
    attributeTypeId = H5Acreate(_iDatasetId, _pstName, H5T_NATIVE_INT, attributeSpace, H5P_DEFAULT, H5P_DEFAULT);
    if (attributeTypeId < 0)
    {
        return -1;
    }

    status = H5Awrite(attributeTypeId, H5T_NATIVE_INT, &_iVal);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Aclose(attributeTypeId);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(attributeSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

static herr_t addAttribute(int _iDatasetId, const char *_pstName, const char *_pstValue)
{
    hsize_t attributeDims[1] = { 1 };
    hid_t attributeTypeId, attributeSpace, attr;
    herr_t status;

    //Create attribute dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    attributeSpace = H5Screate_simple(1, attributeDims, NULL);

    //Create special attribute type
    attributeTypeId = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(attributeTypeId, strlen(_pstValue));
    if (status < 0)
    {
        return -1;
    }

    //Create the attribute and write it.
    attr = H5Acreate(_iDatasetId, _pstName, attributeTypeId, attributeSpace, H5P_DEFAULT, H5P_DEFAULT);
    if (attr < 0)
    {
        return -1;
    }

    status = H5Awrite(attr, attributeTypeId, _pstValue);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Aclose(attr);
    if (status < 0)
    {
        return -1;
    }

    status = H5Tclose(attributeTypeId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}


int updateScilabVersion(int _iFile)
{
    herr_t status;
    //try to read attribute
    char* pstScilabVersion = getScilabVersionAttribute(_iFile);
    if (pstScilabVersion)
    {
        FREE(pstScilabVersion);

        //delete before write
        status = H5Adelete(_iFile, g_SCILAB_CLASS_SCI_VERSION);
        if (status < 0)
        {
            return -1;
        }
    }

    if (strstr(SCI_VERSION_STRING, "branch"))
    {
        //compiled by user
        char pstVersion[64];
        sprintf(pstVersion, "%s %d.%d.%d", SCI_VERSION_STRING, SCI_VERSION_MAJOR, SCI_VERSION_MINOR, SCI_VERSION_MAINTENANCE);
        status = addAttribute(_iFile, g_SCILAB_CLASS_SCI_VERSION, pstVersion);
    }
    else
    {
        //compiled by compilation chain
        status = addAttribute(_iFile, g_SCILAB_CLASS_SCI_VERSION, SCI_VERSION_STRING);
    }

    return status;
}

int updateFileVersion(int _iFile)
{
    herr_t status;
    //try to read attribute
    int iHdf5Version = getSODFormatAttribute(_iFile);
    if (iHdf5Version != -1)
    {
        status = H5Adelete(_iFile, g_SCILAB_CLASS_SOD_VERSION);
        if (status < 0)
        {
            return -1;
        }
    }

    return addIntAttribute(_iFile, g_SCILAB_CLASS_SOD_VERSION, SOD_FILE_VERSION);
}

int writeStringMatrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, char **data)
{
    int iSize = 0;
    hsize_t* piDims = NULL;
    hid_t typeId = 0, space = 0, dset = 0;
    herr_t status = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);
    //Create string dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(_iDims, piDims, NULL);
    if (space < 0)
    {
        FREE(piDims);
        return -1;
    }

    //Create special string type
    typeId = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(typeId, H5T_VARIABLE);
    if (status < 0)
    {
        FREE(piDims);
        return -1;
    }

    //Create the data set and write it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(_iFile, _pstDatasetName, typeId, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, typeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = string to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_STRING);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Tclose(typeId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

char *createGroupName(const char *_pstGroupName)
{
    char *pstSlash = NULL;
    char *pstGroupName = (char *)MALLOC((strlen(_pstGroupName) + 3) * sizeof(char));

    // Generate groupname #<dataSetName>#
    sprintf(pstGroupName, "#%s#", _pstGroupName);
    pstSlash = strstr(pstGroupName, "/");
    if (pstSlash != NULL)
    {
        pstSlash[0] = '_';
    }

    return pstGroupName;
}

char* createPathName(char *_pstGroupName, int _iIndex)
{
    char *pstName = NULL;
    char *pstPathName = NULL;

    int iNameLen = (int)log10((double)_iIndex + 1) + 1;
    iNameLen += 2; //for both '#'
    iNameLen += 1; //for null termanation

    pstName = (char *)MALLOC(iNameLen * sizeof(char));
    //1 for null termination, 2 for '#' characters
    sprintf(pstName, "#%d#", _iIndex);

    pstPathName = (char *)MALLOC((strlen(_pstGroupName) + strlen(pstName) + 2) * sizeof(char));
    //1 for null termination, 1 for separator, 2 for '#' characters
    sprintf(pstPathName, "%s/%s", _pstGroupName, pstName);
    FREE(pstName);
    return pstPathName;
}

int writeVoid(int _iFile, char *_pstDatasetName)
{
    hsize_t piDims[1] = { 1 };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    char cData = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT8, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, &cData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_VOID);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeUndefined(int _iFile, char *_pstDatasetName)
{
    hsize_t piDims[1] = { 1 };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    char cData = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT8, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, &cData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_UNDEFINED);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeDoubleMatrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, double *_pdblData)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hsize_t *piDims = NULL;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int i = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    if (_iDims == 2 && piDims[0] == 0 && piDims[1] == 0)
    {
        // []
        space = H5Screate_simple(0, NULL, NULL);
        if (space < 0)
        {
            free(piDims);
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, _iDims, piDims);
        free(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_DOUBLE, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }

        //Add attribute SCILAB_Class = double to dataset
        status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
        if (status < 0)
        {
            return -1;
        }

        //Close and release resources.
        status = H5Dclose(dset);
        if (status < 0)
        {
            return -1;
        }

        status = H5Pclose(dprop);
        if (status < 0)
        {
            return -1;
        }

        status = H5Sclose(space);
        if (status < 0)
        {
            return -1;
        }
        return 0;
    }

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(_iDims, piDims, NULL);
    if (space < 0)
    {
        free(piDims);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    free(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_DOUBLE, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pdblData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return status;
}

int writeDoubleComplexMatrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, double *_pdblReal, double *_pdblImg)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hsize_t *piDims = NULL;
    hid_t iCompress = 0;
    hid_t compoundId = 0;
    hid_t dprop = 0;
    int iSize = 1;
    doublecomplex* pData = NULL;

    //create sub group only for non empty matrix
    if (_iDims == 2 && _piDims[0] == 0 && _piDims[1] == 0)
    {
        // [] complex
        //a revoir
        return -1;
    }

    compoundId = H5Tcreate (H5T_COMPOUND, sizeof(doublecomplex));
    H5Tinsert(compoundId, "real", HOFFSET(doublecomplex, r), H5T_NATIVE_DOUBLE);
    H5Tinsert(compoundId, "imag", HOFFSET(doublecomplex, i), H5T_NATIVE_DOUBLE);
    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(_iDims, piDims, NULL);
    if (space < 0)
    {
        free(piDims);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    free(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(_iFile, _pstDatasetName, compoundId, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    //convert double data doublecomplex data
    pData = oGetDoubleComplexFromPointer(_pdblReal, _pdblImg, iSize);
    status = H5Dwrite(dset, compoundId, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);
    FREE(pData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return status;
}

int writeBooleanMatrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, int *_piData)
{
    int iSize = 0;
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iCompress = 0;
    hid_t iDataset = 0;
    hid_t dprop = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, _piData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_BOOLEAN);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

static int writeCommonPolyMatrix(int _iFile, char *_pstDatasetName, char *_pstVarName, int _iComplex, int _iDims, int* _piDims, int *_piNbCoef, double **_pdblReal, double **_pdblImg)
{
    int i = 0;
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t space = 0;
    hid_t dset = 0;
    hid_t group = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    hobj_ref_t *pData = 0;
    int iSize = 0;

    char *pstPathName = NULL;
    char *pstGroupName = NULL;

    piDims = convertDims(&_iDims, _piDims, &iSize);
    // Create ref matrix
    pData = (hobj_ref_t *)MALLOC(iSize * sizeof(hobj_ref_t));

    // Generate groupname #<dataSetName>#
    pstGroupName = createGroupName(_pstDatasetName);

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Gclose(group);

    //Now create each String as a dedicated DataSet.
    for (i = 0 ; i < iSize ; i++)
    {
        pstPathName = createPathName(pstGroupName, i);

        // Write the string to ref
        if (_iComplex)
        {
            status = writeDoubleComplexMatrix(_iFile, pstPathName, 1, &_piNbCoef[i], _pdblReal[i], _pdblImg[i]);
        }
        else
        {
            status = writeDoubleMatrix(_iFile, pstPathName, 1, &_piNbCoef[i], _pdblReal[i]);
        }

        if (status < 0)
        {
            FREE(pstPathName);
            FREE(pstGroupName);
            FREE(pData);
            FREE(piDims);
            return -1;
        }

        // create the ref
        status = H5Rcreate(&pData[i], _iFile, pstPathName, H5R_OBJECT, -1);
        if (status < 0)
        {
            FREE(pstPathName);
            FREE(pstGroupName);
            FREE(pData);
            FREE(piDims);
            return -1;
        }

        FREE(pstPathName);
    }

    FREE(pstGroupName);
    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(_iDims, piDims, NULL);
    if (status < 0)
    {
        FREE(piDims);
        FREE(pData);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        FREE(pData);
        return -1;
    }

    status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);
    if (status < 0)
    {
        FREE(pData);
        return -1;
    }

    FREE(pData);
    //Add attribute SCILAB_Class = poly to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_POLY);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute Varname attribute to dataset
    status = addAttribute(dset, g_SCILAB_CLASS_VARNAME, _pstVarName);
    if (status < 0)
    {
        return -1;
    }

    if (_iComplex)
    {
        //Add attribute Varname attribute to dataset
        status = addAttribute(dset, g_SCILAB_CLASS_COMPLEX, "true");
        if (status < 0)
        {
            return -1;
        }
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writePolyMatrix(int _iFile, char *_pstDatasetName, char *_pstVarName, int _iDims, int* _piDims, int *_piNbCoef, double **_pdblReal)
{
    return writeCommonPolyMatrix(_iFile, _pstDatasetName, _pstVarName, 0, _iDims, _piDims, _piNbCoef, _pdblReal, NULL);
}

int writePolyComplexMatrix(int _iFile, char *_pstDatasetName, char *_pstVarName, int _iDims, int* _piDims, int *_piNbCoef, double **_pdblReal,
                           double **_pdblImg)
{
    return writeCommonPolyMatrix(_iFile, _pstDatasetName, _pstVarName, 1, _iDims, _piDims, _piNbCoef, _pdblReal, _pdblImg);
}

int writeInteger8Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, char *_pcData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT8, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pcData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "8");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeInteger16Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, short *_psData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT16, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }
    status = H5Dwrite(iDataset, H5T_NATIVE_INT16, H5S_ALL, H5S_ALL, H5P_DEFAULT, _psData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "16");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeInteger32Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, int *_piData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, piDims);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT32, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, _piData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "32");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeInteger64Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, long long *_pllData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT64, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pllData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "64");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeUnsignedInteger8Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, unsigned char *_pucData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT8, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_UINT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pucData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "u8");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeUnsignedInteger16Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, unsigned short *_pusData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT16, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_UINT16, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pusData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "u16");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeUnsignedInteger32Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, unsigned int *_puiData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT32, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, _puiData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "u32");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeUnsignedInteger64Matrix(int _iFile, char *_pstDatasetName, int _iDims, int* _piDims, unsigned long long *_pullData)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int iSize = 0;

    piDims = convertDims(&_iDims, _piDims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(_iDims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, _iDims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT64, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_UINT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pullData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(iDataset, g_SCILAB_CLASS_PREC, "u64");
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeCommonSparseComplexMatrix(int _iFile, char *_pstDatasetName, int _iComplex, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow,
                                   int *_piColPos, double *_pdblReal, double *_pdblImg)
{
    hsize_t dims[1] = { 3 };
    herr_t status = 0;
    hid_t space = 0;
    hid_t dset = 0;
    hid_t group = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    hobj_ref_t pDataRef[3] = {0};

    char *pstRowPath = NULL;
    char *pstColPath = NULL;
    char *pstDataPath = NULL;
    char *pstGroupName = NULL;

    // Generate groupname #<dataSetName>#
    pstGroupName = createGroupName(_pstDatasetName);

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Gclose(group);
    if (status < 0)
    {
        FREE(pstGroupName);
        return -1;
    }

    //Create each sub dataset and insert data
    pstRowPath = createPathName(pstGroupName, 0);
    status = writeInteger32Matrix(_iFile, pstRowPath, 1, &_iRows, _piNbItemRow);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstGroupName);
        return -1;
    }

    status = H5Rcreate(&pDataRef[0], _iFile, pstRowPath, H5R_OBJECT, -1);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstGroupName);
        return -1;
    }

    pstColPath = createPathName(pstGroupName, 1);
    status = writeInteger32Matrix(_iFile, pstColPath, 1, &_iNbItem, _piColPos);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstColPath);
        FREE(pstGroupName);
        return -1;
    }

    status = H5Rcreate(&pDataRef[1], _iFile, pstColPath, H5R_OBJECT, -1);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstColPath);
        FREE(pstGroupName);
        return -1;
    }

    pstDataPath = createPathName(pstGroupName, 2);
    if (_iComplex)
    {
        status = writeDoubleComplexMatrix(_iFile, pstDataPath, 1, &_iNbItem, _pdblReal, _pdblImg);
    }
    else
    {
        status = writeDoubleMatrix(_iFile, pstDataPath, 1, &_iNbItem, _pdblReal);
    }

    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstColPath);
        FREE(pstDataPath);
        FREE(pstGroupName);
        return -1;
    }

    status = H5Rcreate(&pDataRef[2], _iFile, pstDataPath, H5R_OBJECT, -1);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstColPath);
        FREE(pstDataPath);
        FREE(pstGroupName);
        return -1;
    }

    //FREE group names
    FREE(pstRowPath);
    FREE(pstColPath);
    FREE(pstDataPath);
    FREE(pstGroupName);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(1, dims, NULL);
    if (space < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, dims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pDataRef);
    if (status < 0)
    {
        return -1;
    }
    //Add attribute SCILAB_Class = poly to dataset
    //sprintf(pstRow, "%d", _iRows);
    //sprintf(pstCol, "%d", _iCols);
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_SPARSE);
    if (status < 0)
    {
        return -1;
    }

    status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
    if (status < 0)
    {
        return -1;
    }

    status = addIntAttribute(dset, g_SCILAB_CLASS_ITEMS, _iNbItem);
    if (status < 0)
    {
        return -1;
    }

    if (_iComplex)
    {
        //Add attribute Varname attribute to dataset
        status = addAttribute(dset, g_SCILAB_CLASS_COMPLEX, "true");
        if (status < 0)
        {
            return -1;
        }
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeSparseMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos, double *_pdblReal)
{
    return writeCommonSparseComplexMatrix(_iFile, _pstDatasetName, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

int writeSparseComplexMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos,
                             double *_pdblReal, double *_pdblImg)
{
    return writeCommonSparseComplexMatrix(_iFile, _pstDatasetName, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

int writeBooleanSparseMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos)
{
    hsize_t dims[1] = { 2 };
    herr_t status = 0;
    hid_t space = 0;
    hid_t dset = 0;
    hid_t group = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    hobj_ref_t pDataRef[2] = {0};

    char *pstRowPath = NULL;
    char *pstColPath = NULL;
    char *pstGroupName = NULL;

    // Generate groupname #<dataSetName>#
    pstGroupName = createGroupName(_pstDatasetName);

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Gclose(group);
    if (status < 0)
    {
        FREE(pstGroupName);
        return -1;
    }

    //Create each sub dataset and insert data
    pstRowPath = createPathName(pstGroupName, 0);
    status = writeInteger32Matrix(_iFile, pstRowPath, 1, &_iRows, _piNbItemRow);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstGroupName);
        return -1;
    }

    status = H5Rcreate(&pDataRef[0], _iFile, pstRowPath, H5R_OBJECT, -1);
    if (status < 0)
    {
        FREE(pstRowPath);
        FREE(pstGroupName);
        return -1;
    }

    pstColPath = createPathName(pstGroupName, 1);
    if (_iNbItem != 0)
    {
        status = writeInteger32Matrix(_iFile, pstColPath, 1, &_iNbItem, _piColPos);
        if (status < 0)
        {
            FREE(pstRowPath);
            FREE(pstColPath);
            FREE(pstGroupName);
            return -1;
        }

        status = H5Rcreate(&pDataRef[1], _iFile, pstColPath, H5R_OBJECT, -1);
        if (status < 0)
        {
            FREE(pstRowPath);
            FREE(pstColPath);
            FREE(pstGroupName);
            return -1;
        }
    }
    else
    {
        dims[0] = 1;
    }


    //FREE group names
    FREE(pstRowPath);
    FREE(pstColPath);
    FREE(pstGroupName);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(1, dims, NULL);
    if (space < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, dims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pDataRef);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = boolean sparse to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_BSPARSE);
    if (status < 0)
    {
        return -1;
    }

    status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
    if (status < 0)
    {
        return -1;
    }

    status = addIntAttribute(dset, g_SCILAB_CLASS_ITEMS, _iNbItem);
    if (status < 0)
    {
        return -1;
    }
    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

//create a group and create hobj_ref_t array
void *openList(int _iFile, char *pstDatasetName, int _iNbItem)
{
    herr_t status = 0;
    hid_t group = 0;
    hobj_ref_t *pobjArray = NULL;

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstDatasetName, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Gclose(group);
    if (status < 0)
    {
        return NULL;
    }

    if (_iNbItem)
    {
        pobjArray = (hobj_ref_t*)MALLOC(sizeof(hobj_ref_t) * _iNbItem);
    }

    return pobjArray;
}

int addItemInList(int _iFile, void *_pvList, int _iPos, char *_pstItemName)
{
    hobj_ref_t *pobjArray = (hobj_ref_t *) _pvList;

    return H5Rcreate(&pobjArray[_iPos], _iFile, _pstItemName, H5R_OBJECT, -1);
}

int closeList(int _iFile, void *_pvList, char *_pstListName, int _iNbItem, int _iVarType)
{
    herr_t status = 0;
    hsize_t dims[1] = { _iNbItem };
    hid_t space = 0;
    hid_t dset = 0;
    hid_t dprop = 0;
    hid_t iCompress = 0;
    const char *pcstClass = NULL;

    switch (_iVarType)
    {
        case sci_list:
            pcstClass = g_SCILAB_CLASS_LIST;
            break;
        case sci_tlist:
            pcstClass = g_SCILAB_CLASS_TLIST;
            break;
        case sci_mlist:
            pcstClass = g_SCILAB_CLASS_MLIST;
            break;
        default:
            return 1;
    }

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    if (_iNbItem == 0)
    {
        //tips for empty list
        //insert a fake refence in the array, value = 0

        hobj_ref_t pvList[1];

        pvList[0] = 0;
        //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.

        dims[0] = 1;
        space = H5Screate_simple(1, dims, NULL);
        if (space < 0)
        {
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, 1, dims);

        dset = H5Dcreate(_iFile, _pstListName, H5T_STD_REF_OBJ, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }

        status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, (hobj_ref_t *) pvList);
        if (status < 0)
        {
            return -1;
        }

        //Add attribute SCILAB_Class = string to dataset
        status = addAttribute(dset, g_SCILAB_CLASS, pcstClass);
        if (status < 0)
        {
            return -1;
        }

        status = addAttribute(dset, g_SCILAB_CLASS_EMPTY, "true");
        if (status < 0)
        {
            return -1;
        }
    }
    else
    {
        //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
        space = H5Screate_simple(1, dims, NULL);
        if (space < 0)
        {
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, 1, dims);

        dset = H5Dcreate(_iFile, _pstListName, H5T_STD_REF_OBJ, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }

        status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, (hobj_ref_t *) _pvList);
        if (status < 0)
        {
            return -1;
        }

        //Add attribute SCILAB_Class = string to dataset
        status = addAttribute(dset, g_SCILAB_CLASS, pcstClass);
        if (status < 0)
        {
            return -1;
        }

        status = addIntAttribute(dset, g_SCILAB_CLASS_ITEMS, _iNbItem);
        if (status < 0)
        {
            return -1;
        }
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    FREE(_pvList);
    return 0;
}

static int deleteHDF5group(int _iFile, const char* _pstName)
{
    H5G_info_t groupInfo;
    hid_t status = 0;
    //open group
    hid_t groupID = H5Gopen(_iFile, _pstName, H5P_DEFAULT);
    //hid_t groupID = H5Gopen(_iFile, _pstName, H5P_DEFAULT);

    if (groupID < 0)
    {
        return -1;
    }

    //get children count
    status = H5Gget_info(groupID, &groupInfo);
    if (status != -1)
    {
        int index = 0;
        int i = 0;

        //for each child,
        for (i = 0; i < groupInfo.nlinks; i++)
        {
            int ret = 0;
            //get child name
            ssize_t size = H5Lget_name_by_idx(groupID, ".", H5_INDEX_NAME, H5_ITER_INC, index, 0, 0, H5P_DEFAULT) + 1;
            char* pstChildName = (char*)MALLOC(sizeof(char) * size);
            H5Lget_name_by_idx(groupID, ".", H5_INDEX_NAME, H5_ITER_INC, index, pstChildName, size, H5P_DEFAULT);
            ret = deleteHDF5group(groupID, pstChildName);
            if (ret == -1)
            {
                ++index;
            }
            ////unlink child
            //status = H5Ldelete(groupID, pstChildName, H5P_DEFAULT);
            FREE(pstChildName);

            //if (status < 0)
            //{
            //    return -1;
            //}
        }
    }

    //close group
    status = closeList6(groupID);
    if (status < 0)
    {
        return -1;
    }

    //delete group
    status = H5Ldelete(_iFile, _pstName, H5P_DEFAULT);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

//According to 5.5.2. Deleting a Dataset from a File and Reclaiming Space of http://www.hdfgroup.org/HDF5/doc/UG/10_Datasets.html
//it is actually impossible to really remove data from HDF5 file so unlink dataset to main group
int deleteHDF5Var(int _iFile, const char* _pstName)
{
    int ret = 0;
    void *oldclientdata = NULL;
    H5E_auto2_t oldfunc;

    /* Save old error handler */
    H5Eget_auto2(H5E_DEFAULT, &oldfunc, &oldclientdata);

    /* Turn off error handling */
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);

    //try to unlink potential subgroups
    ret = deleteHDF5group(_iFile, _pstName);
    if (ret == -1)
    {
        //delete current dataset link
        hid_t status = H5Ldelete(_iFile, _pstName, H5P_DEFAULT);
        if (status < 0)
        {
            H5Eset_auto2(H5E_DEFAULT, oldfunc, oldclientdata);
            return -1;
        }
        ret = 0;
    }

    H5Eset_auto2(H5E_DEFAULT, oldfunc, oldclientdata);
    return ret;
}

/*Scilab 6*/
int writeDoubleMatrix6(int parent, const char* name, int dims, int* pdims, double* data)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hsize_t *piDims = NULL;
    hid_t iCompress = 0;
    int i = 0;
    int iSize = 0;
    hid_t dprop = 0;

    piDims = convertDims(&dims, pdims, &iSize);

    if (dims == 0 || (dims == 2 && pdims[0] == 0 && pdims[1] == 0))
    {
        // []
        space = H5Screate_simple(0, NULL, NULL);
        if (space < 0)
        {
            free(piDims);
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, dims, piDims);
        free(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(parent, name, H5T_NATIVE_DOUBLE, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }
    }
    else
    {
        //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
        space = H5Screate_simple(dims, piDims, NULL);
        if (space < 0)
        {
            free(piDims);
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, dims, piDims);
        free(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(parent, name, H5T_NATIVE_DOUBLE, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }

        status = H5Dwrite(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
        if (status < 0)
        {
            return -1;
        }
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return dset;
}

writeDoubleComplexMatrix6(int parent, const char* name, int dims, int* pdims, double* real, double* img)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hsize_t *piDims = NULL;
    hid_t iCompress = 0;
    hid_t compoundId = 0;
    hid_t dprop = 0;
    int iSize = 1;
    doublecomplex* pData = NULL;

    //create sub group only for non empty matrix
    if (dims == 2 && pdims[0] == 0 && pdims[1] == 0)
    {
        // [] complex
        //a revoir
        return -1;
    }

    compoundId = H5Tcreate(H5T_COMPOUND, sizeof(doublecomplex));
    H5Tinsert(compoundId, "real", HOFFSET(doublecomplex, r), H5T_NATIVE_DOUBLE);
    H5Tinsert(compoundId, "imag", HOFFSET(doublecomplex, i), H5T_NATIVE_DOUBLE);
    piDims = convertDims(&dims, pdims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(dims, piDims, NULL);
    if (space < 0)
    {
        free(piDims);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, dims, piDims);
    free(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(parent, name, compoundId, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    //convert double data doublecomplex data
    pData = oGetDoubleComplexFromPointer(real, img, iSize);
    status = H5Dwrite(dset, compoundId, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);
    FREE(pData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return dset;
}

int writeStringMatrix6(int parent, const char* name, int dims, int* pdims, char** data)
{
    int iSize = 0;
    hsize_t* piDims = NULL;
    hid_t typeId = 0, space = 0, dset = 0, dprop = 0;
    herr_t status;
    hid_t iCompress;

    piDims = convertDims(&dims, pdims, &iSize);

    //Create special string type
    typeId = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(typeId, H5T_VARIABLE);
    if (status < 0)
    {
        FREE(piDims);
        return -1;
    }

    if (dims == 0 || (dims == 2 && pdims[0] == 0 && pdims[1] == 0))
    {
        // []
        space = H5Screate_simple(0, NULL, NULL);
        if (space < 0)
        {
            free(piDims);
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, dims, piDims);
        free(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(parent, name, typeId, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }
    }
    else
    {
        //Create string dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
        space = H5Screate_simple(dims, piDims, NULL);
        if (space < 0)
        {
            FREE(piDims);
            return -1;
        }

        //Create the data set and write it.
        iCompress = enableCompression(9, dims, piDims);
        FREE(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(parent, name, typeId, space, iCompress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }

        status = H5Dwrite(dset, typeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
        if (status < 0)
        {
            return -1;
        }
    }

    //Add attribute SCILAB_Class = string to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_STRING);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Tclose(typeId);
    if (status < 0)
    {
        return -1;
    }

    return dset;
}

int writeBooleanMatrix6(int parent, const char* name, int dims, int* pdims, int* data)
{
    int iSize = 0;
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iCompress = 0;
    hid_t dset = 0;
    hid_t dprop = 0;

    piDims = convertDims(&dims, pdims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(dims, piDims, NULL);
    if (iSpace < 0)
    {
        FREE(piDims);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, dims, piDims);
    FREE(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(parent, name, H5T_NATIVE_INT, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_BOOLEAN);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return dset;
}

int writeIntegerMatrix6(int parent, const char* name, int type, const char* prec, int dims, int* pdims, void* data)
{
    hsize_t* piDims = NULL;
    herr_t status = 0;
    hid_t space = 0;
    hid_t dset = 0;
    hid_t compress = 0;
    int size = 0;
    hid_t dprop = 0;

    piDims = convertDims(&dims, pdims, &size);

    if (dims == 0 || (dims == 2 && pdims[0] == 0 && pdims[1] == 0))
    {
        // []
        space = H5Screate_simple(0, NULL, NULL);
        if (space < 0)
        {
            free(piDims);
            return -1;
        }

        //Create the dataset and write the array data to it.
        compress = enableCompression(9, dims, piDims);
        free(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(parent, name, H5T_NATIVE_DOUBLE, space, compress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }
    }
    else
    {
        //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
        space = H5Screate_simple(dims, piDims, NULL);
        if (space < 0)
        {
            FREE(piDims);
            return -1;
        }
        //Create the dataset and write the array data to it.
        compress = enableCompression(9, dims, piDims);
        FREE(piDims);

        dprop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_obj_track_times(dprop, 0);
        dset = H5Dcreate(parent, name, type, space, compress, dprop, H5P_DEFAULT);
        if (dset < 0)
        {
            return -1;
        }

        status = H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
        if (status < 0)
        {
            return -1;
        }
    }
    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_INT);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(dset, g_SCILAB_CLASS_PREC, prec);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int openList6(int parent, const char *name, const char* type)
{
    //First create a group to store all referenced objects.
    int group = H5Gcreate(parent, name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //Add attribute SCILAB_Class = string to dataset
    if (addAttribute(group, g_SCILAB_CLASS, type) < 0)
    {
        return -1;
    }

    return group;
}

int closeList6(int lst)
{
    if (H5Gclose(lst) < 0)
    {
        return -1;
    }

    return 0;
}

int addItemStruct6(int dataset, hobj_ref_t * refs, int pos, const char *name)
{
    herr_t status = H5Rcreate(&refs[pos], dataset, name, H5R_OBJECT, -1);
    if (status < 0)
    {
        return -1;
    }

    return status;
}

int writeStructField6(int parent, const char* name, int dims, int* pdims, hobj_ref_t * refs)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hsize_t *piDims = NULL;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    int i = 0;
    int iSize = 0;

    piDims = convertDims(&dims, pdims, &iSize);

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(dims, piDims, NULL);
    if (space < 0)
    {
        free(piDims);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, dims, piDims);
    free(piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    dset = H5Dcreate(parent, name, H5T_STD_REF_OBJ, space, iCompress, dprop, H5P_DEFAULT);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, refs);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return dset;
}

int writeVoid6(int parent, const char* name)
{
    hsize_t piDims[1] = {1};
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    char cData = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(parent, name, H5T_NATIVE_INT8, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, &cData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_VOID);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeUndefined6(int parent, const char* name)
{
    hsize_t piDims[1] = {1};
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;
    hid_t dprop = 0;
    char cData = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);

    dprop = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_obj_track_times(dprop, 0);
    iDataset = H5Dcreate(parent, name, H5T_NATIVE_INT8, iSpace, iCompress, dprop, H5P_DEFAULT);
    if (iDataset < 0)
    {
        return -1;
    }

    status = H5Dwrite(iDataset, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, &cData);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_UNDEFINED);
    if (status < 0)
    {
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(iDataset);
    if (status < 0)
    {
        return -1;
    }

    status = H5Pclose(dprop);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}
