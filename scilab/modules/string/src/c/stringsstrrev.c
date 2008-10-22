
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

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stringsstrrev.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
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
				Output_strings[i] = (char*)MALLOC(sizeof(char)*(strlen(Input_strings[i])+1));
				if (Output_strings[i])
				{
					strcpy(Output_strings[i],scistrrev(Input_strings[i]));
				}
				else
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
#if _MSC_VER
	return _strrev(str);
#else
	if ( !str ) return NULL;
	int i = strlen(str);
	int t = !(i%2)? 1 : 0;      // check the length of the string .
	int j = 0,k = 0;

	/* copy character by character to reverse string */
	k = 0;
	for(j = i-1; j > (i/2 -t) ; j-- ) /* @TODO add comment */
	{
		/* j starts from end of string */
		/* k starts from beginning of string */
		char ch  = str[j]; /* ch temp. character */
		str[j]   = str[k]; /* end and beginning characters are exchanged */
		str[k++] = ch;
	}
	return str;
#endif
}
/*----------------------------------------------------------------------------*/
