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

#include "CallJuigetfile.hxx"
#include "BOOL.h"

extern "C"
{
#include <stdio.h>
#include "gw_gui.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
}

using namespace org_scilab_modules_gui_filechooser;
/*--------------------------------------------------------------------------*/
int sci_uigetdir(char *fname,unsigned long l)
{
  int nbRow = 0, nbCol = 0;

  int titleAdr = 0;
  int initialDirectoryAdr = 0;

  char *title = NULL, *initialDirectory = NULL;

  char **userSelection = NULL;

  char *expandedpath = NULL;
  
  CheckRhs(0,2);
  CheckLhs(1,1);

  if (Rhs >= 1)
    {
      /* First argument is initial directory */
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
              FREE(expandedpath);
              return FALSE;
            }
          initialDirectory = cstk(initialDirectoryAdr);

		  expandedpath = expandPathVariable(initialDirectory);
       }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
		  if (expandedpath) {FREE(expandedpath); expandedpath = NULL;}
          return FALSE;
        }

    }

   if (Rhs == 2)
     {
       /* Second argument is title */
       if (VarType(2) == sci_strings)
         {
           GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
           if (nbCol !=1)
             {
               Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
               if (expandedpath) {FREE(expandedpath); expandedpath = NULL;}
               return FALSE;
             }
           title = cstk(titleAdr);
         }
       else
         {
           Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
           if (expandedpath) {FREE(expandedpath); expandedpath = NULL;}
           return FALSE;
         }
     }
   
  switch (Rhs)
    {
      /* Initial path is given */
    case 1:
      CallJuigetfileForDirectoryWithInitialdirectory(expandedpath);
      break;
      /* Initial path and title are given */
    case 2:
      CallJuigetfileForDirectoryWithInitialdirectoryAndTitle(expandedpath, title);
      break;
      /* Default call with default path and title */
    default:
      CallJuigetfileForDirectoryWithoutInput();
      break;
    }

  /* Read the size of the selection, if 0 then no file selected */
  nbRow = getJuigetfileSelectionSize();
  /* Read the selection */
  userSelection = getJuigetfileSelection();

  if (nbRow !=0 )
    {
      /* The user selected a file --> returns the files names */
      nbCol = 1;

      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, userSelection);
      freeArrayOfString(userSelection, nbRow);

    }
  else
    {
      /* The user canceled the selection --> returns an empty matrix */
      nbRow = 1;
      nbCol = 1;
      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, NULL);
    }

  LhsVar(1)=Rhs+1;
  PutLhsVar();

  if (expandedpath) {FREE(expandedpath); expandedpath = NULL;}
  
  return TRUE;
}
/*--------------------------------------------------------------------------*/
