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

matvar_t *GetStructVariable(int stkPos, const char *name, int matfile_version, char **fieldNames, int nbFields)
{
  int newStkPos = 0;
  int ilStruct = 0;
  int firstItemAdr = 0;
  int fieldIndex = 0;
  int firstValueAdr = 0, valueIndex = 0;
  int ilListEntries = 0;
  int nbRow = 0;

  int K = 0;

  int prodDims = 1;

  matvar_t *dimensionsVariable = NULL;
  matvar_t **structEntries = NULL;

  newStkPos = stkPos + Top - Rhs; 
  
  ilStruct = iadr(*Lstk(newStkPos));
  if (*istk(ilStruct) < 0) /* Reference */
    {
      ilStruct = iadr(*istk(ilStruct + 1));
    }

  firstItemAdr = sadr(ilStruct+2+nbFields+1);
  
  /* SECOND LIST ENTRY: dimensions */
  *Lstk(newStkPos) = firstItemAdr + *istk(ilStruct + 3) - 1; /* Address of the second list entry */
  dimensionsVariable = GetMatlabVariable(stkPos, "data", /* Do not need to give the format because this variable is just temp */ 0);

  /* Total number of entries */
  for (K=0; K<dimensionsVariable->rank; K++)
    {
      prodDims *= ((int *)dimensionsVariable->data)[K];
    }

  /* OTHERS LIST ENTRIES: ALL STRUCT VALUES */
  if ((structEntries = (matvar_t **) MALLOC (sizeof(matvar_t*)*(prodDims*(nbFields-2)+1))) == NULL)
    {
      freeArrayOfString(fieldNames, nbRow * nbFields);
      Scierror(999, _("%s: No more memory.\n"), "GetStructVariable");
      return NULL;
    }
  for (K = 0; K < prodDims*(nbFields-2)+1; K++)
    {
      structEntries[K] = NULL;
    }

  if (prodDims == 1) /* Scalar struct array */
    {
      for (fieldIndex = 2; fieldIndex < nbFields; fieldIndex++)
        {
          *Lstk(newStkPos) = firstItemAdr + *istk(ilStruct + 3 + fieldIndex - 1) - 1;
          structEntries[fieldIndex - 2] = GetMatlabVariable(stkPos ,fieldNames[fieldIndex], matfile_version);
        }
    }
  else
    {
      /* All the values of each field are stored in a list */

      /* Read all entries */
      for (fieldIndex = 2; fieldIndex < nbFields; fieldIndex++)
        {
          ilListEntries = iadr(firstItemAdr + *istk(ilStruct + 4 + fieldIndex - 2) - 1);
          firstValueAdr = sadr(ilListEntries + 2 + prodDims + 1);
          
          for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
            {
              *Lstk(newStkPos) = firstValueAdr + *istk(ilListEntries + 2 + valueIndex) - 1;
              structEntries[(fieldIndex-1) + (nbFields-2)*valueIndex] = GetMatlabVariable(stkPos ,fieldNames[fieldIndex], matfile_version);
            }
        }
    }

  //freeArrayOfString(fieldNames, nbRow * nbFields);
  return Mat_VarCreate(name, MAT_C_STRUCT, MAT_T_STRUCT, dimensionsVariable->rank, dimensionsVariable->data, structEntries, 0);
}
