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

#include "freeArrayOfString.h"
#include "MALLOC.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

#define DEBUG 1

matvar_t *GetMlistVariable(int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
  char **fieldNames = NULL;
  int * pilen = NULL;
  int nbRow = 0, nbFields = 0, i;
  int * var_addr = NULL;
  int var_type;
  SciErr _SciErr;
  matvar_t * tmp_res = NULL;

  if ((parent==NULL)&&(item_position==-1))
    {
      _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
    }
  else if ((parent!=NULL)&&(item_position==-1))
    {
      var_addr = parent;
    }
  else
    {
      _SciErr = getListItemAddress(pvApiCtx, parent, item_position, &var_addr); MATIO_ERROR;
    }

  _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;

  if (var_type == sci_mlist)
    {
      /* FIRST LIST ENTRY: fieldnames */
      _SciErr    = getMatrixOfStringInList(pvApiCtx, var_addr, 1, &nbRow, &nbFields, NULL, NULL); MATIO_ERROR;
      pilen      = (int *)MALLOC(nbRow*nbFields*sizeof(int));
      fieldNames = (char **)MALLOC(nbRow*nbFields*sizeof(char *));
      _SciErr    = getMatrixOfStringInList(pvApiCtx, var_addr, 1, &nbRow, &nbFields, pilen, NULL); MATIO_ERROR;
      for(i=0;i<nbRow*nbFields;i++)
      	{
      	  fieldNames[i] = (char *)MALLOC((pilen[i]+1)*sizeof(char));
      	}
      _SciErr = getMatrixOfStringInList(pvApiCtx, var_addr, 1, &nbRow, &nbFields, pilen, fieldNames); MATIO_ERROR;
      FREE(pilen);
      
      if (strcmp(fieldNames[0], "ce")==0)
        {
          freeArrayOfString(fieldNames, nbRow * nbFields);
          return GetCellVariable(iVar, name, matfile_version, parent, item_position);
        }
      else if (strcmp(fieldNames[0], "st")==0)
        {
          tmp_res = GetStructVariable(iVar, name, matfile_version, fieldNames, nbFields, parent, item_position);
          freeArrayOfString(fieldNames, nbRow * nbFields);
	  return tmp_res;
        }
      else if (strcmp(fieldNames[0], "hm")==0)
        {
          /* TODO */
          Scierror(999, _("%s: Mlists of type %s can not be written to Matlab binary files.\n"), "GetMlistVariable", fieldNames[0]);
          freeArrayOfString(fieldNames, nbRow * nbFields);
          return NULL;
        }
      else
        {
          Scierror(999, _("%s: Mlists of type %s can not be written to Matlab binary files.\n"), "GetMlistVariable", fieldNames[0]);
          freeArrayOfString(fieldNames, nbRow * nbFields);
          return NULL;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Mlist expected.\n"), "GetMlistVariable");
      return NULL;
    }
}
