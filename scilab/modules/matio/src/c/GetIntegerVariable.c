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

#include "GetMatlabVariable.h"

matvar_t *GetIntegerVariable(int stkPos, const char *name)
{
  SciIntMat integerMatrix;

  int rank = 0;
  int *dims = NULL;

  matvar_t *createdVar = NULL;

  if(VarType(stkPos) == sci_ints) /* 2-D array */
    {
      rank = 2;
      if ((dims = (int*) MALLOC (sizeof(int) * rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetIntegerVariable");
          return NULL;
        }

      GetRhsVar(stkPos, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &dims[0], &dims[1], (int *) &integerMatrix);

      switch(integerMatrix.it)
        {
        case I_CHAR: /* INT8 */
          createdVar = Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, rank, dims, integerMatrix.D, 0);
          break;
        case I_INT16: /* INT16 */
          createdVar = Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, rank, dims, integerMatrix.D, 0);
          break;
       case I_INT32: /* INT32 */
          createdVar = Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, rank, dims, integerMatrix.D, 0);
          break;
        case I_UCHAR: /* UINT8 */
          createdVar = Mat_VarCreate(name, MAT_C_UINT8, MAT_T_UINT8, rank, dims, integerMatrix.D, 0);
          break;
        case I_UINT16: /* UINT16 */
          createdVar = Mat_VarCreate(name, MAT_C_UINT16, MAT_T_UINT16, rank, dims, integerMatrix.D, 0);
           break;
       case I_UINT32: /* UINT32 */
          createdVar = Mat_VarCreate(name, MAT_C_UINT32, MAT_T_UINT32, rank, dims, integerMatrix.D, 0);
          break;
        default:
          createdVar = NULL;
          break;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Integer matrix expected.\n"), "GetIntegerVariable");
    }

  FREE(dims);

  return createdVar;
}

