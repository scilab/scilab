/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"

int CreateBooleanVariable(int stkPos, matvar_t *matVariable)
{

  int nbRow = 0, nbCol = 0;

  int *intPtr = NULL;

  int K = 0;
  
  if (matVariable->rank==2) /* 2-D array */
    {
      nbRow = matVariable->dims[0];
      nbCol = matVariable->dims[1];
      
      if (nbRow*nbCol != 0)
        {
          if ((intPtr = (int*) MALLOC(sizeof(int) * nbRow * nbCol)) == NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateBooleanVariable");
              return FALSE;
            }
          
          for (K = 0; K < nbRow*nbCol; K++)
            {
              intPtr[K] = ((unsigned char*)matVariable->data)[K];
            }
      
          CreateVarFromPtr(stkPos, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &intPtr);
          
          FREE(intPtr);
        }
      else
        {
          CreateVarFromPtr(stkPos, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &intPtr);
        }

    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      CreateHyperMatrixVariable(stkPos, MATRIX_OF_BOOLEAN_DATATYPE,  NULL, &matVariable->rank, matVariable->dims, matVariable->data, NULL);
    }
  
  return TRUE;
}

