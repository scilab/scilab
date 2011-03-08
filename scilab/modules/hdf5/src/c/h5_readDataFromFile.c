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

#ifndef _MSC_VER
#include <sys/time.h>
#else
#include <windows.h>
//#include <winbase.h>
#endif

#include <string.h>
#include <hdf5.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "sci_types.h"
#include "h5_attributeConstants.h"
#include "h5_readDataFromFile.h"

//#define TIME_DEBUG

static herr_t find_attr_by_name( hid_t loc_id, const char* name, void* data )
{ 
    return !strcmp(name, (const char*)data);
}

/************************************************************

Operator function.  Prints the name and type of the object
being examined.

************************************************************/
static herr_t op_func (hid_t loc_id, const char *name, void *operator_data)
{
    H5G_stat_t statbuf;
    herr_t status   = 0;
    int *pDataSetId = operator_data;

    /*
    * Get type of the object and return only datasetId
    * through operator_data.
    */
    status = H5Gget_objinfo(loc_id, name, 0, &statbuf);
    if(status < 0)
    {
        return -1;
    }

    switch (statbuf.type)
    {
    case H5G_GROUP :
        break;
    case H5G_DATASET:
        *pDataSetId = H5Dopen(loc_id, name);
        break;
    case H5G_TYPE:
        break;
    default:
        break;
    }

    return 0;
}

static int readIntAttribute(int _iDatasetId, const char *_pstName)
{
    hid_t iAttributeId;
    herr_t status;
    int iVal = 0;

    iAttributeId = H5Aopen_name(_iDatasetId, _pstName);
    if(iAttributeId < 0)
    {
        return 0;
    }

    status = H5Aread(iAttributeId, H5T_NATIVE_INT, &iVal);
    if(status < 0)
    {
        return 0;
    }

    status = H5Aclose(iAttributeId);
    if(status < 0)
    {
        return 0;
    }

    return iVal;
}

/*
** WARNING : this function returns an allocated value that must be freed.
*/
static char *readAttribute(int _iDatasetId, const char *_pstName)
{
    hid_t iAttributeId;
    hid_t iFileType, memtype, iSpace;
    herr_t status;
    hsize_t dims[1];
    size_t iDim;

    char *pstValue  = NULL;

    if(H5Aiterate(_iDatasetId, NULL, find_attr_by_name, (void*)_pstName))
    {
        iAttributeId = H5Aopen_name(_iDatasetId, _pstName);
        if(iAttributeId < 0)
        {
            return NULL;
        }
        /*
        * Get the datatype and its size.
        */
        iFileType = H5Aget_type (iAttributeId);
        iDim = H5Tget_size (iFileType);
        iDim++;                         /* Make room for null terminator */

        /*
        * Get dataspace and allocate memory for read buffer.  This is a
        * two dimensional attribute so the dynamic allocation must be done
        * in steps.
        */
        iSpace = H5Aget_space (iAttributeId);
        if(iSpace < 0)
        {
            return NULL;
        }

        status = H5Sget_simple_extent_dims (iSpace, dims, NULL);
        if(status < 0)
        {
            return NULL;
        }

        /*
        * Allocate space for string data.
        */
        pstValue = (char *) MALLOC ( (size_t)((dims[0] * iDim + 1 ) * sizeof (char)));

        /*
        * Create the memory datatype.
        */
        memtype = H5Tcopy (H5T_C_S1);
        status = H5Tset_size (memtype, iDim);
        if(status < 0)
        {
            return NULL;
        }

        /*
        * Read the data.
        */
        status = H5Aread (iAttributeId, memtype, pstValue);
        if(status < 0)
        {
            return NULL;
        }

        status = H5Tclose(memtype);
        if(status < 0)
        {
            return NULL;
        }

        status = H5Sclose(iSpace);
        if(status < 0)
        {
            return NULL;
        }

        status = H5Tclose(iFileType);
        if(status < 0)
        {
            return NULL;
        }

        status = H5Aclose(iAttributeId);
        if(status < 0)
        {
            return NULL;
        }
    }
    return pstValue;

}

static int checkAttribute(int _iDatasetId, char* _pstAttribute, char* _pstValue)
{
    int iRet                = 0;
    char *pstScilabClass    = NULL;

    //status = H5Giterate (_iFile, "/", NULL, op_func, &iDatasetId);
    pstScilabClass = readAttribute(_iDatasetId, _pstAttribute);
    if(pstScilabClass != NULL && strcmp(pstScilabClass, _pstValue) == 0)
    {
        iRet = 1;
    }
    if(pstScilabClass)
    {
        FREE(pstScilabClass);
    }
    return iRet;
}

int getDatasetDimension(int _iDatasetId, int* _piRows, int * _piCols)
{
    int iRet                = 0;
    int iDummy              = 0;
    char *pstScilabClass    = NULL;

    *_piRows = readIntAttribute(_iDatasetId, g_SCILAB_CLASS_ROWS);
    *_piCols = readIntAttribute(_iDatasetId, g_SCILAB_CLASS_COLS);

    return iRet;
}

int getSparseDimension(int _iDatasetId, int* _piRows, int * _piCols, int* _piNbItem)
{
    int iRet                = 0;
    int iDummy              = 0;
    char *pstScilabClass    = NULL;

    //get number of item in the sparse matrix
    getDatasetDims(_iDatasetId, _piRows, _piCols);
    *_piNbItem = readIntAttribute(_iDatasetId, g_SCILAB_CLASS_ITEMS);

    return iRet;
}

static int isEmptyDataset(int _iDatasetId)
{
    return checkAttribute(_iDatasetId, (char*)g_SCILAB_CLASS_EMPTY, "true");
}

int isComplexData(int _iDatasetId)
{
    return checkAttribute(_iDatasetId, (char*)g_SCILAB_CLASS_COMPLEX, "true");
}

int getDatasetPrecision(int _iDatasetId, int* _piPrec)
{
    int iRet                = 0;
    char* pstScilabClass    = readAttribute(_iDatasetId, g_SCILAB_CLASS_PREC);

    if(pstScilabClass == NULL)
    {
        return -1;
    }
    else if(strcmp(pstScilabClass, "8") == 0)
    {
        *_piPrec    = SCI_INT8;
    }
    else if(strcmp(pstScilabClass, "u8") == 0)
    {
        *_piPrec    = SCI_UINT8;
    }
    else if(strcmp(pstScilabClass, "16") == 0)
    {
        *_piPrec = SCI_INT16;
    }
    else if(strcmp(pstScilabClass, "u16") == 0)
    {
        *_piPrec = SCI_UINT16;
    }
    else if(strcmp(pstScilabClass, "32") == 0)
    {
        *_piPrec = SCI_INT32;
    }
    else if(strcmp(pstScilabClass, "u32") == 0)
    {
        *_piPrec = SCI_UINT32;
    }
    else if(strcmp(pstScilabClass, "64") == 0)
    {
        *_piPrec = SCI_INT64;
    }
    else if(strcmp(pstScilabClass, "u64") == 0)
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

int getVariableNames(int _iFile, char **pstNameList)
{
    hsize_t i           = 0;
    hsize_t iCount      = 0;
    herr_t status       = 0;
    int iNbItem         = 0;

    status = H5Gget_num_objs(_iFile, &iCount);
    if(status != 0)
    {
        return 0;
    }
    
    for(i = 0 ; i < iCount ; i++)
    {
        if(H5Gget_objtype_by_idx(_iFile, i) == H5G_DATASET)
        {
            if(pstNameList != NULL)
            {
                int iLen = 0;
                iLen = (int)H5Gget_objname_by_idx(_iFile, i, NULL, iLen);
                pstNameList[iNbItem] = (char*)MALLOC(sizeof(char) * (iLen + 1)); //null terminated
                H5Gget_objname_by_idx(_iFile, i, pstNameList[iNbItem], iLen + 1);
            }
            iNbItem++;
        }
    }
    return iNbItem;
}

int getDataSetIdFromName(int _iFile, char *_pstName)
{
    return H5Dopen(_iFile, _pstName);
}

int getDataSetId(int  _iFile)
{
    herr_t status   = 0;
    int iDatasetId  = 0;

    /*
    * Begin iteration.
    */
    status = H5Giterate (_iFile, "/", NULL, op_func, &iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return iDatasetId;
}

int getListDims(int _iDatasetId, int *_piItems)
{
    /*
    * Get dataspace and dimensions of the dataset. This is a
    * two dimensional dataset.
    */
    if(isEmptyDataset(_iDatasetId))
    {
        *_piItems = 0;
    }
    else
    {
        *_piItems = readIntAttribute(_iDatasetId, g_SCILAB_CLASS_ITEMS);
    }
    return 0;
}

int getDatasetDims(int _iDatasetId, int *_piRows, int *_piCols)
{
    /*
    * Get dataspace and dimensions of the dataset. This is a
    * two dimensional dataset.
    */
    if(isEmptyDataset(_iDatasetId))
    {
        *_piCols = 0;
        *_piRows = 0;
    }
    else
    {
        *_piRows = readIntAttribute(_iDatasetId, g_SCILAB_CLASS_ROWS);
        *_piCols = readIntAttribute(_iDatasetId, g_SCILAB_CLASS_COLS);
    }
    return 0;
}

int readDouble(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pdblData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readDoubleMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
    herr_t status;

    if(_iRows != 0 && _iCols != 0)
    {
        hid_t obj;
        hobj_ref_t *pRef = (hobj_ref_t *) MALLOC (1 * sizeof (hobj_ref_t));

        //Read the data.
        status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);

        //Open the referenced object, get its name and type.
        obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
        readDouble(obj,_iRows, _iCols, _pdblData);
        FREE(pRef);
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readDoubleComplexMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg)
{
    hid_t obj;
    herr_t status;
    hobj_ref_t *pRef = (hobj_ref_t *) MALLOC (2 * sizeof (hobj_ref_t));

    //Read the data.
    status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);

    //Open the referenced object, get its name and type.
    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readDouble(obj,_iRows, _iCols, _pdblReal);
    if(status < 0)
    {
        return -1;
    }

    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readDouble(obj,_iRows, _iCols, _pdblImg);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readEmptyMatrix(int _iDatasetId)
{//close dataset
    herr_t status;
    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readBooleanMatrix(int _iDatasetId, int _iRows, int _iCols, int* _piData)
{
    herr_t status = 0;
    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, _piData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}


static int readString(int _iDatasetId, char **_pstData)
{
    hid_t iFileType, memtype, iSpace;
    herr_t status;
    hsize_t dims[1];
    size_t iDim;

    /*
    * Get the datatype and its size.
    */
    iFileType = H5Dget_type (_iDatasetId);
    iDim = H5Tget_size (iFileType);
    iDim++;                         /* Make room for null terminator */

    /*
    * Get dataspace and allocate memory for read buffer.  This is a
    * two dimensional attribute so the dynamic allocation must be done
    * in steps.
    */
    iSpace = H5Dget_space (_iDatasetId);
    if(iSpace< 0)
    {
        return -1;
    }
    
    status = H5Sget_simple_extent_dims (iSpace, dims, NULL);
    if(status < 0)
    {
        return -1;
    }

    /*
    * Allocate space for string data.
    */
    *_pstData = (char *) MALLOC ( (size_t)((dims[0] * iDim + 1) * sizeof (char)));

    /*
    * Create the memory datatype.
    */
    memtype = H5Tcopy (H5T_C_S1);
    status = H5Tset_size (memtype, iDim);
    if(status < 0)
    {
        return -1;
    }

    /*
    * Read the data.
    */
    status = H5Dread (_iDatasetId, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_pstData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Tclose(memtype);
    if(status < 0)
    {
        return -1;
    }

    status = H5Sclose(iSpace);
    if(status < 0)
    {
        return -1;
    }

    status = H5Tclose(iFileType);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readStringMatrix(int _iDatasetId, int _iRows, int _iCols, char **_pstData)
{
    int i;
    herr_t status;
    hsize_t dims[1];
    hsize_t subdims[1]  = {1};
    hid_t space, memspace, filetype, memtype;
    size_t iDim;
    size_t iAllocSize   = 0;

#ifdef TIME_DEBUG
    LARGE_INTEGER *piStart;
    LARGE_INTEGER *piEnd;
    LARGE_INTEGER iFreq;

    QueryPerformanceFrequency(&iFreq);

    piStart = (LARGE_INTEGER*)MALLOC(sizeof(LARGE_INTEGER) * (_iRows * _iCols + 1));
    piEnd   = (LARGE_INTEGER*)MALLOC(sizeof(LARGE_INTEGER) * (_iRows * _iCols + 1));

    QueryPerformanceCounter(&piStart[0]);
#endif

    /*
    * Get the datatype and its size.
    */
    filetype = H5Dget_type (_iDatasetId);
    iDim = H5Tget_size (filetype);
    iDim++;                         /* Make room for null terminator */


    /*create sub space*/
    memspace = H5Screate_simple(1, subdims, NULL);
    if(memspace < 0)
    {
        return -1;
    }

    
    status = H5Sget_simple_extent_dims (memspace, dims, NULL);
    if(status < 0)
    {
        return -1;
    }

    space = H5Dget_space (_iDatasetId);
    if(space < 0)
    {
        return -1;
    }


    /*
    * Create the memory datatype.
    */
    memtype = H5Tcopy (H5T_C_S1);
    status = H5Tset_size (memtype, iDim);
    if(status < 0)
    {
        return -1;
    }

    /*
    * Allocate space for string data.
    */
    iAllocSize = (size_t)((iDim + 1) * sizeof (char));
    for (i = 0 ; i < _iRows * _iCols; i++)
    {
        _pstData[i] = (char *) MALLOC (iAllocSize);
    }

    /*
    * Read the data.
    */
    for (i = 0 ; i < _iRows * _iCols; i++)
    {
        hssize_t start[1]={i};
        hsize_t count[1]={1};
#ifdef TIME_DEBUG
        QueryPerformanceCounter(&piStart[i+1]);
#endif
        status = H5Sselect_hyperslab (space, H5S_SELECT_SET, start, NULL, count, NULL);
        if(status < 0)
        {
            return -1;
        }

        /*
        * Read the data.
        */
        status = H5Dread (_iDatasetId, memtype, memspace, space, H5P_DEFAULT, _pstData[i]);
        if(status < 0)
        {
            return -1;
        }
#ifdef TIME_DEBUG
        QueryPerformanceCounter(&piEnd[i+1]);
#endif
    }

    status = H5Sclose(space);
    if(status < 0)
    {
        return -1;
    }

    status = H5Sclose(memspace);
    if(status < 0)
    {
        return -1;
    }


    status = H5Tclose(filetype);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

#ifdef TIME_DEBUG
    QueryPerformanceCounter(&piEnd[0]);

    //print debuf timer
    printf("\nGlobalTime : %0.3f ms\n", ((piEnd[0].QuadPart - piStart[0].QuadPart) * 1000.0) / iFreq.QuadPart);
    for (i = 0 ; i < _iRows * _iCols; i++)
    {
        double dblTime    =((piEnd[i+1].QuadPart - piStart[i+1].QuadPart) * 1000.0) / iFreq.QuadPart;
        printf("SubTime %d : %0.3f ms\n", i, dblTime);
    }
#endif
    return 0;
}

static int readComplexPoly(int _iDatasetId, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    int iRows   = 0;
    int iCols   = 0;

    //Get the datatype and its size.
    getDatasetDims(_iDatasetId, &iRows, &iCols);

    //Allocate space for string data.
    *_piNbCoef  = iRows * iCols;
    *_pdblReal  = (double*)MALLOC(*_piNbCoef * sizeof(double));
    *_pdblImg   = (double*)MALLOC(*_piNbCoef * sizeof(double));

    //Read the data and return result.
    return readDoubleComplexMatrix(_iDatasetId, 1, *_piNbCoef, *_pdblReal, *_pdblImg);
}

static int readPoly(int _iDatasetId, int* _piNbCoef, double** _pdblData)
{
    int iRows   = 0;
    int iCols   = 0;

    //Get the datatype and its size.
    getDatasetDims(_iDatasetId, &iRows, &iCols);

    *_piNbCoef  = iRows * iCols;
    *_pdblData  = (double*)MALLOC(*_piNbCoef * sizeof(double));

    //Read the data and return result.
    return readDoubleMatrix(_iDatasetId, 1, *_piNbCoef, *_pdblData);
}

int readCommonPolyMatrix(int _iDatasetId, char* _pstVarname, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double **_pdblReal, double **_pdblImg)
{
    int    i            = 0;
    hid_t obj           = 0;
    char* pstVarName    = 0;
    hobj_ref_t *pData   = (hobj_ref_t *)MALLOC(_iRows * _iCols * sizeof (hobj_ref_t));
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);
    if(status < 0)
    {
        return -1;
    }

    for (i = 0 ; i < _iRows * _iCols; i++)
    {
        /*
        * Open the referenced object, get its name and type.
        */
        obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pData[i]);
        if(_iComplex)
        {
            status = readComplexPoly(obj, &_piNbCoef[i], &_pdblReal[i], &_pdblImg[i]);
        }
        else
        {
            status = readPoly(obj, &_piNbCoef[i], &_pdblReal[i]);
        }

        if(status < 0)
        {
            return -1;
        }
    }

    pstVarName = readAttribute(_iDatasetId, g_SCILAB_CLASS_VARNAME);
    strcpy(_pstVarname, pstVarName);
    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    FREE(pData);
    FREE(pstVarName);

    return 0;
}

int readPolyMatrix(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblData)
{
    return readCommonPolyMatrix(_iDatasetId, _pstVarname, 0, _iRows, _iCols, _piNbCoef, _pdblData, NULL);
}

int readPolyComplexMatrix(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblReal, double **_pdblImg)
{
    return readCommonPolyMatrix(_iDatasetId, _pstVarname, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

int readInterger8Matrix(int _iDatasetId, int _iRows, int _iCols, char* _pcData)
{
    herr_t status = 0;
    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pcData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readInterger16Matrix(int _iDatasetId, int _iRows, int _iCols, short* _psData)
{
    herr_t status = 0;
    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT16, H5S_ALL, H5S_ALL, H5P_DEFAULT, _psData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readInterger32Matrix(int _iDatasetId, int _iRows, int _iCols, int* _piData)
{
    herr_t status = 0;
    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, _piData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readInterger64Matrix(int _iDatasetId, int _iRows, int _iCols, long long* _pllData)
{
    herr_t status = 0;
    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_INT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pllData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInterger8Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned char* _pucData)
{
    herr_t status = 0;
    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pucData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInterger16Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned short* _pusData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT16, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pusData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInterger32Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned int* _puiData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, _puiData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readUnsignedInterger64Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned long long* _pullData)
{
    herr_t status = 0;

    /*
    * Read the data.
    */
    status = H5Dread(_iDatasetId, H5T_NATIVE_UINT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pullData);
    if(status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int readCommonSparseComplexMatrix(int _iDatasetId, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,    int* _piColPos, double *_pdblReal, double *_pdblImg)
{
    int i               = 0;
    int j               = 0;
    hid_t obj           = 0;
    hobj_ref_t *pRef    = (hobj_ref_t *)MALLOC(3 * sizeof (hobj_ref_t));
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if(status < 0)
    {
        return -1;
    }

    //read Row data
    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readInterger32Matrix(obj, 1, _iRows, _piNbItemRow);
    if(status < 0)
    {
        return -1;
    }

    //read cols data
    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readInterger32Matrix(obj, 1, _iNbItem, _piColPos);
    if(status < 0)
    {
        return -1;
    }

    //read sparse data
    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[2]);

    if(_iComplex)
    {
        status = readDoubleComplexMatrix(obj, 1, _iNbItem, _pdblReal, _pdblImg);
    }
    else
    {
        status = readDoubleMatrix(obj, 1, _iNbItem, _pdblReal);
    }

    if(status < 0)
    {
        return -1;
    }

    FREE(pRef);

    return 0;
}

int readSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,    int* _piColPos, double *_pdblReal)
{
    return readCommonSparseComplexMatrix(_iDatasetId, 0, _iRows, _iCols, _iNbItem, _piNbItemRow,    _piColPos, _pdblReal, NULL);
}

int readSparseComplexMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,    int* _piColPos, double *_pdblReal, double *_pdblImg)
{
    return readCommonSparseComplexMatrix(_iDatasetId, 1, _iRows, _iCols, _iNbItem, _piNbItemRow,    _piColPos, _pdblReal, _pdblImg);
}

int readBooleanSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,    int* _piColPos)
{
    int i               = 0;
    int j               = 0;
    hid_t obj           = 0;
    hobj_ref_t *pRef    = (hobj_ref_t *)MALLOC(2 * sizeof (hobj_ref_t));
    herr_t status;

    /*
    * Read the data.
    */
    status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if(status < 0)
    {
        return -1;
    }

    //read Row data
    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readInterger32Matrix(obj, 1, _iRows, _piNbItemRow);
    if(status < 0)
    {
        return -1;
    }

    //read cols data
    obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readInterger32Matrix(obj, 1, _iNbItem, _piColPos);
    if(status < 0)
    {
        return -1;
    }

    FREE(pRef);

    return 0;
}

int getScilabTypeFromDataSet(int _iDatasetId)
{
    int iVarType            = 0;
    char *pstScilabClass    = readAttribute(_iDatasetId, g_SCILAB_CLASS);

    if(pstScilabClass == NULL)
    {
        return unknow_type;
    }
    /* HDF5 Float type + SCILAB_Class = double <=> double */
    if(strcmp(pstScilabClass, g_SCILAB_CLASS_DOUBLE) == 0)
    {
        iVarType = sci_matrix;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_STRING) == 0)
    {
        iVarType = sci_strings;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_BOOLEAN) == 0)
    {
        iVarType = sci_boolean;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_BOOLEAN) == 0)
    {
        iVarType = sci_boolean;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_POLY) == 0)
    {
        iVarType = sci_poly;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_INT) == 0)
    {
        iVarType = sci_ints;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_SPARSE) == 0)
    {
        iVarType = sci_sparse;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_BSPARSE) == 0)
    {
        iVarType = sci_boolean_sparse;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_LIST) == 0)
    {
        iVarType = sci_list;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_TLIST) == 0)
    {
        iVarType = sci_tlist;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_MLIST) == 0)
    {
        iVarType = sci_mlist;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_VOID) == 0)
    {
        iVarType = sci_void;
    }
    else if(strcmp(pstScilabClass, g_SCILAB_CLASS_UNDEFINED) == 0)
    {
        iVarType = sci_undefined;
    }

    if(iVarType == 0)
    {
        return 0;
    }
    FREE(pstScilabClass);
    return iVarType;
}


int getListItemReferences(int _iDatasetId, hobj_ref_t** _piItemRef)
{
    int iItem       = 0;
    herr_t status   = 0;

    getListDims(_iDatasetId, &iItem);

    *_piItemRef = (hobj_ref_t *) MALLOC (iItem * sizeof (hobj_ref_t));

    status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_piItemRef);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}

int getListItemDataset(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset)
{
    hobj_ref_t poRef    = ((hobj_ref_t*)_piItemRef)[_iItemPos];
    *_piItemDataset     = H5Rdereference (_iDatasetId, H5R_OBJECT, &poRef);

    if(*_piItemDataset == 0)
    {
        return -1;
    }

    return 0;
}

int deleteListItemReferences(int _iDatasetId, void* _piItemRef)
{
    herr_t status;
    if(_piItemRef)
    {
        hobj_ref_t *poRef    = (hobj_ref_t*)_piItemRef;
        FREE(poRef);
    }

    status = H5Dclose(_iDatasetId);
    if(status < 0)
    {
        return -1;
    }

    return 0;
}
