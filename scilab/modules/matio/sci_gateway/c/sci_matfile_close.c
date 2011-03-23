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

#include "matfile_manager.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr)	     \
    {					     \
      printError(&_SciErr, 0);		     \
      return 0;				     \
    }

/*******************************************************************************
Interface for MATIO function called Mat_Close
Scilab function name : matfile_close
*******************************************************************************/
int sci_matfile_close(char *fname,unsigned long fname_len)
{
  mat_t * matfile = NULL;
  int fileIndex = 0; 
  int nbRow = 0, nbCol = 0;
  int * fd_addr = NULL;
  int flag = 1, var_type;
  double * fd_val = NULL;
  SciErr _SciErr;
  
  CheckRhs(1, 1);
  CheckLhs(1, 1);
  
  /* First Rhs is the index of the file to close */
  
  _SciErr = getVarAddressFromPosition(pvApiCtx, 1, &fd_addr); MATIO_ERROR;
  _SciErr = getVarType(pvApiCtx, fd_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_matrix)
    {
      _SciErr = getMatrixOfDouble(pvApiCtx, fd_addr, &nbRow, &nbCol, &fd_val); MATIO_ERROR;
      if (nbRow * nbCol != 1)
	{
	  Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
	  return FALSE;
	}
      fileIndex = (int)*fd_val;
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
  var_type = (flag == 0);
  createScalarBoolean(pvApiCtx, Rhs+1, var_type); MATIO_ERROR;
  
  LhsVar(1) = Rhs+1;
  
  PutLhsVar();
  
  return TRUE;
}
