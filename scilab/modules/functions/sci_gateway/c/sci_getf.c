/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_functions.h"
#include "warningmode.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intgetf)(); /* fortran */
/*--------------------------------------------------------------------------*/
int C2F(sci_getf)(char *fname,unsigned long fname_len)
{
	if (getWarningMode())
	{
		/* recursion calls of getf by deff */
		if (strcmp(fname,"getf") == 0)
		{
			sciprint(_("WARNING: Function %s is obsolete.\n"), fname);
			sciprint(_("WARNING: Please use %s instead.\n"), "exec");
			sciprint(_("WARNING: This function will be permanently removed in Scilab %s.\n"), "5.3");
		}
	}

	C2F(intgetf)();
	return 0;
}
/*--------------------------------------------------------------------------*/
