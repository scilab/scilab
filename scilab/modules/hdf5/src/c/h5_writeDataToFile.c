/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#define H5_USE_16_API

#include <hdf5.h>
#include <MALLOC.h>
#include <math.h>
#include <string.h>
#include "sci_types.h"
#include "version.h"
#include "core_math.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"

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

static herr_t addIntAttribute(int _iDatasetId, const char *_pstName, const int _iVal)
{
    hsize_t attributeDims[1] = { 1 };
    hid_t attributeTypeId, attributeSpace;
    herr_t status;

    //Create attribute dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    attributeSpace = H5Screate_simple(1, attributeDims, NULL);

    //Create the attribute and write it.
    attributeTypeId = H5Acreate(_iDatasetId, _pstName, H5T_NATIVE_INT, attributeSpace, H5P_DEFAULT);
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
    attr = H5Acreate(_iDatasetId, _pstName, attributeTypeId, attributeSpace, H5P_DEFAULT);
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

int writeStringMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, char **data)
{
    int i;
    hsize_t dims[1] = { _iRows * _iCols };
    hsize_t subdims[1] = { 1 };
    hid_t typeId, space, dset, memspace;
    herr_t status;
    hid_t iCompress;
    size_t iMaxLen = 0;
    char *pstDataTemp = NULL;

    for (i = 0; i < _iRows * _iCols; i++)
    {
        iMaxLen = Max(iMaxLen, strlen(data[i]));
    }

    //Create string dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(1, dims, NULL);
    if (space < 0)
    {
        return -1;
    }

    //Create special string type
    typeId = H5Tcopy(H5T_C_S1);

    /* Bug 6474 */
    /* we allocate datas in fixed length string */
    /* workaround for memcpy in hdf5 with wrong size */
    pstDataTemp = MALLOC(sizeof(char) * (iMaxLen + 1));

    if (iMaxLen > 0)
    {
        status = H5Tset_size(typeId, iMaxLen);
        if (status < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }
    }

    //Create the data set and write it.
    iCompress = enableCompression(9, 1, dims);
    dset = H5Dcreate(_iFile, _pstDatasetName, typeId, space, iCompress);
    if (dset < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    for (i = 0; i < _iRows * _iCols; i++)
    {
        hssize_t start[1] = { i };
        hsize_t count[1] = { 1 };

        strcpy(pstDataTemp, data[i]);

        space = H5Dget_space(dset);
        if (space < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }

        status = H5Sselect_hyperslab(space, H5S_SELECT_SET, start, NULL, count, NULL);
        if (status < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }

        /*create sub space */
        memspace = H5Screate_simple(1, subdims, NULL);
        if (memspace < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }

        status = H5Dwrite(dset, typeId, memspace, space, H5P_DEFAULT, pstDataTemp);

        if (status < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }

        status = H5Sclose(space);
        if (status < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }

        status = H5Sclose(memspace);
        if (status < 0)
        {
            FREE(pstDataTemp);
            return -1;
        }

    }

    //Add attribute SCILAB_Class = string to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_STRING);
    if (status < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    //Add attribute SCILAB_rows = _iRows
    status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    //Add attribute SCILAB_cols = _iCols
    status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
    if (status < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    status = H5Tclose(typeId);
    if (status < 0)
    {
        FREE(pstDataTemp);
        return -1;
    }

    FREE(pstDataTemp);

    return 0;
}

char *createGroupName(char *_pstGroupName)
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
    char cData = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT8, iSpace, iCompress);
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
    char cData = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT8, iSpace, iCompress);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

static hobj_ref_t writeCommomDoubleMatrix(int _iFile, char *_pstGroupName, char *_pstDatasetName, int _iIndex, int _iRows, int _iCols,
        double *_pdblData)
{
    hid_t space;
    hid_t dset;
    hid_t iCompress = 0;
    hsize_t dims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hobj_ref_t iRef = 0;

    if (_iRows * _iCols == 0)
    {
        double dblZero = 0;

        //tips for empty double matrix

        //Create dataspace.  Setting maximum size to NULL sets the maximum
        //size to be the current size.
        dims[0] = 1;

        space = H5Screate_simple(1, dims, NULL);
        if (space < 0)
        {
            return (hobj_ref_t) (-1);
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, 1, dims);
        dset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_DOUBLE, space, iCompress);
        if (dset < 0)
        {
            return (hobj_ref_t) (-1);
        }

        status = H5Dwrite(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &dblZero);
        if (status < 0)
        {
            return (hobj_ref_t) (-1);
        }

        //Add attribute SCILAB_Class = double to dataset
        status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
        if (status < 0)
        {
            return (hobj_ref_t) (-1);
        }

        status = addAttribute(dset, g_SCILAB_CLASS_EMPTY, "true");
        if (status < 0)
        {
            return (hobj_ref_t) (-1);
        }
    }
    else
    {
        char *pstPathName = NULL;

        //Create dataspace.  Setting maximum size to NULL sets the maximum
        //size to be the current size.
        space = H5Screate_simple(1, dims, NULL);
        if (space < 0)
        {
            return (hobj_ref_t) (-1);
        }

        //createGroupe and dataset name
        pstPathName = createPathName(_pstGroupName, _iIndex);

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, 1, dims);
        dset = H5Dcreate(_iFile, pstPathName, H5T_NATIVE_DOUBLE, space, iCompress);
        if (dset < 0)
        {
            FREE(pstPathName);
            return (hobj_ref_t) (-1);
        }

        status = H5Dwrite(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pdblData);
        if (status < 0)
        {
            FREE(pstPathName);
            return (hobj_ref_t) (-1);
        }

        //Add attribute SCILAB_Class = double to dataset
        status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
        if (status < 0)
        {
            FREE(pstPathName);
            return (hobj_ref_t) (-1);
        }

        //Add attribute SCILAB_Class_rows = double to dataset
        status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
        if (status < 0)
        {
            FREE(pstPathName);
            return (hobj_ref_t) (-1);
        }

        //Add attribute SCILAB_Class_cols = double to dataset
        status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
        if (status < 0)
        {
            FREE(pstPathName);
            return (hobj_ref_t) (-1);
        }

        // create the ref
        status = H5Rcreate(&iRef, _iFile, pstPathName, H5R_OBJECT, -1);
        if (status < 0)
        {
            FREE(pstPathName);
            return (hobj_ref_t) (-1);
        }

        FREE(pstPathName);
    }

    //Close and release resources.
    status = H5Dclose(dset);
    if (status < 0)
    {
        return (hobj_ref_t) (-1);
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return (hobj_ref_t) (-1);
    }

    return iRef;
}

int writeDoubleMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, double *_pdblData)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hsize_t dims[1] = { 1 };
    hid_t iCompress = 0;
    hobj_ref_t pRef[1] = { 0 };

    hid_t group = 0;
    char *pstGroupName = NULL;

    pstGroupName = createGroupName(_pstDatasetName);

    //create sub group only for non empty matrix
    if (_iRows * _iCols != 0)
    {
        group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT);
        status = H5Gclose(group);
        if (status < 0)
        {
            FREE(pstGroupName);
            return -1;
        }
    }

    pRef[0] = writeCommomDoubleMatrix(_iFile, pstGroupName, _pstDatasetName, 0, _iRows, _iCols, _pdblData);

    //don't create reference for empty matrix
    if (_iRows * _iCols == 0)
    {
        return 0;
    }

    if (pRef[0] == 0)
    {
        return -1;
    }

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(1, dims, NULL);
    if (space < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, dims);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress);
    if (dset < 0)
    {
        return -1;
    }

    status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
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

    //Add attribute SCILAB_Class_rows = double to dataset
    status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols = double to dataset
    status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    FREE(pstGroupName);
    return status;
}

int writeDoubleComplexMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg)
{
    hid_t space = 0;
    hid_t dset = 0;
    herr_t status = 0;
    hid_t iCompress = 0;
    hsize_t dims[1] = { 2 };
    hobj_ref_t pRef[2] = { 0 };

    hid_t group = 0;
    char *pstGroupName = NULL;

    pstGroupName = createGroupName(_pstDatasetName);
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT);
    status = H5Gclose(group);

    if (status < 0)
    {
        FREE(pstGroupName);
        return -1;
    }

    pRef[0] = writeCommomDoubleMatrix(_iFile, pstGroupName, _pstDatasetName, 0, _iRows, _iCols, _pdblReal);
    pRef[1] = writeCommomDoubleMatrix(_iFile, pstGroupName, _pstDatasetName, 1, _iRows, _iCols, _pdblImg);
    FREE(pstGroupName);
    if (pRef[0] == 0 || pRef[1] == 0)
    {
        return 1;
    }

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(1, dims, NULL);
    if (space < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, dims);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress);
    if (dset < 0)
    {
        printf("\nH5Dcreate\n");
        return -1;
    }
    status = H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if (status < 0)
    {
        printf("\nH5Dwrite\n");
        return -1;
    }

    //Add attribute SCILAB_Class = double to dataset
    status = addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_rows = double to dataset
    status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols = double to dataset
    status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
    if (status < 0)
    {
        return -1;
    }

    status = addAttribute(dset, g_SCILAB_CLASS_COMPLEX, "true");
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

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeBooleanMatrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, int *_piData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status;
    hid_t iSpace;
    hid_t iCompress;
    hid_t iDataset;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows = double to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols = double to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

static int writeCommonPolyMatrix(int _iFile, char *_pstDatasetName, char *_pstVarName, int _iComplex, int _iRows, int _iCols, int *_piNbCoef,
                                 double **_pdblReal, double **_pdblImg)
{
    int i = 0;
    hsize_t dims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t space = 0;
    hid_t dset = 0;
    hid_t group = 0;
    hid_t iCompress = 0;
    hobj_ref_t *pData = 0;

    char *pstPathName = NULL;
    char *pstGroupName = NULL;

    // Create ref matrix
    pData = (hobj_ref_t *)MALLOC(dims[0] * sizeof(hobj_ref_t));

    // Generate groupname #<dataSetName>#
    pstGroupName = createGroupName(_pstDatasetName);

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT);
    status = H5Gclose(group);

    //Now create each String as a dedicated DataSet.
    for (i = 0 ; i < dims[0] ; i++)
    {
        pstPathName = createPathName(pstGroupName, i);

        // Write the string to ref
        if (_iComplex)
        {
            status = writeDoubleComplexMatrix(_iFile, pstPathName, 1, _piNbCoef[i], _pdblReal[i], _pdblImg[i]);
        }
        else
        {
            status = writeDoubleMatrix(_iFile, pstPathName, 1, _piNbCoef[i], _pdblReal[i]);
        }

        if (status < 0)
        {
            FREE(pstPathName);
            FREE(pstGroupName);
            FREE(pData);
            return -1;
        }

        // create the ref
        status = H5Rcreate(&pData[i], _iFile, pstPathName, H5R_OBJECT, -1);
        if (status < 0)
        {
            FREE(pstPathName);
            FREE(pstGroupName);
            FREE(pData);
            return -1;
        }

        FREE(pstPathName);
    }

    FREE(pstGroupName);
    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    space = H5Screate_simple(1, dims, NULL);
    if (status < 0)
    {
        FREE(pData);
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, dims);
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress);
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

    //Add attribute SCILAB_Class_rows = double to dataset
    status = addIntAttribute(dset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols = double to dataset
    status = addIntAttribute(dset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writePolyMatrix(int _iFile, char *_pstDatasetName, char *_pstVarName, int _iRows, int _iCols, int *_piNbCoef, double **_pdblReal)
{
    return writeCommonPolyMatrix(_iFile, _pstDatasetName, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

int writePolyComplexMatrix(int _iFile, char *_pstDatasetName, char *_pstVarName, int _iRows, int _iCols, int *_piNbCoef, double **_pdblReal,
                           double **_pdblImg)
{
    return writeCommonPolyMatrix(_iFile, _pstDatasetName, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

int writeInteger8Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, char *_pcData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT8, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeInteger16Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, short *_psData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }
    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT16, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeInteger32Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, int *_piData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT32, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int writeInteger64Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, long long *_pllData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_INT64, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

HDF5_SCILAB_IMPEXP int writeUnsignedInterger8Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, unsigned char *_pucData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT8, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

HDF5_SCILAB_IMPEXP int writeUnsignedInterger16Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, unsigned short *_pusData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT16, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

HDF5_SCILAB_IMPEXP int writeUnsignedInterger32Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, unsigned int *_puiData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT32, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

HDF5_SCILAB_IMPEXP int writeUnsignedInterger64Matrix(int _iFile, char *_pstDatasetName, int _iRows, int _iCols, unsigned long long *_pullData)
{
    hsize_t piDims[1] = { _iRows * _iCols };
    herr_t status = 0;
    hid_t iSpace = 0;
    hid_t iDataset = 0;
    hid_t iCompress = 0;

    //Create dataspace.  Setting maximum size to NULL sets the maximum size to be the current size.
    iSpace = H5Screate_simple(1, piDims, NULL);
    if (iSpace < 0)
    {
        return -1;
    }

    //Create the dataset and write the array data to it.
    iCompress = enableCompression(9, 1, piDims);
    iDataset = H5Dcreate(_iFile, _pstDatasetName, H5T_NATIVE_UINT64, iSpace, iCompress);
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

    //Add attribute SCILAB_Class_rows to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_ROWS, _iRows);
    if (status < 0)
    {
        return -1;
    }

    //Add attribute SCILAB_Class_cols to dataset
    status = addIntAttribute(iDataset, g_SCILAB_CLASS_COLS, _iCols);
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
    hobj_ref_t pDataRef[3] = {0};

    char *pstRowPath = NULL;
    char *pstColPath = NULL;
    char *pstDataPath = NULL;
    char *pstGroupName = NULL;

    // Generate groupname #<dataSetName>#
    pstGroupName = createGroupName(_pstDatasetName);

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT);
    status = H5Gclose(group);
    if (status < 0)
    {
        FREE(pstGroupName);
        return -1;
    }

    //Create each sub dataset and insert data
    pstRowPath = createPathName(pstGroupName, 0);
    status = writeInteger32Matrix(_iFile, pstRowPath, 1, _iRows, _piNbItemRow);
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
    status = writeInteger32Matrix(_iFile, pstColPath, 1, _iNbItem, _piColPos);
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
        status = writeDoubleComplexMatrix(_iFile, pstDataPath, 1, _iNbItem, _pdblReal, _pdblImg);
    }
    else
    {
        status = writeDoubleMatrix(_iFile, pstDataPath, 1, _iNbItem, _pdblReal);
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
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress);
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
    hobj_ref_t pDataRef[2] = {0};

    char *pstRowPath = NULL;
    char *pstColPath = NULL;
    char *pstGroupName = NULL;

    // Generate groupname #<dataSetName>#
    pstGroupName = createGroupName(_pstDatasetName);

    //First create a group to store all referenced objects.
    group = H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT);
    status = H5Gclose(group);
    if (status < 0)
    {
        FREE(pstGroupName);
        return -1;
    }

    //Create each sub dataset and insert data
    pstRowPath = createPathName(pstGroupName, 0);
    status = writeInteger32Matrix(_iFile, pstRowPath, 1, _iRows, _piNbItemRow);
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
    status = writeInteger32Matrix(_iFile, pstColPath, 1, _iNbItem, _piColPos);
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
    dset = H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, iCompress);
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
    group = H5Gcreate(_iFile, pstDatasetName, H5P_DEFAULT);
    status = H5Gclose(group);
    if (status < 0)
    {
        return NULL;
    }

    if (_iNbItem)
    {
        pobjArray = MALLOC(sizeof(hobj_ref_t) * _iNbItem);
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
        dset = H5Dcreate(_iFile, _pstListName, H5T_STD_REF_OBJ, space, iCompress);
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
        if (status < 0)
        {
            return -1;
        }

        //Create the dataset and write the array data to it.
        iCompress = enableCompression(9, 1, dims);
        dset = H5Dcreate(_iFile, _pstListName, H5T_STD_REF_OBJ, space, iCompress);
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

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    FREE(_pvList);
    return 0;
}
