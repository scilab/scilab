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

/*--------------------------------------------------------------------------*/
int sci_helpbrowser(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, keywordAdr = 0;
  int nbRowHelp = 0, nbColHelp = 0, helpAdr = 0;
  
  CheckRhs(1,2);
  CheckLhs(0,1);

  if (Rhs == 1)
    {
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowHelp, &nbColHelp, &helpAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "helpbrowser");
          return FALSE;
        }

      launchHelpBrowser(getStringMatrixFromStack(helpAdr), nbRowHelp*nbColHelp);
    }
  else
    {
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowHelp, &nbColHelp, &helpAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single matrix expected.\n"), "helpbrowser");
          return FALSE;
        }

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
      
      if (!searchKeyword(getStringMatrixFromStack(helpAdr), nbRowHelp*nbColHelp, getStringMatrixFromStack(keywordAdr)[0])) {
        sciprint(_("Could not find help page for function: %s.\n"),getStringMatrixFromStack(keywordAdr)[0]);
      }
    }
  
  LhsVar(1) = 0;
    
  PutLhsVar();

  return TRUE;

}
/*--------------------------------------------------------------------------*/
