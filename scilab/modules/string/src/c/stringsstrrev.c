
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stringsstrrev.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*----------------------------------------------------------------------------*/
char **strings_strrev(char **Input_strings,int Dim_Input_strings)
{
	char **Output_strings = NULL;
	if (Input_strings)
	{
		Output_strings = (char **)MALLOC(sizeof(char*)*Dim_Input_strings);
		if (Output_strings)
		{
			int i = 0;	
			for (i = 0;i < Dim_Input_strings;i++)
			{
				Output_strings[i] = scistrrev(Input_strings[i]);
				if (!Output_strings[i])
				{
					freeArrayOfString(Output_strings,i);
					return Output_strings;
				}
			}
		}
	}
	return Output_strings;
}
/*----------------------------------------------------------------------------*/
char* scistrrev(char* str)
{
	char *revstr = NULL;
	if (str)
	{
		wchar_t *wcstr = to_wide_string(str);
#ifdef _MSC_VER
		wchar_t *wcrevstr = _wcsrev(wcstr);
		revstr = wide_string_to_UTF8(wcrevstr);
#else
		int i = 0;
		int t = 0;
		int j = 0, k = 0;

		if (wcstr) i = (int)wcslen(wcstr);
		t = !(i%2)? 1 : 0;      // check the length of the string .
		
		/* copy character by character to reverse string */
		k = 0;
		for(j = i-1; j > (i/2 -t) ; j-- )
		{
			/* j starts from end of string */
			/* k starts from beginning of string */
			wchar_t ch  = wcstr[j]; /* ch temp. character */
			wcstr[j]   = wcstr[k]; /* end and beginning characters are exchanged */
			wcstr[k++] = ch;
		}
		revstr = wide_string_to_UTF8(wcstr);
#endif
		if (wcstr) {FREE(wcstr); wcstr = NULL;}
	}
	return revstr;
}
/*----------------------------------------------------------------------------*/
