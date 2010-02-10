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

int CreateHyperMatrixVariable(int number, const char *type, int *iscomplex, int * rank, int *dims, double *realdata, double *complexdata)
{
  static const char *tlistFields[] = {"hm", "dims","entries"};

  int tlistSize = 3;

  int nbRow = 0, nbCol = 0;

  int K = 0;

  SciIntMat integerMatrix;

  /* mlist fields */
  nbRow = 1;
  nbCol = 3;
  CreateVarFromPtr(number, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &tlistFields);

  /* hm dimensions */
  integerMatrix.it = I_INT32;
  integerMatrix.m = 1;
  integerMatrix.n = *rank;
  integerMatrix.D = dims;

  CreateVarFromPtr(number + 1, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &integerMatrix.m, &integerMatrix.n, &integerMatrix);


  /* hm entries */
  nbRow = 1;
  for (K=0; K<*rank; K++)
    {
      nbRow *= dims[K];
    }

  nbCol = 1;
  
  if (strcmp(type,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE) == 0)
    {
      integerMatrix.D = realdata;
      integerMatrix.it = *iscomplex;
      integerMatrix.m = nbRow;
      integerMatrix.n = nbCol;
      CreateVarFromPtr(number + 2, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &nbRow, &nbCol, &integerMatrix);
    }
  else if (strcmp(type, MATRIX_OF_BOOLEAN_DATATYPE) == 0)
    {
      CreateVarFromPtr(number + 2, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &realdata);
    }
  else
    {
       if (iscomplex == 0)
        {
          CreateVarFromPtr(number + 2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &realdata);
        }
      else
        {
          CreateCVarFromPtr(number + 2, MATRIX_OF_DOUBLE_DATATYPE, iscomplex, &nbRow, &nbCol, &realdata, &complexdata);
        }
    }

  C2F(mkmlistfromvars)(&number, &tlistSize);

  return TRUE;
}
