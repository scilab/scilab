/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
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
#include "CallMessageBox.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len))
{
  int nbRow = 0, nbCol = 0;

  int messageBoxID = 0;

  int buttonsTextAdr = 0;

  int messageAdr = 0;

  int buttonNumber = 0;
  int buttonNumberAdr = 0;

  CheckRhs(1,2);
  CheckLhs(0,1);

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &messageAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"), "x_message");
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Message"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack(messageAdr), nbCol*nbRow);
    
  if (Rhs == 2)
    {
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&buttonsTextAdr);
          if ((nbCol*nbRow!=1) && (nbCol*nbRow!=2))
            {
              Scierror(999, _("%s: Wrong type for second input argument: one or two button names expected.\n"), "x_message");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for second input argument: one or two button names expected.\n"), "x_message");
          return FALSE;
        }

      setMessageBoxButtonsLabels(messageBoxID, getStringMatrixFromStack(buttonsTextAdr), nbCol*nbRow);
    }

  /* Display it and wait for a user input */
  messageBoxDisplayAndWait(messageBoxID);

  if (Rhs==2 && (nbCol*nbRow==2)) /* If two buttons -> return the index of the button selected */
    {
      /* Read the user answer */
      buttonNumber = getMessageBoxSelectedButton(messageBoxID);
      
      nbRow = 1; nbCol = 1;
      CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &buttonNumberAdr);
      *stk(buttonNumberAdr) = buttonNumber;
      
      LhsVar(1) = Rhs+1;
      C2F(putlhsvar)();
      
      return TRUE;
    }
  else
    {
      LhsVar(1) = 0;
      C2F(putlhsvar)();
      return TRUE;
    }

}
/*--------------------------------------------------------------------------*/
