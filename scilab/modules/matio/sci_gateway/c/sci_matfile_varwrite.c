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

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "gw_matio.h"
#include "GetMatlabVariable.h"
#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr)	     \
    {					     \
      printError(&_SciErr, 0);		     \
      return 0;				     \
    }

enum matfile_errors {
  NO_MORE_VARIABLES = -1,
  UNKNOWN_VARIABLE_TYPE = 0
};

int sci_matfile_varwrite(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  mat_t *matfile = NULL;
  matvar_t *matvar = NULL;
  int fileIndex = 0;
  char *varname = NULL;
  int flag = 0;
  int compressionFlag = 0;
  int var_type;
  int * fd_addr = NULL, * name_addr = NULL, * cp_flag_addr = NULL;
  double tmp_dbl;
  SciErr _SciErr;
  
  CheckRhs(4, 4);
  CheckLhs(1, 1);
  
  /* Input argument is the index of the file to write */
  
  _SciErr = getVarAddressFromPosition(pvApiCtx, 1, &fd_addr); MATIO_ERROR;
  _SciErr = getVarType(pvApiCtx, fd_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_matrix)
    {
      getScalarDouble(pvApiCtx, fd_addr, &tmp_dbl);
      if (!isScalar(pvApiCtx, fd_addr))
	{
	  Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
	  return FALSE;
	}
      fileIndex = (int)tmp_dbl;
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
      return FALSE;
    }
  
  /* Gets the corresponding matfile */
  matfile_manager(MATFILEMANAGER_GETFILE, &fileIndex, &matfile);
  
  /* Second argument is the variable name */
  
  _SciErr = getVarAddressFromPosition(pvApiCtx, 2, &name_addr); MATIO_ERROR;
  _SciErr = getVarType(pvApiCtx, name_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_strings)
    {
      getAllocatedSingleString(pvApiCtx, name_addr, &varname);
      _SciErr = getVarDimension(pvApiCtx, name_addr, &nbRow, &nbCol); MATIO_ERROR;
      if (nbCol != 1)
	{
	  Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), fname);
	  
	  freeAllocatedSingleString(varname);
	  
	  return FALSE;
	}
    }
  else
    {
      Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"), fname);
      
      freeAllocatedSingleString(varname);
      
      return FALSE;
    }
  
  /* Third argument is the variable data */
  matvar = GetMatlabVariable(3, varname, matfile->version, NULL, -1);
  
  /* Fourth argument is the compression flag */
  
  _SciErr = getVarAddressFromPosition(pvApiCtx, 4, &cp_flag_addr); MATIO_ERROR;
  _SciErr = getVarType(pvApiCtx, cp_flag_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_boolean)
    {
      getScalarBoolean(pvApiCtx, cp_flag_addr, &compressionFlag);
      if (!isScalar(pvApiCtx, cp_flag_addr))
	{
	  Scierror(999, _("%s: Wrong size for fourth input argument: Single boolean expected.\n"), fname);
	  
	  freeAllocatedSingleString(varname);
	  
	  return FALSE;
	}
    }
  else
    {
      Scierror(999, _("%s: Wrong type for fourth input argument: Single boolean expected.\n"), fname);
      
      freeAllocatedSingleString(varname);
      
      return FALSE;
    }
  
  flag = Mat_VarWrite(matfile, matvar, compressionFlag);
  
  /* Return execution flag */
  var_type = (flag==0);
  createScalarBoolean(pvApiCtx, Rhs+1, var_type);
  
  freeAllocatedSingleString(varname);
  LhsVar(1) = Rhs+1;
  PutLhsVar();
  
  return TRUE;
}
