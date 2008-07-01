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
	const char *linewithoutspaceatbegin = NULL;
	const char *wordToFind = NULL;
	char *pch = NULL;
	int i = 0;

	/* remove blanks beginning of line */
	for (i = 0;i<(int)strlen(current_line);i++)
	{
		if (current_line[i] != ' ')
		{
			linewithoutspaceatbegin = &current_line[i];
			break;
		}
	}

	pch = strrchr (linewithoutspaceatbegin,' ');

	if (pch)
	{
		if ( (pch-linewithoutspaceatbegin) > 0 )
		{
			pch++;
			wordToFind = pch;
		}
	}
	else wordToFind = linewithoutspaceatbegin;

	return wordToFind;
}
/*--------------------------------------------------------------------------*/
