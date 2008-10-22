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
#include "Scierror.h"
#include "CallFontChooser.h"
#include "getPropertyAssignedValue.h"

/*--------------------------------------------------------------------------*/
int sci_uigetfont(char *fname,unsigned long fname_len)
{
  int fontChooserID = 0;

  int nbRow = 0, nbCol = 0;

  char **fontNameAdr = NULL;
  int fontSizeAdr = 0;
  int boldAdr = 0;
  int italicAdr = 0;

  char *selectedFontName = NULL;
  int selectedFontSize = 0;
  BOOL selectedBold = FALSE;
  BOOL selectedItalic = FALSE;


  CheckRhs(0,4);
  CheckLhs(1,4);

  /* Default font name */
  if (Rhs >= 1) {
    if (VarType(1) == sci_strings) 
      {
        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &fontNameAdr);
        if (nbRow*nbCol != 1)
          {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
            return FALSE;
          }
      }
    else
      {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return FALSE;
      }
  }

  /* Default font size */
  if (Rhs >= 2) 
    {
      if (VarType(2) == sci_matrix)
        {
          GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fontSizeAdr);
          if (nbRow*nbCol!=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 2);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
          return FALSE;
        }
    }

  /* Is the default font bold ? */
  if (Rhs >= 3)
    {
      if (VarType(3) == sci_boolean)
        {
          GetRhsVar(3,MATRIX_OF_BOOLEAN_DATATYPE,&nbRow,&nbCol,&boldAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
              return FALSE;
            }

        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
          return FALSE;
        }
    }

  /* Is the default font italic ? */
  if (Rhs >= 4)
    {
      if (VarType(4) == sci_boolean)
        {
          GetRhsVar(4,MATRIX_OF_BOOLEAN_DATATYPE,&nbRow,&nbCol,&italicAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 4);
              return FALSE;
            }

        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 4);
          return FALSE;
        }
    }

  /* Create the Java Object */
  fontChooserID = createFontChooser();

  /* Default font */
  if (fontNameAdr != 0)
    {
      setFontChooserFontName(fontChooserID, getStringMatrixFromStack((size_t)fontNameAdr)[0]);
    }
  
  /* Default size */
  if (fontSizeAdr != 0)
    {
      setFontChooserFontSize(fontChooserID, (int)stk(fontSizeAdr)[0]);
    }
  
  /* Default bold */
  if (boldAdr != 0)
    {
      setFontChooserBold(fontChooserID, istk(boldAdr)[0]);
    }
  
  /* Default italic */
  if (italicAdr != 0)
    {
      setFontChooserItalic(fontChooserID, istk(italicAdr)[0]);
    }
  
  /* Display it and wait for a user input */
  fontChooserDisplayAndWait(fontChooserID);

  /* Return the selected font */
  
  /* Read the user answer */
  selectedFontName = getFontChooserFontName(fontChooserID);
  

  if (strcmp(selectedFontName,"")) /* The user selected a font */
    {
      selectedFontSize = getFontChooserFontSize(fontChooserID);
      selectedBold = getFontChooserBold(fontChooserID);
      selectedItalic = getFontChooserItalic(fontChooserID);

      nbRow = 1; nbCol = 1;
      if (Lhs>=1)
        {
          CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &selectedFontName);
        }
  
      if (Lhs>=2)
        {
          CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fontSizeAdr);
          *stk(fontSizeAdr) = selectedFontSize;
        }

      if (Lhs>=3)
        {
          CreateVar(Rhs+3, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &boldAdr);
          *istk(boldAdr) = selectedBold;
        }

      if (Lhs>=4)
        {
          CreateVar(Rhs+4, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &italicAdr);
          *istk(italicAdr) = selectedItalic;
        }
    }
  else /* The user canceled */
    {
      nbRow = 0; nbCol = 0;
      if (Lhs>=1)
        {
          /* Return "" as font name */
          CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &fontNameAdr);
        }
  
      if (Lhs>=2)
        {
          /* Return [] as font size */
          CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fontSizeAdr);
        }

      if (Lhs>=3)
        {
          /* Return [] as bold value */
          CreateVar(Rhs+3, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &boldAdr);
        }

      if (Lhs>=4)
        {
           /* Return [] as italic value */
         CreateVar(Rhs+4, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &italicAdr);
        }
    }
  
  /* TO DO delete [] selectedFontName */
  
  LhsVar(1) = Rhs+1;
  LhsVar(2) = Rhs+2;
  LhsVar(3) = Rhs+3;
  LhsVar(4) = Rhs+4;
  
  C2F(putlhsvar)();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
