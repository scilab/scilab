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
#include "freeArrayOfString.h"

matvar_t *GetMlistVariable(int stkPos, const char *name, int matfile_version)
{
  char **fieldNames = NULL;

  int nbRow = 0, nbFields = 0;

  int ilStruct = 0;

  int pointerSave = 0;

  int newStkPos = 0;

  if(VarType(stkPos) == sci_mlist)
    {
      newStkPos = stkPos + Top - Rhs; 
      
      ilStruct = iadr(*Lstk(newStkPos));
      if (*istk(ilStruct) < 0) /* Reference */
        {
          ilStruct = iadr(*istk(ilStruct + 1));
        }

      nbFields = *istk(ilStruct+1);
  
      pointerSave = *Lstk(newStkPos);
      *Lstk(newStkPos) = sadr(ilStruct+2+nbFields+1); /* Address of the first list entry */
 
      /* FIRST LIST ENTRY: fieldnames */
      GetRhsVar(stkPos, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbFields, &fieldNames);

      if (strcmp(fieldNames[0], "ce")==0)
        {
          *Lstk(newStkPos) = pointerSave;
          freeArrayOfString(fieldNames, nbRow * nbFields);
          return GetCellVariable(stkPos, name, matfile_version);
        }
      else if (strcmp(fieldNames[0], "st")==0)
        {
          *Lstk(newStkPos) = pointerSave;
          return GetStructVariable(stkPos, name, matfile_version, fieldNames, nbFields);
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
