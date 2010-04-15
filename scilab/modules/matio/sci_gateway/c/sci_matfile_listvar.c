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
#include "MALLOC.h"
#include "stack-c.h" /* stack-def.h, stack1.h, stack2.h, stack3.h included */
#include "gw_matio.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "api_scilab.h"
#include "api_oldstack.h"

#define MATIO_ERROR if(_SciErr.iErr)	     \
    {					     \
      printError(&_SciErr, 0);		     \
      return 1;				     \
    }

int sci_matfile_listvar(char* fname, int* _piKey)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;
  mat_t *matfile = NULL;
  matvar_t *matvar = NULL;
  int fileIndex = 0;
  char **varnames = NULL;
  double *varclasses = NULL;
  double *vartypes = NULL;
  int nbvar = 0, var_type;
  int * fd_addr = NULL;
  double tmp_dbl;
  SciErr _SciErr;
  
  CheckRhs(1, 1);
  CheckLhs(1, 3);
  
  /* First Rhs is the index of the file to read */
  
  _SciErr = getVarAddressFromPosition(_piKey, 1, &fd_addr); MATIO_ERROR;
  _SciErr = getVarType(_piKey, fd_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_matrix)
    {
      getScalarDouble(_piKey, fd_addr, &tmp_dbl);
      if (!isScalar(_piKey, fd_addr))
	{
	  Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
	  return 1;
	}
      fileIndex = (int)tmp_dbl;
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
      return 1;
    }
  
  /* Gets the corresponding matfile */
  matfile_manager(MATFILEMANAGER_GETFILE, &fileIndex, &matfile);
  
  /* Back to the beginning of the file */
  if (Mat_Rewind(matfile) != 0)
    {
      Scierror(999, _("%s: Could not rewind the file %s.\n"), "matfile_listvar", matfile->filename);
      return 1;
    }
  
  matvar = Mat_VarReadNext(matfile);
  while (matvar != NULL && matvar->name != NULL)
    {
      nbvar++;
      varnames = (char**) REALLOC(varnames, nbvar*sizeof(char*));
      if (varnames == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"), "matfile_listvar");
	  return 1;
	}
      varnames[nbvar-1] = strdup(matvar->name);
      varclasses = (double*) REALLOC(varclasses, nbvar*sizeof(double));
      if (varnames == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"), "matfile_listvar");
	  return 1;
	}
      varclasses[nbvar-1] = (double) matvar->class_type;
      vartypes = (double*) REALLOC(vartypes, nbvar*sizeof(double));
      if (varnames == NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"), "matfile_listvar");
	  return 1;
	}
      vartypes[nbvar-1] = (double) matvar->data_type;
      
      Mat_VarFree(matvar);
      matvar = Mat_VarReadNext(matfile);
    }
  
  Mat_VarFree(matvar);
  
  /* Return the variable names list */
  nbRow = nbvar; nbCol = 1;
  _SciErr = createMatrixOfString(_piKey, Rhs+1, nbRow, nbCol, varnames); MATIO_ERROR;
  LhsVar(1) = Rhs+1;
  
  /* Return the variable classes */
  if (Lhs >= 2)
    {
      _SciErr = createMatrixOfDouble(_piKey, Rhs+2, nbRow, nbCol, varclasses); MATIO_ERROR;
      LhsVar(2) = Rhs+2;
    }
  
  /* Return the variable types */
  if (Lhs >= 3)
    {
      _SciErr = createMatrixOfDouble(_piKey, Rhs+3, nbRow, nbCol, vartypes); MATIO_ERROR;
      LhsVar(3) = Rhs+3;
    }
  
  PutLhsVar();
  
  freeArrayOfString(varnames, nbvar);
  FREE(varclasses);
  FREE(vartypes);
  
  return 0;
}
