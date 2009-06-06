/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "funcprot.h"
/*--------------------------------------------------------------------------*/
extern MPROT_struct C2F(mprot);
/*--------------------------------------------------------------------------*/
int C2F(initfuncprot)()
{
	C2F(mprot).macprt = 1;
	return 0;
}
/*--------------------------------------------------------------------------*/
int getfuncprot(void)
{
	return C2F(mprot).macprt;
}
/*--------------------------------------------------------------------------*/
BOOL setfuncprot(int level)
{
	if ( (level == 0) || (level == 1) || (level == 2) )
	{
		C2F(mprot).macprt = level;
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
