/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
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
/*--------------------------------------------------------------------------*/
int C2F(sci_x_dialog)(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;

  int messageBoxID = 0;

  char **initialValueAdr = 0;

  char **labelsAdr = 0;

  int userValueSize = 0;
  char **userValue = NULL;

  int emptyMatrixAdr = 0;

  CheckRhs(1,2);
  CheckLhs(0,1);

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &labelsAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 1);
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Input Value Request"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)labelsAdr), nbCol*nbRow);
    
  if (Rhs == 2)
    {
      if (VarType(2) ==  sci_strings)
        {
          GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&initialValueAdr);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
          return FALSE;
        }

      setMessageBoxInitialValue(messageBoxID, getStringMatrixFromStack((size_t)initialValueAdr), nbCol*nbRow);
    }

  /* Display it and wait for a user input */
  messageBoxDisplayAndWait(messageBoxID);

  /* Read the user answer */
  userValueSize = getMessageBoxValueSize(messageBoxID);
  if (userValueSize == 0)
    {
      nbRow = 0;nbCol = 0;
      CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &emptyMatrixAdr);
    }
  else
    {
      userValue = getMessageBoxValue(messageBoxID);
	  
  
      nbCol = 1;
      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &userValueSize, &nbCol, userValue);
      /* TO DO : delete of userValue */
      
    }

  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
