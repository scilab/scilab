/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006-2009 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_demo.c                                                       */
/* desc : function used to launch a demo of a command                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "stack-c.h"

int sci_demo( char * fname, int fname_len)
{
  int lw = 0;
  C2F(overload)(&lw, fname, fname_len);
  return 0;
}
