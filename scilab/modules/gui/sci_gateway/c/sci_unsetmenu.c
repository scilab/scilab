/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
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
#include "Scierror.h"
#include "InitUIMenu.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
int sci_unsetmenu(char *fname,unsigned long fname_len)
{
  int nbRow=0, nbCol=0;
  int menuNameAdr=0;
  int figureNumberAdr=0;
  int subMenuIndexAdr=0;

  // Check parameter number
  CheckRhs(1,3);
  CheckLhs(1,1);

  if (Rhs == 1)
    {
      // Error message in not in standard mode
      if(getScilabMode() != SCILAB_STD)
        {
          Scierror(999,_("%s: figure number must be given when used in no window mode."),"delmenu");
        }

      // Unset a Menu of Scilab Main Window
      if (VarType(1) != sci_strings)
        {
          Scierror(999, _("%s: Wrong type for first input argument: String expected.\n"),fname);
          return FALSE;
        }

      GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &menuNameAdr);

      if (nbCol != 1)
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"),fname);
          return FALSE;
        }

     EnableRootMenu(cstk(menuNameAdr), FALSE);
    }
  else if (Rhs == 2)
    {
      // Unset a Menu a Scilab Graphic Window
      if (VarType(1) == sci_matrix && VarType(2) == sci_strings)
        {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &figureNumberAdr);
      
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong type for first input argument: Scalar expected.\n"),fname);
              return FALSE;
            }
          
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &menuNameAdr);
          
          EnableFigureMenu((int)*stk(figureNumberAdr), cstk(menuNameAdr), FALSE);
        }
      else if (VarType(1) == sci_strings && VarType(2) == sci_matrix) // Unset a submenu in main window
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &menuNameAdr);
          
          GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &subMenuIndexAdr);
      
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong type for first input argument: Scalar expected.\n"),fname);
              return FALSE;
            }
          
          EnableRootSubMenu(cstk(menuNameAdr), (int)*stk(subMenuIndexAdr), FALSE);
        }
      else
        {
          Scierror(999, _("%s: Wrong input arguments: (button,nsub) or (gwin,button) expected.\n"),fname);
          return FALSE;
        }
    }
  else // Unset a submenu in graphics window
    {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &figureNumberAdr);
      
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong type for first input argument: Scalar expected.\n"),fname);
              return FALSE;
            }
          
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &menuNameAdr);
          
          GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &subMenuIndexAdr);
      
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong type for third input argument: Scalar expected.\n"),fname);
              return FALSE;
            }
          
          EnableFigureSubMenu((int)*stk(figureNumberAdr), cstk(menuNameAdr), (int)*stk(subMenuIndexAdr), FALSE);
    }
  
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
