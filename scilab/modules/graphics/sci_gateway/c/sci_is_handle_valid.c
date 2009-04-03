/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_is_handle_valid.h                                            */
/* desc : interface for xaxis routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_is_handle_valid.h"
#include "gw_graphics.h"
#include "stack-c.h"
#include "HandleManagement.h"
#include "Scierror.h"
#include "localization.h"
#include "getPropertyAssignedValue.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_is_handle_valid(char *fname,unsigned long fname_len)
{
  
  /* Call isValid = is_handle_valid(h) with h a matrix of handle */
  /* and isValid a matrix of boolean */

  int nbCol;
  int nbRow;
  int nbHandle;
  int handleStackPointer;
  int resultStackPointer;
  int i;

  CheckRhs(1,1);
  CheckLhs(0,1);

  /* Get handles matrix */
  if (VarType(1) != sci_handles)
  {
    Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of handle expected.\n"), fname,1);
    return  -1;
  }

  GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleStackPointer);
  nbHandle = nbRow * nbCol;

  /* create output matrix */
  CreateVar(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &resultStackPointer)
  
  /* Check each handle */
  for (i = 0; i < nbHandle; i++)
  {
    *istk(resultStackPointer + i) = (int) isHandleValid(getHandleFromStack(handleStackPointer + i));
  }

  LhsVar(1) = Rhs + 1;
  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/
