/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET 
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java implementation)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "freeArrayOfString.h"

/*--------------------------------------------------------------------------*/
int sci_x_choice(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;
  int nbRowDefaultValues = 0, nbColDefaultValues = 0;
  int nbRowLineLabels = 0, nbColLineLabels = 0;

  int messageBoxID = 0;

  char **labelsAdr = NULL;
  char **lineLabelsAdr = NULL;
  int defaultValuesAdr = 0;
  double *defaultValues = NULL;
  int *defaultValuesInt = NULL;

  int userValueSize = 0;
  int *userValue = NULL;
  double *userValueDouble = NULL;

  int emptyMatrixAdr = 0;

  int K = 0;

  CheckRhs(3,3);
  CheckLhs(0,1);

  /* READ THE DEFAULT VALUES */
  if (VarType(1) ==  sci_matrix)
    {
      GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&nbRowDefaultValues,&nbColDefaultValues,&defaultValuesAdr);
      defaultValues = getDoubleMatrixFromStack(defaultValuesAdr);
      
      defaultValuesInt = (int *)MALLOC(nbRowDefaultValues*nbColDefaultValues*sizeof(int));
      for (K = 0; K < nbRowDefaultValues*nbColDefaultValues; K++)
        {
          defaultValuesInt[K] = (int)defaultValues[K];
        }
    }
  else 
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"), fname, 1);
      return FALSE;
    }
  
  /* READ THE MESSAGE */
  if (VarType(2) == sci_strings)
    {
      GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &labelsAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Choices Request"));

  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)labelsAdr), nbCol*nbRow);
  freeArrayOfString(labelsAdr, nbRow*nbCol);

  /* READ THE LABELS */
  if (VarType(3) ==  sci_strings)
    {
      GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRowLineLabels,&nbColLineLabels,&lineLabelsAdr);
      if (nbRow !=1 && nbCol !=1)
      {
        freeArrayOfString(lineLabelsAdr, nbRowLineLabels*nbColLineLabels);
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 3);
        return FALSE;
      }
      setMessageBoxLineLabels(messageBoxID, getStringMatrixFromStack((size_t)lineLabelsAdr), nbColLineLabels*nbRowLineLabels);
      freeArrayOfString(lineLabelsAdr, nbRowLineLabels*nbColLineLabels);
    }
  else 
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 3);
      return FALSE;
    }
  
  /* Default selected buttons */
  setMessageBoxDefaultSelectedButtons(messageBoxID, defaultValuesInt, nbRowDefaultValues*nbColDefaultValues);

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
	  
      userValue = (int*)getMessageBoxUserSelectedButtons(messageBoxID);
  
      userValueDouble = (double *)MALLOC(nbRowDefaultValues*nbColDefaultValues*sizeof(double));
      for (K = 0; K < nbRowDefaultValues*nbColDefaultValues; K++)
        {
          userValueDouble[K] = userValue[K];
        }

      CreateVarFromPtr(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRowDefaultValues, &nbColDefaultValues, &userValueDouble);
      /* TO DO : do a delete []  getMessageBoxUserSelectedButton */
    }

  FREE(defaultValuesInt);

  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
