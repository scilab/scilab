
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
#include "AddFunctionInTable.h"
#include "dynamic_link.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#define MAXNAME 32
static char buf[MAXNAME];
/*--------------------------------------------------------------------------*/
static BOOL SearchComp(FTAB *Ftab, char *op, void (**realop) ());
static void Emptyfunc(void) {}
/*--------------------------------------------------------------------------*/
voidf AddFunctionInTable (char *name, int *rep, FTAB *table)
{
	void (*loc)();
	char *s = NULL;

	strncpy(buf,name,MAXNAME);
	s=buf;
	while ( *s != ' ' && *s != '\0') { s++;};
	*s= '\0';

	if ( ( SearchComp(table,buf,&loc) == TRUE) || ( SearchInDynLinks(buf,&loc) >= 0 ) )
	{
		*rep = 0;
	}
	else
	{
		loc = Emptyfunc;
		*rep = 1;
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
