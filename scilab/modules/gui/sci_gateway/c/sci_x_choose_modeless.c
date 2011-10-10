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
#include "CallMessageBox.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_x_choose_modeless(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  int nbRowItems = 0, nbColItems = 0;

  int messageBoxID = 0;

  char **itemsAdr = NULL;
  char **buttonLabelAdr = NULL;

  char **messageAdr = NULL;

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
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 1);
      return FALSE;
    }

  if (VarType(2) == sci_strings)
    {
      GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &messageAdr);
    }
  else
    {
      freeArrayOfString(itemsAdr, nbColItems*nbRowItems);
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Choose Message"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)messageAdr), nbCol*nbRow);
  /* ListBox Items */
  setMessageBoxListBoxItems(messageBoxID, getStringMatrixFromStack((size_t)itemsAdr), nbColItems*nbRowItems);
  /* Modality */
  setMessageBoxModal(messageBoxID, FALSE);
    
  freeArrayOfString(itemsAdr, nbColItems*nbRowItems);
  freeArrayOfString(messageAdr, nbCol*nbRow);
    
  if (Rhs == 3)
    {
      if (VarType(3) ==  sci_strings)
        {
          GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&buttonLabelAdr);
          if (nbRow*nbCol != 1)
          {
            freeArrayOfString(buttonLabelAdr, nbRow*nbCol);
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
            return FALSE;
          }
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
          return FALSE;
        }

      setMessageBoxButtonsLabels(messageBoxID, getStringMatrixFromStack((size_t)buttonLabelAdr), nbCol*nbRow);
      freeArrayOfString(buttonLabelAdr, nbRow*nbCol);
    }

  /* Display it and wait for a user input */
  messageBoxDisplayAndWait(messageBoxID);

  /* Read the user answer */
  userValue = getMessageBoxSelectedItem(messageBoxID);

  nbRow = 1;nbCol = 1;
  CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &userValueAdr);
  *stk(userValueAdr) = userValue;

  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
