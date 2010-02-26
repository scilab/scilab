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
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_x_mdialog(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  int nbRowDefaultValues = 0, nbColDefaultValues = 0;
  int nbRowLineLabels = 0, nbColLineLabels = 0;
  int nbRowColumnLabels = 0, nbColColumnLabels = 0;

  int messageBoxID = 0;

  char **labelsAdr = NULL;
  char **lineLabelsAdr = NULL;
  char **columnLabelsAdr = NULL;
  char **defaultValuesAdr = NULL;

  int userValueSize = 0;
  char **userValue = NULL;

  int emptyMatrixAdr = 0;

  CheckRhs(3,4);
  CheckLhs(0,1);

  /* READ THE LABELS */
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
  setMessageBoxTitle(messageBoxID, _("Scilab Multiple Values Request"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)labelsAdr), nbCol*nbRow);
  freeArrayOfString(labelsAdr, nbCol*nbRow);
    
  /* READ THE LINE LABELS */
  if (VarType(2) ==  sci_strings)
    {
      GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRowLineLabels,&nbColLineLabels,&lineLabelsAdr);
      if (nbRow !=1 && nbCol !=1)
      {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 2);
        return FALSE;
      }
      setMessageBoxLineLabels(messageBoxID, getStringMatrixFromStack((size_t)lineLabelsAdr), nbColLineLabels*nbRowLineLabels);
      freeArrayOfString(lineLabelsAdr, nbColLineLabels*nbRowLineLabels);
    }
  else 
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
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
              Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 3);
              return FALSE;
            }
          setMessageBoxDefaultInput(messageBoxID, getStringMatrixFromStack((size_t)defaultValuesAdr), nbColDefaultValues*nbRowDefaultValues);
          freeArrayOfString(defaultValuesAdr, nbColDefaultValues*nbRowDefaultValues);
        }
      else
        {
          GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRowColumnLabels,&nbColColumnLabels,&columnLabelsAdr);
          if (nbRow !=1 && nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 3);
              return FALSE;
            }
          setMessageBoxColumnLabels(messageBoxID, getStringMatrixFromStack((size_t)columnLabelsAdr), nbColColumnLabels*nbRowColumnLabels);
          freeArrayOfString(columnLabelsAdr, nbColColumnLabels*nbRowColumnLabels);
       }
    }
  else 
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 3);
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
              Scierror(999, _("%s: Wrong size for input argument #%d: %d x %d matrix of strings expected.\n"),fname, 4, nbRowLineLabels*nbColLineLabels, nbRowColumnLabels*nbColColumnLabels);
              return FALSE;
            }
          setMessageBoxDefaultInput(messageBoxID, getStringMatrixFromStack((size_t)defaultValuesAdr), nbColDefaultValues*nbRowDefaultValues);
          freeArrayOfString(defaultValuesAdr, nbColDefaultValues*nbRowDefaultValues);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 4);
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
      nbRowDefaultValues = nbColLineLabels*nbRowLineLabels;
      nbColDefaultValues = 1;
      if (Rhs == 4) {
        nbColDefaultValues = nbColColumnLabels*nbRowColumnLabels; 
      }
      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRowDefaultValues, &nbColDefaultValues, userValue);
      /* TO DO : delete of userValue */
    }

  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
