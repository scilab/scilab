/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : interface for xs2png routine
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
#include "HandleManagement.h"
#include "GetProperty.h"
#include "localization.h"
#include "Scierror.h"
#include "exportUserInterface.hxx"

/*--------------------------------------------------------------------------*/
int sci_exportUI( char * fname, unsigned long fname_len )
{
  // call, exportUI(figId) exportUI(fig)
  // with figId an int and fig a figure handle

  // id of the figure to export
  int figureId = 0;

  CheckLhs(0,1);
  CheckRhs(1,1);

  if (GetType(1) == sci_handles)
  {
    int nbRow;
    int nbCol;
    size_t stackPointer = 0;
    sciPointObj * figure = NULL;
    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer);
    if (nbRow * nbCol != 1)
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
    }
    figure = sciGetPointerFromHandle((unsigned long) *(hstk( stackPointer )));
    if (sciGetEntityType(figure) != SCI_FIGURE)
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
    }
    figureId = sciGetNum(figure);
  }
  else if (GetType(1) == sci_matrix)
  {
    int nbRow;
    int nbCol;
    size_t stackPointer = 0;
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stackPointer);
    if (nbRow * nbCol != 1)
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
    }
    figureId = (int) *(stk( stackPointer ));
  }
  else
  {
    Scierror(999,_("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
  }

  // call the export function
  exportUserInterface(figureId);

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
