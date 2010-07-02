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

#include "CreateMatlabVariable.h"
#include "stack-c.h"
#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

int CreateHyperMatrixVariable(int number, const char *type, int *iscomplex, int * rank, int *dims, double *realdata, double *complexdata, int * parent, int item_position)
{
  static const char *tlistFields[] = {"hm", "dims","entries"};
  int nbRow = 0, nbCol = 0;
  int K = 0, i;
  int * hm_addr = NULL;
  SciErr _SciErr;

  if (parent==NULL)
    {
      _SciErr = createMList(pvApiCtx, number, 3, &hm_addr); MATIO_ERROR;
    }
  else
    {
      _SciErr = createMListInList(pvApiCtx, number, parent, item_position, 3, &hm_addr); MATIO_ERROR;
    }

  /* mlist fields */
  nbRow = 1; nbCol = 3;
  _SciErr = createMatrixOfStringInList(pvApiCtx, number, hm_addr, 1, nbRow, nbCol, (char **)tlistFields); MATIO_ERROR;

  /* hm dimensions */
  _SciErr = createMatrixOfInteger32InList(pvApiCtx, number, hm_addr, 2, 1, *rank, dims);

  /* hm entries */
  nbRow = 1;
  for (K=0; K<*rank; K++)
    {
      nbRow *= dims[K];
    }

  nbCol = 1;
  
  if (strcmp(type,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE) == 0)
    {
      int * tmp_int32 = (int *)MALLOC(nbRow*nbCol*sizeof(int));
      if (tmp_int32==NULL)
	{
	  Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
	  
	  return FALSE;
	}
      for(i=0;i<nbRow*nbCol; i++) tmp_int32[i] = (int)realdata[i];
      _SciErr = createMatrixOfInteger32InList(pvApiCtx, number, hm_addr, 3, nbRow, nbCol, tmp_int32); MATIO_ERROR;
      FREE(tmp_int32);
    }
  else if (strcmp(type, MATRIX_OF_BOOLEAN_DATATYPE) == 0)
    {
      _SciErr = createMatrixOfDoubleInList(pvApiCtx, number, hm_addr, 3, nbRow, nbCol, realdata); MATIO_ERROR;
    }
  else
    {
       if (*iscomplex == 0)
        {
	  _SciErr = createMatrixOfDoubleInList(pvApiCtx, number, hm_addr, 3, nbRow, nbCol, realdata); MATIO_ERROR;
        }
      else
        {
	  _SciErr = createComplexMatrixOfDoubleInList(pvApiCtx, number, hm_addr, 3, nbRow, nbCol, realdata, complexdata); MATIO_ERROR;
        }
    }

  return TRUE;
}
