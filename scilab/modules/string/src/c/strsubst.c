
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include "strsubst.h"
#include "MALLOC.h"
#include "pcre_private.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
char **strsubst(char **strings_input,int strings_dim,char *string_to_search,char *replacement_string)
{
	char **replacedStrings = NULL;

	if ( (strings_input) && (string_to_search) && (replacement_string) )
	{
		int i = 0;
		replacedStrings = (char**)MALLOC(sizeof(char*)*strings_dim);
		for (i = 0; i < strings_dim; i++)
		{
			char *str = strings_input[i];
			replacedStrings[i] = strsub (str,string_to_search,replacement_string);
		}
	}
	return replacedStrings;
}
/*--------------------------------------------------------------------------*/
char **strsubst_reg(char **strings_input,int strings_dim,char *string_to_search,char *replacement_string, int *ierr)
{
	char **replacedStrings = NULL;

	if ( (strings_input) && (string_to_search) && (replacement_string) )
	{
		int i = 0;
		replacedStrings = (char**)MALLOC(sizeof(char*)*strings_dim);
		for (i = 0; i < strings_dim; i++)
		{
			char *str = strings_input[i];
			replacedStrings[i] = strsub_reg(str,string_to_search,replacement_string, ierr);
		}
	}
	return replacedStrings;
}
/*-------------------------------------------------------------------------------------*/
char *strsub(char* input_string, const char* string_to_search, const char* replacement_string)
{
	char *occurrence_str = NULL, *result_str = NULL;
	

	char *replacedString = NULL;
	int count = 0, len = 0;

	if (input_string == NULL) return NULL;

	if (string_to_search == NULL || replacement_string == NULL) 
	{
		return strdup(input_string);
	}

	occurrence_str = strstr (input_string, string_to_search);
	if (occurrence_str == NULL)
	{
		return strdup(input_string);
	}

	if (strlen (replacement_string) > strlen (string_to_search)) 
	{
		count = 0;
		len = (int)strlen (string_to_search);
		if (len)
		{
			occurrence_str = input_string;
			while(occurrence_str != NULL && *occurrence_str != '\0') 
			{
				occurrence_str = strstr (occurrence_str, string_to_search);
				if (occurrence_str != NULL) 
				{
					occurrence_str += len;
					count++;
				}
			}
		}
		len = count * ((int)strlen(replacement_string) - (int)strlen(string_to_search)) + (int)strlen(input_string);
	}
	else len = (int)strlen(input_string);

	replacedString = MALLOC (sizeof(char)*(len + 1));
	if (replacedString == NULL) return NULL;

	occurrence_str = input_string;
	result_str = replacedString;
	len = (int)strlen (string_to_search);
	while(*occurrence_str != '\0') 
	{
		if (*occurrence_str == string_to_search[0] && strncmp (occurrence_str, string_to_search, len) == 0) 
		{
			const char *N = NULL;
			N = replacement_string;
			while (*N != '\0') *result_str++ = *N++;
			occurrence_str += len;
		}
		else *result_str++ = *occurrence_str++;
	}
	*result_str = '\0';

	return replacedString;
}/*-------------------------------------------------------------------------------------*/
char *strsub_reg(char* input_string, const char* string_to_search, const char* replacement_string, int *ierr)
{
    pcre_error_code w = PCRE_FINISHED_OK;

	int Output_Start = 0;
    int Output_End = 0;

	char *replacedString = NULL;
	wchar_t *wcreplacedString = NULL;

	wchar_t *wcreplacement_string = NULL;
	wchar_t *wcinput_string = NULL;

	int len = 0;

	*ierr = (int)PCRE_FINISHED_OK;

	if (input_string == NULL) return NULL;

	if (string_to_search == NULL || replacement_string == NULL) 
	{
		return strdup(input_string);
	}

    w = pcre_private(input_string,(char*)string_to_search,&Output_Start,&Output_End);
	if (w != PCRE_FINISHED_OK)
	{
		*ierr = (int)w;
		return strdup(input_string);
	}

	wcreplacement_string = to_wide_string((char*)replacement_string);
	wcinput_string = to_wide_string((char*)input_string);

	if (wcreplacement_string == NULL || wcreplacement_string == NULL) 
	{
		*ierr = (int)NOT_ENOUGH_MEMORY_FOR_VECTOR;
		return strdup(input_string);
	}

	if (w == PCRE_FINISHED_OK) 
	{
		len = (int)wcslen(wcreplacement_string) + (int)wcslen(wcinput_string);
	}
	else
	{
		len = (int)wcslen(wcinput_string);
	}

	wcreplacedString = (wchar_t*)MALLOC (sizeof(wchar_t)*(len + 1));
	if (wcreplacedString == NULL) return NULL;

	{
		/* converts to wide characters */

		wchar_t *wctail = NULL;

		int wcOutput_Start = 0;
		int wcOutput_End = 0;

		char *	strOutput_Start = strdup(input_string);
		char *  strOutput_End =  strdup(input_string);

		wchar_t *wcstrOutput_Start = NULL;
		wchar_t *wcstrOutput_End = NULL;

		/* calculates positions with wide characters */
		strOutput_Start[Output_Start] = '\0';
		strOutput_End[Output_End] = '\0';

		wcstrOutput_Start = to_wide_string(strOutput_Start);
		wcstrOutput_End = to_wide_string(strOutput_End);

		if (strOutput_Start) {FREE(strOutput_Start);strOutput_Start = NULL;}
		if (strOutput_End) {FREE(strOutput_End);strOutput_End = NULL;}

		if (wcstrOutput_Start)
		{
			wcOutput_Start = (int)wcslen(wcstrOutput_Start);
			FREE(wcstrOutput_Start);wcstrOutput_Start = NULL;
		}
		else
		{
			wcOutput_Start = 0;
		}

		if (wcstrOutput_End)
		{
			wcOutput_End = (int)wcslen(wcstrOutput_End);
			FREE(wcstrOutput_End);wcstrOutput_End = NULL;
		}
		else
		{
			wcOutput_End = 0;
		}

		wcsncpy(wcreplacedString,wcinput_string,wcOutput_Start);
		wcreplacedString[wcOutput_Start]=L'\0';
		wcscat(wcreplacedString,wcreplacement_string);
		wctail = wcinput_string + wcOutput_End;
		wcscat(wcreplacedString,wctail);
		replacedString = wide_string_to_UTF8(wcreplacedString);

		if (wcreplacedString) {FREE(wcreplacedString);wcreplacedString = NULL;}
	}

	return replacedString;
}
/*-------------------------------------------------------------------------------------*/
