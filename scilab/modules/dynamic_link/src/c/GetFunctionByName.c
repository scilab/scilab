/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 
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
#include "GetFunctionByName.h"
#include "dynamic_link.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static BOOL SearchComp(FTAB *Ftab, char *op, void (**realop) ());
static void Emptyfunc(void) {}
/*--------------------------------------------------------------------------*/
voidf GetFunctionByName (char *name, int *rep, FTAB *table)
{
	void (*loc)();
	if (name)
	{
		int length_name = 0;
		char *s = name;
		char *buf = name;
		while ( *s != ' ' && *s != '\0') { s++;length_name++;};

		buf = (char*)MALLOC(sizeof(char)*(length_name+1));
		if (buf)
		{
			strncpy(buf,name,length_name);
			buf[length_name] = '\0';

			/* search name in functions table */
			if ( ( SearchComp(table,buf,&loc) == TRUE) || ( SearchInDynLinks(buf,&loc) >= 0 ) )
			{
				*rep = 0;
			}
			else
			{
				loc = Emptyfunc;
				*rep = 1;
			}

			FREE(buf);
			buf = NULL;
		}
		else
		{
			*rep = 0;
		}
	}
	else
	{
		*rep = 0;
	}
	return(loc);
}
/*--------------------------------------------------------------------------*/
static BOOL SearchComp(FTAB *Ftab, char *op, void (**realop) ())
{
	int i=0;
	while ( Ftab[i].name != (char *) 0) 
	{
		int j=0;
		j = strcmp(op,Ftab[i].name);
		if ( j == 0 )
		{
			*realop = Ftab[i].f;
			return TRUE;
		}
		else
		{ 
			if ( j <= 0) return FALSE;
			else i++;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
