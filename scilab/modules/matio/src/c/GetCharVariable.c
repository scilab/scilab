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

matvar_t *GetCharVariable(int stkPos, const char *name)
{
  int dataAdr = 0;

  int rank = 0;
  int *dims = NULL;

  matvar_t *createdVar = NULL;

  if(VarType(stkPos) == sci_strings) /* 2-D array */
    {
      rank = 2;
      if ((dims = (int*) MALLOC (sizeof(int) * rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetCharVariable");
          return NULL;
        }

      GetRhsVar(stkPos, STRING_DATATYPE, &dims[1], &dims[0], &dataAdr);

      if (dims[0]*dims[1] == 0) /* Empty character string */
        {
          createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, cstk(dataAdr), 0);
        }
      else if (dims[0] == 1) /* Scalar character string */
        {
          createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, cstk(dataAdr), 0);
        }
      else /* More than one character string -> save as a Cell */
        {
          /* TODO */
          Scierror(999, _("%s: Scilab string matrix saved as Matlab Cell.\n"), "GetCharVariable");
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "GetCharVariable");
    }

  FREE(dims);

  return createdVar;
}
