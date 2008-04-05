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
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_x_mdialog)(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  int nbRowDefaultValues = 0, nbColDefaultValues = 0;
  int nbRowLineLabels = 0, nbColLineLabels = 0;
  int nbRowColumnLabels = 0, nbColColumnLabels = 0;

  int messageBoxID = 0;

  int labelsAdr = 0;
  int lineLabelsAdr = 0;
  int columnLabelsAdr = 0;
  int defaultValuesAdr = 0;

  int userValueSize = 0;
  char **userValue = NULL;

  int emptyMatrixAdr = 0;

  CheckRhs(3,4);
  CheckLhs(0,1);

  /* READ THE MESSAGE */
  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &labelsAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: String vector expected.\n"), "x_mdialog");
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Multiple Values Request"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack(labelsAdr), nbCol*nbRow);
    
  /* READ THE LINE LABELS */
  if (VarType(2) ==  sci_strings)
    {
      GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRowLineLabels,&nbColLineLabels,&lineLabelsAdr);
      if (nbRow !=1 && nbCol !=1)
      {
        Scierror(999, _("%s: Wrong type for second input argument: String vector expected.\n"), "x_mdialog");
        return FALSE;
      }
      setMessageBoxLineLabels(messageBoxID, getStringMatrixFromStack(lineLabelsAdr), nbColLineLabels*nbRowLineLabels);
    }
  else 
    {
      Scierror(999, _("%s: Wrong type for second input argument: String vector expected.\n"), "x_mdialog");
      return FALSE;
    }
  
  /* READ THE COLUMN LABELS or DEFAULT VALUES */
  if (VarType(3) ==  sci_strings)
    {
      if (Rhs == 3)
        {
          GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRowDefaultValues,&nbColDefaultValues,&defaultValuesAdr);
          if (nbRowDefaultValues !=1 && nbColDefaultValues !=1)
            {
              Scierror(999, _("%s: Wrong type for third input argument: String vector expected.\n"), "x_mdialog");
              return FALSE;
            }
          setMessageBoxDefaultInput(messageBoxID, getStringMatrixFromStack(defaultValuesAdr), nbColDefaultValues*nbRowDefaultValues);
        }
      else
        {
          GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRowColumnLabels,&nbColColumnLabels,&columnLabelsAdr);
          if (nbRow !=1 && nbCol !=1)
            {
              Scierror(999, _("%s: Wrong type for third input argument: String vector expected.\n"), "x_mdialog");
              return FALSE;
            }
          setMessageBoxColumnLabels(messageBoxID, getStringMatrixFromStack(columnLabelsAdr), nbColColumnLabels*nbRowColumnLabels);
       }
    }
  else 
    {
      Scierror(999, _("%s: Wrong type for third input argument: String vector expected.\n"), "x_mdialog");
      return FALSE;
    }

  if (Rhs == 4)
    {
      /* READ  DEFAULT VALUES */
      if (VarType(4) ==  sci_strings)
        {
          GetRhsVar(4,MATRIX_OF_STRING_DATATYPE,&nbRowDefaultValues,&nbColDefaultValues,&defaultValuesAdr);
          if ((nbRowDefaultValues != nbRowLineLabels*nbColLineLabels) || (nbColDefaultValues != nbRowColumnLabels*nbColColumnLabels))
            {
              Scierror(999, _("%s: Wrong size for fourth input argument: should be %d x %d.\n"), "x_mdialog", nbRowLineLabels*nbColLineLabels, nbRowColumnLabels*nbColColumnLabels);
              return FALSE;
            }
          setMessageBoxDefaultInput(messageBoxID, getStringMatrixFromStack(defaultValuesAdr), nbColDefaultValues*nbRowDefaultValues);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for fourth input argument: String matrix expected.\n"), "x_mdialog");
          return FALSE;
        }
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
      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRowDefaultValues, &nbColDefaultValues, userValue);
    }

  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
