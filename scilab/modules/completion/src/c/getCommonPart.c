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
#include <stdlib.h>
#include "getCommonPart.h"
#include "core_math.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static int cmp( const void *a ,const void *b)
{
	return strcmp(*(const char **)a, *(const char **)b );
}
/*--------------------------------------------------------------------------*/
static int cmpPos(char *str1,char *str2)
{
	if (str1 && str2)
	{
		int i = 0;
		char *s1 = NULL;
		char *s2 = NULL;
		int lenstr1 = (int) strlen(str1);
		int lenstr2 = (int) strlen(str2);

		if (lenstr1 > lenstr2) 
		{
			s1 = str2;
			s2 = str1;
			lenstr1 = (int) strlen(str1);
			lenstr2 = (int) strlen(str2);
		}
		else
		{
			s1 = str1;
			s2 = str2;
		}
		for (i = 0; i < lenstr1; i++)
		{
			if ( s1[i] != s2[i] ) return i;
		}

		return Min(lenstr1,lenstr2);
	}
	return -1;
}
/*--------------------------------------------------------------------------*/
char *getCommonPart(char **dictionary, int sizeDictionary)
{
	char *commonpart = NULL;

	if (sizeDictionary == 1)
	{
		return strdup(dictionary[0]);
	}

	if (sizeDictionary >= 2)
	{
		int i = 0;
		int r = 0;
		char *currentstr = dictionary[0];
		qsort(dictionary, sizeof dictionary / sizeof dictionary[0], sizeof dictionary[0], cmp);

		r = cmpPos(currentstr, dictionary[1]);
		for (i = 1; i < sizeDictionary - 1; i++)
		{
			int current_r = cmpPos(currentstr, dictionary[i+1]);
			if (r > current_r  )
			{
				r = current_r;
				currentstr = dictionary[i+1];
			}
		}
		
		commonpart = strdup(currentstr);
		if (r>0) commonpart[r] = '\0';
	}
	return commonpart;
}
/*--------------------------------------------------------------------------*/
