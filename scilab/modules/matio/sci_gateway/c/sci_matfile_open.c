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

#include "api_scilab.h"
#include "MALLOC.h"
#include "matfile_manager.h"
#include "gw_matio.h"
#include "localization.h"
#include "Scierror.h"



#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

/*******************************************************************************
   Interface for MATIO function called Mat_Open
   Scilab function name : matfile_open
*******************************************************************************/
int sci_matfile_open(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  mat_t *matfile;
  int fileIndex = 0;
  char * filename  = NULL;
  char * optionStr = NULL;
  int option = 0, var_type;
  int * filename_addr = NULL, * option_addr = NULL;
  SciErr _SciErr;
  
  CheckRhs(1, 2);
  CheckLhs(1, 1);
  
  _SciErr = getVarAddressFromPosition(pvApiCtx, 1, &filename_addr); MATIO_ERROR;
  _SciErr = getVarType(pvApiCtx, filename_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_strings)
    {
      getAllocatedSingleString(pvApiCtx, filename_addr, &filename);
      _SciErr = getVarDimension(pvApiCtx, filename_addr, &nbRow, &nbCol); 
      MATIO_ERROR;
      
      if (nbCol != 1) 
	{
	  Scierror(999, _("%s: Wrong size for first input argument: Single string expected.\n"), fname);
	  
	  freeAllocatedSingleString(filename);
	  
	  return FALSE;
	}
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"), fname);
      
      freeAllocatedSingleString(filename);
      
      return FALSE;
    }
  
  if (Rhs == 2)
    {
      _SciErr = getVarAddressFromPosition(pvApiCtx, 2, &option_addr); MATIO_ERROR;
      
      _SciErr = getVarType(pvApiCtx, option_addr, &var_type); MATIO_ERROR;
      
      if (var_type == sci_strings)
	{
	  getAllocatedSingleString(pvApiCtx, option_addr, &optionStr);
	  _SciErr = getVarDimension(pvApiCtx, option_addr, &nbRow, &nbCol); MATIO_ERROR;
	  
	  if (nbCol != 1) 
	    {
	      Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), fname);
	      
	      freeAllocatedSingleString(filename);
	      freeAllocatedSingleString(optionStr);
	      
	      return FALSE;
	    }
	  
	  if (strcmp(optionStr, "r")==0)
	    {
	      option = MAT_ACC_RDONLY;
	    }
	  else if (strcmp(optionStr, "w")==0)
	    {
	      option = 0; // MAT_ACC_RDWR option will be used for append mode
	    }
	  else
	    {
	      Scierror(999, _("%s: Wrong value for second input argument: 'r' or 'w' expected.\n"), fname);
	      
	      freeAllocatedSingleString(filename);
	      freeAllocatedSingleString(optionStr);
	      
	      return FALSE;
	    }
	}
      else
	{
	  Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"), fname);
	  
	  freeAllocatedSingleString(filename);
	  freeAllocatedSingleString(optionStr);
	  
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
  createScalarDouble(pvApiCtx, Rhs+1, (double)fileIndex);
  
  freeAllocatedSingleString(filename);
  freeAllocatedSingleString(optionStr);

  LhsVar(1) = Rhs+1;
  PutLhsVar();
  
  return TRUE;
}
