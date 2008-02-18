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
/* file: sci_help_gtk.c                                                   */
/* desc : interface for help_gtk routine                                  */
/*------------------------------------------------------------------------*/

#include "sci_help_gtk.h"
#include "stack-c.h"

/*--------------------------------------------------------------------------*/
int sci_help_gtk(char *fname,unsigned long fname_len)
{
  /* do nothing */
  CheckLhs(0,1);
  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
