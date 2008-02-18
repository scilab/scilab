/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_driver.c                                                     */
/* desc : interface for driver routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_driver.h"
#include "stack-c.h"
#include "returnProperty.h"

/*--------------------------------------------------------------------------*/
int sci_driver(char *fname,unsigned long fname_len)
{
  integer m1=3,n1=1,l1;
  CheckRhs(-1,1);
  CheckLhs(0,1);
  if (Rhs <= 0) 
  {
    return sciReturnString("Rec");
  } 
  else 
  {
    GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

    LhsVar(1)=0;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
