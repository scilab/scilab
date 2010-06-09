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

#include "CreateMatlabVariable.h"
#include "stack-c.h"
#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

int CreateDoubleVariable(int iVar, matvar_t *matVariable, int * parent, int item_position)
{
  int nbRow = 0, nbCol = 0;
  struct ComplexSplit *mat5ComplexData = NULL;
  SciErr _SciErr;

  if(matVariable->rank==2) /* 2-D array */
    {
      nbRow = matVariable->dims[0];
      nbCol = matVariable->dims[1];
      if (matVariable->isComplex == 0)
        {
	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfDouble(pvApiCtx, iVar, nbRow, nbCol, matVariable->data); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, matVariable->data); MATIO_ERROR;
	    }
        }
      else
        {
          /* Since MATIO 1.3.2 data is a ComplexSplit for MAT4 and MAT5 formats */
          mat5ComplexData = matVariable->data;
	  if (parent==NULL)
	    {
	      _SciErr = createComplexMatrixOfDouble(pvApiCtx, iVar, nbRow, nbCol, mat5ComplexData->Re, mat5ComplexData->Im);
	    }
	  else
	    {
	      _SciErr = createComplexMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, 
							  mat5ComplexData->Re, mat5ComplexData->Im);
	    }
        }
    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      if (matVariable->isComplex == 0)
        {
          CreateHyperMatrixVariable(iVar, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &matVariable->rank, 
				    matVariable->dims, matVariable->data, NULL, parent, item_position);
        }
      else
        {
          mat5ComplexData = matVariable->data;
          CreateHyperMatrixVariable(iVar, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &matVariable->rank, 
				    matVariable->dims, mat5ComplexData->Re, mat5ComplexData->Im, parent, item_position);
        }
    }
  return TRUE;
}
