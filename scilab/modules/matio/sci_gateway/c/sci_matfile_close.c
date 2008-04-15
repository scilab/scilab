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

#include "matfile_manager.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

int sci_matfile_close(char *fname,unsigned long fname_len);

/*******************************************************************************
   Interface for MATIO function called Mat_Close
   Scilab function name : matfile_close
*******************************************************************************/
int sci_matfile_close(char *fname,unsigned long fname_len)
{
  mat_t *matfile = NULL;
  int fileIndex = 0; 
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  int flag = 1;

  CheckRhs(1, 1);
  CheckLhs(1, 1);

  /* First Rhs is the index of the file to close */
  if (VarType(1) == sci_matrix)
    {
      GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, & nbCol, &stkAdr);
      if (nbRow * nbCol != 1)
        {
          Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
          return FALSE;
        }
      fileIndex = (int)*stk(stkAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
      return FALSE;
    }
  
  /* Gets the corresponding matfile to close it */
  /* The manager clears its static matfile table */
  matfile_manager(MATFILEMANAGER_DELFILE, &fileIndex, &matfile);

  /* If the file has not already been closed, it's closed */
  if(matfile!=NULL)
    {
      flag = Mat_Close(matfile);
    }
  else /* The user is informed */
    sciprint("File already closed.\n");

  /* Return execution flag */
  nbRow = 1; nbCol = 1;
  CreateVar(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &stkAdr);
  *istk(stkAdr) = flag==0;

  LhsVar(1) = Rhs + 1;

  PutLhsVar();

  return TRUE;
}

