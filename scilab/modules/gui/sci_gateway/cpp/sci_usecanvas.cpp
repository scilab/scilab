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
#include "BOOL.h"
#include"MALLOC.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
int sci_usecanvas( char * fname, unsigned long fname_len )
{
  int m1 = 0, n1 = 0, l1 = 0;
  int *status = NULL;

  CheckLhs(0,1);
  CheckRhs(0,1);

  if (Rhs == 1) /* Sets the status of usecanvas */
    {
      if (VarType(1) != sci_boolean)
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 1);
          return FALSE;
        }
      
      GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);
      
      if (m1*n1 != 1)
        {
          Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
          return FALSE;
        }
      
      CallScilabBridge::useCanvasForDisplay(getScilabJavaVM(), BOOLtobool(*istk(l1)));
   }

  /* Returns current status */
  if ((status = (int*) MALLOC(sizeof(int))) == NULL)
    {
      Scierror(999, _("%s: No more memory.\n"), fname, 0);
      return FALSE;
    }
  
  status[0] = booltoBOOL(CallScilabBridge::useCanvasForDisplay(getScilabJavaVM()));
  
  m1 = 1;
  n1 = 1;
  CreateVarFromPtr(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&status);
  
  FREE(status);
  
  LhsVar(1) = 1;
  PutLhsVar();

  return TRUE;
 
}
/*--------------------------------------------------------------------------*/
