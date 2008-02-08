/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "CallMessageBox.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_x_dialog) _PARAMS((char *fname,unsigned long fname_len))
{
  int nbRow = 0, nbCol = 0;

  int messageBoxID = 0;

  int initialValueAdr = 0;

  int labelsAdr = 0;

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
      Scierror(999, _("%s: Wrong type for first input argument: String vector expected.\n"), "sci_x_dialog");
      return FALSE;
    }

  /* Create the Java Object */
  messageBoxID = createMessageBox();

  /* Title is a default title */
  setMessageBoxTitle(messageBoxID, _("Scilab Input Value Request"));
  /* Message */
  setMessageBoxMultiLineMessage(messageBoxID, (char **)labelsAdr, nbCol*nbRow);
    
  if (Rhs == 2)
    {
      if (VarType(2) ==  sci_strings)
        {
          GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&initialValueAdr);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for first input argument: String vector expected.\n"), "sci_x_dialog");
          return FALSE;
        }

      setMessageBoxInitialValue(messageBoxID, (char **)initialValueAdr, nbCol*nbRow);
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
      
    }

  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
