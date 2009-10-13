/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Xpad.hxx"

extern "C"
{
#include <wchar.h>
#include "gw_xpad.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "getScilabJavaVM.h"
#include "MALLOC.h"
#include "charEncoding.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xpad;
/*--------------------------------------------------------------------------*/
int sci_xpad_highlightline(char *fname,unsigned long fname_len)
{
  StrErr strErr;
  CheckRhs(2,2);
  CheckLhs(0,1);

  int m1 = 0, n1 = 0;
  int *piAddressFilename = NULL;
  int *piAddressLineNumber = NULL;
  int iType = 0;
  wchar_t **pStVarOne = NULL;
  double *pDbleLineNumber = NULL;
  int *lenStVarOne = NULL;

  /*
   * READ FILENAME TO BE OPENED
   */

  strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressFilename);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  strErr = getVarType(pvApiCtx, piAddressFilename, &iType);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  if (iType  != sci_strings )
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
      return 0;
    }

  /* get dimensions */
  strErr = getMatrixOfWideString(pvApiCtx, piAddressFilename, &m1, &n1, lenStVarOne, pStVarOne);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  if (m1 * n1 != 1)
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
      return 0;
    }

  lenStVarOne = (int*)MALLOC(sizeof(int));
  if (lenStVarOne == NULL)
    {
      Scierror(999,_("%s: No more memory.\n"), fname);
      return 0;
    }

  /* get lengths */
  strErr = getMatrixOfWideString(pvApiCtx, piAddressFilename, &m1, &n1, lenStVarOne, pStVarOne);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  pStVarOne = (wchar_t  **)MALLOC(sizeof(wchar_t *));
  if (pStVarOne == NULL)
    {
      Scierror(999,_("%s: No more memory.\n"), fname);
      return 0;
    }
  
  
  pStVarOne[0] = (wchar_t *)MALLOC(sizeof(wchar_t *) * (lenStVarOne[0] + 1));

  /* get file name */
  strErr = getMatrixOfWideString(pvApiCtx, piAddressFilename, &m1, &n1, lenStVarOne, pStVarOne);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  /* Read line number */
  strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressLineNumber);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  strErr = getVarType(pvApiCtx, piAddressLineNumber, &iType);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  if (iType  != sci_matrix )
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A Real Scalar expected.\n"),fname,2);
      return 0;
    }

  /* get dimensions */
  strErr = getMatrixOfDouble(pvApiCtx, piAddressLineNumber, &m1, &n1, &pDbleLineNumber);
  if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }

  if (m1 * n1 != 1)
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A Real Scalar expected.\n"),fname,2);
      return 0;
    }
  
  
  Xpad::xpadHighlightLine(getScilabJavaVM(), wide_string_to_UTF8(expandPathVariableW(pStVarOne[0])), (int) pDbleLineNumber[0]);
  
  freeArrayOfWideString(pStVarOne,m1 * n1);
  
  LhsVar(1) = 0;
  PutLhsVar();
  
return 0;
}
/*--------------------------------------------------------------------------*/
