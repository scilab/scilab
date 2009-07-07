/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_time.h"
#include "api_double.h"
#include "timer.h"
#include "localization.h"
#include "Scierror.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_timer(char *fname,unsigned long fname_len)
{
  double timerval = 0;
  
  Rhs = Max(0, Rhs);
  CheckLhs(0,1);
  CheckRhs(0,0);
  
  timerval = scilab_timer();

  if (timerval >= 0.)
    {
      int n1 = 1, res = 0;
      double * pDblReal = NULL;

      res = allocMatrixOfDouble(Rhs+1, n1, n1, &pDblReal);

      *pDblReal = (double)timerval;
      
      LhsVar(1) = Rhs+1;
      C2F(putlhsvar)();
    }
  else
    {
      Scierror(999,_("%s: An error occurred.\n"), fname);
    }
  
  return 0;
}
/*--------------------------------------------------------------------------*/
