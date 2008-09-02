/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * desc : interface for sci_mpopup routine 
 * (temporary function waiting for uicontextmenu function) 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h> 
#include <string.h> 
/*--------------------------------------------------------------------------*/
#include "sci_mpopup.h"
#include "MALLOC.h" /* MALLOC */
#include "gw_gui.h"
#include "localization.h"
#include "stack-c.h"
#include "sciprint.h"
#include "ContextMenu.h"
#include "returnProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_mpopup(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;

  char * res = NULL;
  char **menuAdr = NULL;
  int resAdr = 0;

  CheckRhs(1,1);
  CheckLhs(0,1);
  
  if (VarType(1) == sci_strings)
    {
      GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &menuAdr);
    }
  else
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A string vector expected.\n"),fname, 1);
      return FALSE;
    }

  //res = createContextMenu(getStringMatrixFromStack(menuAdr), nbRow*nbCol);

  nbRow = 1;
  nbCol = (int)strlen(res);

  CreateVar(Rhs+1,STRING_DATATYPE,&nbRow,&nbCol,&resAdr);
  strncpy(cstk(resAdr), res, nbCol);

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
