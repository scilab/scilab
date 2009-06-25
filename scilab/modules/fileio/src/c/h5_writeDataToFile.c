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
#include "stack-c.h"
#include <malloc.h>
#include <math.h>
#include "h5_writeDataToFile.h"
#include "h5_attributeConstants.h"


static void addAttribute(int dataSetId, const char *name, const char *value)
{
  hsize_t     attributeDims[1] = {1};
  hid_t       attributeTypeId, attributeSpace, attr;
  herr_t      status;

    /*
   * Create attribute dataspace.  Setting maximum size to NULL sets the maximum
   * size to be the current size.
   */
  attributeSpace = H5Screate_simple (1, attributeDims, NULL);

  /*
   * Create special attribute type
   */
  attributeTypeId = H5Tcopy(H5T_C_S1);
  H5Tset_size(attributeTypeId, strlen(value));
  /*
   * Create the attribute and write it.
   */
  attr = H5Acreate (dataSetId, name, attributeTypeId, attributeSpace, H5P_DEFAULT);
  status = H5Awrite (attr, attributeTypeId, value);

  /*
   * Close and release resources.
   */
  status = H5Aclose (attr);
}

static int writeString(int file, char* dataSetName, char *data)
{
  hsize_t     dims[1] = {1};
  hid_t       typeId, space, dset;
  herr_t      status;

  /*
   * Create string dataspace.  Setting maximum size to NULL sets the maximum
   * size to be the current size.
   */
  space = H5Screate_simple (1, dims, NULL);

  /*
   * Create special string type
   */
  typeId = H5Tcopy(H5T_C_S1);
  H5Tset_size(typeId, strlen(data));

  /*
   * Create the data set and write it.
   */
  dset = H5Dcreate (file, dataSetName, typeId, space, H5P_DEFAULT);
  status = H5Dwrite (dset, typeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

 /*
   * Add attribute SCILAB_Class = string to dataset
   */
  addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_STRING);

  /*
   * Close and release resources.
   */
  status = H5Dclose (dset);
  status = H5Sclose (space);
  return status;
}

int writeStringMatrix(int file, char* dataSetName, char **data, int rows, int cols)
{
  hsize_t     dims[2] = {rows, cols};
  herr_t      status;
  hid_t       space, dset, group;
  hobj_ref_t  *wdata; 

  char	      *groupName = (char *) malloc((strlen(dataSetName) + 3) * sizeof(char));
  char        *pstName = NULL;
  char        *pstPathName = NULL;
	char				*pstSlash			= NULL;
  
  int	      i, j = 0;

  /* Create ref matrix */
  wdata = (hobj_ref_t *) malloc(rows * cols * sizeof(hobj_ref_t));

  /* Generate groupname #<dataSetName># */
  sprintf(groupName, "#%s#", dataSetName);
	pstSlash			= strstr(groupName, "/");
	if(pstSlash != NULL)
	{
		pstSlash[0]					= '_';
	}

  /*
   * First create a group to store all referenced objects.
   */
  group = H5Gcreate (file, groupName, H5P_DEFAULT);
  status = H5Gclose (group);

  /*
   * Now create each String as a dedicated DataSet.
   */
  for (i = 0 ; i < rows ; ++i)
	{
		for ( j = 0 ; j < cols ; ++j)
		{ 
			pstName = (char*)malloc(((int)log10((double)(i + rows * j + 1)) + 4) * sizeof(char)); 
			//1 for null termination, 1 for round value, 2 for '#' characters
			sprintf(pstName, "#%d#", i + rows * j);

			pstPathName = (char*)malloc((strlen(groupName) + strlen(pstName) + 2) * sizeof(char)); 
			//1 for null termination, 1 for separator, 2 for '#' characters
			sprintf(pstPathName, "%s/%s", groupName, pstName);

			// Write the string to ref
			writeString(file, pstPathName, data[i + rows * j]);

			// create the ref
			status = H5Rcreate (&wdata[i * cols + j], file, pstPathName, H5R_OBJECT, -1);

			free(pstName);
			free(pstPathName);
		}
	}


  /*
   * Create dataspace.  Setting maximum size to NULL sets the maximum
   * size to be the current size.
   */
  space = H5Screate_simple (2, dims, NULL);
  
  /*
   * Create the dataset and write the array data to it.
   */
  dset = H5Dcreate (file, dataSetName, H5T_STD_REF_OBJ, space, H5P_DEFAULT);
  status = H5Dwrite (dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT,
  		     wdata);

  /*
   * Add attribute SCILAB_Class = string to dataset
   */
  addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_STRING);

  /*
   * Close and release resources.
   */
  status = H5Dclose (dset);
  status = H5Sclose (space);

  free(groupName);

  return status;
  
}

int writeDoubleMatrix(int file, char* dataSetName, double *data, int rows, int cols)
{
  hsize_t     dims[2] = {rows, cols};
  herr_t      status;
  hid_t       space, dset;
  int	      i = 0, j = 0;
  double      *__data;

  __data = (double*)malloc(sizeof(double) * rows * cols);

  for (i = 0 ; i < rows ; ++i)
    {
      for (j = 0 ; j < cols ; ++j)  
	{
	  __data[i * cols + j] = data[i + rows * j];
	}
    }

	if(rows * cols == 0)
	{
		double dblZero = 0;
		//tips for empty double matrix
		
		/*
		* Create dataspace.  Setting maximum size to NULL sets the maximum
		* size to be the current size.
		*/
		dims[0] = 1;
		dims[1] = 1;

		space = H5Screate_simple (2, dims, NULL);

		/*
		* Create the dataset and write the array data to it.
		*/
		dset = H5Dcreate (file, dataSetName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT);
		status = H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
			&dblZero);

		/*
		* Add attribute SCILAB_Class = double to dataset
		*/
		addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
		addAttribute(dset, g_SCILAB_CLASS_EMPTY, "true");
	}
	else
	{
		/*
		* Create dataspace.  Setting maximum size to NULL sets the maximum
		* size to be the current size.
		*/
		space = H5Screate_simple (2, dims, NULL);

		/*
		* Create the dataset and write the array data to it.
		*/
		dset = H5Dcreate (file, dataSetName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT);
		status = H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
			__data);

		/*
		* Add attribute SCILAB_Class = double to dataset
		*/
		addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);
	}

  /*
   * Close and release resources.
   */
  status = H5Dclose (dset);
  status = H5Sclose (space);

  free(__data);

  return status;
}

int writeBooleanMatrix(int _iFile, char* _pstDatasetName, int *_piData, int _iRows, int _iCols)
{
  hsize_t piDims[2] = {_iRows, _iCols};
  herr_t status;
  hid_t iSpace;
	hid_t iDataset;
  int	i							= 0;
	int j							= 0;
  int* piData				= NULL;

  piData = (int*)malloc(sizeof(int) * _iRows * _iCols);

	for (i = 0 ; i < _iRows; i++)
	{
		for (j = 0 ; j < _iCols ; j++)  
		{
			piData[i * _iCols + j] = _piData[i + _iRows * j];
		}
	}

	/*
	* Create dataspace.  Setting maximum size to NULL sets the maximum
	* size to be the current size.
	*/
	iSpace = H5Screate_simple (2, piDims, NULL);

	/*
	* Create the dataset and write the array data to it.
	*/
	iDataset = H5Dcreate (_iFile, _pstDatasetName, H5T_NATIVE_INT, iSpace, H5P_DEFAULT);
	status = H5Dwrite (iDataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		piData);

	/*
	* Add attribute SCILAB_Class = double to dataset
	*/
	addAttribute(iDataset, g_SCILAB_CLASS, g_SCILAB_CLASS_BOOLEAN);

  /*
   * Close and release resources.
   */
  status = H5Dclose (iDataset);
  status = H5Sclose (iSpace);

  free(piData);

  return status;
}

int writePoly(int _iFile, char* _pstDatasetName, int _iNbCoef, double* _pdblReal)
{
  hsize_t dims[1] = {_iNbCoef};
  hid_t space			= 0;
	hid_t dset			= 0;
  herr_t status		= 0;

  /*
   * Create string dataspace.  Setting maximum size to NULL sets the maximum
   * size to be the current size.
   */
  space						= H5Screate_simple (1, dims, NULL);

  /*
   * Create the data set and write it.
   */
  dset						= H5Dcreate (_iFile, _pstDatasetName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT);
  status					= H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pdblReal);

 /*
   * Add attribute SCILAB_Class = string to dataset
   */
  addAttribute(dset, g_SCILAB_CLASS, g_SCILAB_CLASS_DOUBLE);

  /*
   * Close and release resources.
   */
  status					= H5Dclose (dset);
  status					= H5Sclose (space);
  return status;
}

int writePolyMatrix(int _iFile, char* _pstDatasetName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	int i								= 0;
	int j								= 0;
  hsize_t dims[2]			= {_iRows, _iCols};
  herr_t status				= 0;
  hid_t	space					= 0;
	hid_t	dset					= 0;
	hid_t	group					= 0;
  hobj_ref_t* pData		= 0; 

  char* pstName				= NULL;
  char* pstPathName		= NULL;
	char* pstSlash			= NULL;
  
  char* pstGroupName	= (char *)malloc((strlen(_pstDatasetName) + 3) * sizeof(char));


  /* Create ref matrix */
  pData								= (hobj_ref_t *)malloc(_iRows * _iCols * sizeof(hobj_ref_t));

  /* Generate groupname #<dataSetName># */
  sprintf(pstGroupName, "#%s#", _pstDatasetName);
	pstSlash						= strstr(pstGroupName, "/");
	if(pstSlash != NULL)
	{
		pstSlash[0]				= '_';
	}

  /*
   * First create a group to store all referenced objects.
   */
  group								= H5Gcreate(_iFile, pstGroupName, H5P_DEFAULT);
  status							= H5Gclose(group);

  /*
   * Now create each String as a dedicated DataSet.
   */
  for (i = 0 ; i < _iRows ; i++)
	{
		for ( j = 0 ; j < _iCols ; j++)
		{ 
			pstName					= (char*)malloc(((int)log10((double)(i + _iRows * j + 1)) + 4) * sizeof(char)); 
			//1 for null termination, 1 for round value, 2 for '#' characters
			sprintf(pstName, "#%d#", i + _iRows * j);

			pstPathName			= (char*)malloc((strlen(pstGroupName) + strlen(pstName) + 2) * sizeof(char)); 
			//1 for null termination, 1 for separator, 2 for '#' characters
			sprintf(pstPathName, "%s/%s", pstGroupName, pstName);

			// Write the string to ref
			writePoly(_iFile, pstPathName, _piNbCoef[i + _iRows * j],  _pdblReal[i + _iRows * j]);

			// create the ref
			status					= H5Rcreate(&pData[i * _iCols + j], _iFile, pstPathName, H5R_OBJECT, -1);

			free(pstName);
			free(pstPathName);
		}
	}


  /*
   * Create dataspace.  Setting maximum size to NULL sets the maximum
   * size to be the current size.
   */
  space								= H5Screate_simple(2, dims, NULL);
  
  /*
   * Create the dataset and write the array data to it.
   */
	dset								= H5Dcreate(_iFile, _pstDatasetName, H5T_STD_REF_OBJ, space, H5P_DEFAULT);
  status							= H5Dwrite(dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pData);

  /*
   * Add attribute SCILAB_Class = poly to dataset
   */
  addAttribute(dset, g_SCILAB_CLASS					, g_SCILAB_CLASS_POLY);

  /*
   * Add attribute Varname attribute to dataset
   */
	addAttribute(dset, g_SCILAB_CLASS_VARNAME	, _pstVarName);

  /*
   * Close and release resources.
   */
  status							= H5Dclose(dset);
  status							= H5Sclose(space);

  free(pstGroupName);

  return status;
}


//create a group and create hobj_ref_t array
void* openList(int _iFile, char* pstDatasetName, int _iNbItem)
{
  herr_t      status				= 0;
  hid_t       group					= 0;

	char	      *groupName		= (char *) malloc((strlen(pstDatasetName) + 3) * sizeof(char));
  char        *pstName			= NULL;
  char        *pstPathName	= NULL;
  hobj_ref_t* pobjArray			= NULL;

  /* Generate groupname #<dataSetName># */
  sprintf(groupName, "#%s#", pstDatasetName);

  /*
   * First create a group to store all referenced objects.
   */
  group = H5Gcreate(_iFile, groupName, H5P_DEFAULT);
  status = H5Gclose(group);

	//if(_iNbItem <= 0)
	//{
	//	return NULL;
	//}

	pobjArray = malloc(sizeof(hobj_ref_t) * _iNbItem);
	if(pobjArray == NULL)
	{
		return NULL;
	}

	return pobjArray;
}

int addItemInList(int _iFile, void* _pvList, int _iPos, char* _pstItemName)
{
	hobj_ref_t* pobjArray = (hobj_ref_t*)_pvList;
	return H5Rcreate(&pobjArray[_iPos], _iFile, _pstItemName, H5R_OBJECT, -1);
}

int closeList(int _iFile,  void* _pvList, char* _pstListName, int _iNbItem, int _iVarType)
{
	herr_t status					= 0;
  hsize_t dims[1]				= {_iNbItem};
  hid_t space						= 0;
	hid_t dset						= 0;
	const char* pcstClass	= NULL;

	switch(_iVarType)
	{
	case sci_list : 
		pcstClass = g_SCILAB_CLASS_LIST;
		break;
	case sci_tlist : 
		pcstClass = g_SCILAB_CLASS_TLIST;
		break;
	case sci_mlist : 
		pcstClass = g_SCILAB_CLASS_MLIST;
		break;
	default : 
		return 1;
	}

	if(_iNbItem == 0)
	{
		//tips for empty list
		//insert a fake refence in the array, value = 0
	
		hobj_ref_t pvList[1];
		pvList[0] = 0;
		/*
		 * Create dataspace.  Setting maximum size to NULL sets the maximum
		 * size to be the current size.
		 */

		dims[0] = 1;
		space = H5Screate_simple (1, dims, NULL);

		/*
		* Create the dataset and write the array data to it.
		*/

		dset = H5Dcreate (_iFile, _pstListName, H5T_STD_REF_OBJ, space, H5P_DEFAULT);
		status = H5Dwrite (dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT,
			(hobj_ref_t*)pvList);

		/*
		* Add attribute SCILAB_Class = string to dataset
		*/
	  addAttribute(dset, g_SCILAB_CLASS,  pcstClass);
		addAttribute(dset, g_SCILAB_CLASS_EMPTY, "true");
	}
	else
	{
		/*
		 * Create dataspace.  Setting maximum size to NULL sets the maximum
		 * size to be the current size.
		 */
		space = H5Screate_simple (1, dims, NULL);

		/*
		* Create the dataset and write the array data to it.
		*/
		dset = H5Dcreate (_iFile, _pstListName, H5T_STD_REF_OBJ, space, H5P_DEFAULT);
		status = H5Dwrite (dset, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT,
			(hobj_ref_t*)_pvList);

		/*
		* Add attribute SCILAB_Class = string to dataset
		*/
	  addAttribute(dset, g_SCILAB_CLASS,  pcstClass);
	}  


  /*
   * Close and release resources.
   */
  status = H5Dclose (dset);
  status = H5Sclose (space);

	free(_pvList);
	return status;
}

