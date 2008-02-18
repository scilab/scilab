
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "stack-def.h"
#include "basout.h"
#include "../../../fileio/includes/diary.h"
#include "sciprint.h"
#include "MALLOC.h"
#include "../../../shell/includes/more.h"
#include "../../../shell/includes/scilines.h"
/*--------------------------------------------------------------------------*/ 
#define bufferformat "%s\n"
/*--------------------------------------------------------------------------*/ 
extern int C2F(writelunitstring)();
/*--------------------------------------------------------------------------*/ 
int C2F(basout)(integer *io, integer *lunit, char *string,long int nbcharacters)
{
	char *buffer = NULL;
	static integer ich;

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

		buffer = (char *)MALLOC(sizeof(char)*(nbcharacters+strlen(bufferformat)+1));
		if (buffer)
		{
			strncpy(buffer,string,nbcharacters);
			buffer[nbcharacters]='\0';
			sciprint(bufferformat,buffer);
			if (buffer) { FREE(buffer); buffer = NULL;}
		}
	} 
	else
	{
		/* Output to a file */
		if (*lunit == C2F(iop).wio) 
		{
			diary(string, &nbcharacters);
		}
		else 
		{
			C2F(writelunitstring)(lunit, string,nbcharacters);
		}
	}
	return 0;
} 
/*--------------------------------------------------------------------------*/ 
