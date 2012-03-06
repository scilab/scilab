// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

#include <stdlib.h>
#include <api_scilab.h>
#include <Scierror.h>
#include <MALLOC.h>
#include <localization.h>

int sci_bug_8688(char *fname)
{
  SciErr sciErr;
  int N = 20000000;
  char **ptrStrs = (char **)MALLOC(sizeof(char*) * N);
  int i = 0;

  if (ptrStrs == NULL)
  {
    Scierror(999,"memory allocation error");
  }

  for (i = 0;i < N; i++)
  {
    ptrStrs[i] = strdup("TEST BUG 8688.");
  }  

  sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, N, ptrStrs);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
  LhsVar(1) = 1;
  return 0;
}
// =============================================================================
