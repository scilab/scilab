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

#include "CreateMatlabVariable.h"

int CreateCellVariable(int stkPos, matvar_t *matVariable)
{
  static const char *fieldNames[] = {"ce", "dims","entries"};
  int nbFields = 3;

  int nbRow = 0;
  int K = 0;
  int prodDims = 0;
  int listAdr = 0;
  int valueIndex = 0;
  int sizeOfList = 0;

  int ilStruct = 0;
  int il = 0;

  SciIntMat integerMatrix; /* Used for dimensions */

  matvar_t ** allData = NULL;

  /* Returned mlist initialization */
  stkPos = stkPos + Top - Rhs; 

  ilStruct = iadr(*Lstk(stkPos));
  *istk(ilStruct) = 17;
  *istk(ilStruct+1) = nbFields;
  *istk(ilStruct+2) = 1;
  
  *Lstk(stkPos+1) = sadr(ilStruct+2+nbFields+1); /* Address of the first list entry */
 
  /* FIRST LIST ENTRY: fieldnames */
  nbRow = 1;
  CreateVarFromPtr(stkPos + 1 + Rhs - Top, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbFields, fieldNames);
  /* Update the list "pointers" */
  *istk(ilStruct+3) = *istk(ilStruct+2) + *Lstk(stkPos + 2) - *Lstk(stkPos + 1);
  /* Reinit current variable address */
  *Lstk(stkPos + 1) = *Lstk(stkPos + 2);
  
  /* SECOND LIST ENTRY: Dimensions (int32 type) */
  integerMatrix.it = I_INT32;
  integerMatrix.m = 1;
  integerMatrix.n = matVariable->rank;
  integerMatrix.D = matVariable->dims;

  if(matVariable->rank==2) /* Two dimensions */
    {
      CreateVarFromPtr(stkPos + 1 + Rhs - Top, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &integerMatrix.m, &integerMatrix.n, &integerMatrix);
    }
  else /* 3 or more dimensions -> Scilab HyperMatrix */
    {
      CreateHyperMatrixVariable(stkPos + 1 + Rhs - Top, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,  &integerMatrix.it, &matVariable->rank, matVariable->dims, matVariable->data, NULL);
    }

  /* Update the list "pointers" */
  *istk(ilStruct+4) = *istk(ilStruct+3) + *Lstk(stkPos + 2) - *Lstk(stkPos + 1);
  /* Reinit current variable address */
  *Lstk(stkPos + 1) = *Lstk(stkPos + 2);

  /* ALL OTHER ENTRIES: Fields data */
  prodDims = 1;
  for (K=0; K<matVariable->rank; K++)
    {
      prodDims *= matVariable->dims[K];
    }

  allData = (matvar_t**) (matVariable->data);

  if (prodDims == 1) /* Scalar cell */
    {
      listAdr = stkPos + 1;

      for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
        {
          /* Create list entry in the stack */
          if (!CreateMatlabVariable(listAdr + Rhs - Top, allData[valueIndex])) /* Could not Create Variable */
            {
              sciprint("Do not know how to read a variable of class %d.\n", allData[valueIndex]->class_type);
            }
          
          /* Update the returned list "pointers" */
          *istk(ilStruct+5+valueIndex) = *istk(ilStruct+4+valueIndex) + *Lstk(listAdr + 1) - *Lstk(listAdr);
          /* Reinit current variable address */
          *Lstk(listAdr) = *Lstk(listAdr + 1);
        }
    }
  else
    {
      listAdr = stkPos + 1;
      
      sizeOfList = prodDims;
      
      il = iadr(*Lstk(listAdr));
      *istk(il) = 15;
      *istk(il+1) = sizeOfList;
      *istk(il+2) = 1;
      
      *Lstk(listAdr+1) = sadr(il+2+sizeOfList+1);
      
      for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
        {
          /* Create list entry in the stack */
          if (!CreateMatlabVariable(listAdr + 1 + Rhs - Top, allData[valueIndex])) /* Could not Create Variable */
            {
              sciprint("Do not know how to read a variable of class %d.\n", allData[valueIndex]->class_type);
            };
          
          /* Update the list "pointers" */
          *istk(il+2+valueIndex+1) = *istk(il+1+valueIndex+1) + *Lstk(listAdr + 2) - *Lstk(listAdr + 1);
          /* Reinit current variable address */
          *Lstk(listAdr + 1) = *Lstk(listAdr + 2);
        }
      
      /* Update the returned list "pointers" */
      *istk(ilStruct+5) = *istk(ilStruct+4) + *Lstk(stkPos + 2) - *Lstk(stkPos + 1);
      /* Reinit current variable address */
      *Lstk(stkPos + 1) = *Lstk(stkPos + 2);
    }
  
  /* Emulate stack2.c returned values management */
  C2F(intersci).ntypes[stkPos - Top + Rhs - 1] = '$';
  C2F(intersci).lad[stkPos - Top + Rhs - 1] = ilStruct + 2 + nbFields + 1;

  return TRUE;
}
