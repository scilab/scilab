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
#include "sci_types.h"
#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

matvar_t *GetMatlabVariable(int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
  int * var_addr = NULL;
  int var_type;
  SciErr _SciErr;
  matvar_t * tmp_res = NULL;

  if (parent==NULL)
    {
      _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;
    }
  else
    {
      _SciErr = getListItemAddress(pvApiCtx, parent, item_position, &var_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;
    }

  switch(var_type)
    {
    case sci_matrix:
      tmp_res = GetDoubleVariable(iVar, name, matfile_version, parent, item_position);
      break;
    case sci_strings:
      tmp_res = GetCharVariable(iVar, name, parent, item_position);
      break;
    case sci_ints:
      tmp_res = GetIntegerVariable(iVar, name, parent, item_position);
      break;
    case sci_mlist: 
      /* Only cells structs and hypermatrices are managed */
      //tmp_res = GetMlistVariable(iVar, name, matfile_version, parent, item_position);
      tmp_res = GetMlistVariable(iVar, name, matfile_version, parent, -1);
      break;
    case sci_sparse:
      //tmp_res = GetSparseVariable(iVar, name, parent, item_position);
      tmp_res = GetSparseVariable(iVar, name, parent, -1);
      break;
    default:
      sciprint("Do not known how to get variable of type %d\n", var_type);
      tmp_res = NULL;
    }

  return tmp_res;
}
