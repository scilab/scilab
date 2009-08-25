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
#include "getvariablesname.h"
#include "commandwords.h"
#include "getfunctionslist.h"
#include "getmacrosdictionary.h"
#include "BOOL.h"
#include "ScilabKeywords.h"
/*--------------------------------------------------------------------------*/ 
char **GetVariablesName(void)
{
	int returnedArraySize = 0;
	return getVariablesName(&returnedArraySize, TRUE);
}
/*--------------------------------------------------------------------------*/ 
char **GetCommandsName(void)
{
	int returnedArraySize = 0;
	return getcommandkeywords(&returnedArraySize);
}
/*--------------------------------------------------------------------------*/ 
char **GetFunctionsName(void)
{
	int returnedArraySize = 0;
	return GetFunctionsList(&returnedArraySize);
}
/*--------------------------------------------------------------------------*/ 
char **GetMacrosName(void)
{
	int returnedArraySize = 0;
	return getmacrosdictionary(&returnedArraySize);
}
/*--------------------------------------------------------------------------*/ 
