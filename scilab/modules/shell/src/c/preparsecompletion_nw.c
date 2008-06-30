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
	const char *wordToFind = NULL;
	char *pch = NULL;

	pch = strrchr (current_line,' ');

	if (pch)
	{
		if ( (pch-current_line) > 0 )
		{
			pch++;
			wordToFind = pch;
		}
	}
	else wordToFind = current_line;

	return wordToFind;
}
/*--------------------------------------------------------------------------*/
