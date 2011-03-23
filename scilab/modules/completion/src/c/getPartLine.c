/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "getPartLine.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
char *getPartLevel(char *line)
{
	#define MAX_SYMBS 28
	const char symbs[MAX_SYMBS] = "+-*/\\([ ^,;={.&|\'])}:\"\'><~@\t";
	int index = -1;
	int i = 0;

	for (i = 0; i < MAX_SYMBS; i++) 
	{
		int len = 0;
		char *pch = strrchr(line, symbs[i]);
		if (pch) 
		{
			len = (int) (strlen(line) - strlen(pch));
			index = Max(index, len);
		}
	}

	return strdup(&line[index + 1]);
}
/*--------------------------------------------------------------------------*/
char *getFilePartLevel(char *line)
{
	#define MAX_SYMBS_F 2
	char symbs[MAX_SYMBS_F] = ";,";
	char *linebis = NULL;
	int index = -1;
	int i = 0;
	int len = (int) strlen(line);
	int indexspace = -1;
	int indexquote = -1;
	int indexdquote = -1;
	char *pch = NULL;

	for (i = 0; i < MAX_SYMBS_F; i++) 
	{
		int len = 0;
		pch = strrchr(line, symbs[i]);
		if (pch) 
		{
			len = (int) (strlen(line) - strlen(pch));
			index = Max(index, len);
		}
	}

	index++;

	if (index != 0) 
	{
		while (line[index] == ' ')
		{
			index++;
			if (index >= len) return NULL;
		}
	}
	/* Search the beginning of the path or file name */
	/* cd toto */
	/* cd("toto */
	linebis = &line[index];
	index = (int) strlen(linebis);

	/* Searching for the beginning of a white space */
	pch = strchr(linebis,' ');
	if (pch)
	{
		indexspace = (int) ( strlen(linebis) - strlen(pch) );
	}
	else indexspace = -1;

	if (indexspace != -1) 
	{
		len = (int) strlen(linebis);
		/* In case of more than 1 blanks, have to skip all but the last one */
		while(linebis[indexspace] == ' ')
		{
			indexspace++;
			if ( indexspace >= len ) return NULL;
		}

		/* Decrease index because last value read was not a ' ' */
		indexspace--;
		index = Min(index, indexspace);
	}
	/* Searching for the beginning of a character string */
	pch = strchr(linebis,'\'');
	if (pch)
	{
		indexquote = (int) ( strlen(linebis) - strlen(pch) );
	}
	else indexquote = -1;

	if (indexquote != -1) 
	{
		index = Min(index, indexquote);
	}

	/* Searching for the beginning of a character string */
	pch = strchr(linebis,'\"');
	if (pch)
	{
		indexdquote = (int) ( strlen(linebis) - strlen(pch) );
	}
	else indexdquote = -1;

	if (indexdquote != -1) 
	{
		index = Min(index, indexdquote);
	}

	/* If index found in not the end of the line, add 1 to get substring beginning at the next char */
	len = (int) strlen(linebis);
	if (index < len)
	{
		index++;
	}

	/* bug 5105 */
	/* cd "toto */
	if ( (linebis[index] == '\"') || (linebis[index] == '\''))
	{
		if (index < len)
		{
			index++;
		}
	}

	if ( (index <= 0) || (linebis[index] == '\0') ) return NULL;

	return strdup(&linebis[index]);
}
/*--------------------------------------------------------------------------*/
