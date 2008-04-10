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

int CreateDoubleVariable(int stkPos, matvar_t *matVariable)
{
  int nbRow = 0, nbCol = 0;

  double * complexData = NULL;

  struct ComplexSplit *mat5ComplexData = NULL;

  if(matVariable->rank==2) /* 2-D array */
    {
      nbRow = matVariable->dims[0];
      nbCol = matVariable->dims[1];
      if (matVariable->isComplex == 0)
        {
          CreateVarFromPtr(stkPos, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &matVariable->data);
        }
      else
        {
          if(matVariable->fp->version==MAT_FT_MAT4) /* MATLAB4: data is a table of value */
            {
              complexData =  &(((unsigned char *)matVariable->data)[matVariable->nbytes/2]); //&((matVariable->data)[nbRow*nbCol]);
              CreateCVarFromPtr(stkPos, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &nbRow, &nbCol, &matVariable->data, &complexData);
            }
          else /* MATLAB5 file: data is a ComplexSplit */
            {
              mat5ComplexData = matVariable->data;
              CreateCVarFromPtr(stkPos, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &nbRow, &nbCol, &(mat5ComplexData->Re), &(mat5ComplexData->Im));                 
            }
        }
    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      if (matVariable->isComplex == 0)
        {
          CreateHyperMatrixVariable(stkPos, MATRIX_OF_DOUBLE_DATATYPE,  &matVariable->isComplex, &matVariable->rank, matVariable->dims, matVariable->data, NULL);
        }
      else
        {
#ifndef _MSC_VER
          CreateHyperMatrixVariable(stkPos, MATRIX_OF_DOUBLE_DATATYPE,  &matVariable->isComplex, &matVariable->rank, matVariable->dims, matVariable->data, &(matVariable->data[nbRow*nbCol]));
#endif
        }
    }
  return TRUE;
}
