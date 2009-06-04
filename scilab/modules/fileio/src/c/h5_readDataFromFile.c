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
#include "h5_attributeConstants.h"

/************************************************************

  Operator function.  Prints the name and type of the object
  being examined.

 ************************************************************/
static herr_t op_func (hid_t loc_id, const char *name, void *operator_data)
{
    herr_t          status;
    H5G_stat_t      statbuf;
    int		    *pDataSetId = operator_data;

    /*
     * Get type of the object and return only datasetId
     * through operator_data.
     */
    status = H5Gget_objinfo (loc_id, name, 0, &statbuf);
    switch (statbuf.type) {
        case H5G_GROUP:
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
static char *readAttribute(int _iDataSetId, char *_pstName)
{
  hid_t		iAttributeId;
  hid_t		iFileType, memtype, iSpace;
  herr_t	status;
  hsize_t	dims[1];
  size_t	iDim;
  
  char		*pstValue;

  
  iAttributeId = H5Aopen_name(_iDataSetId, _pstName);
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
  pstValue = (char *) malloc (dims[0] * iDim * sizeof (char));

  /*
   * Create the memory datatype.
   */
  memtype = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (memtype, iDim);

  /*
   * Read the data.
   */
  status = H5Aread (iAttributeId, memtype, pstValue);

  return pstValue;

}

int getDataSetId(int  _iFile)
{
  herr_t          status;
  int		  dataSetId;
  /*
   * Begin iteration.
   */
  //  printf ("Objects in root group:\n");
  status = H5Giterate (_iFile, "/", NULL, op_func, &dataSetId);

  return dataSetId;
}


int getDataSetDims(int _iDataSetId, int *_piRows, int *_piCols)
{
  hsize_t	lDims[2];
  hid_t		space;
  /*
   * Get dataspace and dimensions of the dataset. This is a
   * two dimensional dataset.
   */
 space = H5Dget_space (_iDataSetId);
 H5Sget_simple_extent_dims (space , lDims, NULL);
 *_piRows = lDims[0];
 *_piCols = lDims[1];

 return 0;
}

int readDoubleMatrix(int _iDataSetId, double *_pdblData, int _iRows, int _iCols)
{
 herr_t      status;
 double      *pdblLocalData;
 int	     i = 0, j = 0;

  pdblLocalData = (double*)malloc(sizeof(double) * _iRows * _iCols); 
  /*
  * Read the data.
  */
 status = H5Dread(_iDataSetId, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		  pdblLocalData);
 
 for (i = 0 ; i < _iRows ; ++i)
   {
     for (j = 0 ; j < _iCols ; ++j)  
       {
	 _pdblData[i * _iCols + j] = pdblLocalData[i + _iRows * j];
       }
   }
 status = H5Dclose(_iDataSetId);

 free(pdblLocalData);

 return status;
}

static int readString(int _iDataSetId, char **_pstData)
{
  hid_t		iFileType, memtype, iSpace;
  herr_t	status;
  hsize_t	dims[1];
  size_t	iDim;
  
  /*
   * Get the datatype and its size.
   */
  iFileType = H5Dget_type (_iDataSetId);
  iDim = H5Tget_size (iFileType);
  iDim++;                         /* Make room for null terminator */
  
  /*
   * Get dataspace and allocate memory for read buffer.  This is a
   * two dimensional attribute so the dynamic allocation must be done
   * in steps.
   */
  iSpace = H5Dget_space (_iDataSetId);
  H5Sget_simple_extent_dims (iSpace, dims, NULL);

  /*
   * Allocate space for string data.
   */
  *_pstData = (char *) malloc (dims[0] * iDim * sizeof (char));

  /*
   * Create the memory datatype.
   */
  memtype = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (memtype, iDim);

  /*
   * Read the data.
   */
  status = H5Dread (_iDataSetId, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, *_pstData);
  
  return 0;
}

int readStringMatrix(int _iDataSetId, char **_pstData, int _iRows, int _iCols)
{
  hid_t		obj;
  hobj_ref_t	*rdata = (hobj_ref_t *) malloc (_iRows * _iCols * sizeof (hobj_ref_t));
  herr_t	status;
  int		i = 0, j = 0;

  /*
   * Read the data.
   */
  status = H5Dread (_iDataSetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		    rdata);
  for (i = 0 ; i < _iRows ; i++) 
    {
      for (j = 0 ; j < _iCols ; ++j)
	{
	  /*
	   * Open the referenced object, get its name and type.
	   */
	  obj = H5Rdereference (_iDataSetId, H5R_OBJECT, &rdata[i * _iCols + j]);
	  readString(obj, &_pstData[i + j * _iRows]);
	}
    }

  free(rdata);

  return 0;
}

int getScilabTypeFromDataSet(int _iDataSetId)
{
  hid_t iAttributeId;
  char *pstScilabClass = readAttribute(_iDataSetId, SCILAB_CLASS);

  /* HDF5 Float type + SCILAB_Class = double <=> double */
  if (H5Tget_class(H5Dget_type(_iDataSetId)) == H5T_FLOAT
      && strcmp(pstScilabClass, SCILAB_CLASS_DOUBLE) == 0) {
    free(pstScilabClass);
    return sci_matrix;
  }
  
  /* HDF5 Reference type + SCILAB_Class = string <=> strings */
  if (H5Tget_class(H5Dget_type(_iDataSetId)) == H5T_REFERENCE
      && strcmp(pstScilabClass, SCILAB_CLASS_STRING) == 0) {
    free(pstScilabClass);
    return sci_strings;
  }

  /* HDF5 Reference type + SCILAB_Class = list <=> list */
  if (H5Tget_class(H5Dget_type(_iDataSetId)) == H5T_REFERENCE
      && strcmp(pstScilabClass, SCILAB_CLASS_LIST) == 0) {
    free(pstScilabClass);
    return sci_list;
  }

    /* HDF5 Reference type + SCILAB_Class = tlist <=> tlist */
  if (H5Tget_class(H5Dget_type(_iDataSetId)) == H5T_REFERENCE
      && strcmp(pstScilabClass, SCILAB_CLASS_TLIST) == 0) {
    free(pstScilabClass);
    return sci_tlist;
  }

  /* HDF5 Reference type + SCILAB_Class = string <=> MLIST */
  if (H5Tget_class(H5Dget_type(_iDataSetId)) == H5T_REFERENCE
      && strcmp(pstScilabClass, SCILAB_CLASS_MLIST) == 0) {
    free(pstScilabClass);
    return sci_mlist;
  }
  return -1;
}
