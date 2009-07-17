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

#include <hdf5.h>
#include <stdlib.h>
#include "stack-c.h"
#include "h5_attributeConstants.h"
#include "h5_readDataFromFile.h"


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
	herr_t status = 0;
	H5G_stat_t statbuf;
	int	*pDataSetId = operator_data;

	/*
	* Get type of the object and return only datasetId
	* through operator_data.
	*/
	status = H5Gget_objinfo(loc_id, name, 0, &statbuf);
	switch (statbuf.type)
	{
	case H5G_GROUP :
		// printf ("  Group: %s\n", name);
		break;
	case H5G_DATASET:
		//printf ("  Dataset: %s\n", name);
		*pDataSetId = H5Dopen(loc_id, name);
		break;
	case H5G_TYPE:
		//printf ("  Datatype: %s\n", name);
		break;
	default:
		//printf ( "  Unknown: %s\n", name);
		break;
	}

	return 0;
}


/*
** WARNING : this function returns an allocated value that must be freed.
*/
static char *readAttribute(int _iDatasetId, const char *_pstName)
{
	hid_t		iAttributeId;
	hid_t		iFileType, memtype, iSpace;
	herr_t	status;
	hsize_t	dims[1];
	size_t	iDim;

	char		*pstValue			= NULL;

	if(H5Aiterate(_iDatasetId, NULL, find_attr_by_name, (void*)_pstName))
	{
		iAttributeId = H5Aopen_name(_iDatasetId, _pstName);
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
		H5Sget_simple_extent_dims (iSpace, dims, NULL);

		/*
		* Allocate space for string data.
		*/
		pstValue = (char *) malloc ( (size_t)((dims[0] * iDim + 1 ) * sizeof (char)));

		/*
		* Create the memory datatype.
		*/
		memtype = H5Tcopy (H5T_C_S1);
		status = H5Tset_size (memtype, iDim);

		/*
		* Read the data.
		*/
		status = H5Aread (iAttributeId, memtype, pstValue);

		H5Aclose(iAttributeId);
	}
	return pstValue;

}

static int checkAttribute(int _iDatasetId, char* _pstAttribute, char* _pstValue)
{
	int iRet							= 0;
	char *pstScilabClass	= NULL;

	//status = H5Giterate (_iFile, "/", NULL, op_func, &iDatasetId);
	pstScilabClass = readAttribute(_iDatasetId, _pstAttribute);
	if(pstScilabClass != NULL && strcmp(pstScilabClass, _pstValue) == 0)
	{
		iRet = 1;
	}
	if(pstScilabClass)
	{
		free(pstScilabClass);
	}
	return iRet;
}

int getSparseDimension(int _iDatasetId, int* _piRows, int * _piCols, int* _piNbItem)
{
	int iRet							= 0;
	int iDummy						= 0;
	char *pstScilabClass	= NULL;

	//get number of item in the sparse matrix

	iRet = getDataSetDims(_iDatasetId, &iDummy, _piNbItem);
	if(iRet)
	{
		return 1;
	}

	pstScilabClass = readAttribute(_iDatasetId, g_SCILAB_CLASS_ROWS);
	if(pstScilabClass != NULL)
	{
		*_piRows = atoi(pstScilabClass);
	}
	
	if(pstScilabClass)
	{
		free(pstScilabClass);
	}

	pstScilabClass = readAttribute(_iDatasetId, g_SCILAB_CLASS_COLS);
	if(pstScilabClass != NULL)
	{
		*_piCols = atoi(pstScilabClass);
	}
	
	if(pstScilabClass)
	{
		free(pstScilabClass);
	}
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
	int iRet							= 0;
	char* pstScilabClass	= readAttribute(_iDatasetId, g_SCILAB_CLASS_PREC);
	if(pstScilabClass == NULL)
	{
		return 1;
	}
	else if(strcmp(pstScilabClass, "8") == 0)
	{
		*_piPrec	= SCI_INT8;
	}
	else if(strcmp(pstScilabClass, "16") == 0)
	{
		*_piPrec = SCI_INT16;
	}
	else if(strcmp(pstScilabClass, "32") == 0)
	{
		*_piPrec = SCI_INT32;
	}
	else if(strcmp(pstScilabClass, "64") == 0)
	{
		*_piPrec = SCI_INT64;
	}
	else
	{
		iRet = 1;
	}
	
	free(pstScilabClass);
	return iRet;
}

int getVariableNames(int _iFile, char **pstNameList)
{
	hsize_t i						= 0;
	hsize_t iCount			= 0;
	herr_t status				= 0;
	int iNbItem					= 0;

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
				iLen = H5Gget_objname_by_idx(_iFile, i, NULL, iLen);
				pstNameList[iNbItem] = (char*)malloc(sizeof(char) * iLen - 1); //null terminated
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
	herr_t status				= 0;
	int iDatasetId			= 0;

	/*
	* Begin iteration.
	*/
	//  printf ("Objects in root group:\n");
	status = H5Giterate (_iFile, "/", NULL, op_func, &iDatasetId);
	return iDatasetId;
}


int getDataSetDims(int _iDatasetId, int *_piRows, int *_piCols)
{
	hsize_t	lDims[2];
	hid_t		space;
	int		ndims;
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
		space = H5Dget_space (_iDatasetId);
		ndims = H5Sget_simple_extent_dims (space , lDims, NULL);
		*_piRows = (int)lDims[0];
		if (ndims == 1)
		{
			//check if double in this case read child size
			if(getScilabTypeFromDataSet(_iDatasetId) == sci_matrix)
			{
				int iItemDataset				= 0;
				hobj_ref_t* piItemRef		= NULL;

				piItemRef = (hobj_ref_t *) malloc (*_piRows * sizeof (hobj_ref_t));
				H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, piItemRef);

				getListItemDataset(_iDatasetId, piItemRef, 0, &iItemDataset);
				getDataSetDims(iItemDataset, _piRows, _piCols);
				free(piItemRef);
			}
			else if(getScilabTypeFromDataSet(_iDatasetId) == sci_sparse || getScilabTypeFromDataSet(_iDatasetId) == sci_boolean_sparse)
			{
				int iItemDataset				= 0;
				hobj_ref_t* piItemRef		= NULL;

				piItemRef = (hobj_ref_t *) malloc (*_piRows * sizeof (hobj_ref_t));
				H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, piItemRef);

				getListItemDataset(_iDatasetId, piItemRef, 1, &iItemDataset);
				getDataSetDims(iItemDataset, _piRows, _piCols);
				free(piItemRef);
			}
			else
			{
				*_piCols = 1;
			}
		}
		else
		{
			*_piCols = (int)lDims[1];
		}

		H5Sclose(space);
	}
	return 0;
}

int readDouble(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
	herr_t      status;
	double      *pdblLocalData;
	int	     i = 0, j = 0;
	char* pstMajor		= NULL;
	char* pstMinor		= NULL;

	pdblLocalData = (double*)malloc(sizeof(double) * _iRows * _iCols);
	/*
	* Read the data.
	*/
	status = H5Dread(_iDatasetId, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		pdblLocalData);

	for (i = 0 ; i < _iRows ; ++i)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			_pdblData[i + _iRows * j] = pdblLocalData[i * _iCols + j];
		}
	}
	status = H5Dclose(_iDatasetId);

	free(pdblLocalData);

	return status;
}

int readDoubleMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
	hid_t		obj;
	hobj_ref_t	*pRef = (hobj_ref_t *) malloc (1 * sizeof (hobj_ref_t));
	herr_t	status;

	//Read the data.
	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);

	//Open the referenced object, get its name and type.
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
	readDouble(obj,_iRows, _iCols, _pdblData);

	H5Dclose(_iDatasetId);
	return 0;
}

int readDoubleComplexMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg)
{
	hid_t		obj;
	hobj_ref_t	*pRef = (hobj_ref_t *) malloc (2 * sizeof (hobj_ref_t));
	herr_t	status;

	//Read the data.
	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);

	//Open the referenced object, get its name and type.
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
	readDouble(obj,_iRows, _iCols, _pdblReal);
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[1]);
	readDouble(obj,_iRows, _iCols, _pdblImg);

	H5Dclose(_iDatasetId);
	return 0;
}

int readBooleanMatrix(int _iDatasetId, int _iRows, int _iCols, int* _piData)
{
	herr_t status = 0;
	int* piData		= NULL;
	int i					= 0;
	int j					= 0;

	piData = (int*)malloc(sizeof(int) * _iRows * _iCols);
	/*
	* Read the data.
	*/
	status = H5Dread(_iDatasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		piData);

	for (i = 0 ; i < _iRows ; ++i)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			_piData[i + _iRows * j] = piData[i * _iCols + j];
		}
	}
	status = H5Dclose(_iDatasetId);

	free(piData);

	return status;
}


static int readString(int _iDatasetId, char **_pstData)
{
	hid_t		iFileType, memtype, iSpace;
	herr_t	status;
	hsize_t	dims[1];
	size_t	iDim;

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
	H5Sget_simple_extent_dims (iSpace, dims, NULL);

	/*
	* Allocate space for string data.
	*/
	*_pstData = (char *) malloc ( (size_t)((dims[0] * iDim + 1) * sizeof (char)));

	/*
	* Create the memory datatype.
	*/
	memtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (memtype, iDim);

	/*
	* Read the data.
	*/
	status = H5Dread (_iDatasetId, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_pstData);
	H5Dclose(_iDatasetId);
	return 0;
}

int readStringMatrix(int _iDatasetId, int _iRows, int _iCols, char **_pstData)
{
	hid_t		obj;
	hobj_ref_t	*rdata = (hobj_ref_t *) malloc (_iRows * _iCols * sizeof (hobj_ref_t));
	herr_t	status;
	int		i = 0, j = 0;

	/*
	* Read the data.
	*/
	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		rdata);
	for (i = 0 ; i < _iRows ; i++)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			/*
			* Open the referenced object, get its name and type.
			*/
			obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &rdata[i * _iCols + j]);
			readString(obj, &_pstData[i + j * _iRows]);
		}
	}

	H5Dclose(_iDatasetId);
	free(rdata);

	return 0;
}

static int readComplexPoly(int _iDatasetId, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int iRows				= 0;
	int iCols				= 0;

	//Get the datatype and its size.
	getDataSetDims(_iDatasetId, &iRows, &iCols);

	//Allocate space for string data.
	*_piNbCoef			= iRows * iCols;
	*_pdblReal			= (double*)malloc(*_piNbCoef * sizeof(double));
	*_pdblImg				= (double*)malloc(*_piNbCoef * sizeof(double));

	//Read the data and return result.
	return readDoubleComplexMatrix(_iDatasetId, 1, *_piNbCoef, *_pdblReal, *_pdblImg);
}

static int readPoly(int _iDatasetId, int* _piNbCoef, double** _pdblData)
{
	int iRows							= 0;
	int iCols							= 0;

	//Get the datatype and its size.
	getDataSetDims(_iDatasetId, &iRows, &iCols);

	*_piNbCoef = iRows * iCols;
	*_pdblData = (double*)malloc(*_piNbCoef * sizeof(double));

	//Read the data and return result.
	return readDoubleMatrix(_iDatasetId, 1, *_piNbCoef, *_pdblData);
}

int readCommonPolyMatrix(int _iDatasetId, char* _pstVarname, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double **_pdblReal, double **_pdblImg)
{
	int	i								= 0;
	int j								= 0;
	hid_t obj						= 0;
	char* pstVarName		= 0;
	hobj_ref_t *pData		= (hobj_ref_t *)malloc(_iRows * _iCols * sizeof (hobj_ref_t));
	herr_t status;

	/*
	* Read the data.
	*/
	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);
	if(status)
	{
		return 1;
	}

	for (i = 0 ; i < _iRows ; i++)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			/*
			* Open the referenced object, get its name and type.
			*/
			obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pData[i * _iCols + j]);
			if(_iComplex)
			{
				status = readComplexPoly(obj, &_piNbCoef[i + j * _iRows], &_pdblReal[i + j * _iRows], &_pdblImg[i + j * _iRows]);
			}
			else
			{
				status = readPoly(obj, &_piNbCoef[i + j * _iRows], &_pdblReal[i + j * _iRows]);
			}
		}
	}

	if(status)
	{
		return 1;
	}

	pstVarName = readAttribute(_iDatasetId, g_SCILAB_CLASS_VARNAME);
	strcpy(_pstVarname, pstVarName);
	status = H5Dclose(_iDatasetId);
	free(pData);
	free(pstVarName);

	return status;
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
	herr_t status			= 0;
	char* pcLocalData = NULL;
	int i							= 0;
	int j							= 0;
	char* pstMajor		= NULL;
	char* pstMinor		= NULL;

	pcLocalData = (char*)malloc(sizeof(char) * _iRows * _iCols);
	/*
	* Read the data.
	*/
	status = H5Dread(_iDatasetId, H5T_NATIVE_INT8, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		pcLocalData);

	for (i = 0 ; i < _iRows ; ++i)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			_pcData[i + _iRows * j] = pcLocalData[i * _iCols + j];
		}
	}
	status = H5Dclose(_iDatasetId);

	free(pcLocalData);

	return status;
}

int readInterger16Matrix(int _iDatasetId, int _iRows, int _iCols, short* _psData)
{
	herr_t status				= 0;
	short* psLocalData	= NULL;
	int i								= 0;
	int j								= 0;
	char* pstMajor			= NULL;
	char* pstMinor			= NULL;

	psLocalData = (short*)malloc(sizeof(short) * _iRows * _iCols);
	/*
	* Read the data.
	*/
	status = H5Dread(_iDatasetId, H5T_NATIVE_INT16, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		psLocalData);

	for (i = 0 ; i < _iRows ; ++i)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			_psData[i + _iRows * j] = psLocalData[i * _iCols + j];
		}
	}
	status = H5Dclose(_iDatasetId);

	free(psLocalData);

	return status;
}

int readInterger32Matrix(int _iDatasetId, int _iRows, int _iCols, int* _piData)
{
	herr_t status				= 0;
	int* piLocalData		= NULL;
	int i								= 0;
	int j								= 0;
	char* pstMajor			= NULL;
	char* pstMinor			= NULL;

	piLocalData = (int*)malloc(sizeof(int) * _iRows * _iCols);
	/*
	* Read the data.
	*/
	status = H5Dread(_iDatasetId, H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		piLocalData);

	for (i = 0 ; i < _iRows ; ++i)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			_piData[i + _iRows * j] = piLocalData[i * _iCols + j];
		}
	}
	status = H5Dclose(_iDatasetId);

	free(piLocalData);

	return status;
}

int readInterger64Matrix(int _iDatasetId, int _iRows, int _iCols, long long* _pllData)
{
	herr_t status							= 0;
	long long* pllLocalData		= NULL;
	int i											= 0;
	int j											= 0;
	char* pstMajor						= NULL;
	char* pstMinor						= NULL;

	pllLocalData = (long long*)malloc(sizeof(long long) * _iRows * _iCols);
	/*
	* Read the data.
	*/
	status = H5Dread(_iDatasetId, H5T_NATIVE_INT64, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		pllLocalData);

	for (i = 0 ; i < _iRows ; ++i)
	{
		for (j = 0 ; j < _iCols ; ++j)
		{
			_pllData[i + _iRows * j] = pllLocalData[i * _iCols + j];
		}
	}
	status = H5Dclose(_iDatasetId);

	free(pllLocalData);

	return status;
}

int readCommonSparseComplexMatrix(int _iDatasetId, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal, double *_pdblImg)
{
	int	i								= 0;
	int j								= 0;
	hid_t obj						= 0;
	hobj_ref_t *pRef		= (hobj_ref_t *)malloc(3 * sizeof (hobj_ref_t));
	herr_t status;

	/*
	* Read the data.
	*/
	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
	if(status)
	{
		return 1;
	}

	//read Row data
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
	status = readInterger32Matrix(obj, 1, _iRows, _piNbItemRow);
	if(status)
	{
		return 1;
	}

	//read cols data
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[1]);
	status = readInterger32Matrix(obj, 1, _iNbItem, _piColPos);
	if(status)
	{
		return 1;
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

	if(status)
	{
		return 1;
	}

	free(pRef);

	return status;
}

int readSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal)
{
	return readCommonSparseComplexMatrix(_iDatasetId, 0, _iRows, _iCols, _iNbItem, _piNbItemRow,	_piColPos, _pdblReal, NULL);
}

int readSparseComplexMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal, double *_pdblImg)
{
	return readCommonSparseComplexMatrix(_iDatasetId, 1, _iRows, _iCols, _iNbItem, _piNbItemRow,	_piColPos, _pdblReal, _pdblImg);
}

int readBooleanSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos)
{
	int	i								= 0;
	int j								= 0;
	hid_t obj						= 0;
	hobj_ref_t *pRef		= (hobj_ref_t *)malloc(2 * sizeof (hobj_ref_t));
	herr_t status;

	/*
	* Read the data.
	*/
	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
	if(status)
	{
		return 1;
	}

	//read Row data
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[0]);
	status = readInterger32Matrix(obj, 1, _iRows, _piNbItemRow);
	if(status)
	{
		return 1;
	}

	//read cols data
	obj = H5Rdereference (_iDatasetId, H5R_OBJECT, &pRef[1]);
	status = readInterger32Matrix(obj, 1, _iNbItem, _piColPos);
	if(status)
	{
		return 1;
	}

	free(pRef);

	return status;
}

int getScilabTypeFromDataSet(int _iDatasetId)
{
	int iVarType					= 0;
	char *pstScilabClass	= readAttribute(_iDatasetId, g_SCILAB_CLASS);

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

	if(iVarType == 0)
	{
		return 0;
	}
	free(pstScilabClass);
	return iVarType;
}


int getListItemReferences(int _iDatasetId, hobj_ref_t** _piItemRef)
{
	int iRows					= 0;
	int iCols					= 0;
	herr_t status			= 0;

	getDataSetDims(_iDatasetId, &iRows, &iCols);

	*_piItemRef = (hobj_ref_t *) malloc (iRows * iCols * sizeof (hobj_ref_t));

	status = H5Dread (_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_piItemRef);
	return 0;
}

int getListItemDataset(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset)
{
	hobj_ref_t poRef        = ((hobj_ref_t*)_piItemRef)[_iItemPos];
	*_piItemDataset = H5Rdereference (_iDatasetId, H5R_OBJECT, &poRef);

	if(*_piItemDataset == 0)
	{
		return 1;
	}

	return 0;
}

int deleteListItemReferences(int _iDatasetId, void* _piItemRef)
{
	if(_piItemRef)
	{
		hobj_ref_t *poRef	= (hobj_ref_t*)_piItemRef;
		free(poRef);
	}

	H5Dclose(_iDatasetId);
	return 0;
}
