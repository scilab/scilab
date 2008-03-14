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
int sci_help(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, keywordAdr = 0;
  
  CheckRhs(0,1);
  CheckLhs(0,1);

  if (Rhs == 0)
    {
      //launchHelpBrowser();
    }
  else
    {
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &keywordAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for first input argument: Single string expected.\n"), "help");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"), "help");
          return FALSE;
        }
      
      //if (!searchKeyword(getStringMatrixFromStack(keywordAdr)[0])) {
      //  sciprint(_("Could not find help page for function: %s.\n"),getStringMatrixFromStack(keywordAdr)[0]);
      //}
    }
  
  LhsVar(1) = 0;
    
  PutLhsVar();

  return TRUE;

}
/*--------------------------------------------------------------------------*/
