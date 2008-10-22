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
#include "GetMatlabVariable.h"

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif


enum matfile_errors {
  NO_MORE_VARIABLES = -1,
  UNKNOWN_VARIABLE_TYPE = 0};

int sci_matfile_varwrite(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  mat_t *matfile = NULL;

  matvar_t *matvar = NULL;

  int fileIndex = 0;

  char *varname = NULL;

  int flag = 0;
  int compressionFlag = 0;
  int flagAdr = 0;

  CheckRhs(4, 4);
  CheckLhs(1, 1);

  /* Input argument is the index of the file to write */
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

  /* Second argument is the variable name */
  if (VarType(2) == sci_strings)
    {
      GetRhsVar(2, STRING_DATATYPE, &nbRow, & nbCol, &stkAdr);
      if (nbCol != 1)
        {
          Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), fname);
          return FALSE;
        }
      varname = cstk(stkAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"), fname);
      return FALSE;
    }

  /* Third argument is the variable data */
  matvar = GetMatlabVariable(3, varname, matfile->version);
  
  /* Fourth argument is the compression flag */
  if (VarType(4) == sci_boolean)
    {
      GetRhsVar(4, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, & nbCol, &stkAdr);
      if (nbRow * nbCol != 1)
        {
          Scierror(999, _("%s: Wrong size for fourth input argument: Single boolean expected.\n"), fname);
          return FALSE;
        }
      compressionFlag = *istk(stkAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for fourth input argument: Single boolean expected.\n"), fname);
      return FALSE;
    }

  flag = Mat_VarWrite(matfile, matvar, compressionFlag);

  /* Return execution flag */
  nbRow = 1; nbCol = 1;
  CreateVar(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &flagAdr);
  *istk(flagAdr) = flag==0;

  LhsVar(1) = Rhs + 1;

  PutLhsVar();

  return TRUE;
}



