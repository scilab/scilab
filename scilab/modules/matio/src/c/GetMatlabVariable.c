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

matvar_t *GetMatlabVariable(int stkPos, const char *name, int matfile_version)
{
  
  switch(VarType(stkPos))
    {
    case sci_matrix:
      return GetDoubleVariable(stkPos, name, matfile_version);
    case sci_strings:
      return GetCharVariable(stkPos, name);
    case sci_ints:
      return GetIntegerVariable(stkPos, name);
    case sci_mlist: /* Only cells structs and hypermatrices are managed */
      return GetMlistVariable(stkPos, name, matfile_version);
    case sci_sparse:
      return GetSparseVariable(stkPos, name);
    default:
      sciprint("Do not known how to get variable of type %d\n", VarType(stkPos));
      return NULL;
    }
}
