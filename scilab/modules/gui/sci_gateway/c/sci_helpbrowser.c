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
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "CallHelpBrowser.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
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
              Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "helpbrowser");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "helpbrowser");
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
              Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), "helpbrowser");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for second input argument: String matrix expected.\n"), "helpbrowser");
          return FALSE;
        }

      if (helpAdr == NULL) /* No toolboxes loaded */
        {
          launchHelpBrowser(NULL, nbRowHelp*nbColHelp, getStringMatrixFromStack((int)languageAdr)[0]);
        }
      else
        {
          launchHelpBrowser(getStringMatrixFromStack((int)helpAdr), nbRowHelp*nbColHelp, getStringMatrixFromStack((int)languageAdr)[0]);
        }
    }
  else if (Rhs == 4)
    {
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &keywordAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), "helpbrowser");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first second argument: Single string expected.\n"), "helpbrowser");
          return FALSE;
        }
      
      if (VarType(3) == sci_strings)
        {
          GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &languageAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for third input argument: Single string expected.\n"), "helpbrowser");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for third second argument: Single string expected.\n"), "helpbrowser");
          return FALSE;
        }
      
      if (VarType(4) == sci_boolean)
        {
          GetRhsVar(4, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &fullTextAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for fourth input argument: Single boolean expected.\n"), "helpbrowser");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for fourth second argument: Single boolean expected.\n"), "helpbrowser");
          return FALSE;
        }
      
      if (helpAdr == NULL) /* No toolboxes loaded */
        {
          searchKeyword(NULL, nbRowHelp*nbColHelp, getStringMatrixFromStack((int)keywordAdr)[0], getStringMatrixFromStack((int)languageAdr)[0], *istk(fullTextAdr)==1);
        }
      else
        {
          searchKeyword(getStringMatrixFromStack((int)helpAdr), nbRowHelp*nbColHelp, getStringMatrixFromStack((int)keywordAdr)[0], getStringMatrixFromStack((int)languageAdr)[0], *istk(fullTextAdr)==1);
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "helpbrowser", 2, 4);
      return FALSE;
    }
  
  LhsVar(1) = 0;
    
  PutLhsVar();

  return TRUE;

}
/*--------------------------------------------------------------------------*/
