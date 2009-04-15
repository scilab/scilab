
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
char **strsubst_reg(char **strings_input,int strings_dim,char *string_to_search,char *replacement_string)
{
	char **replacedStrings = NULL;

	if ( (strings_input) && (string_to_search) && (replacement_string) )
	{
		int i = 0;
		replacedStrings = (char**)MALLOC(sizeof(char*)*strings_dim);
		for (i = 0; i < strings_dim; i++)
		{
			char *str = strings_input[i];
			replacedStrings[i] = strsub_reg(str,string_to_search,replacement_string);
		}
	}
	return replacedStrings;
}
/*-------------------------------------------------------------------------------------*/
char *strsub(char* input_string, const char* string_to_search, const char* replacement_string)
{
	int count = 0;
	int len = 0;
	wchar_t *wcinput_string = NULL;
	wchar_t *wcstring_to_search = NULL;
	wchar_t *wcreplacement_string = NULL;
	wchar_t *wcoccurrence_str = NULL;
	wchar_t *wcreplacedString = NULL;
	wchar_t *wcresult_str = NULL;

	if (input_string == NULL) return NULL;

	if (string_to_search == NULL || replacement_string == NULL) 
	{
		return strdup(input_string);
	}

	wcinput_string = to_wide_string(input_string);
	wcstring_to_search = to_wide_string((char*)string_to_search);
	wcreplacement_string = to_wide_string((char*)replacement_string);

	wcoccurrence_str = wcsstr (wcinput_string, wcstring_to_search);

	if (wcoccurrence_str == NULL)
	{
		FREE(wcinput_string); wcinput_string = NULL;
		FREE(wcstring_to_search); wcstring_to_search = NULL;
		FREE(wcreplacement_string); wcreplacement_string = NULL;
		return strdup(input_string);
	}

	if (wcslen (wcreplacement_string) > wcslen (wcstring_to_search)) 
	{
		count = 0;
		len = (int)wcslen (wcstring_to_search);
		if (len)
		{
			wcoccurrence_str = wcinput_string;
			while(wcoccurrence_str != NULL && *wcoccurrence_str != L'\0') 
			{
				wcoccurrence_str = wcsstr (wcoccurrence_str, wcstring_to_search);
				if (wcoccurrence_str != NULL) 
				{
					wcoccurrence_str += len;
					count++;
				}
			}
		}
		len = count * ((int)wcslen(wcreplacement_string) - (int)wcslen(wcstring_to_search)) + (int)wcslen(wcinput_string);
	}
	else len = (int)wcslen(wcinput_string);

	wcreplacedString = (wchar_t*)MALLOC (sizeof(wchar_t)*(len + 1));
	if (wcreplacedString == NULL) 
	{
		FREE(wcreplacement_string); wcreplacement_string = NULL;
		FREE(wcstring_to_search); wcstring_to_search = NULL;
		FREE(wcreplacement_string); wcreplacement_string = NULL;
		return NULL;
	}

	wcoccurrence_str = wcinput_string;
	wcresult_str = wcreplacedString;

	len = (int)wcslen (wcstring_to_search);

	while(*wcoccurrence_str != L'\0') 
	{
		if (*wcoccurrence_str == wcstring_to_search[0] && wcsncmp(wcoccurrence_str, wcstring_to_search, len) == 0) 
		{
			const wchar_t *N = NULL;
			N = wcreplacement_string;
			while (*N != L'\0') *wcresult_str++ = *N++;
			wcoccurrence_str += len;
		}
		else *wcresult_str++ = *wcoccurrence_str++;
	}
	*wcresult_str = L'\0';

	return wide_string_to_UTF8(wcreplacedString);
}
/*-------------------------------------------------------------------------------------*/
char *strsub_reg(char* input_string, const char* string_to_search, const char* replacement_string)
{
	char *tail = NULL;
    pcre_error_code w = PCRE_FINISHED_OK;

	int Output_Start = 0;
    int Output_End = 0;

	char *replacedString = NULL;
	int len = 0;

	if (input_string == NULL) return NULL;

	if (string_to_search == NULL || replacement_string == NULL) 
	{
		return strdup(input_string);
	}
    w = pcre_private(input_string,(char*)string_to_search,&Output_Start,&Output_End);
	if (w != PCRE_FINISHED_OK)
	{
		return strdup(input_string);
	}

	if (w == PCRE_FINISHED_OK) 
	{
		len = (int)strlen(replacement_string) + (int)strlen(input_string);
	}
	else len = (int)strlen(input_string);

	replacedString = MALLOC (sizeof(char)*(len+ 1));
	if (replacedString == NULL) return NULL;
	strncpy(replacedString,input_string,Output_Start);
	replacedString[Output_Start]='\0';
	strcat(replacedString,replacement_string);
	tail=input_string+Output_End;
	strcat(replacedString,tail);

	return replacedString;
}
/*-------------------------------------------------------------------------------------*/
