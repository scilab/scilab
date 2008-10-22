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
#include "Scierror.h"
#include "scilabmode.h"
#include "localization.h"
#include "IsAScalar.h"
#include "gw_gui.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
int sci_printfigure(char *fname,unsigned long l)
{
  static int l1,n1,m1;
  int num_win = -2;
  int *status = NULL;

  Rhs=Max(0,Rhs);
  CheckRhs(1,1);
  CheckLhs(0,1);

  if ( getScilabMode() != SCILAB_NWNI )
    {
      if (Rhs == 1)
        {
          GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
          
          if (!IsAScalar(1))
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
              return FALSE;
            }
          num_win=(int)(*stk(l1));

          if (num_win>=0)
            {
              /* Call Java */
              status = new int[1];
              status[0] = (int)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, true, true); /* postscript mode and display dialog */
              m1=1;
              n1=1;
              CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,  &m1, &n1, &status);
              LhsVar(1)=Rhs+1;
              C2F(putlhsvar)();	
              delete[] status;
              return TRUE;
            }
          else
            {
              Scierror(999,_("%s: Wrong value for input argument #%d: Must be >= %d expected.\n"), fname, 1, 0);
              return FALSE;
            }
        }
      else
        {
          Scierror(999,_("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
          return FALSE;
        }
    }
  else
    {
      Scierror(999,_("%s: Function not available in NWNI mode.\n"), fname);
      return FALSE;
    }
  return TRUE;
}
/*--------------------------------------------------------------------------*/
