/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"
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

int CreateCellVariable(int iVar, matvar_t *matVariable, int * parent, int item_position)
{
  static const char *fieldNames[] = {"ce", "dims","entries"};
  int nbFields = 3;
  int K = 0;
  int prodDims = 0;
  int valueIndex = 0, type;
  int * cell_addr = NULL;
  int * cell_entry_addr = NULL;
  matvar_t ** allData = NULL;
  SciErr _SciErr;

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
  _SciErr = createMatrixOfStringInList(pvApiCtx, iVar, cell_addr, 1, 1, nbFields, (char **)fieldNames); MATIO_ERROR;
  
  /* SECOND LIST ENTRY: Dimensions (int32 type) */
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

  if (prodDims == 1) /* Scalar cell */
    {
      /* Create list entry in the stack */
      if (!CreateMatlabVariable(iVar, allData[0], cell_addr, 3)) /* Could not Create Variable */
	{
	  sciprint("Do not know how to read a variable of class %d.\n", allData[0]->class_type);
	}
    }
  else
    {
      _SciErr = createListInList(pvApiCtx, iVar, cell_addr, 3, prodDims, &cell_entry_addr); MATIO_ERROR;

      for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
        {
          /* Create list entry in the stack */
          if (!CreateMatlabVariable(iVar, allData[valueIndex], cell_entry_addr, valueIndex+1)) /* Could not Create Variable */
            {
              sciprint("Do not know how to read a variable of class %d.\n", allData[valueIndex]->class_type);
            }
        }
    }
  
  return TRUE;
}
