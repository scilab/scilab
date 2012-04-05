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

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "CreateMatlabVariable.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

int CreateStructVariable(int iVar, matvar_t *matVariable, int * parent, int item_position)
{
  char **fieldNames = NULL;
  int nbFields = 0;
  int fieldIndex = 0;
  int K = 0;
  int prodDims = 0;
  int valueIndex = 0;
  matvar_t *fieldMatVar = NULL;
  matvar_t ** allData = NULL;
  int * cell_addr = NULL;
  int * cell_entry_addr = NULL;
  int type;
  SciErr _SciErr;
    
  /* Fields of the struct */
  nbFields = 2; /* "st" "dims" */
  nbFields += Mat_VarGetNumberOfFields(matVariable);

  fieldNames = (char**) MALLOC(sizeof(char*) * nbFields);
  if (fieldNames==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
      return FALSE;
    }
  
  fieldNames[0] = strdup("st");
  if (fieldNames[0]==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
      return FALSE;
    }
  fieldNames[1] = strdup("dims");
  if (fieldNames[1]==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
      return FALSE;
    }
  
  for (fieldIndex = 1; fieldIndex < nbFields - 1; fieldIndex++)
    {
      fieldMatVar = Mat_VarGetStructField(matVariable, &fieldIndex, BY_INDEX, 0);
      fieldNames[fieldIndex + 1] = strdup(fieldMatVar->name);
      if (fieldNames[fieldIndex + 1]==NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
          return FALSE;
        }
    }
  
  /* Returned mlist initialization */
  if (parent==NULL)
    {
      _SciErr = createMList(pvApiCtx, iVar, nbFields, &cell_addr); MATIO_ERROR;
    }
  else
    {
      _SciErr = createMListInList(pvApiCtx, iVar, parent, item_position, nbFields, &cell_addr); MATIO_ERROR;
    }

  /* FIRST LIST ENTRY: fieldnames */
  _SciErr = createMatrixOfStringInList(pvApiCtx, iVar, cell_addr, 1, 1, nbFields, fieldNames); MATIO_ERROR;
  
  /* SECOND LIST ENTRY: Dimensions (int32 type) */
  if (nbFields==2) /* Empty struct must have size 0x0 in Scilab */
    {
      matVariable->dims[0] = 0;
      matVariable->dims[1] = 0;
    }

  if(matVariable->rank==2) /* Two dimensions */
    {
      _SciErr = createMatrixOfInteger32InList(pvApiCtx, iVar, cell_addr, 2, 1, matVariable->rank, matVariable->dims); MATIO_ERROR;
    }
  else /* 3 or more dimensions -> Scilab HyperMatrix */
    {
      type = I_INT32;
      CreateHyperMatrixVariable(iVar, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, 
				&type, &matVariable->rank, matVariable->dims, matVariable->data,
				NULL, cell_addr, 2);
    }

  /* ALL OTHER ENTRIES: Fields data */
  prodDims = 1;
  for (K=0; K<matVariable->rank; K++)
    {
      prodDims *= matVariable->dims[K];
    }

  allData = (matvar_t**) (matVariable->data);

  if (prodDims == 1) /* Scalar struct */
    {
      for (fieldIndex = 0; fieldIndex < nbFields - 2; fieldIndex++)
        {
          /* Create list entry in the stack */
          if (!CreateMatlabVariable(iVar, allData[fieldIndex], cell_addr, fieldIndex+3)) /* Could not Create Variable */
            {
              if (allData[fieldIndex]->class_type != 0) /* class is 0 for not initialized fields */
                {
                  sciprint("Do not know how to read a variable of class %d.\n", allData[fieldIndex]->class_type);
                }
            }
        }
    }
  else
    {
      for (fieldIndex = 0; fieldIndex < nbFields - 2; fieldIndex++)
        {
	  _SciErr = createListInList(pvApiCtx, iVar, cell_addr, fieldIndex+3, prodDims, &cell_entry_addr); MATIO_ERROR;
          
          for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
            {
              /* Create list entry in the stack */
              if (!CreateMatlabVariable(iVar, allData[(fieldIndex) + (nbFields-2)*valueIndex], cell_entry_addr, valueIndex+1)) /* Could not Create Variable */
                {
                  if (allData[(fieldIndex) + (nbFields-2)*valueIndex]->class_type != 0) /* class is 0 for not initialized fields */
                    {
                      sciprint("Do not know how to read a variable of class %d.\n", allData[(fieldIndex) + (nbFields-2)*valueIndex]->class_type);
                    }
                }
            }
        }
    }

  freeArrayOfString(fieldNames, nbFields);

  return TRUE;
}

