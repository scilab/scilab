/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AddFunctionInTable.h"
/* TO DO : See with V.C if we need again this file */

/***********************************
* Search Table for fbutn
***********************************/


#define ARGS_fbutn char *,integer*,integer*
typedef integer * (*fbutnf)(ARGS_fbutn);

/***************** fbutn ***************/
extern void haltscicos(ARGS_fbutn);
void C2F(fbutn)(ARGS_fbutn);
void C2F(setfbutn)(char *name, int *rep);

FTAB FTab_fbutn[] =
{
	{"haltscicos", (voidf)  haltscicos},
	{(char *) 0, (voidf) 0}
};




/***********************************
* Search Table for fbutn
**********************************/

/** the current function fixed by setfbutn **/

static fbutnf fbutnfonc ;

/** function call **/

void C2F(fbutn)(char *name, integer *win, integer *entry)
{
	(*fbutnfonc)(name,win,entry);
}

/** fixes the function associated to name **/

void C2F(setfbutn)(char *name, int *rep)
{
	fbutnfonc = (fbutnf) AddFunctionInTable(name,rep,FTab_fbutn);
}
