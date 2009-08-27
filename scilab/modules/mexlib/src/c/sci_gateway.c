/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "sci_gateway.h"
/*-----------------------------------------------------------------------------------*/
static int callExternalFunction(char *fname, GatefuncS F,BOOL withPutLhsVar);
/*-----------------------------------------------------------------------------------*/
/** generic scilab interface **/
int sci_gateway(char *fname, GatefuncS F)
{
	return callExternalFunction(fname,F,TRUE);
}
/*-----------------------------------------------------------------------------------*/
int sci_gateway_without_putlhsvar(char *fname, GatefuncS F)
{
	return callExternalFunction(fname,F,FALSE);
}
/*-----------------------------------------------------------------------------------*/
static int callExternalFunction(char *fname, GatefuncS F,BOOL withPutLhsVar)
{
	(*F)(fname,(int)strlen(fname));
	if (withPutLhsVar)PutLhsVar();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
