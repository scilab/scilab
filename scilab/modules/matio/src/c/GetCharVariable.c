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

#include "GetMatlabVariable.h"

#include "api_common.h"
#include "api_string.h"
#include "api_double.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

matvar_t *GetCharVariable(int iVar, const char *name)
{
  char * dataAdr = NULL;
  int rank = 0;
  int *dims = NULL;
  matvar_t *createdVar = NULL;
  int * var_addr = NULL;
  int var_type;
  SciErr _SciErr;

  _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
  _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;

  if(var_type == sci_strings) /* 2-D array */
    {
      rank = 2;
      if ((dims = (int*)MALLOC(sizeof(int) * rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetCharVariable");
          return NULL;
        }

      getAllocatedSingleString(pvApiCtx, var_addr, &dataAdr);
      _SciErr = getVarDimension(pvApiCtx, var_addr, &dims[1], &dims[0]); MATIO_ERROR;

      if (dims[0] == 0) /* Empty character string */
        {
          createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, dataAdr, 0);
        }
      else if (dims[0] == 1) /* Scalar character string */
        {
          createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, dataAdr, 0);
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
