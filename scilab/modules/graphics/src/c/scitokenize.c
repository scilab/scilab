/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
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
#include "scitokenize.h"
#include "MALLOC.h" /* MALLOC */
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/ 
int scitokenize(char * legend, char *** Strptr, int *n)
{
	int arobaseIndex = 0;
	int i = 0;
	int count = 1;
	int prevIndex = 0;
	int stringLength = 0, j = 0;
	char **Str = NULL;

	/* first get the number of tokens */
	for (i=0; i<(int)strlen(legend);i++) 
	{
		if ( legend[i] == '@') count++;  
	}
	*Strptr = (char **)MALLOC(count*sizeof(char *));
	Str = *Strptr;
	if (Str == NULL)  return 1; /* not enough memory */
	for (i = 0; i < count; i++) Str[i]=NULL;

	for (i = 0; i < count; i++) 
	{
		while( legend[arobaseIndex] != '@' && legend[arobaseIndex] != 0)
		{
			arobaseIndex++;
		}
		/* we found a token */
		stringLength =  arobaseIndex - prevIndex;
		Str[i]=(char *)MALLOC((stringLength+1)*sizeof(char));/*  +1 for null terminating character*/

		if (Str[i]==NULL) 
		{
			freeArrayOfString(Str,i-1);
			return 1;
		}

		for (j = 0; j < stringLength; j++) 
		{
			Str[i][j] = legend[prevIndex + j];
		}
		Str[i][stringLength] = 0;

		/* skip the @*/
		arobaseIndex++;
		prevIndex = arobaseIndex;
	}
	*n=count;
	return 0;
 }
/*--------------------------------------------------------------------------*/ 
