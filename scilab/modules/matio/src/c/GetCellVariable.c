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

matvar_t *GetCellVariable(int stkPos, const char *name, int matfile_version)
{
  int newStkPos = 0;
  int ilCell = 0;
  int nbFields = 0;
  int firstItemAdr = 0;
  int firstEntryAdr = 0;
  int ilListEntries = 0;

  int K = 0;

  int prodDims = 1;

  matvar_t *dimensionsVariable = NULL;
  matvar_t **cellEntries = NULL;

  newStkPos = stkPos + Top - Rhs; 
  
  ilCell = iadr(*Lstk(newStkPos));
  if (*istk(ilCell) < 0) /* Reference */
    {
      ilCell = iadr(*istk(ilCell + 1));
    }

  nbFields = *istk(ilCell+1);
  
  firstItemAdr = sadr(ilCell+2+nbFields+1);
  
  /* FIRST LIST ENTRY: fieldnames --> NO NEED TO BE READ */
  
  /* SECOND LIST ENTRY: dimensions */
  *Lstk(newStkPos) = firstItemAdr + *istk(ilCell + 3) - 1; /* Address of the second list entry */
  dimensionsVariable = GetMatlabVariable(stkPos, "data", /* Do not need to give the format because this variable is just temp */ 0);

  /* OTHERS LIST ENTRIES: ALL CELL VALUES */

  /* Total number of entries */
  for (K=0; K<dimensionsVariable->rank; K++)
    {
      prodDims *= ((int *)dimensionsVariable->data)[K];
    }

  if ((cellEntries = (matvar_t **) MALLOC (sizeof(matvar_t*)*prodDims)) == NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "GetCellVariable");
      return NULL;
    }

  if (prodDims == 1) /* Scalar cell array */
    {
      *Lstk(newStkPos) = firstItemAdr + *istk(ilCell + 4) - 1;
      cellEntries[0] = GetMatlabVariable(stkPos ,"data", matfile_version);
    }
  else
    {
      /* Read all entries */
      ilListEntries = iadr(firstItemAdr + *istk(ilCell + 4) - 1);
      firstEntryAdr = sadr(ilListEntries + 2 + prodDims + 1);
      for (K=0; K<prodDims; K++)
        {
          *Lstk(newStkPos) = firstEntryAdr + *istk(ilListEntries + 2 + K) - 1;
          cellEntries[K] = GetMatlabVariable(stkPos ,"data", matfile_version);
        }
    }
  return Mat_VarCreate(name, MAT_C_CELL, MAT_T_CELL, dimensionsVariable->rank, dimensionsVariable->data, cellEntries, 0);
}
