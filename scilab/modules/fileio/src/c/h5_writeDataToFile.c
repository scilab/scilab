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
#include <malloc.h>
#include <math.h>
#include "h5_writeDataToFile.h"


#define SCILAB_CLASS	"SCILAB_Class"
#define DOUBLE		"double"
#define STRING		"string"
#define LIST			"list"


static void addAttribute(int dataSetId, char *name, char *value)
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
  addAttribute(dset, SCILAB_CLASS, STRING);

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
  addAttribute(dset, SCILAB_CLASS, STRING);

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
  int	      i =0, j = 0;
  double      *__data;

  __data = (double*)malloc(sizeof(double) * rows * cols);

  for (i = i ; i < rows ; ++i)
    {
      for (j = 0 ; j < cols ; ++j)  
	{
	  __data[i * cols + j] = data[i + rows * j];
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
  dset = H5Dcreate (file, dataSetName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT);
  status = H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		     __data);

  /*
   * Add attribute SCILAB_Class = double to dataset
   */
  addAttribute(dset, SCILAB_CLASS, DOUBLE);

  /*
   * Close and release resources.
   */
  status = H5Dclose (dset);
  status = H5Sclose (space);

  free(__data);

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

	if(_iNbItem <= 0)
	{
		return NULL;
	}

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

int closeList(int _iFile,  void* _pvList, char* _pstListName, int _iNbItem)
{
	herr_t			status = 0;
  hsize_t     dims[1] = {_iNbItem};
  hid_t       space, dset;
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
  addAttribute(dset, SCILAB_CLASS, LIST);

  /*
   * Close and release resources.
   */
  status = H5Dclose (dset);
  status = H5Sclose (space);

	free(_pvList);
	return status;
}

/* int readDoubleMatrix(int file, char* dataSetName, double **data, int *rows, int *cols) */
/* { */
  
/*   hsize_t     dims[2] = {rows, cols}; */
/*   herr_t      status; */
/*   hid_t       filetype, memtype, space, dset; */
/*   /\* */
/*    * Create dataspace.  Setting maximum size to NULL sets the maximum */
/*    * size to be the current size. */
/*    *\/ */
/*   space = H5Screate_simple (2, dims, NULL); */

/*   /\* */
/*    * Create the dataset and write the array data to it. */
/*    *\/ */
/*   dset = H5Dcreate (file, dataSetName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT); */
/*   status = H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, */
/* 		     data); */

/*   /\* */
/*    * Close and release resources. */
/*    *\/ */
/*   status = H5Dclose (dset); */
/*   status = H5Sclose (space); */

/*   return status; */
/* } */
