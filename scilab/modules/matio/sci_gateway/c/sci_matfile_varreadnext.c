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

#include "gw_matio.h"
#include "CreateMatlabVariable.h"

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif


enum matfile_errors {
  NO_MORE_VARIABLES = -1,
  UNKNOWN_VARIABLE_TYPE = 0};

int sci_matfile_varreadnext(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  mat_t *matfile = NULL;

  matvar_t *matvar = NULL;

  int fileIndex = 0;

  char *varname = NULL;
  int varnameAdr = 0;
  int valueAdr = 0;
  int classAdr = 0;

  int returnedClass = 0;

  CheckRhs(1, 1);
  CheckLhs(1, 3);

  /* Input argument is the index of the file to read */
  if (VarType(1) == sci_matrix)
    {
      GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, & nbCol, &stkAdr);
      if (nbRow * nbCol != 1)
        {
          Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
          return FALSE;
        }
      fileIndex = (int)*stk(stkAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
      return FALSE;
    }

  /* Gets the corresponding matfile */
  matfile_manager(MATFILEMANAGER_GETFILE, &fileIndex, &matfile);

  matvar = Mat_VarReadNext(matfile);
  if ((matvar == NULL) || (matvar->name == NULL))
    {
      /* Return empty name */
      nbRow = 0; nbCol = 0;
      CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &varnameAdr);

      /* Return empty value */
      nbRow = 0; nbCol = 0;
      CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &valueAdr);
      
      /* Return error flag instead of variable class */
      nbRow = 1; nbCol = 1;
      CreateVar(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &classAdr);
      *stk(classAdr) = NO_MORE_VARIABLES;
      
      LhsVar(1) = Rhs + 1;
      LhsVar(2) = Rhs + 2;
      LhsVar(3) = Rhs + 3;
      PutLhsVar();

      return TRUE;
    }
  
  /* Return the variable name */
  varname = strdup(matvar->name);
  nbRow = strlen(varname);
  nbCol = 1;
  CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &nbRow, &nbCol, &varname);

  returnedClass = matvar->class_type;

  /* Return the values */
  if (!CreateMatlabVariable(Rhs + 2, matvar)) /* Could not Create Variable */
    {
      sciprint("Do not know how to read a variable of class %d.\n", matvar->class_type);
      returnedClass = UNKNOWN_VARIABLE_TYPE;
    }

  /* Create class return value */
  nbRow = 1; nbCol = 1;
  CreateVar(Rhs + 3, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &classAdr);
  *stk(classAdr) = returnedClass;

  LhsVar(1) = Rhs + 1;
  LhsVar(2) = Rhs + 2;
  LhsVar(3) = Rhs + 3;
  PutLhsVar();

  Mat_VarFree(matvar);
  FREE(varname);

  return TRUE;
}



