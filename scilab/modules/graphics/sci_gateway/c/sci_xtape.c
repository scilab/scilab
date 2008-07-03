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
/* file: sci_xtape.c                                                      */
/* desc : interface for xtape routine                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xtape.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_xtape( char *fname,unsigned long fname_len )
{
  Scierror(999,_("%s: This function was only available with old graphic mode.\n"), fname);
  return -1 ;
}
/*--------------------------------------------------------------------------*/
