/*--------------------------------------------------------------------------*/
/* Allan CORNET - INRIA 2006 */
/* Vincent COUVERT - INRIA 2008 (java version) */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len))
{
  int nbRow = 0, nbCol = 0;

  int messageBoxID = 0;

  char **buttonsText = NULL;
  int buttonsTextAdr = 0;

  char *message = NULL;
  int messageAdr = 0;

  int buttonNumber = 0;
  int buttonNumberAdr = 0;

  CheckRhs(1,2);
  CheckLhs(0,1);

  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &messageAdr);
      if (nbCol !=1)
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
          return FALSE;
        }
      message = cstk(messageAdr);
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, "Scilab Message");
  /* Message */
  setMessageBoxMessage(messageBoxID, message);
    
  if (Rhs == 2)
    {
      GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&buttonsTextAdr);
      if ((nbCol*nbRow!=1) && (nbCol*nbRow!=2))
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
          return FALSE;
        }

      setMessageBoxButtonsLabels(messageBoxID, (char **)buttonsTextAdr, nbCol*nbRow);
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
