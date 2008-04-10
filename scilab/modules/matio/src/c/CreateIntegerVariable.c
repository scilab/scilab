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

int CreateIntegerVariable(int stkPos, int integerType, matvar_t *matVariable)
{
  SciIntMat integerMatrix;

  // Integer Type
  integerMatrix.it = integerType;
  
  // Matrix dimensions
  integerMatrix.m = matVariable->dims[0];
  integerMatrix.n = matVariable->dims[1];

  // Matrix data
  integerMatrix.D = matVariable->data;

  if(matVariable->rank==2) /* 2-D array */
    {
      CreateVarFromPtr(stkPos, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &integerMatrix.m, &integerMatrix.n, &integerMatrix);
    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      CreateHyperMatrixVariable(stkPos, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,  &integerMatrix.it, &matVariable->rank, matVariable->dims, matVariable->data, NULL);
    }
  
  return TRUE;
}

