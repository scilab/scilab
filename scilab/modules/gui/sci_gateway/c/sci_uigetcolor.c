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
#include "CallColorChooser.h"

/*--------------------------------------------------------------------------*/
int sci_uigetcolor(char *fname,unsigned long fname_len)
{
  int colorChooserID = 0;
  int firstColorIndex = 0;

  int nbRow = 0, nbCol = 0;

  int redAdr = 0;
  int greenAdr = 0;
  int blueAdr = 0;
  int titleAdr = 0;

  double *selectedRGB = NULL;

  CheckRhs(0, 4);

  if ((Lhs!=1) && (Lhs!=3)) /* Bad use */
    {
      Scierror(999, _("%s: Wrong number of output arguments: %d or %d expected.\n"), fname, 1, 3);
      return FALSE;
    }
  
  /* Rhs==1: title or [R, G, B] given */
  if (Rhs==1)
    {
      if (VarType(1) == sci_matrix)
        {
          GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &redAdr);
          if ((nbRow != 1) || (nbCol != 3))
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A 1 x %d real row vector expected.\n"), fname, 1, 3);
              return FALSE;
            }
        }
      else if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A real or a string expected.\n"), fname, 1);
          return FALSE;
        }
    }

  /* Title and [R, G, B] given */
  if (Rhs==2)
    {
      /* Title */
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbCol != 1)
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

      /* [R, G, B] */
      if (VarType(2) == sci_matrix)
        {
          GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &redAdr);
          if (nbRow*nbCol != 3)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A 1 x %d real row vector expected.\n"), fname, 2, 3);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A 1 x %d real row vector expected.\n"), fname, 2, 3);
          return FALSE;
        }
    }

  /* No title given but colors given with separate values */
  if (Rhs==3)
    {
      firstColorIndex = 1;
    }

  /* Title and colors given with separate values */
  if (Rhs==4)
    {
      firstColorIndex = 2;
      
      /* Title */
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A real or a string expected.\n"), fname, 1);
          return FALSE;
        }
    }

  /* R, G, B given */
  if (Rhs>=3)
    {
      /* Default red value */
      if (VarType(firstColorIndex) == sci_matrix)
        {
          GetRhsVar(firstColorIndex, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &redAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, firstColorIndex);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, firstColorIndex);
          return FALSE;
        }
      
      /* Default green value */
      if (VarType(firstColorIndex + 1) == sci_matrix)
        {
          GetRhsVar(firstColorIndex + 1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &greenAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, firstColorIndex + 1);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, firstColorIndex + 1);
          return FALSE;
        }
      
      /* Default blue value */
      if (VarType(firstColorIndex + 2) == sci_matrix)
        {
          GetRhsVar(firstColorIndex + 2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &blueAdr);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, firstColorIndex + 2);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, firstColorIndex + 2);
          return FALSE;
        }
    }
  
  /* Create the Java Object */
  colorChooserID = createColorChooser();

  /* Title */
  if (titleAdr != 0)
    {
      setColorChooserTitle(colorChooserID, cstk(titleAdr));
    }

  /* Default red value */
  if (redAdr != 0)
    {
      if (greenAdr !=0 ) /* All values given in first input argument */
        {
          setColorChooserDefaultRGBSeparateValues(colorChooserID, (int)stk(redAdr)[0], (int)stk(greenAdr)[0], (int)stk(blueAdr)[0]);
        }
      else
        {
          setColorChooserDefaultRGB(colorChooserID, stk(redAdr));
        }
    }

  /* Display it and wait for a user input */
  colorChooserDisplayAndWait(colorChooserID);

  /* Return the selected color */

  /* Read the user answer */
  selectedRGB = getColorChooserSelectedRGB(colorChooserID);

  if (selectedRGB[0] >= 0) /* The user selected a color */
    {

      nbRow = 1;
      if (Lhs==1)
        {
          nbCol = 3;
          CreateVarFromPtr(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &selectedRGB);
          LhsVar(1) = Rhs+1;
        }

      if (Lhs>=2)
        {
          nbCol = 1;
          CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &redAdr);
          *stk(redAdr) = selectedRGB[0];
          CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &greenAdr);
          *stk(greenAdr) = selectedRGB[1];
          CreateVar(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &blueAdr);
          *stk(blueAdr) = selectedRGB[2];

          LhsVar(1) = Rhs+1;
          LhsVar(2) = Rhs+2;
          LhsVar(3) = Rhs+3;
        }
    }
  else /* The user canceled */
    {
      nbRow = 0; nbCol = 0;
      if (Lhs==1)
        {
          /* Return [] */
          CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &redAdr);

          LhsVar(1) = Rhs+1;
        }

      if (Lhs>=2)
        {
          CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &redAdr);
          CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &greenAdr);
          CreateVar(Rhs+3, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &blueAdr);

          LhsVar(1) = Rhs+1;
          LhsVar(2) = Rhs+2;
          LhsVar(3) = Rhs+3;
        }
    }

  PutLhsVar();
  return TRUE;
}
/*--------------------------------------------------------------------------*/
