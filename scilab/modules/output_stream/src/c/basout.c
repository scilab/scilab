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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack-def.h"
#include "basout.h"
#include "MALLOC.h"
#include "diary.h"
#include "sciprint.h"
#include "charEncoding.h"
#include "../../../shell/includes/more.h"
#include "../../../shell/includes/scilines.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(basouttofile)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/ 
int C2F(basout)(int *io, int *lunit, char *string,long int nbcharacters)
{
	/* bug 3831 */
	if (string)
	{
		int i = 0;
		for (i = 0; i < nbcharacters - 1; i++) 
		{
			if (string[i] == 0) string[i] = ' ';
		}
	}

	if (*lunit == C2F(iop).wte)
	{
		/* Display on the standard output */

		/* We haven't called this function before ... Then we call it and 
		store the result once for all because it won't change 
		*/
		*io = 0;
		if (C2F(iop).lct[0] == -1) { return 0; }
		if (getLinesSize() > 0) 
		{
			/* Management of the page numbering (pagination in French) */
			if (C2F(iop).lct[0] + 3 > getLinesSize())
			{
				int ich = 0;

				/* Number of max line reached, management of the 'more' */
				C2F(iop).lct[0] = 0;

				ich = linesmore();

				if (ich == 1) 
				{
					C2F(iop).lct[0] = -1;
					*io = -1;
					return 0;
				}
			} 
			else
			{
				++C2F(iop).lct[0];
			}
		}

		if (string)
		{
			if (nbcharacters > 1)
			{
				/* on linux , q=[] crashs with previous version 
				in printf.f line 102 
				call basout(io,lunit,'     []')
				if we do basout(io,lunit,'     []',7) it works ...
				temp workaround , we returns to old version with a allocation
				*/
				char *buffer = (char *)MALLOC(sizeof(char)*(nbcharacters+1));
				if (buffer)
				{
					strncpy(buffer,string,nbcharacters);
					buffer[nbcharacters]='\0';
					sciprint("%s\n",buffer);
					FREE(buffer); buffer = NULL;
				}
				else
				{
					sciprint("\n");
				}
			}
			else if (nbcharacters == 1)
			{
				sciprint("%c\n", string[0]);
			}
			else
			{
				sciprint("\n");
			}
		}
		else sciprint("\n");
	} 
	else
	{
		if (*lunit == -2)
		{
			wchar_t *wcBuffer = NULL;

			string[nbcharacters] = '\0';

			/* remove blanks at end of line */
			if (*lunit == -2)
			{
				int i = 0;
				int len = (int) strlen(string) - 2;
				for (i = len; i >= 0; i--)
				{
					if (string[i] == ' ') string[i] = '\0';
					else break;
				}
			}

			wcBuffer = to_wide_string(string);
			if (wcBuffer)
			{
				if (wcscmp(wcBuffer,L"")) diaryWriteln(wcBuffer, TRUE);
				FREE(wcBuffer);
				wcBuffer = NULL;
			}
		}
		else
		{
			C2F(basouttofile)(lunit, string,nbcharacters);
		}
	}
	return 0;
} 
/*--------------------------------------------------------------------------*/ 

