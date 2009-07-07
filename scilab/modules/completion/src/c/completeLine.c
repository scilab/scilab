/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "completeLine.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static char * strrstr(char *string, char *find)
{
	size_t stringlen, findlen;
	char *cp = NULL;

	findlen = strlen(find);
	stringlen = strlen(string);
	if (findlen > stringlen)
		return NULL;

	for (cp = string + stringlen - findlen; cp >= string; cp--)
		if (strncmp(cp, find, findlen) == 0)
			return cp;

	return NULL;
}
/*--------------------------------------------------------------------------*/
char *completeLine(char *currentline,char *stringToAdd,char *filePattern,
				   char *defaultPattern,BOOL stringToAddIsPath)
{
	char *new_line = NULL;

	if (stringToAddIsPath)
	{
		char *ptr_strrchar1 = NULL;

		ptr_strrchar1 = strstr(stringToAdd, defaultPattern);

		if (ptr_strrchar1) 
		{
			char *ptr_strrchar2 = NULL;

			if (strcmp(defaultPattern,"") == 0)
			{
				if ( ((filePattern[strlen(filePattern)-1] == '\\') || (filePattern[strlen(filePattern)-1] == '/')) &&  ((int)strlen(filePattern) > 1) )
				{
					ptr_strrchar2 = strrstr(currentline, defaultPattern);
				}
				else ptr_strrchar2 = strrstr(currentline, filePattern);
			}
			else ptr_strrchar2 = strrstr(currentline, defaultPattern);

			new_line = (char*)MALLOC(sizeof(char)*(strlen(currentline) + strlen(stringToAdd) + 1));

			if (new_line)
			{
				int l = 0;

				if (ptr_strrchar2) l = (int)(strlen(currentline)- strlen(ptr_strrchar2));
				else l = (int)strlen(currentline);

				if (l < 0) l = 0 - l;

				strncpy(new_line,currentline, l);
				new_line[l] = '\0';

				/* special case with files begin with a '.' */
				if (filePattern[0] == '.') strcat(new_line, &(stringToAdd[1]));
				else strcat(new_line, ptr_strrchar1);
			}
		}
	}
	else
	{
		if ( (int)strlen(defaultPattern) < (int)strlen(stringToAdd) )
		{	
			char *partResult =  &stringToAdd[(int)strlen(defaultPattern)];
			new_line = (char*)MALLOC(sizeof(char)*(strlen(currentline) + strlen(partResult) + 1));
			if (new_line)
			{
				strcpy(new_line, currentline);
				strcat(new_line, partResult);
			}
		}
		else
		{
			new_line = strdup(currentline);
		}
	}
	return new_line;
}
/*--------------------------------------------------------------------------*/
