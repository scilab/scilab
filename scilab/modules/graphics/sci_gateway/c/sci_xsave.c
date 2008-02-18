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
/* file: sci_xsave.c                                                      */
/* desc : interface for sci_xsave routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xsave.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xsave( char * fname, unsigned long fname_len )
{

  CheckRhs(1,2);
 
  /* call a scilab macro */
  C2F(callscifun)("xsave",(unsigned long) 6);
  return 0;

}

/*--------------------------------------------------------------------------*/
