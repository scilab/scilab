/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "stricmp.h"
#include "messageboxoptions.h"

/*--------------------------------------------------------------------------*/
int sci_messagebox(char *fname,unsigned long fname_len)
{
  int messageBoxID = 0;

  /* Used to read input arguments */
  int nbRow = 0, nbCol = 0;
  int nbRowButtons = 0, nbColButtons = 0;
  int nbRowMessage = 0, nbColMessage = 0;

  char **buttonsTextAdr = 0;
  char **messageAdr = 0;
  char **titleAdr = 0;
  char **modalOptionAdr = 0;
  char **iconAdr = 0;

  /* Used to write output argument */
  int buttonNumberAdr = 0;
  int buttonNumber = 0;

  CheckRhs(1,5);
  CheckLhs(0,1);

  /* Message to be displayed */
  if (VarType(1) == sci_strings) 
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
      return FALSE;
    }

  /* Title to be displayed */
  if (Rhs >= 2) 
    {
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbRow*nbCol!=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
              return FALSE;
            }
          /* The title argument can be used to give the modal option */
          if (isModalOption(getStringMatrixFromStack((size_t)titleAdr)[0]))
            {
              modalOptionAdr = titleAdr;
              titleAdr = NULL;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
          return FALSE;
        }
    }

  /* Icon to be displayed */
  if (Rhs >= 3)
    {
      if (VarType(3) == sci_strings)
        {
          GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&iconAdr);
          if (nbRow*nbCol == 1)
            {
              /* The icon argument can be used to give the modal option or the buttons names */
              if (isModalOption(getStringMatrixFromStack((size_t)iconAdr)[0]))
                {
                  modalOptionAdr = (char **)iconAdr;
                  iconAdr = NULL;
                }
              else if(!isIconName(getStringMatrixFromStack((size_t)iconAdr)[0]))
                {
                  buttonsTextAdr = (char **)iconAdr;
                  nbRowButtons = nbRow;
                  nbColButtons = nbCol;
                  iconAdr = NULL;
                }
            }
          else  /* More than one string --> buttons names */
            {
              buttonsTextAdr = (char **)iconAdr;
              nbRowButtons = nbRow;
              nbColButtons = nbCol;
              iconAdr = NULL;
            }

        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string or a string vector expected.\n"), fname, 3);
          return FALSE;
        }
    }

  /* Buttons names */
  if (Rhs >= 4)
    {
      if (VarType(4) == sci_strings)
        {
          GetRhsVar(4,MATRIX_OF_STRING_DATATYPE,&nbRowButtons,&nbColButtons,&buttonsTextAdr);
          if (nbRow*nbCol == 1)
            {
              /* The buttons names argument can be used to give the modal option */
              if (isModalOption(getStringMatrixFromStack((size_t)buttonsTextAdr)[0]))
                {
                  modalOptionAdr = buttonsTextAdr;
                  buttonsTextAdr = NULL;
                }
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string or a string vector expected.\n"), fname, 3);
          return FALSE;
        }
    }
  
  /* Modal option */
  if (Rhs == 5)
    {
      if (VarType(5) == sci_strings)
        {
          GetRhsVar(5,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&modalOptionAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 5);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 5);
          return FALSE;
        }
    }
  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, getStringMatrixFromStack((size_t)messageAdr), nbColMessage*nbRowMessage);

  /* Title */
  if (titleAdr != NULL)
    {
      setMessageBoxTitle(messageBoxID, getStringMatrixFromStack((size_t)titleAdr)[0]);
    }
  else
    {
      setMessageBoxTitle(messageBoxID, _("Scilab Message"));
    }

  /* Icon */
  if (iconAdr != NULL)
    {
      setMessageBoxIcon(messageBoxID, getStringMatrixFromStack((size_t)iconAdr)[0]);
    }
    
  /* Buttons */
  if (buttonsTextAdr != NULL)
    {
      setMessageBoxButtonsLabels(messageBoxID, getStringMatrixFromStack((size_t)buttonsTextAdr), nbColButtons*nbRowButtons);
    }

  /* Modal ? */
  if (modalOptionAdr != NULL)
    {
      setMessageBoxModal(messageBoxID, !stricmp(getStringMatrixFromStack((size_t)modalOptionAdr)[0],"modal"));
    }
  else
    {
      setMessageBoxModal(messageBoxID, FALSE);
    }
  
  /* Display it and wait for a user input */
  messageBoxDisplayAndWait(messageBoxID);

  /* Return the index of the button selected */
  
  if (Lhs == 1)
    {
      /* Read the user answer */
      buttonNumber = getMessageBoxSelectedButton(messageBoxID);
      
      nbRow = 1; nbCol = 1;
      CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &buttonNumberAdr);
      *stk(buttonNumberAdr) = buttonNumber;
      
      LhsVar(1) = Rhs+1;
    }
  else
    {
      LhsVar(1) = 0;
    }
  
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
