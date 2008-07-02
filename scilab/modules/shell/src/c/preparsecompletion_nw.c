/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "preparsecompletion_nw.h"
/*--------------------------------------------------------------------------*/
const char *preparse_line_for_completion_nw(const char *current_line)
{
	/* remove some specials cases */
	if (current_line)
	{
		char *pch = NULL;
		const char *pchprev = NULL;
		const char *stringstr = current_line;
		char symbs[] = " +-*/\\([^,;={.&|\'])}:\"";
		int i = 0;

		if ( strcmp(current_line,"") == 0 ) return current_line;

		/* Get string to use for completion */
		for (i = 0;i < (int)strlen(symbs);i++)
		{
			pch = strrchr (stringstr,symbs[i]);
			if (pch) 
			{
				stringstr = pch;
				pchprev = pch;
			}
		}

		if (pch)
		{
			if ( (int)strlen(pch) > 1 ) pch++;
			return pch;
		}
		else
		{
			if (pchprev)
			{
				if ( (int)strlen(pchprev) > 1 ) pchprev++;
				return pchprev;
			}
			else return current_line;
		}
	}
	else return NULL;
}
/*--------------------------------------------------------------------------*/
