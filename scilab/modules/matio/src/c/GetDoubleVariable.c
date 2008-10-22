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

matvar_t * GetDoubleVariable(int stkPos, const char* name, int matfile_version)
{
  int realDataAdr = 0, complexDataAdr = 0;

  int rank = 0;
  int *dims = NULL;
  int isComplex = 0;

  struct ComplexSplit mat5ComplexData;

  matvar_t *createdVar = NULL;

  if(VarType(stkPos) == sci_matrix) /* 2-D array */
    {
      rank = 2;
      if ((dims = (int*) MALLOC (sizeof(int) * rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetDoubleVariable");
          return NULL;
        }

      GetRhsCVar(stkPos, MATRIX_OF_DOUBLE_DATATYPE, &isComplex, &dims[0], &dims[1], &realDataAdr, &complexDataAdr);

      if (isComplex==0)
        {
          createdVar =  Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, dims, stk(realDataAdr), 0);
        }
      else
        {
          if(matfile_version==MAT_FT_MAT4) /* MATLAB4: data is a table of value */
            {
              createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, dims, stk(realDataAdr), MAT_F_COMPLEX);
            }
          else /* MATLAB5 file: data is a ComplexSplit */
            {
              mat5ComplexData.Re = stk(realDataAdr);
              mat5ComplexData.Im = stk(complexDataAdr);
              createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, dims, &mat5ComplexData, MAT_F_COMPLEX);
            }
          createdVar->isComplex = 1;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double matrix expected.\n"), "GetDoubleVariable");
    }

  FREE(dims);

  return createdVar;
}
