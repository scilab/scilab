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
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
int CreateStructVariable(int stkPos, matvar_t *matVariable)
{
  char **fieldNames = NULL;
  int nbFields = 0;
  int nbRow = 0;
  int fieldIndex = 0;
  int K = 0;
  int prodDims = 0;
  int listAdr = 0;
  int valueIndex = 0;
  matvar_t *fieldMatVar = NULL;
  int sizeOfList = 0;

  int ilStruct = 0;
  int il = 0;

  SciIntMat integerMatrix; /* Used for dimensions */

  matvar_t ** allData = NULL;

  /* Fields of the struct */
  nbFields = 2; /* "st" "dims" */
  nbFields += Mat_VarGetNumberOfFields(matVariable);

  fieldNames = (char**) MALLOC(sizeof(char*) * nbFields);
  if (fieldNames==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
      return FALSE;
    }
  
  fieldNames[0] = strdup("st");
  if (fieldNames[0]==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
      return FALSE;
    }
  fieldNames[1] = strdup("dims");
  if (fieldNames[1]==NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
      return FALSE;
    }
  
  for (fieldIndex = 1; fieldIndex < nbFields - 1; fieldIndex++)
    {
      fieldMatVar = Mat_VarGetStructField(matVariable, &fieldIndex, BY_INDEX, 0);
      fieldNames[fieldIndex + 1] = strdup(fieldMatVar->name);
      if (fieldNames[fieldIndex + 1]==NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
          return FALSE;
        }
    }
  
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
  if (nbFields==2) /* Empty struct must have size 0x0 in Scilab */
    {
      matVariable->dims[0] = 0;
      matVariable->dims[1] = 0;
    }
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

  if (prodDims == 1) /* Scalar struct */
    {
      listAdr = stkPos + 1;

      for (fieldIndex = 0; fieldIndex < nbFields - 2; fieldIndex++)
        {
          /* Create list entry in the stack */
          if (!CreateMatlabVariable(listAdr + Rhs - Top, allData[fieldIndex])) /* Could not Create Variable */
            {
              if (allData[fieldIndex]->class_type != 0) /* class is 0 for not initialized fields */
                {
                  sciprint("Do not know how to read a variable of class %d.\n", allData[fieldIndex]->class_type);
                }
            }
          
          /* Update the returned list "pointers" */
          *istk(ilStruct+5+fieldIndex) = *istk(ilStruct+4+fieldIndex) + *Lstk(listAdr + 1) - *Lstk(listAdr);
          /* Reinit current variable address */
          *Lstk(listAdr) = *Lstk(listAdr + 1);
        }
    }
  else
    {
      listAdr = stkPos + 1;
      
      for (fieldIndex = 1; fieldIndex < nbFields - 1; fieldIndex++)
        {
          sizeOfList = prodDims;

          il = iadr(*Lstk(listAdr));
          *istk(il) = 15;
          *istk(il+1) = sizeOfList;
          *istk(il+2) = 1;

          *Lstk(listAdr+1) = sadr(il+2+sizeOfList+1);
          
          for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
            {
              /* Create list entry in the stack */
              if (!CreateMatlabVariable(listAdr + 1 + Rhs - Top, allData[(fieldIndex-1) + (nbFields-2)*valueIndex])) /* Could not Create Variable */
                {
                  if (allData[(fieldIndex-1) + (nbFields-2)*valueIndex]->class_type != 0) /* class is 0 for not initialized fields */
                    {
                      sciprint("Do not know how to read a variable of class %d.\n", allData[(fieldIndex-1) + (nbFields-2)*valueIndex]->class_type);
                    }
                }

              /* Update the list "pointers" */
              *istk(il+2+valueIndex+1) = *istk(il+1+valueIndex+1) + *Lstk(listAdr + 2) - *Lstk(listAdr + 1);
              /* Reinit current variable address */
              *Lstk(listAdr + 1) = *Lstk(listAdr + 2);
            }

          /* Update the returned list "pointers" */
          *istk(ilStruct+4+fieldIndex) = *istk(ilStruct+3+fieldIndex) + *Lstk(stkPos + 2) - *Lstk(stkPos + 1);
          /* Reinit current variable address */
          *Lstk(stkPos + 1) = *Lstk(stkPos + 2);
          
        }
    }
  /* Emulate stack2.c returned values management */
  C2F(intersci).ntypes[stkPos - Top + Rhs - 1] = '$';
  C2F(intersci).lad[stkPos - Top + Rhs - 1] = ilStruct + 2 + nbFields + 1;

  //freeArrayOfString(fieldNames); /* TODO why is this line commented out ? */

  return TRUE;
}

