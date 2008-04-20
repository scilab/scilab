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
 
#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
#include "scilabmode.h"
#include "localization.h"
#include "IsAScalar.h"
#include "PrinterManagement.h"
/*--------------------------------------------------------------------------*/
int sci_printfigure(char *fname,unsigned long l)
{
  static int l1,n1,m1;
  int num_win = -2;

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
              Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
              return FALSE;
            }
          num_win=(int)(*stk(l1));

          if (num_win>=0)
            {
              /* Call Java */
              printFigure(num_win);
              m1=0;
              n1=0;
              CreateVar(1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
              LhsVar(1)=1;
              C2F(putlhsvar)();	
              return TRUE;
            }
          else
            {
              Scierror(999,_("%s: Wrong value for first input argument: must be >= 0 .\n"), fname);
              return FALSE;
            }
        }
      else
        {
          Scierror(999,_("%s: Wrong type for second input argument: Single double expected.\n"), fname);
          return FALSE;
        }
    }
  else
    {
      Scierror(999,_("%s: function not available in NWNI mode.\n"), fname);
      return FALSE;
    }
  return TRUE;
}
/*--------------------------------------------------------------------------*/
