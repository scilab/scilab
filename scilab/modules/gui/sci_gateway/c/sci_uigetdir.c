/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "CallFileChooser.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "cluni0.h"
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
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname,1);
              return FALSE;
            }
          initialDirectory = cstk(initialDirectoryAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
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
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
              return FALSE;
            }
          initialDirectory = cstk(initialDirectoryAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
          return FALSE;
        }

      /* Second argument is title */
      if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
              return FALSE;
            }
          title = cstk(titleAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
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
		char expandedpath[PATH_MAX+1];
		int out_n = 0;
		C2F(cluni0)(initialDirectory ,expandedpath, &out_n,(int)strlen(initialDirectory),PATH_MAX);
		setFileChooserInitialDirectory(fileChooserID, expandedpath);
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
      /* TO DO a delete [] userSelection */

    }
  else
    {
      /* The user canceled the selection --> returns an empty matrix */
      nbRow = 0;
      nbCol = 0;

      CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &voidSelectionAdr);
    }

  LhsVar(1)=Rhs+1;
  PutLhsVar();;

  return TRUE;
}
/*--------------------------------------------------------------------------*/
