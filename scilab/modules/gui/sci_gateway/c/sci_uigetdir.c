/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Vincent COUVERT */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "CallFileChooser.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_uigetdir(char *fname,unsigned long l)
{
  int fileChooserID = 0;
  
  int nbRow = 0, nbCol = 0;

  
  int voidSelectionAdr = 0;

  int titleAdr = 0;
  int initialDirectoryAdr = 0;

  char *title = NULL, *initialDirectory = NULL;

  CheckRhs(0,2);
  CheckLhs(1,1);

  if (Rhs==1)
    {
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
              return FALSE;
            }
          initialDirectory = cstk(initialDirectoryAdr);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
          return FALSE;
        }
    }
  else if (Rhs==2)
    {
      /* First argument is initial directory */
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
              return FALSE;
            }
          initialDirectory = cstk(initialDirectoryAdr);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for first input argument: Single string expected.\n"));
          return FALSE;
        }

      /* Second argument is title */
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"));
              return FALSE;
            }
          title = cstk(titleAdr);
        }
      else 
        {
          Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"));
          return FALSE;
        }
    }
  
  /* Create the Java Object */
  fileChooserID = createFileChooser();
  
  /* Set options */
  if (title != NULL)
    {
      setFileChooserTitle(fileChooserID, title);
    }

  if (initialDirectory != NULL)
    {
      setFileChooserInitialDirectory(fileChooserID, initialDirectory);
    }

  setFileChooserDirectorySelectionOnly(fileChooserID);

  /* Display it and wait for a user input */
  fileChooserDisplayAndWait(fileChooserID);

  /* Read the size of the selection, if 0 then no file selected */
  nbRow = getFileChooserSelectionSize(fileChooserID);

  if (nbRow !=0 ) 
    {
	  char **userSelection = NULL;
      /* The user selected a file --> returns the files names */
      nbCol = 1;
      
      userSelection = getFileChooserSelection(fileChooserID);

      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, userSelection);
    }
  else
    {
      /* The user canceled the selection --> returns an empty matrix */
      nbRow = 0;
      nbCol = 0;

      CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &voidSelectionAdr);
    }
  
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
