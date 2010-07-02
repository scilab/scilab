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

#include "gw_gui.h"
#include "stack-c.h"
#include "localization.h"
#include "CallHelpBrowser.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "freeArrayOfString.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
int sci_helpbrowser(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  char **keywordAdr = NULL;

  int nbRowHelp = 0, nbColHelp = 0;
  char **helpAdr = NULL;
  char **languageAdr = NULL;
  int fullTextAdr = 0;
  
  CheckRhs(2,4);
  CheckLhs(0,1);

  /* We load SciNotes when calling javahelp because we have no way to know
   * to load it when using Javahelp because it can call SciNotes directly */
  if (!loadedDep) 
  {
	  loadOnUseClassPath("SciNotes");
	  loadedDep = TRUE;
  }

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowHelp, &nbColHelp, &helpAdr);
    }
  else
    {
      if (VarType(1) == sci_matrix)
        {
          if (nbRow*nbCol == 0)
            {
              helpAdr = NULL; /* No toolboxes installed */
            }
          else
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: Matrix of strings expected.\n"), fname, 1);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
          return FALSE;
        }
    }
  
  if (Rhs == 2)
    {
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &languageAdr);
          if (nbRow*nbCol != 1)
            {
              if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
              freeArrayOfString(languageAdr, nbRow*nbCol);
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
              return FALSE;
            }
        }
      else
        {
          if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
          return FALSE;
        }

      if (helpAdr == NULL) /* No toolboxes loaded */
        {
          launchHelpBrowser(NULL, nbRowHelp*nbColHelp, getStringMatrixFromStack((size_t)languageAdr)[0]);
        }
      else
        {
          launchHelpBrowser(getStringMatrixFromStack((size_t)helpAdr), nbRowHelp*nbColHelp, getStringMatrixFromStack((size_t)languageAdr)[0]);
          freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
        }
      freeArrayOfString(languageAdr, nbRow*nbCol);
    }
  else if (Rhs == 4)
    {
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &keywordAdr);
          if (nbRow*nbCol != 1)
            {
              if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
              freeArrayOfString(keywordAdr, nbRow * nbCol);
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
              return FALSE;
            }
        }
      else
        {
          if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
          return FALSE;
        }
      
      if (VarType(3) == sci_strings)
        {
          GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &languageAdr);
          if (nbRow*nbCol != 1)
            {
              if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
              freeArrayOfString(keywordAdr, 1);
              freeArrayOfString(languageAdr, nbRow * nbCol);
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
              return FALSE;
            }
        }
      else
        {
          if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
          freeArrayOfString(keywordAdr, 1);
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
          return FALSE;
        }
      
      if (VarType(4) == sci_boolean)
        {
          GetRhsVar(4, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &fullTextAdr);
          if (nbRow*nbCol != 1)
            {
              if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
              freeArrayOfString(keywordAdr, 1);
              freeArrayOfString(languageAdr, 1);
              Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 4);
              return FALSE;
            }
        }
      else
        {
          if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
          freeArrayOfString(keywordAdr, 1);
          freeArrayOfString(languageAdr, 1);
          Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 4);
          return FALSE;
        }
      
      if (helpAdr == NULL) /* No toolboxes loaded */
        {
          searchKeyword(NULL, nbRowHelp*nbColHelp, getStringMatrixFromStack((size_t)keywordAdr)[0], getStringMatrixFromStack((size_t)languageAdr)[0], *istk(fullTextAdr)==1);
        }
      else
        {
          searchKeyword(getStringMatrixFromStack((size_t)helpAdr), nbRowHelp*nbColHelp, getStringMatrixFromStack((size_t)keywordAdr)[0], getStringMatrixFromStack((size_t)languageAdr)[0], *istk(fullTextAdr)==1);
          if (helpAdr) freeArrayOfString(helpAdr, nbRowHelp*nbColHelp);
        }
      freeArrayOfString(keywordAdr, 1);
      freeArrayOfString(languageAdr, 1);
    }
  else
    {
      Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 2, 4);
      return FALSE;
    }
  
  LhsVar(1) = 0;
    
  PutLhsVar();

  return TRUE;

}
/*--------------------------------------------------------------------------*/
