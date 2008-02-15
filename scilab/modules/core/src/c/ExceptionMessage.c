/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "MALLOC.h"
#include "GetExceptionCode.h"
#include "ExceptionMessage.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
extern char *GetExceptionString(DWORD ExceptionCode);
/*--------------------------------------------------------------------------*/
void ExceptionMessage(DWORD ExceptionCode,char *functionname)
{
	char *ExceptionString=GetExceptionString(ExceptionCode);
	if (functionname)
	{
		sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nSave your data and restart Scilab.\n",ExceptionString,functionname);
	}
	else
	{
		sciprint("Warning !!!\nScilab has found a critical error (%s).\nSave your data and restart Scilab.\n",ExceptionString);
	}
	if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
}
/*--------------------------------------------------------------------------*/
