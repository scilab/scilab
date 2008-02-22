/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "TCL_Global.h"
/*--------------------------------------------------------------------------*/
Tcl_Interp *TCLinterp = NULL;
Tk_Window TKmainWindow = NULL;
/*--------------------------------------------------------------------------*/
char *UTF8toANSI(Tcl_Interp *TCLinterpl,char *StringUTF8)
{
	#define AddCharacters 4
	char *ReturnANSIString=NULL;
	if (StringUTF8)
	{
		ReturnANSIString=MALLOC(sizeof(char)*(strlen(StringUTF8)+AddCharacters));
		/* UTF to ANSI */
		Tcl_UtfToExternal(TCLinterpl, NULL, StringUTF8, (int)strlen(StringUTF8), 0, NULL, ReturnANSIString, (int)(strlen(StringUTF8)+AddCharacters), NULL, NULL,NULL);
	}
	return ReturnANSIString;
}
/*--------------------------------------------------------------------------*/
