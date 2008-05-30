/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * desc : interface for sci_uiwait routine 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "ObjectStructure.h"
#include "stack-c.h"
#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
int sci_getcallbackobject(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0;

  sciPointObj *pObj = NULL;

  CheckRhs(1,1);
  CheckLhs(0,1);

  if (VarType(1) == sci_matrix)
    {
      GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
      if (nbRow * nbCol !=1)
        {
          Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
          return FALSE;
        }
      pObj = sciGetPointerFromJavaIndex((int) *stk(stkAdr));
    }
  else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
      return FALSE;
    }
 
  /* Create return variable */
  if (pObj == NULL) /* Non-existing object --> return [] */
    {
      nbRow = 0;
      nbCol = 0;
      CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
    }
  else /* Return the handle */
    {
      nbRow = 1;
      nbCol = 1;
      CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
      *hstk(stkAdr) = sciGetHandle(pObj);
    }
      
  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
