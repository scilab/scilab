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

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

matvar_t *GetMatlabVariable(int iVar, const char *name, int matfile_version)
{
  int * var_addr = NULL;
  int var_type;
  SciErr _SciErr;

  switch(VarType(iVar))
    {
    case sci_matrix:
      return GetDoubleVariable(iVar, name, matfile_version);
    case sci_strings:
      return GetCharVariable(iVar, name);
    case sci_ints:
      return GetIntegerVariable(iVar, name);
    case sci_mlist: /* Only cells structs and hypermatrices are managed */
      return GetMlistVariable(iVar, name, matfile_version);
    case sci_sparse:
      return GetSparseVariable(iVar, name);
    default:
      _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;
      sciprint("Do not known how to get variable of type %d\n", var_type);
      return NULL;
    }
}
