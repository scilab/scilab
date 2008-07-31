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
#include "MALLOC.h"
#include "stack-c.h" /* stack-def.h, stack1.h, stack2.h, stack3.h included */
#include "gw_matio.h"
#include "localization.h"
#include "Scierror.h"

/*******************************************************************************
   Interface for MATIO function called Mat_Open
   Scilab function name : matfile_open
*******************************************************************************/
int sci_matfile_open(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  mat_t *matfile;

  int fileIndex = 0;

  char *filename = NULL;

  char *optionStr = NULL;
  int option = 0;

  CheckRhs(1, 2);
  CheckLhs(1, 1);

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
      if (nbCol != 1) 
        {
          Scierror(999, _("%s: Wrong size for first input argument: Single string expected.\n"), fname);
          return FALSE;
        }
      filename = cstk(stkAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"), fname);
      return FALSE;
    }
  
  if (Rhs == 2)
    {
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
          if (nbCol != 1) 
            {
              Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), fname);
              return FALSE;
            }
          optionStr = cstk(stkAdr);
          
          if (strcmp(optionStr, "r")==0)
            {
              option = MAT_ACC_RDONLY;
            }
          else if (strcmp(optionStr, "w")==0)
            {
              option = MAT_ACC_RDWR;
            }
          else
            {
              Scierror(999, _("%s: Wrong value for second input argument: 'r' or 'w' expected.\n"), fname);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"), fname);
          return FALSE;
        }
    }
  else
    {
      /* Default option value */
      option = MAT_ACC_RDONLY;
    }

  /* Try to open the file (as a Matlab 5 file) */
  matfile = Mat_Open(filename, option);
  
  if(matfile==NULL) /* Opening failed */
    {
      /* Try to open the file (as a Matlab 4 file) */
      matfile = Mat_Open(filename, option | MAT_FT_MAT4);
      
      if(matfile==NULL) /* Opening failed */
        {
          /* Function returns -1 */
          fileIndex = -1;
        }
    }

  if (matfile != NULL) /* Opening succeed */
    {
      /* Add the file to the manager */
      matfile_manager(MATFILEMANAGER_ADDFILE, &fileIndex, &matfile);
    }

  /* Return the index */
  nbRow = 1;
  nbCol = 1;
  CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
  *stk(stkAdr) = fileIndex;

  LhsVar(1) = Rhs + 1;
  PutLhsVar();

  return TRUE;
}

