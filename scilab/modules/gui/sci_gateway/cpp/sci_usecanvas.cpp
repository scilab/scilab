/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CallScilabBridge.hxx"

extern "C"
{
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
int sci_usecanvas( char * fname, unsigned long fname_len )
{
  int m1 = 0, n1 = 0, l1 = 0;

  CheckLhs(1,1);

  if (VarType(1) != sci_boolean)
    {
      Scierror(999, _("Wrong type for input argument #%d: A boolean expected.\n"), 1);
      return FALSE;
    }

  GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);

  if (m1*n1 != 1)
    {
      Scierror(999, _("Wrong size for input argument #%d: A boolean expected.\n"), 1);
      return FALSE;
    }

  CallScilabBridge::useCanvasForDisplay(getScilabJavaVM(), *istk(l1));

  LhsVar(1)=0;
  PutLhsVar();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
