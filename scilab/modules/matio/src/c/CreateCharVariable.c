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
#include "freeArrayOfString.h"

int CreateCharVariable(int stkPos, matvar_t *matVariable)
{
  int nbRow = 0, nbCol = 0;

  char **charData = NULL;

  int K = 0, L = 0;

  if(matVariable->rank==2) /* 2-D array */
    {
      nbRow = matVariable->dims[0];
      nbCol = 1;

      if (nbRow != 0)
        {
          charData =  (char**) MALLOC(sizeof(char*) * nbRow);
          if (charData==NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateCharVariable");
              return FALSE;
            };
        }
      
      for (K=0; K<nbRow; K++)
        {
          charData[K] =  (char*) MALLOC(sizeof(char*) * (matVariable->dims[1] + 1));
          if (charData[K]==NULL)
            {
              Scierror(999, _("%s: No more memory.\n"), "CreateCharVariable");
              return FALSE;
            };
        }
      /* Fill items: data in Matlab file is stored columnwise */
      for(K=0; K<matVariable->dims[0]; K++) /* Loop over items */
        {
          for(L=0; L<matVariable->dims[1]; L++) /* Loop over chars */
            {
              charData[K][L] = ((char *)matVariable->data)[L*matVariable->dims[0]+K];
            }
          charData[K][L] = '\0';
        }

      CreateVarFromPtr(stkPos, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, charData);
      
      freeArrayOfString(charData,nbRow*nbCol);
    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      Scierror(999, _("%s: N-D arrays of chars not implemented.\n"), "CreateCharVariable");
      return FALSE;
    }
  
  return TRUE;
}
