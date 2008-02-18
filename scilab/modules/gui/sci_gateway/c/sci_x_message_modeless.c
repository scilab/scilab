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
 
#include "machine.h"
#include "gw_gui.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_x_message_modeless) _PARAMS((char *fname,unsigned long fname_len))
{
  int nbRow = 0, nbCol = 0;

  int messageBoxID = 0;

  int messageAdr = 0;

  CheckRhs(1,2);
  CheckLhs(0,1);

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &messageAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"), "x_message_modeless");
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Message"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, (char**)messageAdr, nbCol*nbRow);
  /* Modality */
  setMessageBoxModal(messageBoxID, FALSE);
  /* Display it and wait for a user input */
  messageBoxDisplayAndWait(messageBoxID);

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return TRUE;

}
/*--------------------------------------------------------------------------*/
