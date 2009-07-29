/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Xpad.hxx"

extern "C"
{
#include "gw_xpad.h"
#include "stack-c.h"
#include "localization.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
}

using namespace org_scilab_modules_xpad;

/*--------------------------------------------------------------------------*/
int sci_xpad(char *fname,unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0;

  int fileNameAdr = 0;

  CheckRhs(0,1);

  if (Rhs==0) /* Open Xpad */
    {
      Xpad::xpad(getScilabJavaVM());
    }
  else /* Open a file in Xpad */
    {
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileNameAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
              return FALSE;
            }
         Xpad::xpad(getScilabJavaVM(), cstk(fileNameAdr));
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
          return FALSE;
        }
    }
  
  LhsVar(1) = 0;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
