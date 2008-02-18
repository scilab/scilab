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
/* file: sci_xload.c                                                      */
/* desc : interface for sci_xload routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xload.h"
#include "stack-c.h"
#include "GetProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xload(char *fname,unsigned long fname_len)
{
  CheckRhs(1,2);
 
  /* call a Scilab function to handle compatibility */
  C2F(callscifun)("xload",(unsigned long) 5);
  return 0;
}

/*--------------------------------------------------------------------------*/
