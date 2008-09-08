
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
