/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERt
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "callxcos.h"
#include "api_common.h"
#include "api_string.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getFullFilename.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int sci_warnBlockByUID(char *fname,unsigned long fname_len)
{
  CheckRhs(2,2);
  CheckLhs(0,1);

  int m1 = 0, n1 = 0;
  int *piAddressVarOne = NULL;
  int m2 = 0, n2 = 0;
  int *piAddressVarTwo = NULL;
  int i = 0;
  int lw = 1;
  int iType = 0;
  SciErr sciErr;

  char **pStVarOne = NULL;
  int *lenStVarOne = NULL;

  char **pStVarTwo = NULL;
  int *lenStVarTwo = NULL;

  /** READ UID **/
  sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
  if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
        
  sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
  if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
        
  if ( iType == sci_strings )
    {
      char **pStFullFilenames = NULL;
            
      /* get dimensions */
      sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
      if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
            
      lenStVarOne = (int*)MALLOC(sizeof(int)*(m1 * n1));
      if (lenStVarOne == NULL)
        {
          Scierror(999,_("%s: No more memory.\n"), fname);
          return 0;
        }
            
      /* get lengths */
      sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
      if(sciErr.iErr)
        {
          if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
          printError(&sciErr, 0);
          return 0;
        }
            
      pStVarOne = (char **)MALLOC(sizeof(char*)*(m1*n1));
      if (pStVarOne == NULL)
        {
          if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
          Scierror(999,_("%s: No more memory.\n"), fname);
          return 0;
        }
            
      pStFullFilenames = (char **)MALLOC(sizeof(char*)*(m1*n1));
      if (pStFullFilenames == NULL)
        {
          if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
          freeArrayOfString(pStVarOne, m1 * n1);
          Scierror(999,_("%s: No more memory.\n"), fname);
          return 0;
        }

      if (m1 * n1 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
        return 0;
      } 
            
      pStVarOne[0] = (char*)MALLOC(sizeof(char*) * (lenStVarOne[0] + 1));
            
      /* get strings */
      sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
      if(sciErr.iErr)
        {
          freeArrayOfString(pStFullFilenames, m1 * n1);
          freeArrayOfString(pStVarOne, m1 * n1);
          if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
          printError(&sciErr, 0);
          return 0;
        }
            
      if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}

      freeArrayOfString(pStVarOne, m1 * n1);
    }
  else
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
      return 0;
    }
        
  /** READ MESSAGE **/
  sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
  if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
        
  sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType);
  if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
        
  if ( iType == sci_strings )
    {
      char **pStFullFilenames = NULL;
            
      /* get dimensions */
      sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, lenStVarTwo, pStVarTwo);
      if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
            
      lenStVarTwo = (int*)MALLOC(sizeof(int)*(m1 * n1));
      if (lenStVarTwo == NULL)
        {
          Scierror(999,_("%s: No more memory.\n"), fname);
          return 0;
        }
            
      /* get lengths */
      sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, lenStVarTwo, pStVarTwo);
      if(sciErr.iErr)
        {
          if (lenStVarTwo) { FREE(lenStVarTwo); lenStVarTwo = NULL;}
          printError(&sciErr, 0);
          return 0;
        }
            
      pStVarTwo = (char **)MALLOC(sizeof(char*)*(m1*n1));
      if (pStVarTwo == NULL)
        {
          if (lenStVarTwo) { FREE(lenStVarTwo); lenStVarTwo = NULL;}
          Scierror(999,_("%s: No more memory.\n"), fname);
          return 0;
        }
            
      pStFullFilenames = (char **)MALLOC(sizeof(char*)*(m1*n1));
      if (pStFullFilenames == NULL)
        {
          if (lenStVarTwo) { FREE(lenStVarTwo); lenStVarTwo = NULL;}
          freeArrayOfString(pStVarTwo, m1 * n1);
          Scierror(999,_("%s: No more memory.\n"), fname);
          return 0;
        }

      if (m1 * n1 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
        return 0;
      } 
            
      pStVarTwo[0] = (char*)MALLOC(sizeof(char*) * (lenStVarTwo[0] + 1));
            
      /* get strings */
      sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, lenStVarTwo, pStVarTwo);
      if(sciErr.iErr)
        {
          freeArrayOfString(pStFullFilenames, m1 * n1);
          freeArrayOfString(pStVarTwo, m1 * n1);
          if (lenStVarTwo) { FREE(lenStVarTwo); lenStVarTwo = NULL;}
          printError(&sciErr, 0);
          return 0;
        }
            
      if (lenStVarTwo) { FREE(lenStVarTwo); lenStVarTwo = NULL;}

      freeArrayOfString(pStVarTwo, m1 * n1);
    }
  else
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
      return 0;
    }

  Xcos::warnCellByUID(getScilabJavaVM(), pStVarOne[0], pStVarTwo[0]);
  
  LhsVar(1) = 0;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
