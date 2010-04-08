/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetMatlabVariable.h"

#include "api_scilab.h"

#include "freeArrayOfString.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

matvar_t *GetStructVariable(int iVar, const char *name, int matfile_version, char **fieldNames, int nbFields, int * parent, int item_position)
{
  int fieldIndex = 0;
  int valueIndex = 0;
  int nbRow = 0;
  int K = 0;
  int prodDims = 1;
  matvar_t *dimensionsVariable = NULL;
  matvar_t **structEntries = NULL;
  int * var_addr = NULL;
  int * list_addr = NULL;
  int var_type;
  SciErr _SciErr;

  if (parent==NULL)
    {
      _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
    }
  else
    {
      _SciErr = getListItemAddress(pvApiCtx, parent, item_position, &var_addr); MATIO_ERROR;
    }

  _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR; // YC on fait un test de type ??
  _SciErr = getVarDimension(pvApiCtx, var_addr, &nbRow, &nbFields); MATIO_ERROR;

  /* FIRST LIST ENTRY: fieldnames --> NO NEED TO BE READ */
  
  /* SECOND LIST ENTRY: dimensions */
  dimensionsVariable = GetMatlabVariable(iVar, 
					 "data", /* Do not need to give the format because this variable is just temp */ 
					 0, 
					 var_addr, 
					 3);

  /* Total number of entries */
  for (K=0; K<dimensionsVariable->rank; K++)
    {
      prodDims *= ((int *)dimensionsVariable->data)[K];
    }

  /* OTHERS LIST ENTRIES: ALL STRUCT VALUES */
  if ((structEntries = (matvar_t **) MALLOC (sizeof(matvar_t*)*(prodDims*(nbFields-2)+1))) == NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "GetStructVariable");

      freeArrayOfString(fieldNames, nbRow * nbFields);

      return NULL;
    }
  for (K = 0; K < prodDims*(nbFields-2)+1; K++)
    {
      structEntries[K] = NULL;
    }

  if (prodDims == 1) /* Scalar struct array */
    {
      for (fieldIndex = 2; fieldIndex < nbFields; fieldIndex++)
        {
          structEntries[fieldIndex - 2] = GetMatlabVariable(iVar ,fieldNames[fieldIndex], matfile_version, var_addr, fieldIndex+1);
        }
    }
  else
    {
      /* All the values of each field are stored in a list */

      /* Read all entries */
      for (fieldIndex = 2; fieldIndex < nbFields; fieldIndex++)
        {
	  _SciErr = getListInList(pvApiCtx, var_addr, fieldIndex+1, &list_addr); MATIO_ERROR;
          
          for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
            {
              structEntries[(fieldIndex-1) + (nbFields-2)*valueIndex] = GetMatlabVariable(iVar ,fieldNames[fieldIndex], 
											  matfile_version, list_addr, valueIndex+1);
            }
        }
    }

  freeArrayOfString(fieldNames, nbRow * nbFields);

  return Mat_VarCreate(name, MAT_C_STRUCT, MAT_T_STRUCT, dimensionsVariable->rank, dimensionsVariable->data, structEntries, 0);
}
