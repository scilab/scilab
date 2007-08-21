/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "stack-def.h"
#include "basout.h"
#include "../../fileio/includes/diary.h"
#include "sciprint.h"
#include "MALLOC.h"
#include "../../../gui/includes/xscimore.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(writelunitstring)();
/*-----------------------------------------------------------------------------------*/ 
int C2F(basout)(integer *io, integer *lunit, char *string,long int nbcharacters)
{
	static integer ich;

	if (*lunit == C2F(iop).wte)
	{
		/* Display on the standard output */

		/* We haven't called this function before ... Then we call it and 
		   store the result once for all because it won't change */
		*io = 0;
		if (C2F(iop).lct[0] == -1) { return 0; }
		if (C2F(iop).lct[1] > 0) 
		{
			/* Management of the page numbering (pagination in French) */
			if (C2F(iop).lct[0] + 3 > C2F(iop).lct[1])
			{
				/* Number of max line reached, management of the 'more' */
				C2F(iop).lct[0] = 0;
				if (getScilabMode() != SCILAB_STD) 
				{
					int ch;
					/* Scilab has not his own window */
					sciprint(" more ? ");
					ch = getchar();
					if ( (ch != ' ') && (ch != '\n') && (ch != 'y') ) ich = 1;
				}
				else
				{
					/* scilab has his own window */
					C2F(xscimore)(&ich);
				}

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

		if (getScilabMode() != SCILAB_STD)
		{
			C2F(writelunitstring)(lunit, string,nbcharacters);
			/* write to diary file if required */
			C2F(diary)(string, &nbcharacters);
		} 
		else 
		{
			if (nbcharacters > 0)
			{
				#define bufferformat "%s\r\n"
				char *buffer = NULL;
				buffer = (char *)MALLOC(sizeof(char)*(nbcharacters+strlen(bufferformat)+1));
				if (buffer)
				{
					strncpy(buffer,string,nbcharacters);
					buffer[nbcharacters]='\0';
					sciprint(bufferformat,buffer);
					if (buffer) { FREE(buffer); buffer = NULL;}
				}
			}
		}
	} 
	else
	{
		/* Output to a file */
		if (*lunit == C2F(iop).wio) 
		{
			C2F(diary)(string, &nbcharacters);
		}
		else 
		{
			C2F(writelunitstring)(lunit, string,nbcharacters);
		}
	}
	return 0;
} 
/*-----------------------------------------------------------------------------------*/ 
