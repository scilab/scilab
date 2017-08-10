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

#define H5_NO_DEPRECATED_SYMBOLS

#ifndef _MSC_VER
#include <sys/time.h>
#else
#include <windows.h>
//#include <winbase.h>
#endif

#include <string.h>
#include <hdf5.h>
#include <stdlib.h>
#include "sci_malloc.h"
#include "sci_types.h"
#include "h5_attributeConstants.h"
#include "h5_readDataFromFile_v1.h"

//#define TIME_DEBUG

static herr_t find_attr_by_name_v1(hid_t loc_id, const char *name, const H5A_info_t *ainfo, void *data)
{
    return !strcmp(name, (const char *)data);
}

/************************************************************

Operator function.  Prints the name and type of the object
being examined.

************************************************************/
static herr_t op_func_v1(hid_t loc_id, const char *name, const H5L_info_t *info, void *operator_data)
{
    H5O_info_t oinfo;
    herr_t status = 0;
    int *pDataSetId = (int*)operator_data;
    hid_t obj = H5Oopen(loc_id, name, H5P_DEFAULT);
    if (obj < 0)
    {
        return -1;
    }

    /*
     * Get type of the object and return only datasetId
     * through operator_data.
     */
    status = H5Oget_info(obj, &oinfo);
    if (status < 0)
    {
        H5Oclose(obj);
        return -1;
    }

    if (oinfo.type == H5O_TYPE_DATASET)
    {
        *pDataSetId = obj;
    }
    else
    {
        H5Oclose(obj);
    }

    return 0;
}

static int readIntAttribute_v1(int _iDatasetId, const char *_pstName)
{
    hid_t iAttributeId;
    herr_t status;
    hsize_t n = 0;
    int iVal = -1;

    if (H5Aiterate(_iDatasetId, H5_INDEX_NAME, H5_ITER_NATIVE, &n, find_attr_by_name_v1, (void *)_pstName) > 0)
    {
        iAttributeId = H5Aopen_by_name(_iDatasetId, ".", _pstName, H5P_DEFAULT, H5P_DEFAULT);
        if (iAttributeId < 0)
        {
            return -1;
        }

        status = H5Aread(iAttributeId, H5T_NATIVE_INT, &iVal);
        if (status < 0)
        {
            return -1;
        }

        status = H5Aclose(iAttributeId);
        if (status < 0)
        {
            return -1;
        }
    }
    return iVal;
}

/*
** WARNING : this function returns an allocated value that must be freed.
*/
static char* readAttribute_v1(int _iDatasetId, const char *_pstName)
{
    hid_t iAttributeId;
    hid_t iFileType, memtype, iSpace;
    herr_t status;
    hsize_t dims[1];
    hsize_t n = 0;
    size_t iDim;

    char *pstValue = NULL;

    if (H5Aiterate(_iDatasetId, H5_INDEX_NAME, H5_ITER_NATIVE, &n, find_attr_by_name_v1, (void *)_pstName) > 0)
    {
        iAttributeId = H5Aopen_by_name(_iDatasetId, ".", _pstName, H5P_DEFAULT, H5P_DEFAULT);
        if (iAttributeId < 0)
        {
            return NULL;
        }
        /*
        * Get the datatype and its size.
        */
        iFileType = H5Aget_type(iAttributeId);
        iDim = H5Tget_size(iFileType);
        iDim++;                 /* Make room for null terminator */

        /*
        * Get dataspace and allocate memory for read buffer.  This is a
        * two dimensional attribute so the dynamic allocation must be done
        * in steps.
        */
        iSpace = H5Aget_space(iAttributeId);
        if (iSpace < 0)
        {
            return NULL;
        }

        status = H5Sget_simple_extent_dims(iSpace, dims, NULL);
        if (status < 0)
        {
            return NULL;
        }

        /*
        * Allocate space for string data.
        */
        pstValue = (char *)MALLOC((size_t) ((dims[0] * iDim + 1) * sizeof(char)));

        /*
        * Create the memory datatype.
        */
        memtype = H5Tcopy(H5T_C_S1);
        status = H5Tset_size(memtype, iDim);
        if (status < 0)
        {
            FREE(pstValue);
            return NULL;
        }

        /*
        * Read the data.
        */
        status = H5Aread(iAttributeId, memtype, pstValue);
        if (status < 0)
        {
            FREE(pstValue);
            return NULL;
        }

        status = H5Tclose(memtype);
        if (status < 0)
        {
            FREE(pstValue);
            return NULL;
        }

        status = H5Sclose(iSpace);
        if (status < 0)
        {
            FREE(pstValue);
            return NULL;
        }

        status = H5Tclose(iFileType);
        if (status < 0)
        {
            FREE(pstValue);
            return NULL;
        }

        status = H5Aclose(iAttributeId);
        if (status < 0)
        {
            FREE(pstValue);
            return NULL;
        }
    }
    return pstValue;

}

static int checkAttribute_v1(int _iDatasetId, char *_pstAttribute, char *_pstValue)
{
    int iRet = 0;
    char *pstScilabClass = NULL;

    //status = H5Giterate (_iFile, "/", NULL, op_func, &iDatasetId);
    pstScilabClass = readAttribute_v1(_iDatasetId, _pstAttribute);
    if (pstScilabClass != NULL && strcmp(pstScilabClass, _pstValue) == 0)
    {
        iRet = 1;
    }
    if (pstScilabClass)
    {
        FREE(pstScilabClass);
    }
    return iRet;
}

/*
** WARNING : this function returns an allocated value that must be freed.
*/
char* getScilabVersionAttribute_v1(int _iFile)
{
    return readAttribute_v1(_iFile, g_SCILAB_CLASS_SCI_VERSION);
}

int getSODFormatAttribute_v1(int _iFile)
{
    return readIntAttribute_v1(_iFile, g_SCILAB_CLASS_SOD_VERSION);
}

int getDatasetDimension_v1(int _iDatasetId, int *_piRows, int *_piCols)
{
    int iRet = 0;
    int iDummy = 0;

    *_piRows = readIntAttribute_v1(_iDatasetId, g_SCILAB_CLASS_ROWS);
    *_piCols = readIntAttribute_v1(_iDatasetId, g_SCILAB_CLASS_COLS);

    return iRet;
}

int getSparseDimension_v1(int _iDatasetId, int *_piRows, int *_piCols, int *_piNbItem)
{
    int iRet = 0;
    int iDummy = 0;

    //get number of item in the sparse matrix
    getDatasetDims_v1(_iDatasetId, _piRows, _piCols);
    *_piNbItem = readIntAttribute_v1(_iDatasetId, g_SCILAB_CLASS_ITEMS);

    return iRet;
}

static int isEmptyDataset_v1(int _iDatasetId)
{
    return checkAttribute_v1(_iDatasetId, (char *)g_SCILAB_CLASS_EMPTY, "true");
}

int isComplexData_v1(int _iDatasetId)
{
    return checkAttribute_v1(_iDatasetId, (char *)g_SCILAB_CLASS_COMPLEX, "true");
}

int getDatasetPrecision_v1(int _iDatasetId, int *_piPrec)
{
    int iRet = 0;
    char *pstScilabClass = readAttribute_v1(_iDatasetId, g_SCILAB_CLASS_PREC);

    if (pstScilabClass == NULL)
    {
        return -1;
    }
    else if (strcmp(pstScilabClass, "8") == 0)
    {
        *_piPrec = SCI_INT8;
    }
    else if (strcmp(pstScilabClass, "u8") == 0)
    {
        *_piPrec = SCI_UINT8;
    }
    else if (strcmp(pstScilabClass, "16") == 0)
    {
        *_piPrec = SCI_INT16;
    }
    else if (strcmp(pstScilabClass, "u16") == 0)
    {
        *_piPrec = SCI_UINT16;
    }
    else if (strcmp(pstScilabClass, "32") == 0)
    {
        *_piPrec = SCI_INT32;
    }
    else if (strcmp(pstScilabClass, "u32") == 0)
    {
        *_piPrec = SCI_UINT32;
    }
    else if (strcmp(pstScilabClass, "64") == 0)
    {
        *_piPrec = SCI_INT64;
    }
    else if (strcmp(pstScilabClass, "u64") == 0)
    {
        *_piPrec = SCI_UINT64;
    }
    else
    {
        iRet = 1;
    }

    FREE(pstScilabClass);
    return iRet;
}

int getVariableNames_v1(int _iFile, char **pstNameList)
{
    hsize_t i = 0;
    hsize_t iCount = 0;
    herr_t status = 0;
    int iNbItem = 0;
    H5O_info_t oinfo;
    H5G_info_t ginfo;

    status = H5Gget_info(_iFile, &ginfo);
    if (status != 0)
    {
        return 0;
    }

    iCount = ginfo.nlinks;
    for (i = 0; i < iCount; i++)
    {
        status = H5Oget_info_by_idx(_iFile, "/", H5_INDEX_NAME, H5_ITER_NATIVE, i, &oinfo, H5P_DEFAULT);
        if (status < 0)
        {
            return 0;
        }

        if (oinfo.type == H5O_TYPE_DATASET)
        {
            if (pstNameList != NULL)
            {
                ssize_t iLen = H5Lget_name_by_idx(_iFile, ".", H5_INDEX_NAME, H5_ITER_INC, i, 0, 0, H5P_DEFAULT) + 1;
                pstNameList[iNbItem] = (char*)MALLOC(sizeof(char) * iLen);
                H5Lget_name_by_idx(_iFile, ".", H5_INDEX_NAME, H5_ITER_INC, i, pstNameList[iNbItem], iLen, H5P_DEFAULT);
            }
            iNbItem++;
        }
    }
    return iNbItem;
}

int getDataSetIdFromName_v1(int _iFile, char *_pstName)
{
    return H5Dopen(_iFile, _pstName, H5P_DEFAULT);
}

void closeDataSet_v1(int _id)
{
    if (_id > 0)
    {
        herr_t status = H5Dclose(_id);
        if (status < 0)
        {
            return;
        }
    }

    return;
}

int getDataSetId_v1(int _iFile)
{
    herr_t status = 0;
    int iDatasetId = 0;
    hsize_t idx = 0;

    /*
    * Begin iteration.
    */
    status = H5Literate(_iFile, H5_INDEX_NAME, H5_ITER_NATIVE, &idx, op_func_v1, &iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return iDatasetId;
}

int getListDims_v1(int _iDatasetId, int *_piItems)
{
    /*
    * Get dataspace and dimensions of the dataset. This is a
    * two dimensional dataset.
    */
    if (isEmptyDataset_v1(_iDatasetId))
    {
        *_piItems = 0;
    }
    else
    {
        *_piItems = readIntAttribute_v1(_iDatasetId, g_SCILAB_CLASS_ITEMS);
    }
    return 0;
}

int getDatasetDims_v1(int _iDatasetId, int *_piRows, int *_piCols)
{
    /*
    * Get dataspace and dimensions of the dataset. This is a
    * two dimensional dataset.
    */
    if (isEmptyDataset_v1(_iDatasetId))
    {
        *_piCols = 0;
        *_piRows = 0;
    }
    else
    {
        *_piRows = readIntAttribute_v1(_iDatasetId, g_SCILAB_CLASS_ROWS);
        *_piCols = readIntAttribute_v1(_iDatasetId, g_SCILAB_CLASS_COLS);
    }
    return 0;
}

int readDouble_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pdblData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readDoubleMatrix_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
    herr_t status;

    if (_iRows != 0 && _iCols != 0)
    {
        hid_t obj;
        hobj_ref_t Ref;

        //Read the data.
        status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, &Ref);
        if (status < 0)
        {
            return -1;
        }

        //Open the referenced object, get its name and type.
        obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &Ref);
        readDouble_v1(obj, _iRows, _iCols, _pdblData);
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readDoubleComplexMatrix_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg)
{
    hid_t obj;
    herr_t status;
    hobj_ref_t pRef[2] = {0};

    //Read the data.
    status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if (status < 0)
    {
        return -1;
    }

    //Open the referenced object, get its name and type.
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readDouble_v1(obj, _iRows, _iCols, _pdblReal);
    if (status < 0)
    {
        return -1;
    }

    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readDouble_v1(obj, _iRows, _iCols, _pdblImg);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readEmptyMatrix_v1(int _iDatasetId)
{
    //close dataset
    herr_t status;

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readBooleanMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int *_piData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, _piData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

static int readString_v1(int _iDatasetId, char **_pstData)
{
    hid_t iFileType, memtype, iSpace;
    herr_t status;
    hsize_t dims[1];
    size_t iDim;

    /*
    * Get the datatype and its size.
    */
    iFileType = H5Dget_type(_iDatasetId);
    iDim = H5Tget_size(iFileType);
    iDim++;                     /* Make room for null terminator */

    /*
    * Get dataspace and allocate memory for read buffer.  This is a
    * two dimensional attribute so the dynamic allocation must be done
    * in steps.
    */
    iSpace = H5Dget_space(_iDatasetId);
    if (iSpace < 0)
    {
        return -1;
    }

    status = H5Sget_simple_extent_dims(iSpace, dims, NULL);
    if (status < 0)
    {
        return -1;
    }

    /*
    * Allocate space for string data.
    */
    *_pstData = (char *)MALLOC((size_t) ((dims[0] * iDim + 1) * sizeof(char)));

    /*
    * Create the memory datatype.
    */
    memtype = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(memtype, iDim);
    if (status < 0)
    {
        return -1;
    }

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_pstData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Tclose(memtype);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if (status < 0)
    {
        return -1;
    }

    status = H5Tclose(iFileType);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readStringMatrix_v1(int _iDatasetId, int _iRows, int _iCols, char **_pstData)
{
    int i;
    herr_t status;
    hsize_t dims[1];
    hsize_t subdims[1] = { 1 };
    hid_t space, memspace, filetype, memtype;
    size_t iDim;
    size_t iAllocSize = 0;

#ifdef TIME_DEBUG
    LARGE_INTEGER *piStart;
    LARGE_INTEGER *piEnd;
    LARGE_INTEGER iFreq;

    QueryPerformanceFrequency(&iFreq);

    piStart = (LARGE_INTEGER *) MALLOC(sizeof(LARGE_INTEGER) * (_iRows * _iCols + 1));
    piEnd = (LARGE_INTEGER *) MALLOC(sizeof(LARGE_INTEGER) * (_iRows * _iCols + 1));

    QueryPerformanceCounter(&piStart[0]);
#endif

    /*
    * Get the datatype and its size.
    */
    filetype = H5Dget_type(_iDatasetId);
    iDim = H5Tget_size(filetype);
    iDim++;                     /* Make room for null terminator */

    /*create sub space */
    memspace = H5Screate_simple(1, subdims, NULL);
    if (memspace < 0)
    {
        return -1;
    }

    status = H5Sget_simple_extent_dims(memspace, dims, NULL);
    if (status < 0)
    {
        return -1;
    }

    space = H5Dget_space(_iDatasetId);
    if (space < 0)
    {
        return -1;
    }

    /*
    * Create the memory datatype.
    */
    memtype = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(memtype, iDim);
    if (status < 0)
    {
        return -1;
    }

    /*
    * Allocate space for string data.
    */
    iAllocSize = (size_t) ((iDim + 1) * sizeof(char));
    for (i = 0; i < _iRows * _iCols; i++)
    {
        _pstData[i] = (char *)MALLOC(iAllocSize);
    }

    /*
    * Read the data.
    */
    for (i = 0; i < _iRows * _iCols; i++)
    {
        hsize_t start[1] = { i };
        hsize_t count[1] = { 1 };
#ifdef TIME_DEBUG
        QueryPerformanceCounter(&piStart[i + 1]);
#endif
        status = H5Sselect_hyperslab(space, H5S_SELECT_SET, start, NULL, count, NULL);
        if (status < 0)
        {
            return -1;
        }

        /*
        * Read the data.
        */
        status = H5Dread(_iDatasetId, memtype, memspace, space, H5P_DEFAULT, _pstData[i]);
        if (status < 0)
        {
            return -1;
        }
#ifdef TIME_DEBUG
        QueryPerformanceCounter(&piEnd[i + 1]);
#endif
    }

    status = H5Sclose(space);
    if (status < 0)
    {
        return -1;
    }

    status = H5Sclose(memspace);
    if (status < 0)
    {
        return -1;
    }

    status = H5Tclose(filetype);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&piEnd[0]);

    //print debuf timer
    printf("\nGlobalTime : %0.3f ms\n", ((piEnd[0].QuadPart - piStart[0].QuadPart) * 1000.0) / iFreq.QuadPart);
    for (i = 0; i < _iRows * _iCols; i++)
    {
        double dblTime = ((piEnd[i + 1].QuadPart - piStart[i + 1].QuadPart) * 1000.0) / iFreq.QuadPart;

        printf("SubTime %d : %0.3f ms\n", i, dblTime);
    }
#endif
    return 0;
}

static int readComplexPoly_v1(int _iDatasetId, int *_piNbCoef, double **_pdblReal, double **_pdblImg)
{
    int iRows = 0;
    int iCols = 0;

    //Get the datatype and its size.
    getDatasetDims_v1(_iDatasetId, &iRows, &iCols);

    //Allocate space for string data.
    *_piNbCoef = iRows * iCols;
    *_pdblReal = (double *)MALLOC(*_piNbCoef * sizeof(double));
    *_pdblImg = (double *)MALLOC(*_piNbCoef * sizeof(double));

    //Read the data and return result.
    return readDoubleComplexMatrix_v1(_iDatasetId, 1, *_piNbCoef, *_pdblReal, *_pdblImg);
}

static int readPoly_v1(int _iDatasetId, int *_piNbCoef, double **_pdblData)
{
    int iRows = 0;
    int iCols = 0;

    //Get the datatype and its size.
    getDatasetDims_v1(_iDatasetId, &iRows, &iCols);

    *_piNbCoef = iRows * iCols;
    *_pdblData = (double *)MALLOC(*_piNbCoef * sizeof(double));

    //Read the data and return result.
    return readDoubleMatrix_v1(_iDatasetId, 1, *_piNbCoef, *_pdblData);
}

int readCommonPolyMatrix_v1(int _iDatasetId, char *_pstVarname, int _iComplex, int _iRows, int _iCols, int *_piNbCoef, double **_pdblReal,
                            double **_pdblImg)
{
    int i = 0;
    hid_t obj = 0;
    char *pstVarName = 0;
    hobj_ref_t *pData = (hobj_ref_t *) MALLOC(_iRows * _iCols * sizeof(hobj_ref_t));
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);
    if (status < 0)
    {
        FREE(pData);
        return -1;
    }

    for (i = 0; i < _iRows * _iCols; i++)
    {
        /*
        * Open the referenced object, get its name and type.
        */
        obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pData[i]);
        if (_iComplex)
        {
            status = readComplexPoly_v1(obj, &_piNbCoef[i], &_pdblReal[i], &_pdblImg[i]);
        }
        else
        {
            status = readPoly_v1(obj, &_piNbCoef[i], &_pdblReal[i]);
        }

        if (status < 0)
        {
            FREE(pData);
            return -1;
        }
    }

    pstVarName = readAttribute_v1(_iDatasetId, g_SCILAB_CLASS_VARNAME);
    strcpy(_pstVarname, pstVarName);
    FREE(pstVarName);
    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        FREE(pData);
        return -1;
    }

    FREE(pData);

    return 0;
}

int readPolyMatrix_v1(int _iDatasetId, char *_pstVarname, int _iRows, int _iCols, int *_piNbCoef, double **_pdblData)
{
    return readCommonPolyMatrix_v1(_iDatasetId, _pstVarname, 0, _iRows, _iCols, _piNbCoef, _pdblData, NULL);
}

int readPolyComplexMatrix_v1(int _iDatasetId, char *_pstVarname, int _iRows, int _iCols, int *_piNbCoef, double **_pdblReal, double **_pdblImg)
{
    return readCommonPolyMatrix_v1(_iDatasetId, _pstVarname, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

int readInteger8Matrix_v1(int _iDatasetId, int _iRows, int _iCols, char *_pcData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pcData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readInteger16Matrix_v1(int _iDatasetId, int _iRows, int _iCols, short *_psData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT16, H5S_ALL, H5S_ALL, H5P_DEFAULT, _psData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readInteger32Matrix_v1(int _iDatasetId, int _iRows, int _iCols, int *_piData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, _piData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readInteger64Matrix_v1(int _iDatasetId, int _iRows, int _iCols, long long *_pllData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pllData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInteger8Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned char *_pucData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pucData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInteger16Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned short *_pusData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT16, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pusData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInteger32Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned int *_puiData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, _puiData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInteger64Matrix_v1(int _iDatasetId, int _iRows, int _iCols, unsigned long long *_pullData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pullData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readCommonSparseComplexMatrix_v1(int _iDatasetId, int _iComplex, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos,
                                     double *_pdblReal, double *_pdblImg)
{
    hid_t obj = 0;
    hobj_ref_t pRef[3] = {0};
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if (status < 0)
    {
        return -1;
    }

    //read Row data
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readInteger32Matrix_v1(obj, 1, _iRows, _piNbItemRow);
    if (status < 0)
    {
        return -1;
    }

    //read cols data
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readInteger32Matrix_v1(obj, 1, _iNbItem, _piColPos);
    if (status < 0)
    {
        return -1;
    }

    //read sparse data
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[2]);

    if (_iComplex)
    {
        status = readDoubleComplexMatrix_v1(obj, 1, _iNbItem, _pdblReal, _pdblImg);
    }
    else
    {
        status = readDoubleMatrix_v1(obj, 1, _iNbItem, _pdblReal);
    }

    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readSparseMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos, double *_pdblReal)
{
    return readCommonSparseComplexMatrix_v1(_iDatasetId, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

int readSparseComplexMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos, double *_pdblReal,
                               double *_pdblImg)
{
    return readCommonSparseComplexMatrix_v1(_iDatasetId, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

int readBooleanSparseMatrix_v1(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int *_piNbItemRow, int *_piColPos)
{
    hid_t obj = 0;
    hobj_ref_t pRef[2] = {0};
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if (status < 0)
    {
        return -1;
    }

    //read Row data
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readInteger32Matrix_v1(obj, 1, _iRows, _piNbItemRow);
    if (status < 0)
    {
        return -1;
    }

    //read cols data
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readInteger32Matrix_v1(obj, 1, _iNbItem, _piColPos);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int getScilabTypeFromDataSet_v1(int _iDatasetId)
{
    int iVarType = 0;
    char *pstScilabClass = readAttribute_v1(_iDatasetId, g_SCILAB_CLASS);

    if (pstScilabClass == NULL)
    {
        return unknow_type;
    }
    /* HDF5 Float type + SCILAB_Class = double <=> double */
    if (strcmp(pstScilabClass, g_SCILAB_CLASS_DOUBLE) == 0)
    {
        iVarType = sci_matrix;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_STRING) == 0)
    {
        iVarType = sci_strings;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_BOOLEAN) == 0)
    {
        iVarType = sci_boolean;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_POLY) == 0)
    {
        iVarType = sci_poly;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_INT) == 0)
    {
        iVarType = sci_ints;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_SPARSE) == 0)
    {
        iVarType = sci_sparse;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_BSPARSE) == 0)
    {
        iVarType = sci_boolean_sparse;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_LIST) == 0)
    {
        iVarType = sci_list;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_TLIST) == 0)
    {
        iVarType = sci_tlist;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_MLIST) == 0)
    {
        iVarType = sci_mlist;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_VOID) == 0)
    {
        iVarType = sci_void;
    }
    else if (strcmp(pstScilabClass, g_SCILAB_CLASS_UNDEFINED) == 0)
    {
        iVarType = sci_undefined;
    }

    FREE(pstScilabClass);
    return iVarType;
}

int getListItemReferences_v1(int _iDatasetId, hobj_ref_t ** _piItemRef)
{
    int iItem = 0;
    herr_t status = 0;

    getListDims_v1(_iDatasetId, &iItem);

    *_piItemRef = (hobj_ref_t *) MALLOC(iItem * sizeof(hobj_ref_t));

    status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_piItemRef);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int getListItemDataset_v1(int _iDatasetId, void *_piItemRef, int _iItemPos, int *_piItemDataset)
{
    hobj_ref_t poRef = ((hobj_ref_t *) _piItemRef)[_iItemPos];

    *_piItemDataset = H5Rdereference(_iDatasetId, H5R_OBJECT, &poRef);

    if (*_piItemDataset == 0)
    {
        return -1;
    }

    return 0;
}

int deleteListItemReferences_v1(int _iDatasetId, void *_piItemRef)
{
    herr_t status;

    if (_piItemRef)
    {
        hobj_ref_t *poRef = (hobj_ref_t *) _piItemRef;

        FREE(poRef);
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}
