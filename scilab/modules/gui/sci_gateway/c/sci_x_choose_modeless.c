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
#include "CallMessageBox.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_x_choose_modeless)(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  int nbRowItems = 0, nbColItems = 0;

  int messageBoxID = 0;

  int itemsAdr = 0;
  int buttonLabelAdr = 0;

  int messageAdr = 0;

  int userValueAdr = 0;
  int userValue = 0;

  CheckRhs(2,3);
  CheckLhs(0,1);

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowItems, &nbColItems, &itemsAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: String vector expected.\n"), "x_choose_modeless");
      return FALSE;
    }

  if (VarType(2) == sci_strings)
    {
      GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &messageAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for second input argument: String vector expected.\n"), "x_choose_modeless");
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab choose message"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack(messageAdr), nbCol*nbRow);
  /* ListBox Items */
  setMessageBoxListBoxItems(messageBoxID, getStringMatrixFromStack(itemsAdr), nbColItems*nbRowItems);
  /* Modality */
  setMessageBoxModal(messageBoxID, FALSE);
    
  if (Rhs == 3)
    {
      if (VarType(3) ==  sci_strings)
        {
          GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&buttonLabelAdr);
          if (nbRow*nbCol != 1)
          {
            Scierror(999, _("%s: Wrong type for third input argument: Single string expected.\n"), "x_choose_modeless");
            return FALSE;
          }
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for third input argument: Single string expected.\n"), "x_choose_modeless");
          return FALSE;
        }

      setMessageBoxButtonsLabels(messageBoxID, getStringMatrixFromStack(buttonLabelAdr), nbCol*nbRow);
    }

  /* Display it and wait for a user input */
  messageBoxDisplayAndWait(messageBoxID);

  /* Read the user answer */
  userValue = getMessageBoxSelectedItem(messageBoxID);

  nbRow = 1;nbCol = 1;
  CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &userValueAdr);
  *stk(userValueAdr) = userValue;

  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
