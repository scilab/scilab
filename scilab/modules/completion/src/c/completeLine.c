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
#include "getPartLine.h"
#include "splitpath.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
static char * strrstr(char *string, char *find)
{
	char   *cp = NULL;

	if (string == NULL) return NULL;

	cp = string + strlen(string);
	if (*find != '\0')
	{
		if (*string == '\0')
		{
			cp = NULL;
		}
		else
		{ 
			int len = strlen(find + 1);

			do 
			{  
				while (*--cp != *find && cp > string);

				if (*cp == *find && strncmp(cp+1, find+1, len) == 0) break;

				if (cp == string)
				{ 
					cp = NULL;
					break;
				}
			}
			while (TRUE);
		}
	}
	return cp;
}
/*--------------------------------------------------------------------------*/
char *completeLine(char *currentline,char *stringToAdd,char *filePattern,
				   char *defaultPattern,BOOL stringToAddIsPath, char *postCaretLine)
{
	char *new_line = NULL;
	int lengthNewLine = 0;

	char *stringToAddTmp = NULL;
	char *stringToAddAtTheEnd = NULL;

	int lenstringToAdd = 0;
	int lencurrentline = 0;
	int lenstringToAddAtTheEnd = 0;

	int i = 0;
	int j = 0;

	char *result = NULL;

	if (currentline == NULL) 
	{
		return  strdup("");
	}

	lencurrentline = (int)strlen(currentline);

	if (postCaretLine == NULL)
	{
		stringToAddAtTheEnd = strdup("");
		lenstringToAddAtTheEnd = strlen(stringToAddAtTheEnd);
	}
	else
	{
		stringToAddAtTheEnd = strdup(postCaretLine);
		lenstringToAddAtTheEnd = strlen(stringToAddAtTheEnd);
	}

	if ( (stringToAdd == NULL)  || (strcmp(stringToAdd, "") == 0) )
	{
		lengthNewLine = lencurrentline + lenstringToAddAtTheEnd;
		new_line = (char*)MALLOC(sizeof(char) * (lengthNewLine + 1));
		if (new_line)
		{
			strcpy(new_line, currentline);
			strcat(new_line, stringToAddAtTheEnd);
		}
		return new_line;
	}

	if (stringToAddIsPath == FALSE)
	{
		char *filePatternBuf = NULL;
		BOOL bfilePatternBuf = FALSE;

		if (filePattern != NULL)
		{
			filePatternBuf = filePattern;
		}
		else
		{
			filePatternBuf = getFilePartLevel(currentline);
			bfilePatternBuf = TRUE;
		}

		if (filePatternBuf)
		{
			char* drv = (char*)MALLOC(sizeof(char)*(PATH_MAX+1));
			char* dir = (char*)MALLOC(sizeof(char)*(PATH_MAX+1));
			char* name = (char*)MALLOC(sizeof(char)*(PATH_MAX+1));
			char* ext = (char*)MALLOC(sizeof(char)*(PATH_MAX+1));

			splitpath(filePatternBuf,TRUE, drv,dir, name, ext);

			if (bfilePatternBuf)
			{
				FREE(filePatternBuf);
				filePatternBuf = NULL;
				bfilePatternBuf = FALSE;
			}

			if ( strcmp(drv,"") || strcmp(dir,"") )
			{
				/* bug 4365 */
				/*cd SCI/modules/arnoldi/nonreg_tes */

				if (drv) {FREE(drv); drv = NULL;}
				if (dir) {FREE(dir); dir = NULL;}
				if (name) {FREE(name); name = NULL;}
				if (ext) {FREE(ext); ext = NULL;}

				lengthNewLine = lencurrentline + lenstringToAddAtTheEnd;
				new_line = (char*)MALLOC(sizeof(char) * (lengthNewLine + 1));
				if (new_line)
				{
					strcpy(new_line, currentline);
					strcat(new_line, stringToAddAtTheEnd);
				}
				return new_line;
			}

			if (drv) {FREE(drv); drv = NULL;}
			if (dir) {FREE(dir); dir = NULL;}
			if (name) {FREE(name); name = NULL;}
			if (ext) {FREE(ext); ext = NULL;}
			
		}
	}

	lenstringToAdd = (int)strlen(stringToAdd);

	result = strrstr(currentline, stringToAdd);
	if (result)
	{
		j = lenstringToAdd;
	}
	else
	{
		for(i = 0; i < lenstringToAdd; i++)
		{
			stringToAddTmp = (char*)MALLOC((i+1)*sizeof(char));;
			if (stringToAddTmp)
			{
				strncpy(stringToAddTmp,stringToAdd,i);
				stringToAddTmp[i] = 0;

				if (stringToAddTmp[0] != 0)
				{
					result = strrstr(currentline, stringToAddTmp);
					if (result)
					{
						j = i;
					}
					else
					{
						FREE(stringToAddTmp);
						stringToAddTmp = NULL;
						break;
					}
				}

				FREE(stringToAddTmp);
				stringToAddTmp = NULL;
			}
		}
	}

	{
		char *pos = &currentline[lencurrentline - j];
		stringToAddTmp = strdup(stringToAdd);
		stringToAddTmp[j] = 0;
		if (stringToAddTmp[0] != 0)
		{
			if (strcmp(pos, stringToAddTmp) == 0)
			{
				lengthNewLine = (int)(strlen(currentline)+ strlen(stringToAdd) + lenstringToAddAtTheEnd);
				new_line = (char*)MALLOC(sizeof(char)*(lengthNewLine + 1));
				strcpy(new_line, currentline);
				new_line[lencurrentline - j] = 0;
				strcat(new_line, stringToAdd);
				strcat(new_line, stringToAddAtTheEnd);
			}
			else
			{
				lengthNewLine = (int)(strlen(currentline)+ strlen(stringToAdd) + lenstringToAddAtTheEnd);
				new_line = (char*)MALLOC(sizeof(char)*(lengthNewLine + 1));
				if (new_line)
				{
					strcpy(new_line, currentline);
					strcat(new_line, stringToAdd);
					strcat(new_line, stringToAddAtTheEnd);
				}
			}
		}
		else
		{
			lengthNewLine = (int)(strlen(currentline)+ strlen(stringToAdd) + lenstringToAddAtTheEnd);
			new_line = (char*)MALLOC(sizeof(char)*(lengthNewLine + 1));
			if (new_line)
			{
				strcpy(new_line, currentline);
				strcat(new_line, stringToAdd);
				strcat(new_line, stringToAddAtTheEnd);
			}
		}
	}
	return new_line;
}
/*--------------------------------------------------------------------------*/
